/* 
Project         : Capstone 2021 S23 Future Of Health
Project Name    : LINKing
Company         : Changi General Hospital and Singapore University Of Technology And Design
File            : ble.h
Author          : Tan Qi Feng
Date Created    : 15/06/2021
Date Modified   : 15/08/2021
  
*/

#ifndef _BLE_H_
#define _BLE_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "lib/util.h"

#define SERVICE_UUID        "b6e4af9e-e48a-11eb-ba80-0242ac130004" 
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define MTU_SIZE 517
#define MSG_FRONT_SIZE 4

int is_ble_init = 0;
bool is_waiting_for_user = false;
int deviceConnected = 0;
bool deviceConnectedBool = false;
int oldDeviceConnected = 0;
BLEServer *pServer;
BLEService *pService;
BLECharacteristic *pCharacteristic;
BLEAdvertising *pAdvertising;
std::string msg = "BeaconS23 Batt ";
std::string rx_test_msg = "61"; // Identifier to test against with data from the app to authenticate the app data
int rx_cmd = 0;
unsigned long start_time = 0;
int count_rx = 0;
int is_rx_completed = 0;
unsigned long val_size = 0;


std::__cxx11::string update_ble_msg(String m){
  msg = "BeaconS23 Batt ";
  msg += m.c_str();
  return msg;
}

// void set_adv_interval(int speed){
//   switch(speed){
//     case 0:
//     // if(rx_msg.size()>2){
//       // pAdvertising->setMaxInterval(0x0010); // 40 ms interval 
//       // pAdvertising->setMinInterval(0x0002);
//       // pAdvertising->setMaxInterval(0x0040); // 100 ms interval 
//       // pAdvertising->setMinInterval(0x0020);
//       // pAdvertising->setMaxInterval(0x0190); // 270 ms interval
//       // pAdvertising->setMinInterval(0x015E);
//       pAdvertising->setMaxInterval(0x0290); // 410 ms interval 
//       pAdvertising->setMinInterval(0x025E);
//     // }
//     break;
//     case 1:
//     pAdvertising->setMaxInterval(0x0590); // 800 ms interval 
//     pAdvertising->setMinInterval(0x055E);
//   }
// }

/**
 *  Function  :   void ble_decode_rx_msg(std::string value)
 *  Purpose   :   Decode the received characteristic by
 *                converting the each character in a string 
 *                into single char and save into a predefined 
 *                saveImage[48000] variable. 
**/
void ble_decode_rx_msg(std::string value){
 unsigned long size = value.size();
  // Serial.println(val_size);
  /* 
    If received msg has length larger than 2 bytes
    -> then decode the msg
  */
  if(size > 2){
    
    is_rx_completed = 0;
    int rx_size = int(convert_string_to_char(value.substr(MSG_FRONT_SIZE-2,1),1));
  
    rx_cmd = int(convert_string_to_char(value.substr(MSG_FRONT_SIZE-1,1),1));

    Serial.print("rx_size: ");Serial.print(rx_size);Serial.print("\t");
    Serial.print("time: ");Serial.print(millis()-start_time);Serial.print(" ms");Serial.print("\t");
    Serial.print("Command: ");Serial.println(rx_cmd);

    /** Received command 
     *  1 -> reset the image to default
     *  2 -> show the image that was sent over
     *  3 -> send image over but do not change the image display
     *  4 -> send and change the image
    **/
    switch(rx_cmd){
      case 1:
      // Serial.println("Resetting the image...");
      break;
      case 2:
      // Serial.println("show changed image...");
      break;
      case 3:
      case 4:
      unsigned long size_b = size - MSG_FRONT_SIZE;
      val_size += size_b;
      Serial.println("Saving the image...");
      int j = MSG_FRONT_SIZE;
      for (int i=count_rx*size_b; i < count_rx*size_b+size_b; i++){
        saveImage1[i] = convert_string_to_char(value.substr(j,1),1);
        j++;
      }
      break;
    }

    count_rx++;
    
    /* 
      if all message to be send from app have completed based on the number of Bytes
      specified in the third byte of the whole received bytes per transfer.
    */
    if (count_rx >= rx_size - 1){
      is_rx_completed = 1;
    } 

    /* 
      if the recieved command is invalid
    */
    if(rx_cmd>5){
      rx_cmd = 0;
      is_rx_completed = 0;
    }

  }else{
    /* 
      No message bytes send over
      Only the 2 bytes that represent the identifier of the app.
    */
    count_rx = 0;
  }
}

void ble_onReceived_rx_msg(void){
  if (is_rx_completed == 1){
    is_rx_completed = 2;   
    Serial.print("Completed Transfered! ");
    Serial.print("Message size: ");
    Serial.print(val_size);
    Serial.print("\tDuration: ");
    Serial.print(millis()-start_time);
    Serial.println(" ms");
    val_size = 0;
  }
}

void ble_onStartReceived(void){
  start_time = millis();
  Serial.print("Started receiving... time: ");
  Serial.println(start_time);
}

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) 
    { 
      // set_adv_interval(1);
      deviceConnected++;
      BLEDevice::startAdvertising();
      Serial.print("Device connected. Now there are ");
      Serial.print(deviceConnected);
      Serial.println(" devices connected.");
      ble_onStartReceived();
      
    };

    void onDisconnect(BLEServer* pServer) 
    {
      ble_onReceived_rx_msg();
      deviceConnectedBool = false;
      // set_adv_interval(0);
      deviceConnected--;
      pServer->startAdvertising(); // restart advertising
      Serial.print("Device disconnected. Now there are ");
      Serial.print(deviceConnected);
      Serial.println(" devices connected.");
    };
};

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic -> getValue();

    pCharacteristic -> setValue(update_ble_msg(String(battery_level)));
   
    ble_decode_rx_msg(value);

    /* 
      the identifier from the app matched
      -> then authentication successful
      -> then set connection true.
    */
    if(value.substr(0,2) == rx_test_msg){
      deviceConnectedBool = true;
    }
  }
};

int ble_init(void){
  is_ble_init = 1;
  // Serial.println("BLE Initiated!");
  BLEDevice::init("BeaconS23");
  BLEDevice::setPower(ESP_PWR_LVL_N14);
  // BLEDevice::setMTU(MTU_SIZE);
  pServer = BLEDevice::createServer();
  pService = pServer -> createService(SERVICE_UUID);
  pCharacteristic = pService -> createCharacteristic(
    CHARACTERISTIC_UUID, 
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
  );
  pCharacteristic -> setCallbacks(new MyCallbacks);
  pServer->setCallbacks(new MyServerCallbacks());
  
  pCharacteristic -> setValue(update_ble_msg(String(battery_level)));
  
  pService -> start();
  pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  // pAdvertising->setMaxInterval(0x0190); // 270 ms interval
  // pAdvertising->setMinInterval(0x015E);
  // pAdvertising->setMaxInterval(0x0290); // 410 ms interval 
  // pAdvertising->setMinInterval(0x025E);
  // pAdvertising->setMaxInterval(0x0390); // 570 ms interval 
  // pAdvertising->setMinInterval(0x035E);
  // pAdvertising->setMaxInterval(0x0490); // 670 ms interval 
  // pAdvertising->setMinInterval(0x045E);
  pAdvertising->setMaxInterval(0x0590); // 800 ms interval set maximum advertising interval
  pAdvertising->setMinInterval(0x055E); //                 set minimum advertising interval
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  return 0;
}

void ble_deinit(){
  Serial.println("Stopping BLE work!");
  is_ble_init = 0;
  BLEDevice::deinit(false);
}

#endif