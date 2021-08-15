/* 
Project         : Capstone 2021 S23 Future Of Health
Project Name    : LINKing
Company         : Changi General Hospital and Singapore University Of Technology And Design
File            : battery.h
Author          : Tan Qi Feng
Date Created    : 15/06/2021
Date Modified   : 15/08/2021
  
*/


#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "lib/ESP32AnalogRead.h"

/* Battery Level */
double battery_level = 0.0;
ESP32AnalogRead adc;

double get_battery_level(void){
    return adc.readVoltage() * 2.0; // the battery level is double as specified by Adafruit.
}

#endif