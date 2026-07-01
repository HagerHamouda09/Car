#include "config.h"
#include "Motor.h"
#include "Relay.h"
#include "Button.h"
#include "Buzzer.h"
#include "Servo.h"
#include "Hall.h"
#include "MPU.h"
#include "Ultrasonic.h"
#include "BLE.h"
#include "SelfTest.h"

SYSTEM_CASES systemState = SYSTEM_CHECK;

bool motorLocked = false;

int Trials = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("ESP32 is running!");
  Servo_Init();
  Motor_Init();
  Relay_Init();
  Button_Init();
  Buzzer_Init();
  Hall_Init();
  MPU_init();
  Ultrasonic_Init();
  ble_begin();
}

void ControlSpeed(int x, int y) {
  bool emergencyActive = (systemState == SYSTEM_EMERGENCY);
  if (x == 1) {
    Serial.print("reducing speed suddenly");
       motorLocked = true; 
    Set_Motor_Speed(150);
    Set_Motor_Speed(75);
    Set_Motor_Speed(0);

    if (systemState == SYSTEM_NORMAL)
      {
          if (Crash_Detect())
          {
              systemState = SYSTEM_CRASH;
          }
          else 
          {
          systemState = SYSTEM_SAFESTOP;
          }
      }
  }

  else if (x == 2) {
    motorLocked = true; 
if (!emergencyActive)
    systemState = SYSTEM_SAFESTOP;
      for (int i = 1; i <= 5; i++) {
      Serial.print("reducing speed ");
      Set_Motor_Speed(250 - (50 * i));
      if (Crash_Detect()) {
        systemState = SYSTEM_CRASH;
        return;
      }

      delay(1000);
    }

    Set_Motor_Speed(0);
        Calc_Speed();

    delay(2000);
    Calc_Speed();

    if (systemState == SYSTEM_NORMAL)
    {
if (!emergencyActive)
    systemState = SYSTEM_SAFESTOP;  
    }
    
    
  }

// safe stop variable 
// driver stopped the car himself (implement(traffic or something))
  // 0 is safe
  if (!emergencyActive)
{  if (y == 1)
  {
    Serial.println("obstacle");
    systemState = SYSTEM_OBSTACLESTOP;

  }
  else
  {
    systemState = SYSTEM_SAFESTOP;

  }
}
}



void loop() {
    delay(10);

         Calc_Speed();

      int obs = Obstacle();
      int speed = Get_Speed();
      int crash = Crash_Detect();
      int distance = Get_Distance();
  
    ble_sendData(systemState);


  switch (systemState) {
    case SYSTEM_CHECK:
    
      delay(500);  // let sensor stabilize
      
      if (Run_Car_SelfTest())
      {
          ble_send_systemcheck(true);
          systemState = SYSTEM_IDLE;
          
      }
      else
      {
          ble_send_systemcheck(false);
          systemState = SYSTEM_CHECK_FAIL;
      }
      break;

    case SYSTEM_READY:
    {
        
        Relay_ON();

        Calc_Speed();
        speed = Get_Speed();

 

        if (speed > 0)
        {
            systemState = SYSTEM_NORMAL;

            Serial.println("SYSTEM_NORMAL");
        }

        break;
    }
    case SYSTEM_NORMAL:{

        if (motorLocked)
        {
            Set_Motor_Speed(0);
            break;
        }
                Set_Motor_Speed(250);

      while (!crash && speed != 0 && obs== 3 && systemState == SYSTEM_NORMAL)
      {
                if (!motorLocked)
                {
                  Set_Motor_Speed(250);
                }

      obs = Obstacle();
      speed = Get_Speed();
      crash = Crash_Detect();
      distance = Get_Distance();
      
        Serial.print("normal");
        Calc_Speed();

        ble_sendData(systemState);

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
       if (motorLocked)
        {
            Set_Motor_Speed(0);
            break;
        }
     // 1 obstacle
      if (obs != 3)
        ControlSpeed(obs, 1);

      break;
  }

    case SYSTEM_OBSTACLESTOP:
      obs = Obstacle();

        if (obs == 3)
        {
          motorLocked = true;
          systemState = SYSTEM_SAFESTOP;
        }

    break;

    case SYSTEM_SAFESTOP:
      Calc_Speed();
      speed = Get_Speed();

      Serial.print("Case Safestop:");
      Serial.print(speed);

        if (!motorLocked)
        {
          motorLocked = false;
        systemState = SYSTEM_NORMAL;
        }
      
    break;

    case SYSTEM_CRASH:
      
      ble_sendCrash();
      Set_Motor_Speed(0);
      Relay_Off();
      Buzzer_On();

            motorLocked = true;

      break;

    case SYSTEM_EMERGENCY:

      Set_Servo_Angle(160);
      delay(2000);
      if (Get_Button_State()) 
      {
        motorLocked = true;
        Buzzer_On();

        //No obtacle + Stop  (emergecncy)
        if (obs == 3)
          ControlSpeed(2, 1);
        else
          //obstacle +stop suddenly
          //obstacle +stop gradually
          ControlSpeed(obs, 1);

      Relay_Off();

      systemState = SYSTEM_END_TRIP;
            
      }
      break;

      case SYSTEM_END_TRIP:
            Set_Servo_Angle(90);
            delay(1000);
            Relay_Off();
            Serial.flush();
            delay(200);
            esp_deep_sleep_start();

          break;

      case SYSTEM_MISUSE:

            Serial.flush();
            delay(200);
            esp_deep_sleep_start();
        
          break;
  }
}

