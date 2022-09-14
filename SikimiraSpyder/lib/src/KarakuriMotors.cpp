#include <KarakuriMotors.h>
#include <Arduino.h>

#define BIN1 25
#define BIN2 26
#define PWM_B 33

#define AIN1 14
#define AIN2 12
#define PWM_A 13

#define STBY 27
#define Vmax 255

static bool flipLeft = false;
static bool flipRight = false;

// use first channel of 16 channels (started from zero)
#define CHANNEL_0 0
#define CHANNEL_1 1

// use 13 bit precision for LED_C timer
#define TIMER_13_BIT 13

// use 50 KHz as a  base frequency
#define BASE_FREQ 50000

int brightness = 0; // how bright the LED is
int fadeAmount = 5; // how many points to fade the LED by

float velocitysmoothed_R;
float velocityPrev_R;
float velocitysmoothed_L;
float velocityPrev_L;

// Arduino like analogWrite
// value has to be between 0 and valueMax
void analogWriteESP(uint8_t channel, uint32_t value, uint32_t valueMax = 255)
{
    // calculate duty, 8191 from 2 ^ 13 - 1
    uint32_t duty = (8191 / valueMax) * min(value, valueMax);

    // write duty to LED_C
    ledcWrite(channel, duty);
}

void KarakuriMotors::init2()
{
    // Configuramos los pines motores
    pinMode(STBY, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(BIN1, OUTPUT);
    pinMode(BIN2, OUTPUT);
    // Setup timer and attach timer to a led pin
    ledcSetup(CHANNEL_0, BASE_FREQ, TIMER_13_BIT);
    ledcSetup(CHANNEL_1, BASE_FREQ, TIMER_13_BIT);
    ledcAttachPin(PWM_A, CHANNEL_0);
    ledcAttachPin(PWM_B, CHANNEL_1);
}

// enable/disable flipping of left motor
void KarakuriMotors::flipLeftMotor(bool flip)
{
    flipLeft = flip;
}

// enable/disable flipping of right motor
void KarakuriMotors::flipRightMotor(bool flip)
{
    flipRight = flip;
}

// set speed for left motor; speed is a number between -255 and 255
void KarakuriMotors::setLeftSpeed(int16_t speed)
{
    init();
    digitalWrite(STBY, HIGH);
    bool reverse = 0;

    if (speed < 0)
    {
        speed = -speed; // Make speed a positive quantity.
        reverse = 1;    // Preserve the direction.
    }
    if (speed > 255) // Max PWM duty cycle.
    {
        speed = 255;
    }

    if (reverse ^ flipLeft)
    {
        digitalWrite(BIN1, HIGH);
        digitalWrite(BIN2, LOW);
    }
    else
    {
        digitalWrite(BIN1, LOW);
        digitalWrite(BIN2, HIGH);
    }
    analogWriteESP(CHANNEL_1, speed);
}

// set speed for left motor; speed is a number between -255 and 255
void KarakuriMotors::setRightSpeed(int16_t speed)
{
    init();
    digitalWrite(STBY, HIGH);
    bool reverse = 0;

    if (speed < 0)
    {
        speed = -speed; // Make speed a positive quantity.
        reverse = 1;    // Preserve the direction.
    }
    if (speed > 255) // Max PWM duty cycle.
    {
        speed = 255;
    }

    if (reverse ^ flipRight)
    {
        digitalWrite(AIN1, HIGH);
        digitalWrite(AIN2, LOW);
    }
    else
    {
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, HIGH);
    }
    analogWriteESP(CHANNEL_0, speed);
}

// set speed for both motors
void KarakuriMotors::setSpeeds(int16_t leftSpeed, int16_t rightSpeed)
{
    digitalWrite(STBY, HIGH);
    setLeftSpeed(leftSpeed);
    setRightSpeed(rightSpeed);
}

void KarakuriMotors::attenuatedSpeeds(float leftSpeed,int16_t rightSpeed)
{
    //float leftS=leftSpeed;
    //float rightS=rightSpeed;
    bool atenuacion =true;
    while (atenuacion){
        
        velocitysmoothed_R = (rightSpeed*0.05)+(velocityPrev_R*0.95);
        velocityPrev_R = velocitysmoothed_R;
        setRightSpeed(velocitysmoothed_R);

        velocitysmoothed_L = (leftSpeed*0.05)+(velocityPrev_L*0.95);
        velocityPrev_L = velocitysmoothed_L;

        setLeftSpeed(velocitysmoothed_L);
        Serial.print(leftSpeed);
        Serial.print(", ");
        Serial.println(velocitysmoothed_L);
        //delay(10);
        if (velocitysmoothed_R<rightSpeed+0.5 && velocitysmoothed_R>rightSpeed-0.5)
        {
            atenuacion=false;
        }
    }

}