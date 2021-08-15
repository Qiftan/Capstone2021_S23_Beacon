/* 
Project         : Capstone 2021 S23 Future Of Health
Project Name    : LINKing
Company         : Changi General Hospital and Singapore University Of Technology And Design
File            : main.h
Author          : Tan Qi Feng
Date Created    : 15/06/2021
Date Modified   : 15/08/2021
  
*/

#ifndef _MAIN_H_
#define _MAIN_H_

#include <Arduino.h>

#include "lib/epaper/EPaper.h"
#include <lib/battery.h>
#include <lib/buzzer.h>
#include <lib/Sleep.h>
#include <lib/settimer.h>
#include <lib/ble.h>
#include <lib/led.h>

Sleep light_sleep;
EpaperDriver epd;
LED led;

//debug: false  -> disable all serial print
//debug: true   -> enable all serial print
#define DEBUG false
#define Serial if(DEBUG)Serial

#define serial_speed 115200
#define adc_pin 35
#define count_clear_timeOut 30 //15
#define count_command_timeout 10
#define count_battery_timeout 300
#define count_waiting_timeout 3
#define sleep_timeout 1000
#define cpu_frequency_mhz 80

void print_cpu_freq(void){
  int Freq = getCpuFrequencyMhz();
  Serial.print("CPU Freq = ");
  Serial.print(Freq);
  Serial.println(" MHz");
}

void set_cpu_freq(void){
  setCpuFrequencyMhz(cpu_frequency_mhz);
  print_cpu_freq();
}

#endif