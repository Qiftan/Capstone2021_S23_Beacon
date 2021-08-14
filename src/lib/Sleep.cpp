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


