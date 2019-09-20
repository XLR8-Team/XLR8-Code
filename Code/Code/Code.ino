//////////////////////////
// INCLUDE de LIBRERIAS //
//////////////////////////
/**
 * Calibración del PID desde la APP android PIDfromBT
 * https://github.com/robotaleh/PIDfromBT
 */
#include <TimerOne.h>
#include <Servo.h>

////////////////////////
// SELECTOR DE ROBOT  //
////////////////////////
// #define DRAGON_A 1
#define DRAGON_B 1

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
#define BTN 2
#define BTN_CALIBRACION 7

///////////////////////////////
//       MOTOR_SUCCION       //
///////////////////////////////
Servo esc; 

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
#endif
int posicionMaxima = 6500;
int posicionMinima = -6500;

///////////////////////////////
// VARIABLES DE CALIBRACIÓN  //
///////////////////////////////
#ifdef MORRO_ANCHO
int valoresCalibracionMinimos[] = {1023, 1023, 1023, 1023, 1023, 1023, 1023, 1023};
int valoresCalibracionMaximos[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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



void setup() {
  inicia_todo(); //Iniciar todos los compoenentes
  
  //Secuencia para arrancar turbina con Servo.h
  //***esc.writeMicroseconds(1000); //Señal a mil (Está detenido) entre 1000 y 2000

  calibra_sensores(); //Calibracion sensores
  delay(100);
}

void loop() {
  if(millis() - t > periodo)
  {
    t = millis();
    leer_pulsadores();

    switch(estado) {
      case INICIALIZADO:
        if(b1 == HIGH)
        {          
          estado = CALIBRANDO_SENSORES;
          SET_LED_G_LOW;
        }
        break;
        
      case CALIBRANDO_SENSORES:
        if(b2 == HIGH)
        {
          
          estado = PARADO;
          led_g_blink = 1;
        }
        break;

      case PARADO:
        if(b1 == HIGH)
        {
          estado = RASTREANDO;
          led_g_blink = 0;
        }
        break;
        
      case RASTREANDO:
        if(b2 == HIGH)
        {
          estado = PARADO;
            //  Inicializa los motores a estado parado
          pausa_timer_PID();
          //integral = 0;
          led_g_blink = 1;
        }
        break;
    }

    switch (estado) {
      case CALIBRANDO_SENSORES:
        calibra_sensores(); //Calibracion sensores        
        break;
        
      case PARADO:
        if(t - t_blink < 250)
          SET_LED_G_LOW;
        else if(t - t_blink < 500)
          SET_LED_G_HIGH;
        else
          t_blink = t;
        break;

      case RASTREANDO:
        leer_sensores_linea(ADC_linea);
        proporcional = posicion_linea(ADC_linea);
        PID();
        control_motores();
        break;
    }
  }
}
