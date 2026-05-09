#include "config.h"

#include "Motor.h"
#include "Relay.h"
#include "Button.h"
#include "Buzzer.h"
#include "Servo.h"
#include "Hall.h"

void setup() {
  Serial.begin(115200);
  delay(1000); // give time for serial monitor

  Serial.println("ESP32 is running!");
  
  Motor_Init();
  Relay_Init();
  // Button_Init();
  // Buzzer_Init();
  // Servo_Init();
  Hall_Init();


}

void loop() {
//   Serial.println("Loop working...");

//MOTOR TEST
//   Relay_ON();
//   delay(200);
//   Set_Motor_Speed(150);
//   delay(5000);

//   Set_Left_Motor_Speed(250);
// Set_Right_Motor_Speed(0);

//    delay(5000);
//    Set_Right_Motor_Speed(250);
//    Set_Left_Motor_Speed(0);
//   delay(5000);

///////////////////////////////////////////////////////////

//BUTTON TEST

// if(Get_Button_State()){
//   Serial.println("button pressed");
// }
// else
// {  Serial.println("Nottttttt");
// }

//////////////////////////////////////////////////////////////

//BUZZER TEST

  // Buzzer_On();
  // delay(500);
  // Buzzer_Off();

//////////////////////////////////////////////////////////////

// //Servo test
// Set_Servo_Angle(0);
// delay(2000);

// Set_Servo_Angle(90);
// delay(2000);

// Set_Servo_Angle(180);
// delay(2000);

///////////////////////////////////////////////////////////////

// //Hall test

Relay_ON();
delay(200);

Set_Motor_Speed(150);

Calc_Speed();

Serial.print("Pulses: ");
Serial.println(HallPulseCount);

Serial.print("RPM: ");
Serial.println(Get_Speed());

delay(200);


}
