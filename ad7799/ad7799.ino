/*
 AD7799 sketch
 Arduino Uno
 */
#include <SPI.h>
#include "AD7799lib.h"

void setup() {
  Serial.begin(9600);
  spi_init();
 Serial.println("Begin");
 ad7799_init();
 Serial.print("device:");Serial.println(ad7799_get_ID());//should return 7799
 Serial.print("mode:");Serial.println(ad7799_read_mode(),HEX);//mode after reset is 0x0A

ad7799_set_channel(1);
//ad7799_set_gain();
//ad7799_read_input();

 
// ad7799_init();
// ad7799_calibrate();
// ad7799_set_mode(AD7799_SINGLE_CONVERSION_MODE,0,AD7799_4_17_HZ);
// ad7799_write_config(0,1,AD7799_1_GAIN,1,0,AD7799_AIN1_CHAN);
//// uint8_t burnout, uint8_t unipolar, uint8_t gain,uint8_t ref_det, uint8_t buf, uint8_t chan)
// digitalWrite(10,HIGH);
// 
//ad7799_request_data(0);
//ad7799_read_data();


}

void loop() {
//  long data;
//  
//  digitalWrite(10,LOW);
//  ad7799_request_data(0);
//  delay(100);
//  data=ad7799_read_data();
//  digitalWrite(10,HIGH);
//  Serial.println(data);
//  delay(1000);
}

