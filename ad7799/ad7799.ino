/*
 AD7799 sketch
 Arduino Uno
 */
#include <SPI.h>
#include "AD7799lib.h"

void setup() {
  Serial.begin(9600);
  spi_init();
 
}

void loop() {
 unsigned long value;
 AD7799_Reset();
 delay(200);	
 if(AD7799_Init())	{
   Serial.println("AD7799 OK ");
 }else{
   Serial.println("AD7799 Err ");
   for(;;);		
}	
 AD7799_SetGain(AD7799_GAIN_1);                // set gain to 1
 AD7799_SetChannel(AD7799_CH_AIN1P_AIN1M);     // use AIN1(+) - AIN1(-) 
 AD7799_SetReference(AD7799_REFDET_ENA); 
 AD7799_SetMode(AD7799_MODE_CONT);
 Serial.println("rolling");
delay(200);
 while(1) {
   delay(100);
  if(AD7799_Ready())
        {
            value = AD7799_GetRegisterValue( AD7799_REG_DATA,3);
            Serial.print("Data:");
            if(value > 0x800000) {
                value -= 0x800000;
             value = ((value * 2500) >> 23);
                Serial.print("+");
            } else {
                value = 0x800000 - value;
                value = ((value * 2500) >> 23);
                Serial.print("-");
            }
            Serial.print(value);
            Serial.println(" mV");
	}	 
 
       delay(1000);
 }
 
}

