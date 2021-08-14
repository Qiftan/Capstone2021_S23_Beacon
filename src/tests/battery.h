#ifndef TEST_BATTERY_H
#define TEST_BATTERY_H

#include "lib/battery.h"

int test_battery_read(void) {
    if (get_battery_level()<4.3 && get_battery_level() > 0.0){
        return 1;
    }else{
        return 0;
    }
}

#endif