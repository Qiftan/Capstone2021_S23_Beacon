#ifndef TEST_BLE_H
#define TEST_BLE_H

#include <unity.h>
#include "lib/ble.h"

int test_ble_routine(void){
    int start_time = millis();
    while(!deviceConnectedBool){
        if((millis()-start_time)>60000){
            return 1;
        }
    }
    return 0;
}

void test_ble(void){
    TEST_ASSERT_EQUAL(0, ble_init());
    TEST_ASSERT_EQUAL(0, test_ble_routine());
    Serial.println("Disconnect nRF Bluetooth, if using nRF.");
    delay(5000);
}

#endif