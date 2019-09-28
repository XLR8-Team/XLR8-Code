//////////////////////////
// INCLUDE de LIBRERIAS //
//////////////////////////
/**
 * Calibración del PID desde la APP android PIDfromBT
 * https://github.com/robotaleh/PIDfromBT
 */
//#include <TimerOne.h>
#include <Servo.h>
// Pololu QTR-8A analog array readout
#include <QTRSensors.h>

////////////////////////
// SELECTOR DE ROBOT  //
////////////////////////
#define DRAGON_A 1
//#define DRAGON_B 1

///////////////////
// CONFIGURACION //
///////////////////

#define CALIBRAR_SENSORES true

//////////////
// SENSORES //
//////////////

QTRSensors qtr;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

uint16_t position;
/////////////
// MOTORES //
/////////////
#ifdef DRAGON_A
#define MOTOR_DERECHO_ADELANTE 12
#define MOTOR_DERECHO_ATRAS 6
#define MOTOR_IZQUIERDO_ADELANTE 5
#define MOTOR_IZQUIERDO_ATRAS 4
#endif

#ifdef DRAGON_B
#define MOTOR_DERECHO_ADELANTE 6
#define MOTOR_DERECHO_ATRAS 12
#define MOTOR_IZQUIERDO_ADELANTE 4
#define MOTOR_IZQUIERDO_ATRAS 5
#endif

#define MOTOR_DERECHO_PWM 11
#define MOTOR_IZQUIERDO_PWM 3
#define MOTOR_SUCCION 10 //TURBINA!!

//////////
// LEDS //
//////////
#define RED 8
#define GREEN 9

/////////////
// BOTONES //
/////////////
#define MO_START 0
#define MO_STOP 1

#define BTN_IZQ 2
#define BTN_DER 3

// Este arreglo contiene los pines utilizados para los botones
uint8_t button[4] = {
  0,
  1,
  2,
  7
};

uint8_t button_state[4];



///////////////////////////////
//       MOTOR_SUCCION       //
///////////////////////////////
Servo esc; 

//////////////////////////
// VARIABLES DE CONTROL //
//////////////////////////
int velocidad = 100;

int posicionIdeal = 3500;

//variables PID
int error_anterior = 0; // Para el PID (derivativo)
int error_acumulado = 0; // Para el PID (integral)

//Recomendacion bajar kp
float kp=0.23;
float ki=0;
float kd=0.75;
int correccion = 0;

///////////////////////////////
// VARIABLES DE COMPETICIÓN  //
///////////////////////////////
bool competicionIniciada = true;

///////////////////////////////
//          TIMER            //
///////////////////////////////
int periodo = 1; // milisegundos de periodo del timer
unsigned long t = 0;


///////////////////////////////
//          ESTADOS          //
///////////////////////////////
#define INICIALIZADO          0
#define CALIBRANDO_SENSORES   1
#define PARADO                2
#define RASTREANDO            3
int estado = INICIALIZADO;

unsigned long t_blink = 0;

//bool timerPID_pause = false;

uint8_t flancoSubida(int btn) {
  uint8_t valor_nuevo = digitalRead(button[btn]);
  uint8_t result = button_state[btn]!=valor_nuevo && valor_nuevo == 1;
  button_state[btn] = valor_nuevo;
  return result;
}

uint8_t flancoBajada(int btn) {
  uint8_t valor_nuevo = digitalRead(button[btn]);
  uint8_t result = button_state[btn]!=valor_nuevo && valor_nuevo == 0;
  button_state[btn] = valor_nuevo;
  return result;
}


void setup() {
  inicia_todo(); //Iniciar todos los compoenentes 
  delay(100);  
}

void loop() {
  
  if(millis() - t > periodo)
  {
    t = millis();
    
    switch(estado) {
      case INICIALIZADO:
        if(flancoSubida(BTN_IZQ))
        {          
          estado = CALIBRANDO_SENSORES;     
        }
        break;
        
      case CALIBRANDO_SENSORES:
        if(flancoSubida(BTN_DER))
        {          
          estado = PARADO;             
        }
        break;
        
      case PARADO:
        if(flancoSubida(BTN_IZQ))//|| flancoSubida(MO_START)
        {
          estado = RASTREANDO;
        }
        break;
        
      case RASTREANDO:
        if(flancoSubida(BTN_DER)) //|| flancoBajada(MO_STOP)
        {
          estado = PARADO;
          //  Inicializa los motores a estado parado
          digitalWrite(MOTOR_DERECHO_ADELANTE, LOW);
          digitalWrite(MOTOR_DERECHO_ATRAS, LOW);
          digitalWrite(MOTOR_IZQUIERDO_ADELANTE, LOW);
          digitalWrite(MOTOR_IZQUIERDO_ATRAS, LOW);
        }
        break;
    }

    switch (estado) {
      case CALIBRANDO_SENSORES:
        if(competicionIniciada){
          digitalWrite(RED, HIGH);  
                  
          // analogRead() takes about 0.1 ms on an AVR.
          // 0.1 ms per sensor * 4 samples per sensor read (default) * 6 sensors
          // * 10 reads per calibrate() call = ~24 ms per calibrate() call.
          // Call calibrate() 400 times to make calibration take about 10 seconds.
          for (uint16_t i = 0; i < 200; i++)
          {
            qtr.calibrate();
          }
          digitalWrite(RED, LOW);   // turn off Arduino's LED to indicate we are through with calibration
          competicionIniciada=false;
        }                
        break;
        
      case PARADO:      
        if(t - t_blink < 250)
          digitalWrite(GREEN, LOW);
        else if(t - t_blink < 500)
          digitalWrite(GREEN, HIGH);
        else
          t_blink = t;
        break;

      case RASTREANDO:
        esc.writeMicroseconds(1200);//Señal a mil (Está CORRIENDO) entre 1000 y 2000
        //Realizar el cálculo de posición y PID
        position = qtr.readLineWhite(sensorValues); // MODIFICACIÓN        
        correccion = calcular_PID(position);  
        dar_velocidad(correccion);
        break;
    }
  }
  
}
