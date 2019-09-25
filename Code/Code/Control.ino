/**
 * Función para calcular a posición para cada tipo de pista.
 * @param  ultimaPosicion Última posición calculada para usar en caso de pérdida de pista
 * @return [int]          Posición actual sobre la línea
 */
int calcular_posicion(int ultimaPosicion) {
  switch (PISTA) {
  case MODO_LINEA:
    return calcula_posicion_linea(ultimaPosicion);
    break;
  }
  return 0;
}

/**
 * Función para realizar el cálculo de la posición en base a la lectura de los sensores.
 * @param  ultimaPosicion Última posición calculada para usar en caso de pérdida de línea
 * @return [int]           Posición actual sobre la línea
 */
int calcula_posicion_linea(int ultimaPosicion) {
  lectura_sensores_calibrados();

  unsigned long sumaSensoresPonderados = 0;
  unsigned long sumaSensores = 0;
  int sensoresDetectando = 0;
  bool detectandoAnterior = false;

  for (int sensor = 0; sensor < NUMERO_SENSORES; sensor++) {
    if (valoresSensores[sensor] > umbralesCalibracionSensores[sensor]) {
      sensoresDetectando++;
    }
    // Realiza los cálculos para la posición
    sumaSensoresPonderados += (sensor + 1) * valoresSensores[sensor] * 1000L;
    sumaSensores += (long)valoresSensores[sensor];
  }
  
  int pos;
  if (sensoresDetectando > 0) {
    pos = ((sumaSensoresPonderados / sumaSensores) - (NUMERO_SENSORES + 1) * (float)(1000 / 2));
  } else {
    pos = (ultimaPosicion > 0) ? (1000 * (NUMERO_SENSORES + 1) / 2) : -(1000 * (NUMERO_SENSORES + 1) / 2);
  }
  return map(pos, -6500, 6500, -1000, 1000);
}

/**
 * Función para calcular la corrección a aplicar a los motores para mantenerse en la posición deseada de la pista.
 * @param  posicionActual Posición actual sobre la pista.
 * @return [int]          Corrección que se debe aplicar al control de la velocidad.
 */
int calcular_PID(int posicionActual) {
  float p = 0;
  float i = 0;
  float d = 0;
  int error = 0;
  error = posicionIdeal - posicionActual;

  p = kp * error;
  if (error < 100) {
    integralErrores += error;
    i = ki * integralErrores;
  } else {
    i = 0;
    integralErrores = 0;
  }
  d = kd * (error - errorAnterior);
  errorAnterior = error;
  return p + i + d;
}

/**
 * Función para asignar velocidad a los motores teniendo en cuenta la corrección calculada por el PID
 * @param correccion Parámetro calculado por el PID para seguir la posición deseada en la pista
 */
void dar_velocidad(int correccion) {
  if (velocidad > 200) {
    velocidad = 200;
    //set_color_RGB(0, 0, 255); secuencia leds!!
  }
  int velocidadIzquierda = velocidad;
  int velocidadDerecha = velocidad;
  if (!timerPID_pause) {
    velocidadIzquierda = velocidadIzquierda - correccion;
    velocidadDerecha = velocidadDerecha + correccion;
  }

  int MOTOR_DERECHO_ADELANTE_STATE = HIGH;
  int MOTOR_DERECHO_ATRAS_STATE = LOW;
  int MOTOR_IZQUIERDO_ADELANTE_STATE = HIGH;
  int MOTOR_IZQUIERDO_ATRAS_STATE = LOW;

  if (velocidadDerecha > velocidadMaxima) {
    velocidadDerecha = velocidadMaxima;
  } else if (velocidadDerecha < 0) {
    velocidadDerecha = abs(velocidadDerecha);
    if (velocidadDerecha > velocidadMaxima) {
      velocidadDerecha = velocidadMaxima;
    }
    MOTOR_DERECHO_ADELANTE_STATE = LOW;
    MOTOR_DERECHO_ATRAS_STATE = HIGH;
  }
  if (velocidadIzquierda > velocidadMaxima) {
    velocidadIzquierda = velocidadMaxima;
  } else if (velocidadIzquierda < 0) {
    velocidadIzquierda = abs(velocidadIzquierda);
    if (velocidadIzquierda > velocidadMaxima) {
      velocidadIzquierda = velocidadMaxima;
    }
    MOTOR_IZQUIERDO_ADELANTE_STATE = LOW;
    MOTOR_IZQUIERDO_ATRAS_STATE = HIGH;
  }

  digitalWrite(MOTOR_DERECHO_ADELANTE, MOTOR_DERECHO_ADELANTE_STATE);
  digitalWrite(MOTOR_DERECHO_ATRAS, MOTOR_DERECHO_ATRAS_STATE);
  digitalWrite(MOTOR_IZQUIERDO_ADELANTE, MOTOR_IZQUIERDO_ADELANTE_STATE);
  digitalWrite(MOTOR_IZQUIERDO_ATRAS, MOTOR_IZQUIERDO_ATRAS_STATE);

  analogWrite(MOTOR_DERECHO_PWM, velocidadDerecha);
  analogWrite(MOTOR_IZQUIERDO_PWM, velocidadIzquierda);
}
