#ifndef TEST_TIMER_H
#define TEST_TIMER_H

#include <unity.h>
#include <Arduino.h>

void test_settimer_1sec(void){
    TEST_ASSERT_EQUAL(0, set_one_second_timer());
}

#endif