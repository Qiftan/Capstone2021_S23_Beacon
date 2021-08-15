/* 
Project         : Capstone 2021 S23 Future Of Health
Project Name    : LINKing
Company         : Changi General Hospital and Singapore University Of Technology And Design
File            : Sleep.cpp
Author          : Tan Qi Feng
Date Created    : 15/06/2021
Date Modified   : 15/08/2021
  
*/

#include "Sleep.h"

void Sleep::start(unsigned long SLEEP_MILLISSECONDS) {
    const uint32_t SLEEP_DURATION = SLEEP_MILLISSECONDS * 1000; // µs
	esp_sleep_enable_timer_wakeup(SLEEP_DURATION);
    esp_deep_sleep_start();
    // esp_light_sleep_start();
}

bool Sleep::enable_operations(void){
    return true;
}

bool Sleep::disable_operations(void){
    return true;
}

void Sleep::wake_up(void){
    enable_operations();
    hasSleep = false;
}

void Sleep::sleep(void){
    hasSleep = true;
}

bool Sleep::is_sleep(void){
    return hasSleep;
}


