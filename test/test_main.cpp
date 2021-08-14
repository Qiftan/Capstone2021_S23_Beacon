#include "main.h"
#include "tests/test.h"
#include <unity.h>

int test_completed = 0;
int test_count = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(serial_speed);
    UNITY_BEGIN();
}

void loop() {    

    RUN_TEST(test_cpu_speed);
    RUN_TEST(test_battery_read);
    RUN_TEST(test_epaper_config);
    RUN_TEST(test_settimer_1sec);
    RUN_TEST(test_buzzer);
    RUN_TEST(test_led);
    RUN_TEST(test_epaper);
    RUN_TEST(test_ble);
    RUN_TEST(test_integrated);

    UNITY_END();
}