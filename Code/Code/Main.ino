//////////////////////////
// INCLUDE de LIBRERIAS //
//////////////////////////
/**
 * Calibración del PID desde la APP android PIDfromBT
 * https://github.com/robotaleh/PIDfromBT
 */
//#include <Filter.h>
//#include <MegunoLink.h>
//#include <TimerOne.h>
#include <PIDfromBT.h>
//#include <Wire.h>

////////////////////////
// SELECTOR DE ROBOT  //
////////////////////////
// #define DRAGON_A 1
#define DRAGON_B 1

#define MORRO_ANCHO 1
// #define MORRO_ESTRECHO 1

////////////
// MODOS  //
////////////
#define MODO_LINEA 1

#define LINEA_NEGRA 1
#define LINEA_BLANCA 2

#define LIPO_2S 1
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


#elif MORRO_ESTRECHO
#define NUMERO_SENSORES 14
#define SENSOR_1 0
#define SENSOR_2 1
#define SENSOR_3 2
#define SENSOR_4 3
#define SENSOR_5 4
#define SENSOR_6 5
#define SENSOR_7 7
#define SENSOR_8 15
#define SENSOR_9 14
#define SENSOR_10 13
#define SENSOR_11 12
#define SENSOR_12 11
#define SENSOR_13 10
#define SENSOR_14 9
#endif
//#define SENSOR_ROBOTRACER_DERECHO PA4
//#define SENSOR_ROBOTRACER_IZQUIERDO PA5

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
#define MOTOR_SUCCION 10

//////////
// LEDS //
//////////
//#define RED_RGB_R PA1
#define RED 8
#define GREEN 9

/////////////
// BOTONES //
/////////////
#define BTN 2
#define BTN_CALIBRACION 7

///////////////
// VARIABLES //
///////////////
/*float anguloGiro = 0;
float anguloGiroR = 0;
float velocidadW = 0;
float velocidadMs = 0;
float posXm = 0;
float posYm = 0;*/
int velocidad = 0;
float velocidadMsIdeal = 0;
float velocidadMsIdealBase = 0;
int velocidadSuccion = 0;
int velocidadSuccionBase = 50;
int velocidadMaxima = 255;
long ultimaLinea = 0;
//long ultimaBateria = 0;
//bool avisoBateria = false;
//int intervaloAvisoBateria = 500;
long millisInitESC = -1;
bool ESCIniciado = false;



//////////////////////////
// VARIABLES DE CONTROL //
//////////////////////////
int posicionActual = 0;
int posicionIdeal = 0;
int posicionIdealObjetivo;
float errorAnterior = 0;
float integralErrores = 0;
float kp;
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
int valoresSensores[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int valoresSensoresRaw[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#elif MORRO_ESTRECHO
int pinesSensores[] = {SENSOR_1, SENSOR_2, SENSOR_3, SENSOR_4, SENSOR_5, SENSOR_6, SENSOR_7, SENSOR_8, SENSOR_9, SENSOR_10, SENSOR_11, SENSOR_12, SENSOR_13, SENSOR_14};
int valoresSensores[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int valoresSensoresRaw[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
int posicionMaxima = 6500;
int posicionMinima = -6500;

///////////////////////////////
// VARIABLES DE CALIBRACIÓN  //
///////////////////////////////
#ifdef MORRO_ANCHO
int valoresCalibracionMinimos[] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
int valoresCalibracionMaximos[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#elif MORRO_ESTRECHO
int valoresCalibracionMinimos[] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
int valoresCalibracionMaximos[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
#endif
int umbralesCalibracionSensores[NUMERO_SENSORES];
int valorCalibradoMaximo;
int valorCalibradoMinimo;

///////////////////////////////
// VARIABLES DE COMPETICIÓN  //
///////////////////////////////
bool competicionIniciada = false;


// TIMER
int periodo = 1; // milisegundos de periodo del timer
unsigned long t = 0;

// ESTADOS
#define INICIALIZADO          0
#define CALIBRANDO_SENSORES   1
#define PARADO                2
#define RASTREANDO            3
int estado = INICIALIZADO;

//////////////////////////////
// INICIALIZACION LIBRERIAS //
//////////////////////////////

//***********************INICIALIZAR TIMER EN ARDUINO

//HardwareTimer TimerPID(2); //Que timer se usa el 2
bool timerPID_pause = false;
//HardwareTimer TimerBrushless(3);//Que timer se usa el 3
//PIDfromBT CalibracionPID(&kp, &ki, &kd, &velocidad, &posicionIdeal, &velocidadSuccion, DEBUG);
//ExponentialFilter<long> filtroBateria(15, 0);


void setup() {
  inicia_todo();
  inicia_timer_Brushless();  
  calibra_sensores();
  delay(100);
}

void loop() {

  //AGREGAR MAQUINA DE ESTADOS!!

  
  // CalibracionPID.update();
  if (!competicionIniciada) {
    btn_cruceta();
    if (!btn_pulsado()) {
      delay(100);
      if (btn_pulsado()) {

        //*******leds!!!!
        
        //set_color_RGB(255, 0, 0);
        while (btn_pulsado()) {
        }
        long millisIniciales = millis();
        int tiempoPasado = 5;
        while (millis() < (millisIniciales + MILLIS_INICIO)) {
          tiempoPasado = millis() - millisIniciales;
          //byte r = 0, g = 0;
          //r = map(tiempoPasado, 0, MILLIS_INICIO, 255, 0);
          //g = map(tiempoPasado, 0, 1000, 0, 255);
          //set_color_RGB(r, g, 0);
          if ((tiempoPasado > MILLIS_INICIO * 0.75f || MILLIS_INICIO == 0) && velocidadSuccion == 0 && velocidadSuccionBase > 0) {
            velocidadSuccion = 50;
          }
        }
        /**
        ticksDerecho = 0;
        ticksIzquierdo = 0;
        ticksDerechoAnteriores = 0;
        ticksIzquierdoAnteriores = 0;
        **/
        velocidadMs = 0;
        inicia_timer_PID();
        competicionIniciada = true;
        //set_color_RGB(0, 0, 0);
        velocidadMsIdeal = velocidadMsIdealBase;
        velocidadSuccion = velocidadSuccionBase;
      }
    }
  }
}
