#include "Arduino.h"
#include <SPI.h>
#include "AD7799lib.h"

uint8_t ad7799_init_status;

void ad7799_reset(void){
  int i;
  for(i = 0; i<4; i++)
    spi_transfer(0xff);
}


void  SPI_Write(unsigned char *data,int size){
  for(int i=0;i<size;i++) {
    (void) SPI.transfer(data[i]);
  }
}


void  SPI_Read(unsigned char *data,int size){
  for(int i=0;i<size;i++) {
    data[i]= SPI.transfer(0xFF);
  }
}

/***************************************************************************//**
 * @brief Writes the value to the register
 *
 * @param -  regAddress - The address of the register to write to.
 * @param -  regValue - The value to write to the register.
 * @param -  size - The size of the register to write.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_SetRegisterValue(unsigned char regAddress,
unsigned long regValue, 
unsigned char size)
{
  unsigned char data[5] = {
    0x03, 0x00, 0x00, 0x00, 0x00  };        
  data[1] = AD7799_COMM_WRITE |  AD7799_COMM_ADDR(regAddress);
  if(size == 1)
  {
    data[2] = (unsigned char)regValue;
  }
  if(size == 2)
  {
    data[3] = (unsigned char)((regValue & 0x0000FF) >> 0);
    data[2] = (unsigned char)((regValue & 0x00FF00) >> 8);
  }
  if(size == 3)
  {
    data[4] = (unsigned char)((regValue & 0x0000FF) >> 0);
    data[3] = (unsigned char)((regValue & 0x00FF00) >> 8);
    data[2] = (unsigned char)((regValue & 0xFF0000) >> 16);
  }
  AD7799_CS_LOW;            
  SPI_Write(data,(1 + size));
  AD7799_CS_HIGH;

}

unsigned long AD7799_GetRegisterValue(unsigned char regAddress, unsigned char size)
{
  unsigned char data[5] = {
    0x03, 0x00, 0x00, 0x00, 0x00  };
  unsigned long receivedData = 0x00;        
  data[1] = AD7799_COMM_READ |  AD7799_COMM_ADDR(regAddress);
  AD7799_CS_LOW;  
 // SPI_Write(data,1);
  SPI_Write(&data[1],1);
  SPI_Read(data,size);
  AD7799_CS_HIGH;
  if(size == 1)
  {
    receivedData += (data[0] << 0);
  }
  if(size == 2)
  {
    receivedData += (data[0] << 8);
    receivedData += (data[1] << 0);
  }
  if(size == 3)
  {
    receivedData += (data[0] << 16);
    receivedData += (data[1] << 8);
    receivedData += (data[2] << 0);
  }
  return receivedData;
}

void ad7799_set_channel(int channel) {
  unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_CONFIG_REG,2);
  command &= ~AD7799_CONF_CHAN(0xFF);
  command |= AD7799_CONF_CHAN(channel);
  AD7799_SetRegisterValue(
  AD7799_CONFIG_REG,
  command,
  2
    );

}

void spi_init() {
  pinMode(10,OUTPUT);
  // start the SPI library:
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  SPI.begin();
}

void ad7799_comm(uint8_t reg, uint8_t read, uint8_t cont)
{
  spi_transfer((read ? 0x40 : 0x00) | (reg << 3) | (cont ? 0x04 : 0x00));
}

void ad7799_write_config(uint8_t burnout, uint8_t unipolar, uint8_t gain,
uint8_t ref_det, uint8_t buf, uint8_t chan)
{
  ad7799_comm(AD7799_CONFIG_REG, 0, 1);
  spi_transfer((burnout ? 0x40 : 0x00) | (unipolar ? 0x20 : 0x00) | gain);
  spi_transfer((ref_det ? 0x20 : 0x00) | (buf ? 0x10 : 0x00) | chan);
}

/* determine if data is ready to be read, could also be
 implemented by reading the status register */
int8_t ad7799_data_ready(void)
{
  //return !(PINB & _BV(DD_MISO));
  return(!digitalRead(12));

}

/* request a read from the data register */
void ad7799_request_data(uint8_t continuous)
{
  ad7799_comm(AD7799_DATA_REG, 1, continuous);
}
/* read from data register, it should be previously requested
 from ad7799_request_data, the value is signed */
int32_t  ad7799_read_data(void)
{
  uint32_t val;

  val = spi_transfer(0);
  val <<= 8;
  val |= spi_transfer(0);
  val <<= 8;
  if(ad7799_init_status & AD7799_STATUS_IS_AD7799)
    val |= spi_transfer(0);

  return val - 0x800000;
}

/***************************************************************************//**
 * @brief Sends 32 consecutive 1's on SPI in order to reset the part.
 *
 * @param None.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_Reset(void)
{
  unsigned char dataToSend[5] = {
    0x03, 0xff, 0xff, 0xff, 0xff  };
  AD7799_CS_LOW;            
  SPI_Write(dataToSend,4);
  AD7799_CS_HIGH;        
}

/***************************************************************************//**
 * @brief Sets the operating mode of AD7799.
 *
 * @param mode - Mode of operation.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_SetMode(unsigned long mode)
{
  unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_REG_MODE,2);
  command &= ~AD7799_MODE_SEL(0xFF);
  command |= AD7799_MODE_SEL(mode);
  AD7799_SetRegisterValue(
  AD7799_REG_MODE,
  command,
  2
    );
}

unsigned char spi_transfer(int address){
  return  SPI.transfer(address);
}

/* helper request function, assumes slaveselect is already set */

/***************************************************************************//**
 * @brief Initializes the AD7799 and checks if the device is present.
 *
 * @param None.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 1 - if initialization was successful (ID is 0x0B).
 *                           0 - if initialization was unsuccessful.
 *******************************************************************************/

unsigned char AD7799_Init(void){ 
  unsigned char status = 0x1;
  status = AD7799_GetRegisterValue(AD7799_REG_ID, 1);
 
  if((status & 0x0F) != AD7799_ID){
    status = 0x0;
  }
  return(status);
}

/* return the contents of the OFFSET register */
int32_t ad7799_read_offset(void)
{
  ad7799_comm(AD7799_OFFSET_REG, 1, 0);
  int32_t val = 0;
  if(ad7799_init_status & AD7799_STATUS_IS_AD7799)
    val |= (int32_t)spi_transfer(0) << 16;
  val |= (int32_t)spi_transfer(0) << 8;
  val |= (int32_t)spi_transfer(0);
  return val;
}

/* return the contents of the FULL-SCALE register */
int32_t ad7799_read_scale(void)
{
  ad7799_comm(AD7799_SCALE_REG, 1, 0);
  int32_t val = 0;

  if(ad7799_init_status & AD7799_STATUS_IS_AD7799)
    val |= (int32_t)spi_transfer(0) << 16;
  val |= (int32_t)spi_transfer(0) << 8;
  val |= (int32_t)spi_transfer(0);
  return val;
}

uint8_t ad7799_status(void)
{
  ad7799_comm(AD7799_STATUS_REG, 1, 0);
  return spi_transfer(0);
}

/*  Setting the mode, pick one for mode and rate,
 psw is either zero or non-zero */


void ad7799_set_mode(uint8_t mode, uint8_t psw, uint8_t rate)
{
  ad7799_comm(AD7799_MODE_REG, 0, 0);
  spi_transfer(mode << 5 | (psw ? 0x10 : 0x00));
  spi_transfer(rate);
}

void ad7799_get_mode(uint8_t *mode, uint8_t *psw, uint8_t *rate)
{
  ad7799_comm(AD7799_MODE_REG, 1, 0);
  uint8_t val = spi_transfer(0);
  if(mode)
    *mode = val >> 5;
  if(psw)
    *psw = val & 0x10 ? 1 : 0;
  val = spi_transfer(0);
  if(rate)
    *rate = val;
}
/***************************************************************************//**
 * @brief  Sets the gain of the In-Amp.
 *
 * @param  gain - Gain.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_SetGain(unsigned long gain)
{
  unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
  command &= ~AD7799_CONF_GAIN(0xFF);
  command |= AD7799_CONF_GAIN(gain);
  AD7799_SetRegisterValue(
  AD7799_REG_CONF,
  command,
  2
    );
}

/***************************************************************************//**
 * @brief Selects the channel of AD7799.
 *
 * @param  channel - ADC channel selection.
 *
 * @return  None.    
 *******************************************************************************/
void AD7799_SetChannel(unsigned long channel)
{
  unsigned long command;
  command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
  command &= ~AD7799_CONF_CHAN(0xFF);
  command |= AD7799_CONF_CHAN(channel);
  AD7799_SetRegisterValue(
  AD7799_REG_CONF,
  command,
  2
    );
}
/***************************************************************************//**
 * @brief Reads /RDY bit of status reg.
 *
 * @param None.
 *
 * @return rdy        - 0 if RDY is 1.
 *              - 1 if RDY is 0.
 *******************************************************************************/
unsigned char AD7799_Ready(void)
{
  unsigned char rdy = 0;
  rdy = (AD7799_GetRegisterValue( AD7799_REG_STAT,1) & 0x80);   

  return(!rdy);
}

/***************************************************************************//**
 * @brief Enables or disables the reference detect function.
 *
 * @param state - State of the reference detect function.
 *               Example: 0        - Reference detect disabled.
 *                        1        - Reference detect enabled.
 *
 * @return None.    
 *******************************************************************************/
void AD7799_SetReference(unsigned char state)
{
  unsigned long command = 0;
  command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
  command &= ~AD7799_CONF_REFDET(1);
  command |= AD7799_CONF_REFDET(state);
  AD7799_SetRegisterValue(AD7799_REG_CONF,
  command,
  2);
}

void ad7799_calibrate()
{
  int32_t off = ad7799_read_offset();

  /* cal */
  ad7799_set_mode(AD7799_INTERNAL_OFFSET_CAL_MODE, 0, AD7799_16_7_1_HZ);
  while(!ad7799_data_ready());

  off = ad7799_read_offset();
  Serial.print("Offset:");
  Serial.println(off);
}

void ad7799_init()
{
  ad7799_reset();
  ad7799_init_status = ad7799_status();
}

int ad7799_get_ID(){
  int id;
  //  CR7     CR6    CR5    CR4    CR3    CR2      CR1 CR0
  //  WEN(0) R/W(0) RS2(0) RS1(0) RS0(0) CREAD(0) 0(0) 0(0)
  //  RS2, RS1, RS0 = 1, 0, 0; Power-On/Reset = 0xX8 (AD7798)/0xX9 (AD7799)

  //write 0b01100000
  digitalWrite(CS,LOW); //enable AD7799
  SPI.transfer(0x60);  //ask for ID, 8 bits
  id = SPI.transfer(0xff); // clock it in
  id &= 0x0f;
  digitalWrite(CS,HIGH); //disable AD7799
  return 7790+id;
}
unsigned int ad7799_read_mode() {
  unsigned int mode;

  digitalWrite(CS,LOW); //enable AD7799

  delay(100);
  SPI.transfer(0x48);  //ask for ID, 8 bits
  mode = (SPI.transfer(0xff) << 8)  | SPI.transfer(0xff); // clock it in
  digitalWrite(CS,HIGH); //deselect AD7799

  return mode;
}

unsigned int ad7799_write_mode() {
  unsigned int mode;

  digitalWrite(CS,LOW); //enable AD7799
  SPI.transfer(0x48);  //ask for ID, 8 bits
  mode = (SPI.transfer(0xff) << 8)  | SPI.transfer(0xff); // clock it in
  digitalWrite(CS,HIGH); //disable AD7799
  return mode;
}


