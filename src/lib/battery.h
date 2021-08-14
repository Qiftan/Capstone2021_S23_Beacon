#ifndef _BATTERY_H_
#define _BATTERY_H_

#include "lib/ESP32AnalogRead.h"

/* Battery Level */
double battery_level = 0.0;
ESP32AnalogRead adc;

double get_battery_level(void){
    return adc.readVoltage() * 2.0;
}

#endif