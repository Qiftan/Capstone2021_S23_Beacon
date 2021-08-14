#ifndef TEST_BATTERY_H
#define TEST_BATTERY_H

#include <unity.h>
#include "lib/battery.h"

int battery_read(void) {
    if (get_battery_level()<4.3 && get_battery_level() > 0.0){
        return 0;
    }else{
        return 1;
    }
}

void test_battery_read(void){
    adc.attach(adc_pin);
    TEST_ASSERT_EQUAL(0, battery_read());
}

#endif