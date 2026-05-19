// PINS
#ifndef CONFIG_H
#define CONFIG_H

//Motor Driver Pins
#define A01     26
#define A02     27
#define B01     12
#define B02     13

#define PWMA     25
#define PWMB     14

#define STBY    32

//Left Channel
#define PWMA_Channel    0

//Right Channel
#define PWMB_Channel    1

#define Frequency   10000
#define Resolution  8
//Sensor PINS

//ULTRASONIC
#define TRIG    17
#define ECHO    16

//Button
#define BUTTON  5

//SERVO
#define SERVO   19

//BUZZER
#define BUZZER  18

//RELAY
#define RELAY   23

//HALL
#define HALL    36

//MPU
#define SDA     21
#define SCL     22


#define MaxTrials 5

typedef enum {
    SYSTEM_IDLE,
    SYSTEM_NORMAL,
    SYSTEM_CRASH,
    SYSTEM_EMERGENCY,
    SYSTEM_OBSTACLESTOP,
    SYSTEM_READY,
    SYSTEM_SAFESTOP//CAR JUST STOPPED NO ISSUES 
} SYSTEM_CASES;

#endif