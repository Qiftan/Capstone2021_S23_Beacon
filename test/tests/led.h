#ifndef TEST_LED_H
#define TEST_LED_H

#include <unity.h>
#include <Arduino.h>
#include "lib/led.h"

int test_led_routine(void){
    led.start();
    int led_test_done = 0;
    int start_time = millis();
    while(!led_test_done){
        if((millis()-start_time)>100000){
            return 1;
        }
        if((millis()-start_time)>20000){
            led.set_state(2);
        }
        led.handle2();
        if(led.get_state()==0){
            led_test_done = 1;
        }
    }
    return 0;
}

void test_led(void){
    TEST_ASSERT_EQUAL(0, led.init());
    TEST_ASSERT_EQUAL(0, test_led_routine());
}


#endif