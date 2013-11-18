#ifndef AD7799
#define AD7799 


#define AD7799_CS_LOW  digitalWrite(10,LOW);
#define AD7799_CS_HIGH  digitalWrite(10,HIGH);


/*AD7799 Registers*/
#define AD7799_REG_COMM                0 /* Communications Register(WO, 8-bit) */
#define AD7799_REG_STAT            0 /* Status Register            (RO, 8-bit) */
#define AD7799_REG_MODE            1 /* Mode Register                     (RW, 16-bit */
#define AD7799_REG_CONF            2 /* Configuration Register (RW, 16-bit)*/
#define AD7799_REG_DATA            3 /* Data Register                     (RO, 16-/24-bit) */
#define AD7799_REG_ID            4 /* ID Register                     (RO, 8-bit) */
#define AD7799_REG_IO            5 /* IO Register                     (RO, 8-bit) */
#define AD7799_REG_OFFSET   6 /* Offset Register            (RW, 24-bit */
#define AD7799_REG_FULLSALE        7 /* Full-Scale Register        (RW, 24-bit */

/* Communications Register Bit Designations (AD7799_REG_COMM) */
#define AD7799_COMM_WEN                (1 << 7)                         /* Write Enable */
#define AD7799_COMM_WRITE        (0 << 6)                         /* Write Operation */
#define AD7799_COMM_READ    (1 << 6)                         /* Read Operation */
#define AD7799_COMM_ADDR(x)        (((x) & 0x7) << 3)        /* Register Address */
#define AD7799_COMM_CREAD        (1 << 2)                         /* Continuous Read of Data Register */

/* Status Register Bit Designations (AD7799_REG_STAT) */
#define AD7799_STAT_RDY                (1 << 7) /* Ready */
#define AD7799_STAT_ERR                (1 << 6) /* Error (Overrange, Underrange) */
#define AD7799_STAT_CH3                (1 << 2) /* Channel 3 */
#define AD7799_STAT_CH2                (1 << 1) /* Channel 2 */
#define AD7799_STAT_CH1                (1 << 0) /* Channel 1 */

/* Mode Register Bit Designations (AD7799_REG_MODE) */
#define AD7799_MODE_SEL(x)                (((x) & 0x7) << 13)        /* Operation Mode Select */
#define AD7799_MODE_PSW(x)                (1 << 12)                        /* Power Switch Control Bit */        
#define AD7799_MODE_RATE(x)                ((x) & 0xF)                 /* Filter Update Rate Select */

/* AD7799_MODE_SEL(x) options */
#define AD7799_MODE_CONT                 0 /* Continuous Conversion Mode */
#define AD7799_MODE_SINGLE                 1 /* Single Conversion Mode */
#define AD7799_MODE_IDLE                 2 /* Idle Mode */
#define AD7799_MODE_PWRDN                 3 /* Power-Down Mode */
#define AD7799_MODE_CAL_INT_ZERO 4 /* Internal Zero-Scale Calibration */
#define AD7799_MODE_CAL_INT_FULL 5 /* Internal Full-Scale Calibration */
#define AD7799_MODE_CAL_SYS_ZERO 6 /* System Zero-Scale Calibration */
#define AD7799_MODE_CAL_SYS_FULL 7 /* System Full-Scale Calibration */

/* Configuration Register Bit Designations (AD7799_REG_CONF) */
#define AD7799_CONF_BO_EN          (1 << 13)                         /* Burnout Current Enable */
#define AD7799_CONF_UNIPOLAR  (1 << 12)                         /* Unipolar/Bipolar Enable */
#define AD7799_CONF_GAIN(x)          (((x) & 0x7) << 8)         /* Gain Select */
#define AD7799_CONF_REFDET(x) (((x) & 0x1) << 5)         /* Reference detect function */
#define AD7799_CONF_BUF                  (1 << 4)                                 /* Buffered Mode Enable */
#define AD7799_CONF_CHAN(x)          ((x) & 0x7)                         /* Channel select */

/* AD7799_CONF_GAIN(x) options */
#define AD7799_GAIN_1       0
#define AD7799_GAIN_2       1
#define AD7799_GAIN_4       2
#define AD7799_GAIN_8       3
#define AD7799_GAIN_16      4
#define AD7799_GAIN_32      5
#define AD7799_GAIN_64      6
#define AD7799_GAIN_128     7

/* AD7799_CONF_REFDET(x) options */
#define AD7799_REFDET_ENA   1        
#define AD7799_REFDET_DIS   0

/* AD7799_CONF_CHAN(x) options */
#define AD7799_CH_AIN1P_AIN1M        0 /* AIN1(+) - AIN1(-) */
#define AD7799_CH_AIN2P_AIN2M        1 /* AIN2(+) - AIN2(-) */
#define AD7799_CH_AIN3P_AIN3M        2 /* AIN3(+) - AIN3(-) */
#define AD7799_CH_AIN1M_AIN1M        3 /* AIN1(-) - AIN1(-) */
#define AD7799_CH_AVDD_MONITOR        7 /* AVDD Monitor */

/* ID Register Bit Designations (AD7799_REG_ID) */
#define AD7799_ID                        0x9
#define AD7799_ID_MASK                   0xF

/* IO (Excitation Current Sources) Register Bit Designations (AD7799_REG_IO) */
#define AD7799_IOEN                        (1 << 6)
#define AD7799_IO1(x)                (((x) & 0x1) << 4)
#define AD7799_IO2(x)                (((x) & 0x1) << 5)

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
#define AD7799_CONF_CHAN(x)          ((x) & 0x7)                         /* Channel select */


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

 /***************************************************************************//**

 * @brief Initializes the AD7799 and checks if the device is present.

 *

 * @param None.

 *

 * @return status - Result of the initialization procedure.

 *                  Example: 1 - if initialization was successful (ID is 0x0B).

 *                           0 - if initialization was unsuccessful.

*******************************************************************************/

unsigned char AD7799_Init(void);

