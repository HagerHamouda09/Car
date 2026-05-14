#include "config.h"
#include "Motor.h"
#include "Relay.h"
#include "Button.h"
#include "Buzzer.h"
#include "Servo.h"
#include "Hall.h"
#include "MPU.h"
#include "Ultrasonic.h"

SYSTEM_CASES systemState = SYSTEM_IDLE;


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 is running!");
  Motor_Init();
  Relay_Init();
  Button_Init();
  Buzzer_Init();
  Servo_Init();
  Hall_Init();
  MPU_init();
  Ultrasonic_Init();
}

void ControlSpeed(int x, int y) {
  if (x == 1) {
    Serial.print("reducing speed suddenly");
    Set_Motor_Speed(150);
    Set_Motor_Speed(75);
    Set_Motor_Speed(0);
    if (Crash_Detect()) {
      systemState = SYSTEM_CRASH;
    }
  }

  else if (x == 2) {
    for (int i = 1; i <= 5; i++) {
      Serial.print("reducing speed ");
      Set_Motor_Speed(250 - (50 * i));
      if (Crash_Detect()) {
        systemState = SYSTEM_CRASH;
        break;
      }
      delay(1000);
    }
  }

  else if (y == 1)
    systemState = SYSTEM_SAFESTOP;
}



void loop() {
  systemState = SYSTEM_NORMAL;

  switch (systemState) {
    case SYSTEM_IDLE:
      break;

    case SYSTEM_NORMAL:
      Relay_ON();
      Set_Motor_Speed(250);
      while (!Crash_Detect() && Get_Speed() != 0 && Obstacle() == 3) {
        Serial.print("normal");
        Calc_Speed();
        delay(2000);
      }
      if (Crash_Detect()) {
        systemState = SYSTEM_CRASH;
        Serial.print("crash detected");
        break;
      }
      if (Get_Speed() == 0) {
        systemState = SYSTEM_SAFESTOP;
        Serial.print("safe stop");

        break;
      }
      ControlSpeed(Obstacle(), 1);
      break;

    case SYSTEM_SAFESTOP:
      if (Get_Speed() > 0)
        systemState = SYSTEM_NORMAL;
      break;

    case SYSTEM_CRASH:
      Set_Motor_Speed(0);
      Relay_Off();
      Buzzer_On();
      //INFORM MOBILE THERE IS A CRASH
      //flag to stop loop
      //sleep mode for esp
      break;

    case SYSTEM_EMERGENCY:
      Set_Servo_Angle(110);
      
      if (Get_Button_State()) {

        //mafesh obstacle+aw2f
        if (Obstacle() == 3)
          ControlSpeed(2, 0);
        else
          //obstacle +aw2f basor3a
          //obstacle +aw2f blraha
          ControlSpeed(Obstacle(), 0);
      }
      Relay_Off();
      Buzzer_On();

      break;
  }
}
