// main.cpp for ESP32-WROOM
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// FreeRTOS tasks
void lockerManagementTask(void *pvParameters);
void sensorMonitoringTask(void *pvParameters);
void keypadInputTask(void *pvParameters);
void oledDisplayTask(void *pvParameters);

// Global objects
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  
  // Initialize hardware
  Wire.begin();
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED allocation failed");
    while(1);
  }
  
  // Create FreeRTOS tasks
  xTaskCreate(lockerManagementTask, "LockerMgmt", 4096, NULL, 1, NULL);
  xTaskCreate(sensorMonitoringTask, "SensorMon", 4096, NULL, 1, NULL);
  xTaskCreate(keypadInputTask, "Keypad", 2048, NULL, 2, NULL);
  xTaskCreate(oledDisplayTask, "OLED", 2048, NULL, 1, NULL);
}

void loop() {
  // Not used - FreeRTOS handles tasks
}

// Task implementations would go here...