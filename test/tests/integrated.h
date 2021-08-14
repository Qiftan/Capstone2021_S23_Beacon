#ifndef TEST_INTEGRATED_H
#define TEST_INTEGRATED_H

#include <unity.h>
#include <Arduino.h>
#include "lib/led.h"
#include "lib/ble.h"
#include "lib/buzzer.h"
#include "lib/epaper/EPaper.h"

int test_integrated_routine(void){
    int test_done = 0;
    int test_start = 0;
    while(!test_done){
        // if epaper is idling, 
        // initiate epaper by clearing the screen, then display the defualt page.
        if(epd.get_state() == 0 && !epd.is_epaper_active()){
            // epd.put_in_queue('c');
            epd.put_in_queue('p');
            epd.set_select(0);
            epd.set_state(1);
            if(led.get_state()==1){
                led.set_state(2);
                test_start = 1;
            }
        }

        if(led.get_state()==0&&test_start==1){
            test_done = 1;
        }

        // if epaper is initiated, 
        // wait for user to approach the beacon
        // if BLE connection made (user approach), display the welcome page and play welcome tone. 
        if(epd.get_state() == 1 && !epd.is_epaper_active()){
            is_waiting_for_user = true;
            if (deviceConnectedBool) {
                count_waiting = 0; 
                is_waiting_for_user = false;
                if(rx_cmd==0){
                    start_buzzer = 1;
                    led.start_led = 1;        
                    epd.put_in_queue('p');
                    epd.set_select(1);
                    epd.set_state(2);
                }
            }
        }

        if((led.get_state()==0 || led.get_state()==2) && led.start_led){
            led.start_led = 0;
            led.start(); 
        }

        // Save the image transfer over the BLE from the app
        // if image is tranferred over BLE, set the save to happen.
        if(is_rx_completed==2){
            is_rx_completed = 0;
            switch (rx_cmd){
            case 1:
            Serial.println("resetting image...");
            epd.set_is_change_image(0);
            break;
            case 2:
            case 4:
            Serial.println("show changed image...");
            epd.set_is_change_image(1);
            break;
            }
            rx_cmd = 0;
        }
        
        // Run Epaper functions
        epd.run();

        // Play the welcome tune when BLE is connected together with the epaper
        if(start_buzzer == 1){
            start_buzzer = 0;
            play_tone();
        }

        // Handles the LEDs operation when BLE is connected.
        led.handle2();

        // timeout to re-initiate the epaper screen after the displaying the welcome page
        if (count_clear > count_clear_timeOut){
            count_clear = 0;
            if(epd.get_state() == 2 && !epd.is_epaper_active()){
                epd.set_state(0);
            }
        }

        // Check on every timeout if the epaper is able to work,
        // if epaper fail or non-exist, try re-initiate epaper.
        // every 10 seconds
        if (count_command > count_command_timeout) {
            count_command = 0; 
            // epd.print_current_command();
            if(epd.isEpaperFail()){
                epd.set_state(0);
                return 1;
            }
        }

        // Increment the action counter of the epaper
        // or clear the action counter 
        // every second
        if(count_action>0){
            count_action = 0;
            if(epd.isEpaperProcessing()){
                epd.increment_epaper_action_counter();
            }else{
                epd.clear_epaper_action_counter();
            }
        }

        // read battery level at set intervals
        //every 5 minutes
        if (count_battery>count_battery_timeout){
            battery_level = get_battery_level();
            pCharacteristic -> setValue(update_ble_msg(String(battery_level)));
            count_battery = 0;
        }

        // Time interrupt trigger counter : 1 second
        if (trigger>0) {
            
            portENTER_CRITICAL_ISR(&timerMux);
            trigger--;
            portEXIT_CRITICAL_ISR(&timerMux);

            count_action++;
            count_command++;
            count_battery++;

            if(epd.get_state() == 2){
                count_clear++;
            }

            if(is_waiting_for_user){
                count_waiting++;
            }
        }
    }
    return 0;
}

void test_integrated(void){
    TEST_ASSERT_EQUAL(0, test_integrated_routine());
}

#endif