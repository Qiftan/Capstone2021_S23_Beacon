#ifndef _BLE_H_
#define _BLE_H_

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
// #include <stdlib.h>
#include "lib/util.h"

/***
  "b6e4af9e-e48a-11eb-ba80-0242ac130004"  NA20 Fruit Tree
  "c0b9a99a-e488-11eb-ba80-0242ac130004"  NA12 Com Garden
  "71e81e3a-e48a-11eb-ba80-0242ac130004"  G2 Dog Run
***/

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
std::string rx_test_msg = "61";
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

void ble_decode_rx_msg(std::string value){
 unsigned long size = value.size();
  // Serial.println(val_size);
  if( size > 2){
    
    is_rx_completed = 0;
    int rx_size = int(convert_string_to_char(value.substr(MSG_FRONT_SIZE-2,1),1));
  
    rx_cmd = int(convert_string_to_char(value.substr(MSG_FRONT_SIZE-1,1),1));

    Serial.print("rx_size: ");Serial.print(rx_size);Serial.print("\t");
    Serial.print("time: ");Serial.print(millis()-start_time);Serial.print(" ms");Serial.print("\t");
    Serial.print("Command: ");Serial.println(rx_cmd);


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
    
    if (count_rx >= rx_size - 1){
      is_rx_completed = 1;
    } 

    if(rx_cmd>5){
      rx_cmd = 0;
      is_rx_completed = 0;
    }

  }else{
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

int StrToHex(char str[])
{
  return (int) strtol(str, 0, 16);
}

class MyCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic -> getValue();

    pCharacteristic -> setValue(update_ble_msg(String(battery_level)));
    
    // if (value.length() > 0 ) {
    //   Serial.print("\r\nNew value: ");
    //   for (int i = 0; i < value.length(); i++) {
    //     Serial.print(value[i]);
    //   }
    // }
    // Serial.print("\n msg ?? : ");
    // Serial.println(value.c_str());
    // count_rx++;
    // Serial.print("Received times: ");
    // Serial.print(count_rx);
    // Serial.print(", \tsize: ");
   
    ble_decode_rx_msg(value);

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
  pAdvertising->setMaxInterval(0x0590); // 800 ms interval 
  pAdvertising->setMinInterval(0x055E);
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