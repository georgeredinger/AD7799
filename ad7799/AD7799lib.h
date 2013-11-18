#ifndef AD7799
#define AD7799 

#define AD7799_STATUS_RDY       0x80
#define AD7799_STATUS_ERROR     0x40
#define AD7799_STATUS_NOREF     0x20
#define AD7799_STATUS_IS_AD7799 0x08
#define AD7799_STATUS_CHAN_MASK 0x07

extern uint8_t ad7799_init_status;

#define AD7799_STATUS_REG 0x0
#define AD7799_MODE_REG   0x1
#define AD7799_CONFIG_REG 0x2
#define AD7799_DATA_REG   0x3
#define AD7799_ID_REG     0x4
#define AD7799_IO_REG     0x5
#define AD7799_OFFSET_REG 0x6
#define AD7799_SCALE_REG  0x7


#define CS 10

enum { AD7799_CONTINUOUS_CONVERSION_MODE = 0,  AD7799_SINGLE_CONVERSION_MODE,
       AD7799_IDLE_MODE, AD7799_POWERDOWN_MODE, AD7799_INTERNAL_OFFSET_CAL_MODE,
       AD7799_INTERNAL_SCALE_CAL_MODE,  AD7799_SYSTEM_OFFSET_CAL_MODE,
       AD7799_SYSTEM_SCALE_CAL_MODE};
       
enum { AD7799_470_HZ = 1, AD7799_242_HZ, AD7799_123_HZ, AD7799_62_HZ,
       AD7799_50_HZ, AD7799_39_HZ, AD7799_33_2_HZ, AD7799_19_6_HZ,
       AD7799_16_7_1_HZ, AD7799_16_7_2_HZ, AD7799_12_5_HZ, AD7799_10_HZ,
       AD7799_8_33_HZ, AD7799_6_25_HZ, AD7799_4_17_HZ };
       
enum { AD7799_1_GAIN, AD7799_2_GAIN, AD7799_4_GAIN, AD7799_8_GAIN,
       AD7799_16_GAIN, AD7799_32_GAIN, AD7799_64_GAIN, AD7799_128_GAIN };
enum { AD7799_AIN1_CHAN, AD7799_AIN2_CHAN, AD7799_AIN3_CHAN,
       AD7799_AIN11_CHAN, AD7799_AVDD_CHAN };

#endif

void spi_init();
void ad7799_reset(void);
unsigned char spi_transfer(int address);
void ad7799_comm(uint8_t reg, uint8_t read, uint8_t cont);

void ad7799_write_config(uint8_t burnout, uint8_t unipolar, uint8_t gain,
                                uint8_t ref_det, uint8_t buf, uint8_t chan);

/* determine if data is ready to be read, could also be
   implemented by reading the status register */
int8_t ad7799_data_ready(void);

void ad7799_set_channel(int channel); 

/* request a read from the data register */
void  ad7799_request_data(uint8_t continuous);

/* read from data register, it should be previously requested
   from ad7799_request_data, the value is signed */
int32_t  ad7799_read_data(void);

int32_t ad7799_read_offset(void);
int32_t ad7799_read_scale(void);
uint8_t ad7799_status(void);


void ad7799_set_mode(uint8_t mode, uint8_t psw, uint8_t rate);
void ad7799_get_mode(uint8_t *mode, uint8_t *psw, uint8_t *rate);

       
void ad7799_calibrate();
void ad7799_init();
int ad7799_get_ID();
unsigned int ad7799_read_mode();
unsigned int ad7799_write_mode();

 
