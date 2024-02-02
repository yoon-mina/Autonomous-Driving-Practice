#include "iFMotors.h"
#include <NeoPixelBus.h>
#define Left_IR 35// Left_IR 
#define L_M_IR 34
#define R_M_IR 38
#define Right_IR 37 // Right_IR 
#define Trigger 7
#define Echo 8

unsigned int colorSaturation = 40; 
unsigned int L_value, L_M_value, R_M_value, R_value;
unsigned int time_dis;
unsigned int temp = 0;
const uint16_t PixelCount = 4; // 4 LEDS 
const uint8_t PixelPin = 2;  // 4 LEDS PIN NUMBER
float distance;

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod> strip(PixelCount, PixelPin);
iFMotors ifMotor;

RgbColor red(colorSaturation, 0, 0);
RgbColor yellow(colorSaturation, colorSaturation, 0); 
RgbColor white(colorSaturation,colorSaturation,colorSaturation);
RgbColor black(0); 

void iFMotors::init(){
    // including this does not change anything
    pinMode(LEFT_F, OUTPUT);
    pinMode(LEFT_R, OUTPUT);
    pinMode(RIGHT_F, OUTPUT);
    pinMode(RIGHT_R, OUTPUT);
    pinMode(25, INPUT_PULLDOWN);
    pinMode(14, INPUT_PULLDOWN);
    pinMode(13, INPUT_PULLDOWN);
    pinMode(15, INPUT_PULLDOWN);

    ledcSetup(LEFT_F_CH, FREQUENCY, _nPWMResBit);
    ledcSetup(LEFT_R_CH, FREQUENCY, _nPWMResBit);
    ledcSetup(RIGHT_F_CH, FREQUENCY, _nPWMResBit);                
    ledcSetup(RIGHT_R_CH, FREQUENCY, _nPWMResBit);

    ledcAttachPin(LEFT_F, LEFT_F_CH);                  
    ledcAttachPin(LEFT_R, LEFT_R_CH);
    ledcAttachPin(RIGHT_F, RIGHT_F_CH);   
    ledcAttachPin(RIGHT_R, RIGHT_R_CH);   

    ledcWrite(LEFT_F_CH, 0 );
    ledcWrite(LEFT_R_CH, 0 );  
    ledcWrite(RIGHT_F_CH, 0 );
    ledcWrite(RIGHT_R_CH, 0 );

    LT_LW_MS = TURNING_MIN_SPEED;
    LT_RW_MS = TURNING_MIN_SPEED;
    RT_LW_MS = TURNING_MIN_SPEED;
    RT_RW_MS = TURNING_MIN_SPEED;

    ui32MAX_FORWARD_SPEED = MAX_FORWARD_SPEED;
    ui32MIN_FORWARD_SPEED = MIN_FORWARD_SPEED;
    LW_FS = MIN_FORWARD_SPEED;
    RW_FS = MIN_FORWARD_SPEED;
    LW_BS = MIN_BACK_SPEED;
    RW_BS = MIN_BACK_SPEED;
}

// pwmWrite(WHEEL_LEFT, FORWARD, 800)
// 전방, 좌측, 속도 800

// 모터를 돌리는 메서드 (wheel : 좌우 direct : 앞뒤 value : 속도)
void iFMotors::pwmWrite(int wheel, int direct, uint32_t value)
{
    if( direct == BACKWARD ){
        if( wheel == WHEEL_LEFT ){
            ledcWrite(LEFT_R_CH, 0 ); 
            ledcWrite(LEFT_F_CH, value );
        }
        else{
            ledcWrite(RIGHT_R_CH, 0 );       
            ledcWrite(RIGHT_F_CH, value );     
        }
    }
    else{
        if( wheel == WHEEL_LEFT ){
            ledcWrite(LEFT_F_CH, 0 );            
            ledcWrite(LEFT_R_CH, value ); 
        }
        else{
            ledcWrite(RIGHT_F_CH, 0 ); 
            ledcWrite(RIGHT_R_CH, value );  
        }
    }
}

// 모든 바퀴의 속도를 0
void iFMotors::motorStop()
{
    pwmWrite(WHEEL_LEFT, FORWARD, 0);
    pwmWrite(WHEEL_RIGHT, FORWARD, 0);
    pwmWrite(WHEEL_LEFT, BACKWARD, 0);
    pwmWrite(WHEEL_RIGHT, BACKWARD, 0);            
}


void iFMotors::motorGo()
{
    // pwmWrite(WHEEL_LEFT, FORWARD, FORWARD_SPEED);
    // pwmWrite(WHEEL_RIGHT, FORWARD, FORWARD_SPEED);

    //// this slowly increases speed
    if (LW_FS < ui32MAX_FORWARD_SPEED) LW_FS += MOTOR_SPEED_STEP;
    else LW_FS = ui32MIN_FORWARD_SPEED;

    if (RW_FS < ui32MAX_FORWARD_SPEED) RW_FS += MOTOR_SPEED_STEP;
    else RW_FS = ui32MIN_FORWARD_SPEED;
    //// this slowly increases speed
    
    pwmWrite(WHEEL_LEFT, FORWARD, LW_FS);
    pwmWrite(WHEEL_RIGHT, FORWARD, RW_FS);
}

void iFMotors::motorBack()
{
    //// this slowly increases speed
    if (LW_BS < MAX_BACK_SPEED) LW_BS += MOTOR_SPEED_STEP;
    else LW_BS = MIN_BACK_SPEED;
    if (RW_BS < MAX_BACK_SPEED) RW_BS += MOTOR_SPEED_STEP;
    else RW_BS = MIN_BACK_SPEED;

    pwmWrite(WHEEL_LEFT, BACKWARD, LW_BS);
    pwmWrite(WHEEL_RIGHT, BACKWARD, RW_BS);
    //// this slowly increases speed
}
          
void iFMotors::motorLeftTurn()
{
    //// this slowly increases speed
    if (LT_LW_MS < TURNING_LM_MAX_SPEED) LT_LW_MS += MOTOR_SPEED_STEP;
    else LT_LW_MS = TURNING_MIN_SPEED;

    if (LT_RW_MS < TURNING_RM_MAX_SPEED) LT_RW_MS += MOTOR_SPEED_STEP;
    else LT_RW_MS = TURNING_MIN_SPEED;
    //// this slowly increases speed
    
    pwmWrite(WHEEL_LEFT, FORWARD, 0);
    pwmWrite(WHEEL_RIGHT, FORWARD, LT_RW_MS);
}

void iFMotors::motorRightTurn()
{
    //// this slowly increases speed
    if (RT_LW_MS < TURNING_LM_MAX_SPEED) RT_LW_MS += MOTOR_SPEED_STEP;
    else RT_LW_MS = TURNING_MIN_SPEED;

    if (RT_RW_MS < TURNING_RM_MAX_SPEED) RT_RW_MS += MOTOR_SPEED_STEP;
    else RT_RW_MS = TURNING_MIN_SPEED;
    //// this slowly increases speed

    pwmWrite(WHEEL_RIGHT, FORWARD, 0);
    pwmWrite(WHEEL_LEFT, FORWARD, RT_LW_MS);
}

void iFMotors::reset_ltrt_ms()
{
    LT_LW_MS = TURNING_MIN_SPEED;
    LT_RW_MS = TURNING_MIN_SPEED;
    RT_LW_MS = TURNING_MIN_SPEED;
    RT_RW_MS = TURNING_MIN_SPEED;
}

void iFMotors::reset_max_forwardSpeed()
{
    ui32MAX_FORWARD_SPEED = MAX_FORWARD_SPEED;
}

void setup() {
  strip.Begin();
  Serial.begin(9600);
  ifMotor.init();
  pinMode(Left_IR, INPUT);
  pinMode(L_M_IR, INPUT);
  pinMode(R_M_IR, INPUT);
  pinMode(Right_IR, INPUT);
  pinMode(Trigger, OUTPUT);
  pinMode(Echo, INPUT);
}

void loop() {
  L_value = analogRead(Left_IR); // Read values
  L_M_value = analogRead(L_M_IR); 
  R_M_value = analogRead(R_M_IR); 
  R_value = analogRead(Right_IR);
  
  if (temp == 0) 
  {
    digitalWrite(Trigger, LOW);
    digitalWrite(Trigger, HIGH);
    delayMicroseconds(10); // HIGH 상태가 최소 10 마이크로세컨즈정도 있어야지 초음파를 쏴라라고 이해 할 수 있음.
    digitalWrite(Trigger, LOW); // HIGH 상태 유지한 후 다시 LOW로 전환

    // 시간계수 until echo receive
    time_dis = pulseIn(Echo,HIGH); // Echo가 HIGH인 PULSE를 인식한 시간 (초음파가 물체를 인식하고 다시 ESP32로 돌아왔을 때의 시간 = 즉 왕복거리이므로 2배해줘야함)

    // 시간 값을 거리로 환산
    distance = 17.0*time_dis/100;
    if (distance > 80)
    {
      temp = 1;
    } 
  }
  else {
    if((R_value < 50) && (L_value > 1000)) // 우회전
    {
      ifMotor.pwmWrite(WHEEL_RIGHT, BACKWARD, 700);
      ifMotor.pwmWrite(WHEEL_LEFT, FORWARD, 700);
      strip.SetPixelColor(0, yellow); 
      strip.SetPixelColor(1, black); 
      strip.SetPixelColor(2, yellow); 
      strip.SetPixelColor(3, black); 
      strip.Show(); // Light ON 
    }
    else if((R_value > 1000) && (L_value < 50)) // 좌회전
    {
      ifMotor.pwmWrite(WHEEL_RIGHT, FORWARD, 700);
      ifMotor.pwmWrite(WHEEL_LEFT, BACKWARD, 700);
      strip.SetPixelColor(0, black); 
      strip.SetPixelColor(1, yellow); 
      strip.SetPixelColor(2, black); 
      strip.SetPixelColor(3, yellow); 
      strip.Show(); // Light ON 
    }
    else if((R_value > 1000) && (R_M_value > 1000) && (L_M_value > 1000) && (L_value > 1000)) // 후진 
    {
      ifMotor.pwmWrite(WHEEL_RIGHT, BACKWARD, 750);
      ifMotor.pwmWrite(WHEEL_LEFT, BACKWARD, 750);
      strip.SetPixelColor(0, red); 
      strip.SetPixelColor(1, red); 
      strip.SetPixelColor(2, black); 
      strip.SetPixelColor(3, black); 
      strip.Show(); // Light ON 
    }
    else // 직진
    {
      ifMotor.pwmWrite(WHEEL_RIGHT, FORWARD, 700);
      ifMotor.pwmWrite(WHEEL_LEFT, FORWARD, 700);
      strip.SetPixelColor(0, black); 
      strip.SetPixelColor(1, black); 
      strip.SetPixelColor(2, white); 
      strip.SetPixelColor(3, white); 
      strip.Show(); // Light ON 
    }
  }
}
