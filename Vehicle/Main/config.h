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
#define PWMA_Channel    1

//Right Channel
#define PWMB_Channel    2

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
#define HALL   4 

//LED
#define LED    36

//MPU
#define SDA     21
#define SCL     22


#define MaxTrials 5

typedef enum {
    SYSTEM_CHECK,
    SYSTEM_IDLE,
    SYSTEM_READY,
    SYSTEM_NORMAL,
    SYSTEM_CRASH,
    SYSTEM_EMERGENCY,
    SYSTEM_OBSTACLESTOP,
    SYSTEM_EXCEEDED_TRIALS,
    SYSTEM_CHECK_FAIL,
    SYSTEM_END_TRIP,
    SYSTEM_MISUSE,
    SYSTEM_SAFESTOP//CAR JUST STOPPED NO ISSUES 
} SYSTEM_CASES;


// system check flags
extern bool selfTestPassed;
extern bool selfTestDone;


#endif