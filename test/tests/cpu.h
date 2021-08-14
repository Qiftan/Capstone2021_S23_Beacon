#ifndef TEST_CPU_H
#define TEST_CPU_H

#include <unity.h>
#include <Arduino.h>

void test_cpu_speed(void){
    set_cpu_freq();
    TEST_ASSERT_EQUAL(80, getCpuFrequencyMhz());
}

#endif