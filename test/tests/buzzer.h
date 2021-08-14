#ifndef TEST_BUZZER_H
#define TEST_BUZZER_H

#include <unity.h>
#include "lib/buzzer.h"

void test_buzzer(void){
    TEST_ASSERT_EQUAL(0, play_tone());
}

#endif