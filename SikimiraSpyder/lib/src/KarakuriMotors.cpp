#include <KarakuriMotors.h>
#include <Arduino.h>

//#define BIN1 25
//#define BIN2 26
//#define PWM_B 33

#define AIN1 25
#define AIN2 26
#define PWM_A 33

#define STBY 27
#define Vmax 255

static bool flipLeft = false;
static bool flipRight = false;

// use first channel of 16 channels (started from zero)
#define CHANNEL_0 0
#define CHANNEL_1 1

// use 13 bit precision for LED_C timer
#define TIMER_13_BIT 8

// use 50 KHz as a  base frequency
#define BASE_FREQ 5000

int brightness = 0; // how bright the LED is
int fadeAmount = 5; // how many points to fade the LED by

float velocitysmoothed_R[2];
float velocityPrev_R;
float velocitysmoothed_L;
float velocityPrev_L;

volatile int contador = 0;


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
    //pinMode(BIN1, OUTPUT);
    //pinMode(BIN2, OUTPUT);

    // Setup timer and attach timer to a led pin
    ledcSetup(CHANNEL_0, BASE_FREQ, TIMER_13_BIT);
    //ledcSetup(CHANNEL_1, BASE_FREQ, TIMER_13_BIT);
    ledcAttachPin(PWM_A, CHANNEL_0);
    //ledcAttachPin(PWM_B, CHANNEL_1);
       attachInterrupt(sensor,interrupcion, FALLING);

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
void KarakuriMotors::setSpeed(int16_t speed)
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

void KarakuriMotors::attenuatedSpeeds(float leftSpeed,int16_t rightSpeed)
{
    //float leftS=leftSpeed;
    //float rightS=rightSpeed;
    bool atenuacion =true;
    while (atenuacion){
        
        velocitysmoothed_R[0] = (rightSpeed*0.05)+(velocityPrev_R*0.95);
        velocityPrev_R = velocitysmoothed_R[0];
        setSpeed(velocitysmoothed_R[0]);

        velocitysmoothed_L = (leftSpeed*0.05)+(velocityPrev_L*0.95);
        velocityPrev_L = velocitysmoothed_L;

        //delay(10);
        if (velocitysmoothed_R[0]<rightSpeed+0.5 && velocitysmoothed_R[0]>rightSpeed-0.5)
        {
            atenuacion=false;
        }
    }

}

void KarakuriMotors::moveSpyder(int intervalTime, bool directtionBool){

        float temp =float (intervalTime)/1000.0f;
       
        float dp=revs*temp*2*pi*radius;

        if(directtionBool==true){lenght=lenght+dp;}
        else{lenght=lenght-dp;}

        if(lenght<=0){lenght=0;}
  }

void KarakuriMotors::speedSpyder(int intervalTime){
      revs=float (contador)/float (n_holes)*(1000.0f/ float(intervalTime));
      contador=0;
    }

void KarakuriMotors::interrupcion() {
  contador++;
  }