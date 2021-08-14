#ifndef _BUZZER_H_
#define _BUZZER_H_

#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>

#define NOTE_E4  330
#define NOTE_C4  262
#define NOTE_G4  392

int start_buzzer = 0;

TaskHandle_t Task_buzzer;
int n = 0;
int melody[6] = {NOTE_E4, NOTE_E4, NOTE_E4, NOTE_C4, NOTE_E4, NOTE_G4};
int noteDurations[6] = {8,4,4,8,4,2};
int BUZZER_PIN = 13;

int play_tone(void){
// void play_tone(void){
    // Pause the task again for 500ms
    delay(200 / portTICK_PERIOD_MS);
    Serial.print("Task Buzzer is running on: ");
    Serial.println(xPortGetCoreID());
    for (int i = 0; i < 6; i++) {
        int noteDuration = 1000/noteDurations[i];
        tone(BUZZER_PIN, melody[i],noteDuration);
        int pauseBetweenNotes = noteDuration;
        vTaskDelay(pauseBetweenNotes);
        noTone(BUZZER_PIN);
    }
    Serial.println("Completed Buzzer!");
    // Delete(NULL);
    return 0;
}

// void createBuzzerTask(void){
//     xTaskCreatePinnedToCore(
//         play_tone,    // Function that should be called
//         "play_tone",   // Name of the task (for debugging)
//         1000,            // Stack size (bytes)
//         NULL,            // Parameter to pass
//         1,               // Task priority
//         &Task_buzzer,             // Task handle
//         1
//     );
// }

#endif