#ifndef _LED_H_
#define _LED_H_

/* LED Strip */
#include "Adafruit_NeoPixel.h"
/* LED Strip defines */
#define PIN        4 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 10 // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define count_led_timeout 100//750 //340
#define count_led_done_timeout 2
// #define DELAYVAL 500 // Time (in milliseconds) to pause between pixels



class LED {
private:
    unsigned long start_time = 0;
    int count_led = 0;
    int start_bool = 0;
    int count_led_done = 0;
    int led_direction = 0;
    int led_pos = 0;
    int led_state = 0;
    int count_led_0 = 0;
    
public:
    
    int start_led = 0;

    int init(void){
        /* LED Strip setup */
        pixels.setBrightness(100);
        pixels.begin(); // INITIALIZE NeoPixel strip object 
        pixels.clear();
        pixels.show();
        Serial.println("LED initiated");
        return 0;
    }

    void set(int i){
        pixels.setPixelColor(i, pixels.Color(100, 100, 100)); // can go up to 255
        pixels.show();   // Send the updated pixel colors to the hardware.
    }

    void handle(void){
        if (start_bool==1){
            unsigned long time = millis();
            if(time-start_time >= 100){
                if(count_led < NUMPIXELS){
                    set(led_pos); 
                    if (led_direction==0){
                        led_pos++;
                    }else{
                        led_pos--;
                    }
                }
                start_time = time;
                count_led++;
                if(count_led > NUMPIXELS && count_led_done <= count_led_done_timeout){
                    pixels.clear();
                    pixels.show();
                    count_led = 0;
                    count_led_done++;
                    if (led_direction==0){
                        led_direction = 1;
                        led_pos = NUMPIXELS - 1;
                    }else{
                        led_direction = 0;
                        led_pos = 0;
                    }
                }
            }
            if(count_led > (NUMPIXELS + count_led_timeout)){
                stop();
            }
        }
    }

    int handle2(void){
        unsigned long time = millis();
        switch(led_state){
            case 1:
            if(time-start_time >= 100){
                if(count_led < NUMPIXELS){
                    set(led_pos); 
                    if (led_direction==0){
                        led_pos++;
                    }else{
                        led_pos--;
                    }
                }
                start_time = time;
                count_led++;
                if(count_led > NUMPIXELS && count_led_done <= count_led_done_timeout){
                    pixels.clear();
                    pixels.show();
                    count_led = 0;
                    count_led_done++;
                    if (led_direction==0){
                        led_direction = 1;
                        led_pos = NUMPIXELS - 1;
                    }else{
                        led_direction = 0;
                        led_pos = 0;
                    }
                }
            }
            break;
            case 2:
            if(time-start_time >= 100){
                if(count_led_0 > count_led_timeout){
                    stop();
                }
                count_led_0++;
                start_time = millis();
            }
            break;
        }
        return 0;
    }

    int get_state(void){
        return led_state;
        // return start_bool;
    }

    void set_state(int state){
        led_state = state;
    }

    void start(void){
        Serial.println("Started LED");
        start_bool = 1;
        led_state = 1;
        count_led = 0;
        led_direction = 0 ;
        led_pos = 0;
        count_led = 0;
        count_led_done = 0;
        count_led_0 = 0;
    }

    void stop(void){
        /* LED Strip */
        pixels.clear(); // Set all pixel colors to 'off'
        pixels.show();
        Serial.println("Stopped LED");
        start_bool = 0;
        count_led = 0;
        led_state = 0;
    }

};


#endif