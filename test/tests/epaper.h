#ifndef TEST_EPAPER_H
#define TEST_EPAPER_H

#include <unity.h>
#include <Arduino.h>
#include "lib/epaper/EPaper.h"

void test_epaper_config(void){
    TEST_ASSERT_EQUAL(0, DEV_Module_Init());
}

int test_epaper_routine1(void){
    epd.put_in_queue('p');
    epd.set_select(0);
    epd.set_state(1);
    int test_done = 0;
    int start_time = millis();
    while(!test_done){

        if((millis()-start_time)>6000){
            test_done = 1;
        }
        
        epd.run();

        if (count_command > count_command_timeout) {
            count_command = 0; 
            if(epd.isEpaperFail()){
                return 1;
            }
        }

        if(count_action>0){
            count_action = 0;
            if(epd.isEpaperProcessing()){
            epd.increment_epaper_action_counter();
            }else{
            epd.clear_epaper_action_counter();
            }
        }

        if (trigger>0) {
            portENTER_CRITICAL_ISR(&timerMux);
            trigger--;
            portEXIT_CRITICAL_ISR(&timerMux);
            count_action++;
            count_command++;
        }
    }
    return 0;
}

int test_epaper_routine2(void){
    epd.put_in_queue('p');
    epd.set_select(1);
    epd.set_state(2);
    int test_done = 0;
    // int start_time = millis();
    while(!test_done){
        
        epd.run();

        if (count_clear > count_clear_timeOut){
            count_clear = 0;
            if(epd.get_state() == 2 && !epd.is_epaper_active()){
                epd.set_state(0);
                test_done = 1;
            }
        }

        if (count_command > count_command_timeout) {
            count_command = 0; 
            if(epd.isEpaperFail()){
                return 1;
            }
        }

        if(count_action>0){
            count_action = 0;
            if(epd.isEpaperProcessing()){
            epd.increment_epaper_action_counter();
            }else{
            epd.clear_epaper_action_counter();
            }
        }

        if (trigger>0) {
            portENTER_CRITICAL_ISR(&timerMux);
            trigger--;
            portEXIT_CRITICAL_ISR(&timerMux);
            count_action++;
            count_command++;
            if(epd.get_state() == 2){
                count_clear++;
            }
        }
    }
    return 0;
}

void test_epaper(void){
    TEST_ASSERT_EQUAL(0, test_epaper_routine1());
    TEST_ASSERT_EQUAL(0, test_epaper_routine2());
}

#endif