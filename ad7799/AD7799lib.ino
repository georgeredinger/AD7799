#include "Arduino.h"
#include <SPI.h>
#include "AD7799lib.h"

uint8_t ad7799_init_status;

void ad7799_reset(void)
{
   int i;
   for(i = 0; i<4; i++)
      spi_transfer(0xff);
}

void ad7799_set_channel(int channel) {
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




unsigned char spi_transfer(int address){
  return  SPI.transfer(address);
}

/* helper request function, assumes slaveselect is already set */


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




void ad7799_calibrate()
{
   int32_t off = ad7799_read_offset();

   /* cal */
   ad7799_set_mode(AD7799_INTERNAL_OFFSET_CAL_MODE, 0, AD7799_16_7_1_HZ);
   while(!ad7799_data_ready());

   off = ad7799_read_offset();
   Serial.print("Offset:");Serial.println(off);
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

