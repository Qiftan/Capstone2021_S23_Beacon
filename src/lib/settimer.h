/* 
Project         : Capstone 2021 S23 Future Of Health
Project Name    : LINKing
Company         : Changi General Hospital and Singapore University Of Technology And Design
File            : settimer.h
Author          : Tan Qi Feng
Date Created    : 15/06/2021
Date Modified   : 15/08/2021
  
*/

#ifndef _SETTIMER_H_
#define _SETTIMER_H_

/* Timer */
int count_action;
int count_command;
int count_clear;
int count_battery;
int count_waiting;
volatile int trigger = 0;    // Trigger 
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
// Code with critica section
void IRAM_ATTR onTime() {
   portENTER_CRITICAL_ISR(&timerMux);
   trigger++;
   portEXIT_CRITICAL_ISR(&timerMux);
}

int set_one_second_timer(void){
    // Configure the Prescaler at 80 the quarter of the ESP32 is cadence at 80Mhz
    // 80000000 / 80 = 1000000 tics / seconde
    timer = timerBegin(0, 80, true);                
    timerAttachInterrupt(timer, &onTime, true);    

    // Sets an alarm to sound every second -> 50ms
    // 1 sec -> 2000000
    // 1 ms  -> 2000
    // 50ms  -> 2000*50 = 100,000
    timerAlarmWrite(timer, 2000000, true);         
    timerAlarmEnable(timer);
    return 0;
}

#endif