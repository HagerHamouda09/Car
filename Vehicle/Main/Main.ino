#include "config.h"
#include "Motor.h"
#include "Relay.h"
#include "Button.h"
#include "Buzzer.h"
#include "Servo.h"
#include "Hall.h"
#include "MPU.h"
#include "Ultrasonic.h"

// SYSTEM_CASES systemState = SYSTEM_IDLE;
SYSTEM_CASES systemState = SYSTEM_NORMAL;

bool motorLocked = false;

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
       motorLocked = true; 
    Set_Motor_Speed(150);
    Set_Motor_Speed(75);
    Set_Motor_Speed(0);

    if (Crash_Detect()) {
      systemState = SYSTEM_CRASH;
      return;
    }
  }

  else if (x == 2) {
    motorLocked = true; 
    for (int i = 1; i <= 5; i++) {
      Serial.print("reducing speed ");
      Set_Motor_Speed(250 - (50 * i));
      if (Crash_Detect()) {
        systemState = SYSTEM_CRASH;
        return;
      }
      delay(1000);
    }
    delay(2000);
  }

// safe stop variable 
// driver stopped the car himself (implement(traffic or something))
  // 0 is safe
  if (y == 1)
  {
    systemState = SYSTEM_OBSTACLESTOP;

  }
  else
  {
    systemState = SYSTEM_SAFESTOP;

  }
}



void loop() {
  // systemState = SYSTEM_NORMAL;
         Calc_Speed();

      int obs = Obstacle();
      int speed = Get_Speed();
      int crash = Crash_Detect();
  
  switch (systemState) {
    case SYSTEM_IDLE:
      break;

    case SYSTEM_NORMAL:{
      Relay_ON();
      if (!motorLocked)
      {
      Set_Motor_Speed(250);
      }      

      // Serial.println(speed);
      // Serial.println(obs);
      // Serial.println(Get_Distance());
      // Serial.println("DO WE HAVE A CRASH??");
      // Serial.println(crash);
      // Serial.println("DO WE HAVE A CRASH??");

      while (!crash && speed != 0 && obs== 3) {
          
                 motorLocked = false;
                Set_Motor_Speed(250);

      obs = Obstacle();
      speed = Get_Speed();
      crash = Crash_Detect();
      
        Serial.print("normal");
        Calc_Speed();
        delay(60);
      }
      if (crash) {
        systemState = SYSTEM_CRASH;
        Serial.print("crash detected");
        break;
      }
      if (speed == 0) {
        systemState = SYSTEM_SAFESTOP;
        Serial.print("safe stop");

        break;
      }
     // 1 obstacle
      ControlSpeed(obs, 1);

      break;
  }

    case SYSTEM_OBSTACLESTOP:
      obs = Obstacle();

        if (obs == 3)
        {
          motorLocked = false;
          systemState = SYSTEM_NORMAL;
        }

    break;

    case SYSTEM_SAFESTOP:
      Calc_Speed();
      speed = Get_Speed();

      Serial.print("Case Safestop:");
      Serial.print(speed);

        if (speed > 0)
        {
          motorLocked = false;
        systemState = SYSTEM_NORMAL;
        }
      
    break;

    case SYSTEM_CRASH:
      Set_Motor_Speed(0);
      Relay_Off();
      Buzzer_On();
      //INFORM MOBILE THERE IS A CRASH
      //flag to stop loop
      //sleep mode for esp
            motorLocked = true;

      break;

    case SYSTEM_EMERGENCY:
      Set_Servo_Angle(110);
      
      if (Get_Button_State()) {
        motorLocked = false;

        //mafesh obstacle+aw2f (emergecncy)
        if (obs == 3)
          ControlSpeed(2, 1);
        else
          //obstacle +aw2f basor3a
          //obstacle +aw2f blraha
          ControlSpeed(obs, 1);
      }
      Relay_Off();
      Buzzer_On();

      break;
  }
}


