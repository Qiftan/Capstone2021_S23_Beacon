/* 
Author  : Tan Qi Feng
Date    : 15/06/2021
Description:
  Updated on 15/07/2021
  Updated on 13/07/2021
  
*/

#include "main.h"

/******************************************************************************
Setup
******************************************************************************/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(serial_speed);
  Serial.print("main is running on: ");
  Serial.println(xPortGetCoreID());
  set_cpu_freq();

  // Battery setup
  adc.attach(adc_pin);
  battery_level = get_battery_level();

  ble_init();

  // E-paper setup
  DEV_Module_Init();
  // EPD_7IN5BC_Init();
  // EPD_7IN5B_V2_Init();
  // EPD_7IN5_V2_Init();
  
  //set one second timer interrupt
  set_one_second_timer();

  // LED setup
  led.init();

}

/******************************************************************************
Loop
******************************************************************************/
void loop() {

  // if epaper is idling, 
  // initiate epaper by clearing the screen, then display the defualt page.
  if(epd.get_state() == 0 && !epd.is_epaper_active()){
    // epd.put_in_queue('c');
    epd.put_in_queue('p');
    epd.set_select(0);
    epd.set_state(1);
    if(led.get_state()==1){
      led.set_state(2);
    }
  }

  // if epaper is initiated, 
  // wait for user to approach the beacon
  // if BLE connection made (user approach), display the welcome page and play welcome tone. 
  if(epd.get_state() == 1 && !epd.is_epaper_active()){
    is_waiting_for_user = true;
    if (deviceConnectedBool) {
      count_waiting = 0; 
      is_waiting_for_user = false;
      // deviceConnectedBool = false;
      if(rx_cmd==0){
        start_buzzer = 1;
        led.start_led = 1;        
        epd.put_in_queue('p');
        epd.set_select(1);
        epd.set_state(2);
      }
      // createBuzzerTask();
      // led.start();
    }
  }

  // if BLE connected and LED operation not activatied, do start led operation.
  // if(led.get_state()==0 && led.start_led){
  //   led.start_led = 0;
  //   led.start();
  // }

  if((led.get_state()==0 || led.get_state()==2) && led.start_led){
    led.start_led = 0;
    led.start(); 
  }
           
  // when epaper is initiate and in-active,
  // Beacon go to sleep at set intervals
  // Beacon wakes after sleep duration. 
  // if(count_waiting>count_waiting_timeout){
  //   count_waiting = 0;
  //   Serial.println("going to sleep...");
  //   delay(100);
  //   light_sleep.start(sleep_timeout-100);
  //   Serial.println("waking up...");
  // }

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
    createBuzzerTask(); 
    // play_tone();
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
    epd.print_current_command();
    if(epd.isEpaperFail()){
      epd.set_state(0);
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

    // print_cpu_freq();

  }

}
