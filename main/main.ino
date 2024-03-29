#include "task.h"
#include "laser.h"
#include "accelerometer.h"
//#include "display.h"
#include "wifi.h"
#include "holder.h"
#include "vest.h"

// TASK SCHEDULER
const unsigned short tasksNum = 3;
task tasks[tasksNum];

void setup() {
  Serial.begin(9600); //115200
  // Specify pins
  pinMode(LASER_OUTPUT, OUTPUT);
  pinMode(BUTTON_INPUT, INPUT);

  //all pins below from vest.h
  pinMode(photo_sensor, INPUT);
  pinMode(blue_LED, OUTPUT);
  pinMode(red_LED, OUTPUT);
  pinMode(white_LED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Set up LCD
  //lcd.begin(16,2);
  //lcd.init();
  //lcd.backlight();
  // Set up MPU
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
	Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
	delay(500);
  }
  checkSettings();
  // TASK SCHEDULER
  int i = 0;
  tasks[i].state = Laser_Init;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Laser_TickFct;
  i++;
  tasks[i].state = Vest_Init;
  tasks[i].period = 200;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &Vest_TickFct;
  //i++;
  //tasks[i].state = 0;
  //tasks[i].period = 200;
  //tasks[i].elapsedTime = 0;
  //tasks[i].TickFct = &Screen_TickFct;
  i++;
  tasks[i].state = MPU_Read;
  tasks[i].period = 100;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &MPU_TickFct;
  
  setup_wifi();
  _ID = generate(); // Global ID in holder.h
}

void loop() {
  //commented code below tests vest.h
  //Serial.println(analogRead(photo_sensor)); //809-810 in classroom lighting
  //Serial.println(curr_respawn_time);

  // TASK SCHEDULER

  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
      tasks[i].state = tasks[i].TickFct(tasks[i].state);
      tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
}
