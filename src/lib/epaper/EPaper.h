#ifndef _EPAPER_H_
#define _EPAPER_H_

#include <Arduino.h>
/* E-paper */
#include "assets/ImageData.h"
// #include "stdlib.h"
#include "utility/Debug.h"
#include "DEV_Config.h"
#include "EPD.h"
// #include "string"

class EpaperDriver {
    private:
        bool is_e_paper_exist = true;
        bool is_e_paper_initiated = false;
        bool is_e_paper_clear = false;
        bool is_e_paper_successful = false;
        bool is_e_paper_cmd_issued = false;
        bool is_display_shown = false;
        String epaper_state = "idle";
        int epaper_state_int = 0;
        unsigned long timeOut = 30;
        char epaper_command = 'r';
        char e_paper_cmd = 0;
        int epaper_action_counter = 0;
        String s = "running";
        int epaper_img_select = 0;
        String queue = "";
        int is_change_image = 0;

        int debug_start_time = 0;

    public:
        void handlingEpaperDisplay(void){
            // if(!EPD_7IN5BC_isBusy()){
            // if(!EPD_7IN5B_V2_isBusy()){
            if(!EPD_7IN5_V2_isBusy()){
                switch (e_paper_cmd){
                case 1:
                // EPD_7IN5BC_TurnOnDisplay_1();
                // EPD_7IN5B_V2_TurnOnDisplay_1();
                EPD_7IN5_V2_TurnOnDisplay_1();
                e_paper_cmd = 2;
                break;
                case 2:
                // EPD_7IN5BC_TurnOnDisplay_2();
                // EPD_7IN5B_V2_TurnOnDisplay_2();
                EPD_7IN5_V2_TurnOnDisplay_2();
                e_paper_cmd = 0;
                break;
                case 0:
                is_e_paper_successful = true;
                is_e_paper_cmd_issued = false;
                epaper_command = 'r';
                Serial.println("e_paper: running!");
                if(!is_epaper_active()){
                    Serial.println("e_paper: sleeping!");
                    // EPD_7IN5B_V2_Sleep();
                    EPD_7IN5_V2_Sleep();
                    is_e_paper_initiated = false;
                }
                break;
                }
                // EPD_7IN5B_V2_TurnOnDisplay_1();
                // e_paper_cmd = 0;
                // is_e_paper_successful = true;
                // is_e_paper_cmd_issued = false;
                // epaper_command = 'r';
                // Serial.println("e_paper: running!");
            }else{
                if (epaper_action_counter>timeOut){
                    is_e_paper_successful = false;
                    is_e_paper_cmd_issued = false;
                    epaper_command = 'f';
                    Serial.println("e_paper: fail!");
                    // EPD_7IN5B_V2_Sleep();
                    EPD_7IN5_V2_Sleep();
                    is_e_paper_initiated = false;
                }
            }
        }
        void epaper_clearDisplay(void){
            if(!is_e_paper_cmd_issued){
                Serial.println("Clearing epaper...");
                delay(500);
                // EPD_7IN5BC_Clear();
                // EPD_7IN5B_V2_Clear();
                EPD_7IN5_V2_Clear();
                e_paper_cmd = 1;
                is_e_paper_cmd_issued = true;
            }else{
                handlingEpaperDisplay();
            }
        }
        void epaper_printDisplay(char imgSelect){
            if(!is_e_paper_cmd_issued){
                Serial.println("printing epaper...");
                delay(3);
                // EPD_7IN5BC_Display(pikachu, heart);
                switch(imgSelect){
                    case 0:  
                    // EPD_7IN5BC_Display(logo, black);
                    // EPD_7IN5B_V2_Display(logo,black);
                    EPD_7IN5_V2_Display(logo);
                    break;
                    case 1:  
                    // EPD_7IN5BC_Display(welcomeplant, base);
                    // EPD_7IN5BC_Display(fruittree, black);
                    // EPD_7IN5B_V2_Display(garden,black);
                    // EPD_7IN5_V2_Display(garden);
                    if (is_change_image==1){
                        EPD_7IN5_V2_Display(saveImage1);
                        // EPD_7IN5B_V2_Display(saveImage1, black);
                    }else{
                        // EPD_7IN5_V2_Display(campus);
                        // EPD_7IN5B_V2_Display(terry,black);
                        EPD_7IN5_V2_Display(opera);
                        // EPD_7IN5_V2_Display(fruittree);
                        // EPD_7IN5_V2_Display(garden);
                    }
                    break;
                }
                e_paper_cmd = 1;
                is_e_paper_cmd_issued = true;
            }else{
                handlingEpaperDisplay();
            }
        }

        // bool isEpaperSuccessful(void){
        //     return is_e_paper_successful;
        // }

        // bool isEpaperReady(void){
        //     return epaper_command == 'r';
        // }

        void set_is_change_image(int n){
            is_change_image = n;
        }
        int get_is_change_image(void){
            return is_change_image;
        }

        bool isEpaperFail(void){
            return epaper_command == 'f';
        }
        bool isEpaperProcessing(void){
            return !(epaper_command == 'f' || epaper_command == 'r');
        }
        bool isEpaperInitiated(void){
            return is_e_paper_initiated;
        }
        // bool isDisplayShown(void){
        //     return is_display_shown;
        // }

        void put_in_queue(char cmd){
            queue += String(cmd);
        }

        bool is_epaper_active(void){
            return queue.length()>0;
        }

        void set_state(int state){
            epaper_state_int = state;
            // if (epaper_state_int == 0){
            //     epaper_state = "idle";
            // }
            // else if (epaper_state_int == 1){
            //     epaper_state = "initiated";
            // }
            // else if (epaper_state_int == 2){
            //     epaper_state = "displaying";
            // }
        }
        int get_state(void){
            return epaper_state_int;
        }

        void set_command(char cmd){
            epaper_command = cmd;
        }
        char get_current_command(void){
            return epaper_command;
        }

        void set_select(int select){
            epaper_img_select = select;
        }
        int get_selected(void){
            return epaper_img_select;
        }

        void set_epaper_initiated(bool b){
            is_e_paper_initiated = b;
        }
        // void set_epaper_display_shown(bool b){
        //     is_display_shown = b;
        // }
        void increment_epaper_action_counter(void){
            epaper_action_counter++;
        }
        void clear_epaper_action_counter(void){
            epaper_action_counter = 0;
        }
        void print_current_command(void){
            Serial.print("Current epaper_command: '");
            Serial.print(get_current_command());
            Serial.println("'");
        }

        void run(void){
           
            if(!isEpaperProcessing() && is_epaper_active()){
                if(!is_e_paper_initiated){
                    // EPD_7IN5B_V2_Init();
                    EPD_7IN5_V2_Init();
                    is_e_paper_initiated = true;
                }
                // Serial.println(!isEpaperProcessing());
                set_command(queue[0]);
                // Serial.println(String(get_current_command()));
                queue.remove(0, 1);
            }
            switch(get_current_command()){
                case 'c': 
                epaper_clearDisplay();
                break;
                case 'p': 
                epaper_printDisplay(get_selected());
                break;
            }
        }
};

#endif