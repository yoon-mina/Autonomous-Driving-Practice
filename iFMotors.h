
#ifndef iFMotors_v1_h
#define iFMotors_v1_h
#define iFMOTORS_LIBRARY_VERSION	1.1.1
#include <Arduino.h>

////////// for DC Motor
#define FREQUENCY (3000)

#define LEFT_F     32
#define LEFT_R     33
#define RIGHT_F    26
#define RIGHT_R    27

#define LEFT_F_CH (0)
#define LEFT_R_CH (1)
#define RIGHT_F_CH (2)
#define RIGHT_R_CH (3)

#define FORWARD         0
#define BACKWARD        1

#define WHEEL_LEFT      1
#define WHEEL_RIGHT     0

// Above 599 System restart (void setup() called)
// At 500 wheel does not rotate

#define MAX_FORWARD_SPEED 850
#define MIN_FORWARD_SPEED 550

#define MAX_BACK_SPEED 900
#define MIN_BACK_SPEED 550

#define TURNING_LM_MAX_SPEED ((uint32_t) 950)
#define TURNING_RM_MAX_SPEED ((uint32_t) 950)
#define TURNING_MIN_SPEED ((uint32_t) 0)
#define MOTOR_SPEED_STEP ((uint32_t) 1)

class iFMotors{
    private:
        const int _nPWMResBit = 10;
        const int _nMaxSpeed = pow(2, _nPWMResBit);
        
    public:
        uint32_t LT_LW_MS = TURNING_MIN_SPEED;
        uint32_t LT_RW_MS = TURNING_MIN_SPEED;
        uint32_t RT_LW_MS = TURNING_MIN_SPEED;
        uint32_t RT_RW_MS = TURNING_MIN_SPEED;

        uint32_t ui32MAX_FORWARD_SPEED = MAX_FORWARD_SPEED;
        uint32_t ui32MIN_FORWARD_SPEED = MIN_FORWARD_SPEED;
        uint32_t LW_FS, RW_FS;
        uint32_t LW_BS, RW_BS;

        iFMotors(){}

        void init();
        void pwmWrite(int wheel, int direct, uint32_t value);
        void motorStop();
        void motorGo();
        void motorBack();
        void motorRightTurn();
        void motorLeftTurn();
        void reset_ltrt_ms();
        void reset_max_forwardSpeed();
        
};

#endif
