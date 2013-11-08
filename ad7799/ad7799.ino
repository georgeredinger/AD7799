/*
 AD7799 sketch
 Arduino Uno
 */

// the sensor communicates using SPI, so include the library:
#include <SPI.h>

#define AD7799_STATUS_RDY       0x80
#define AD7799_STATUS_ERROR     0x40
#define AD7799_STATUS_NOREF     0x20
#define AD7799_STATUS_IS_AD7799 0x08
#define AD7799_STATUS_CHAN_MASK 0x07

uint8_t ad7799_init_status;

#define AD7799_STATUS_REG 0x0
#define AD7799_MODE_REG   0x1
#define AD7799_CONFIG_REG 0x2
#define AD7799_DATA_REG   0x3
#define AD7799_ID_REG     0x4
#define AD7799_IO_REG     0x5
#define AD7799_OFFSET_REG 0x6
#define AD7799_SCALE_REG  0x7




void ad7799_reset(void)
{
   int i;
   for(i = 0; i<4; i++)
      spi_transfer(0xff);
}

unsigned char spi_transfer(int address){
  return SPI.transfer(address);
}

/* helper request function, assumes slaveselect is already set */
static inline void ad7799_comm(uint8_t reg, uint8_t read, uint8_t cont)
{
   spi_transfer((read ? 0x40 : 0x00) | (reg << 3) | (cont ? 0x04 : 0x00));
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
enum { AD7799_CONTINUOUS_CONVERSION_MODE = 0,  AD7799_SINGLE_CONVERSION_MODE,
       AD7799_IDLE_MODE, AD7799_POWERDOWN_MODE, AD7799_INTERNAL_OFFSET_CAL_MODE,
       AD7799_INTERNAL_SCALE_CAL_MODE,  AD7799_SYSTEM_OFFSET_CAL_MODE,
       AD7799_SYSTEM_SCALE_CAL_MODE};

enum { AD7799_470_HZ = 1, AD7799_242_HZ, AD7799_123_HZ, AD7799_62_HZ,
       AD7799_50_HZ, AD7799_39_HZ, AD7799_33_2_HZ, AD7799_19_6_HZ,
       AD7799_16_7_1_HZ, AD7799_16_7_2_HZ, AD7799_12_5_HZ, AD7799_10_HZ,
       AD7799_8_33_HZ, AD7799_6_25_HZ, AD7799_4_17_HZ };

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

/* setting config, pick gain and chan from below,
   the rest are either zero or non-zero */
enum { AD7799_1_GAIN, AD7799_2_GAIN, AD7799_4_GAIN, AD7799_8_GAIN,
       AD7799_16_GAIN, AD7799_32_GAIN, AD7799_64_GAIN, AD7799_128_GAIN };
enum { AD7799_AIN1_CHAN, AD7799_AIN2_CHAN, AD7799_AIN3_CHAN,
       AD7799_AIN11_CHAN, AD7799_AVDD_CHAN };

inline void ad7799_write_config(uint8_t burnout, uint8_t unipolar, uint8_t gain,
                                uint8_t ref_det, uint8_t buf, uint8_t chan)
{
   ad7799_comm(AD7799_CONFIG_REG, 0, 1);
   spi_transfer((burnout ? 0x40 : 0x00) | (unipolar ? 0x20 : 0x00) | gain);
   spi_transfer((ref_det ? 0x20 : 0x00) | (buf ? 0x10 : 0x00) | chan);
}

/* determine if data is ready to be read, could also be
   implemented by reading the status register */
inline int8_t ad7799_data_ready(void)
{
    //return !(PINB & _BV(DD_MISO));
    return(!digitalRead(12));
    
}

/* request a read from the data register */
void __attribute__((always_inline)) ad7799_request_data(uint8_t continuous)
{
   ad7799_comm(AD7799_DATA_REG, 1, continuous);
}

/* read from data register, it should be previously requested
   from ad7799_request_data, the value is signed */
int32_t __attribute__((always_inline)) ad7799_read_data(void)
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
void setup() {
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  // start the SPI library:
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  SPI.begin();

 Serial.println("Begin");
 digitalWrite(10,LOW);
 
 uint8_t buff[32];
 
 ad7799_init();
 ad7799_calibrate();
 ad7799_set_mode(AD7799_SINGLE_CONVERSION_MODE,0,AD7799_4_17_HZ);
 ad7799_write_config(0,1,AD7799_1_GAIN,1,0,AD7799_AIN1_CHAN);
// uint8_t burnout, uint8_t unipolar, uint8_t gain,uint8_t ref_det, uint8_t buf, uint8_t chan)
 digitalWrite(10,HIGH);
 
ad7799_request_data(0);
ad7799_read_data();

 
 
}

void loop() {
  int data;
  
  digitalWrite(10,LOW);
  ad7799_request_data(0);
  delay(100);
  data=ad7799_read_data();
  digitalWrite(10,HIGH);
  Serial.println(data);
  delay(1000);
}

