#include <Servo.h>
// Pololu QTR-8A analog array readout
#include <QTRSensors.h>

// TB6612 driver pinout
//const int STBY = 15; // standby
const int PWMA = 3; // speed and direction control motor A (left)
const int AIN1 = 5;
const int AIN2 = 4;
const int PWMB = 11; // speed and direction control motor B (right)
const int BIN1 = 6;
const int BIN2 = 12;

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

uint16_t position;

unsigned int IR[8];

Servo esc; 

// parameters and variables for non linear PID
const int vmin=70;
const int vmax=80;
const float kp=.015;
const float ki=0.0003;
const float kd=0.2;
const float kv=0.07;
int p,d,u,vbase;
long i=0;
int p_old=0;

void setup() {
  // put your setup code here, to run once:
  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5,A6,A7}, SensorCount);
  qtr.setEmitterPin(13);
  delay(100);   

  pinMode(AIN1,OUTPUT);
    pinMode(AIN2,OUTPUT);
    pinMode(PWMA,OUTPUT);
    pinMode(PWMB,OUTPUT);
    pinMode(BIN1,OUTPUT);
    pinMode(BIN2,OUTPUT);


  esc.attach(10);
  esc.writeMicroseconds(1000); //Señal a mil (Está detenido
delay(100);
}

void loop() {

  esc.writeMicroseconds(1200);//Señal a mil (Está CORRIENDO) entre 1000 y 2000
  
  // put your main code here, to run repeatedly:

  qtr.readLineBlack(IR);
  //qtra.read(IR); // read raw sensor values
  
  p = -7*IR[0]-5*IR[1]-3*IR[2]-IR[3]+IR[4]+3*IR[5]+5*IR[6]+7*IR[7];
  i=i+p;
  d=p-p_old;
  p_old=p;
  if ((p*i)<0) i=0;  // integral windup

  u=kp*p+ki*i+kd*d;
  vbase=vmin+(vmax-vmin)*exp(-kv*abs(kp*p));
  drive(vbase+u,vbase-u);

  

}

void drive(int L, int R) // speed for wheels Left and Right, positive is forward
{
  L=constrain(L,-255,255); // avoid PWM overflow
  R=constrain(R,-255,255);
  
  digitalWrite(AIN1, L<0); // switch < and >= if left wheel doesnt spin as expected
  digitalWrite(AIN2, L>=0);
  analogWrite(PWMA, abs(L));
  
  digitalWrite(BIN1, R<0);  // switch < and >= if left wheel doesnt spin as expected
  digitalWrite(BIN2, R>=0);
  analogWrite(PWMB, abs(R));
}
