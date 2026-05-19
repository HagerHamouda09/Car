#include <Arduino.h>

#include "Motor.h"

void Motor_Init()
{
    pinMode(A01,OUTPUT);
    pinMode(A02,OUTPUT);
    pinMode(B01,OUTPUT);
    pinMode(B02,OUTPUT);
  
    // pinMode(PWMA,OUTPUT);
    // pinMode(PWMB,OUTPUT);

    pinMode(STBY,OUTPUT);

    // digitalWrite(PWMA, HIGH);
    // digitalWrite(PWMB, HIGH);
    digitalWrite(STBY, HIGH);

    ledcAttach(PWMA,Frequency,Resolution);
    ledcAttach(PWMB,Frequency,Resolution);

    Motor_Stop();
}

void Move_Forward()
{
  digitalWrite(A01, LOW);
  digitalWrite(A02, HIGH);

  digitalWrite(B01, LOW);
  digitalWrite(B02, HIGH);

}

void Move_Backward()
{
  digitalWrite(A01, HIGH);
  digitalWrite(A02, LOW);

  digitalWrite(B01, HIGH);
  digitalWrite(B02, LOW);
}

void Motor_Stop()
{
  digitalWrite(A01, LOW);
  digitalWrite(A02, LOW);

  digitalWrite(B01, LOW);
  digitalWrite(B02, LOW);
}

void Turn_Left()
{
    digitalWrite(A01, HIGH);
    digitalWrite(A02, LOW);

    digitalWrite(B01, LOW);
    digitalWrite(B02, HIGH);


}

void Turn_Right()
{
    digitalWrite(A01, LOW);
    digitalWrite(A02, HIGH);
    
    digitalWrite(B01, HIGH);
    digitalWrite(B02, LOW);
}

void Set_Motor_Speed(int Speed)
{
    Move_Forward();
    ledcWrite(PWMA,Speed);
    ledcWrite(PWMB,Speed);

}

void Set_Left_Motor_Speed(int Left_Speed)
{
    Move_Forward();
    ledcWrite(PWMA,Left_Speed);
}

void Set_Right_Motor_Speed(int Right_Speed)
{
    Move_Forward();
    ledcWrite(PWMB,Right_Speed);

}