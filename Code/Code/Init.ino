/**
 * Función para iniciar todos los componentes del robot.
 */
void inicia_todo() { 

// configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5,A6,A7}, SensorCount);
  qtr.setEmitterPin(13);

  delay(100);   
  inicia_motores();
  delay(100);  
  inicia_leds();
  delay(100);
  inicia_switch_boton();
  delay(100);  
  inicia_turbina();
  delay(100);
}

/**
 * Registra los pines de los Puente-H para los motores y los inicia en parado.
 */
void inicia_motores() {
  pinMode(MOTOR_DERECHO_ADELANTE, OUTPUT);
  pinMode(MOTOR_DERECHO_ATRAS, OUTPUT);
  pinMode(MOTOR_DERECHO_PWM, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_ADELANTE, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_ATRAS, OUTPUT);
  pinMode(MOTOR_IZQUIERDO_PWM, OUTPUT);

  //	Inicializa los motores a estado parado
  digitalWrite(MOTOR_DERECHO_ADELANTE, LOW);
  digitalWrite(MOTOR_DERECHO_ATRAS, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ADELANTE, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ATRAS, LOW);
}

/**
 * Regitra los pines de los Leds
 */
void inicia_leds() {
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);  
}

/**
 * Registra los pines del Switch y Botón
 */
void inicia_switch_boton() {
   
  // Configurar como PULL-UP para ahorrar resistencias
  pinMode(button[BTN_IZQ], INPUT_PULLUP);
  pinMode(button[BTN_DER], INPUT_PULLUP);

  // Se asume que el estado inicial es HIGH
  button_state[2] = HIGH;
  button_state[3] = HIGH;  
}
/**
 * Registra el pin PWM TURBINA
 */
void inicia_turbina() {
  esc.attach(MOTOR_SUCCION);
  esc.writeMicroseconds(1000); //Señal a mil (Está detenido) 60 70 O 20
}
