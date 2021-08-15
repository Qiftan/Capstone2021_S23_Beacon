/* 
Project         : Capstone 2021 S23 Future Of Health
Project Name    : LINKing
Company         : Changi General Hospital and Singapore University Of Technology And Design
File            : main.cpp
Author          : Tan Qi Feng
Date Created    : 15/06/2021
Date Modified   : 15/08/2021
  
*/

// Uncomment the following line when executing the test task in PlatformIO
// #define TEST true

#ifdef TEST
//Test Enabled
#else
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
  Serial.println("CPU speed set!");
  
  adc.attach(adc_pin);
  battery_level = get_battery_level();
  Serial.println("Tested ADC battery read!");
  
  ble_init();
  Serial.println("BLE Initiated!");

  DEV_Module_Init();
  Serial.println("E-paper GPIO Configured!");
  
  set_one_second_timer();
  Serial.println("One second interrupt timer set!");

  led.init();
  Serial.println("LED Initiated!");
}

/******************************************************************************
Loop
******************************************************************************/
void loop() {
  // if epaper is idling, 
  // initiate epaper by displaying the default page (image select = 0, epaper state = 1).
  if(epd.get_state() == 0 && !epd.is_epaper_active()){
    epd.put_in_queue('p');
    epd.set_select(0);
    epd.set_state(1);
    // epaper is returning from the welcome page to default page
    // set the led state = 2
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
      if(rx_cmd==0){
        start_buzzer = 1;
        led.start_led = 1;        
        epd.put_in_queue('p');
        epd.set_select(1);
        epd.set_state(2);
      }
    }
  }

  // if led routine enabled,do start led operation.
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

  // Play the welcome tune when buzzer routine is enabled
  if(start_buzzer == 1){
    start_buzzer = 0;
    play_tone();
    Serial.println("Completed Buzzer!");
  }

  // Handles the LEDs operation when LED routine is enabled
  led.handle2();

  // timeout to re-initiate the epaper screen after displaying the welcome page
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
  // every 5 minutes
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
#endif
