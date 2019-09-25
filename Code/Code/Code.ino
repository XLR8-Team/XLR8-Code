//////////////////////////
// INCLUDE de LIBRERIAS //
//////////////////////////
/**
 * Calibración del PID desde la APP android PIDfromBT
 * https://github.com/robotaleh/PIDfromBT
 */
//#include <TimerOne.h>
#include <Servo.h>

////////////////////////
// SELECTOR DE ROBOT  //
////////////////////////
#define DRAGON_A 1
//#define DRAGON_B 1

#define MORRO_ANCHO 1

////////////
// MODOS  //
////////////
#define MODO_LINEA 1

#define LINEA_NEGRA 1
#define LINEA_BLANCA 2

#define LIPO_3S 2

///////////////////
// CONFIGURACION //
///////////////////
#define MILLIS_INICIO 2000
#define PISTA MODO_LINEA
#define LINEA LINEA_NEGRA
#define LIPO LIPO_3S
#define TIEMPO_CALIBRADO 2500
#define CALIBRAR_SENSORES true

//////////////
// SENSORES //
//////////////
#define TIEMPO_SIN_PISTA 200
#define EMITTER_PIN 13

#ifdef MORRO_ANCHO
#define NUMERO_SENSORES 8
#define SENSOR_1 A0 /****/
#define SENSOR_2 A1
#define SENSOR_3 A2
#define SENSOR_4 A3
#define SENSOR_5 A4
#define SENSOR_6 A5
#define SENSOR_7 A6
#define SENSOR_8 A7 /****/
#endif

#define CALIBRADO_MAXIMO_SENSORES_LINEA 4000
#define CALIBRADO_MINIMO_SENSORES_LINEA 0
#define SATURACION_MAXIMO_SENSORES_LINEA 3000
#define SATURACION_MINIMO_SENSORES_LINEA 1000

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

///////////////
// VARIABLES //
///////////////
int velocidad = 80;
int velocidadMaxima = 255;
long ultimaLinea = 0;



//////////////////////////
// VARIABLES DE CONTROL //
//////////////////////////
int posicionActual = 0;
int posicionIdeal = 0;
int posicionIdealObjetivo;
float errorAnterior = 0;
float integralErrores = 0;
float kp=0.05;
float ki;
float kd;
float kpVelocidad = 5;
float kdVelocidad = 10;
float ultimoErrorVelocidad = 0;
int correccion = 0;

///////////////////////////
// VARIABLES DE SENSORES //
///////////////////////////
int valorSaturacionBajo;
int valorSaturacionAlto;
#ifdef MORRO_ANCHO
int pinesSensores[] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8};
int valoresSensores[] = {0, 0, 0, 0, 0, 0, 0, 0};
int valoresSensoresRaw[] = {0, 0, 0, 0, 0, 0, 0, 0};
#endif
int posicionMaxima = 6500;
int posicionMinima = -6500;

///////////////////////////////
// VARIABLES DE CALIBRACIÓN  //
///////////////////////////////
#ifdef MORRO_ANCHO
int valoresCalibracionMinimos[] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
int valoresCalibracionMaximos[] = {0, 0, 0, 0, 0, 0, 0, 0};
#endif
int umbralesCalibracionSensores[NUMERO_SENSORES];
int valorCalibradoMaximo;
int valorCalibradoMinimo;

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

bool timerPID_pause = false;

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
         // digitalWrite(GREEN, HIGH);     
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
          calibra_sensores(); //Calibracion sensores
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
        handler_timer_PID();                    
        break;
    }
  }
  
}
