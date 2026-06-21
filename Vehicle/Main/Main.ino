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
  bleCar.begin();
}

void ControlSpeed(int x, int y) {
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


    // Calc_Speed();
    // Serial.print("SPPPPPPEEEEEEEEEED Before: ");
    // Serial.print(Get_Speed());
    // Set_Motor_Speed(0);
    // Calc_Speed();
    // Serial.print("SPPPPPPEEEEEEEEEED After: ");
    // Serial.print(Get_Speed());
    // delay(2000);
    // Calc_Speed();
    // Serial.print("SPPPPPPEEEEEEEEEED After Delay: ");
    // Serial.print(Get_Speed());
    if (systemState == SYSTEM_NORMAL)
    {
      systemState = SYSTEM_SAFESTOP;
    }
    //delay(10000);
    
    
  }

// safe stop variable 
// driver stopped the car himself (implement(traffic or something))
  // 0 is safe
  if (y == 1)
  {
    Serial.println("obstacle");
    systemState = SYSTEM_OBSTACLESTOP;

  }
  else
  {
    systemState = SYSTEM_SAFESTOP;

  }
}



void loop() {
    delay(10);

         Calc_Speed();

      int obs = Obstacle();
      int speed = Get_Speed();
      int crash = Crash_Detect();
      int distance = Get_Distance();
  
    bleCar.sendTelemetry(systemState);


  switch (systemState) {
    case SYSTEM_CHECK:
    //this delay represents the wait state in the state machine if the vitals were not ok
      //delay(5000); (will be handled in the mobile )
      //Need system check so when driver wears the band start reading vitals and send it to mobile 
      delay(500);  // let sensor stabilize
      
        if (Run_Car_SelfTest())
        {
            systemState = SYSTEM_IDLE;
            bleCar.sendTelemetry(SYSTEM_IDLE); // or send "IDLE"           
        }
        else
        {
            systemState = SYSTEM_CHECK_FAIL;
            bleCar.sendTelemetry(SYSTEM_CHECK_FAIL); // or FAIL state
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
                 //motorLocked = false;
                Set_Motor_Speed(250);

      // Serial.println(speed);
      // Serial.println(obs);
      // Serial.println(Get_Distance());
      // Serial.println("DO WE HAVE A CRASH??");
      // Serial.println(crash);
      // Serial.println("DO WE HAVE A CRASH??");

      while (!crash && speed != 0 && obs== 3 && systemState == SYSTEM_NORMAL)
      {
       
                // motorLocked = false;
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

        // bleCar.sendTelemetry(systemState, speed, distance);
        bleCar.sendTelemetry(systemState);

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
          //systemState = SYSTEM_NORMAL;
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
      Set_Motor_Speed(0);
      Relay_Off();
      Buzzer_On();

      //INFORM MOBILE THERE IS A CRASH
      //flag to stop loop
      //sleep mode for esp
            motorLocked = true;

      break;

    case SYSTEM_EMERGENCY:
          

            //Set_Servo_Angle(0);

            Set_Servo_Angle(110);
      
      if (Get_Button_State()) 
      {
        motorLocked = true;
        Buzzer_On();

        //mafesh obstacle+aw2f (emergecncy)
        if (obs == 3)
          ControlSpeed(2, 1);
        else
          //obstacle +aw2f basor3a
          //obstacle +aw2f blraha
          ControlSpeed(obs, 1);
      
      Relay_Off();
            Set_Servo_Angle(90);
      }
      break;

      case SYSTEM_END_TRIP:
            // Set_Motor_Speed(0);
            // motorLocked = true;
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

