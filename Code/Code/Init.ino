/**
 * Función para iniciar todos los componentes del robot.
 */
void inicia_todo() {  
  inicia_sensores();
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
 * Registra los pines del Multiplexador para los sensores
 */
 
void inicia_sensores() {  
  pinMode(SENSOR_1 , INPUT);
  pinMode(SENSOR_2 , INPUT);
  pinMode(SENSOR_3 , INPUT);
  pinMode(SENSOR_4 , INPUT);
  pinMode(SENSOR_5 , INPUT);
  pinMode(SENSOR_6 , INPUT);
  pinMode(SENSOR_7 , INPUT);
  pinMode(SENSOR_8 , INPUT);  

  pinMode(EMITTER_PIN, OUTPUT);
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
  pinMode(BTN, INPUT_PULLUP);
  pinMode(BTN_CRUCETA, INPUT_PULLUP);
}
/**
 * Registra el pin PWM TURBINA
 */
void inicia_turbina() {
  //pinMode(MOTOR_SUCCION, OUTPUT);
  esc.attach(MOTOR_SUCCION);
  esc.writeMicroseconds(1000); //Señal a mil (Está detenido) 
}
