/**
 * Función para calcular a posición para cada tipo de pista.
 * @param  ultimaPosicion Última posición calculada para usar en caso de pérdida de pista
 * @return [int]          Posición actual sobre la línea
 
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
 * @param  position Posición actual sobre la pista.
 * @return [int]          Corrección que se debe aplicar al control de la velocidad.
 */
 int calcular_PID(uint16_t posicionActual) {
  
  int error = posicionIdeal - posicionActual;

  error_acumulado += error;
  //[-1000,1000]
  error_acumulado = min(max(-1000, error_acumulado), 1000);

  float PID_proporcional = kp * error;
  float PID_integral = ki * error_acumulado;
  float PID_derivativo = kd * (error - error_anterior);

  int correccion_pid = PID_proporcional + PID_integral + PID_derivativo;
  error_anterior = error;

   return correccion_pid;
  
}

/**
 * Función para asignar velocidad a los motores teniendo en cuenta la corrección calculada por el PID
 * @param correccion Parámetro calculado por el PID para seguir la posición deseada en la pista
 */
void dar_velocidad(int correccion) {

  //  Inicializa los motores a estado parado
  digitalWrite(MOTOR_DERECHO_ADELANTE, HIGH);
  digitalWrite(MOTOR_DERECHO_ATRAS, LOW);
  digitalWrite(MOTOR_IZQUIERDO_ADELANTE, HIGH);
  digitalWrite(MOTOR_IZQUIERDO_ATRAS, LOW);

  int velocidadIzquierda = velocidad - correccion;
  int velocidadDerecha = velocidad + correccion;

  // Nunca menor que 0 ni mayor que 255
  velocidadIzquierda = min(max(0, velocidadIzquierda), 255);
  velocidadDerecha = min(max(0, velocidadDerecha), 255);
  analogWrite(MOTOR_IZQUIERDO_PWM, velocidadIzquierda);
  analogWrite(MOTOR_DERECHO_PWM, velocidadDerecha);
 
  
 /*
  if (velocidad > 200) {
    velocidad = 200;
    //set_color_RGB(0, 0, 255); secuencia leds!!
  }
  int velocidadIzquierda = velocidad;
  int velocidadDerecha = velocidad;
  //if (!timerPID_pause) {
  //  velocidadIzquierda = velocidadIzquierda - correccion;
  //  velocidadDerecha = velocidadDerecha + correccion;
  //}

  velocidadIzquierda = velocidadIzquierda - correccion;
  velocidadDerecha = velocidadDerecha + correccion;
  
  int MOTOR_DERECHO_ADELANTE_STATE = LOW;
  int MOTOR_DERECHO_ATRAS_STATE = HIGH;
  int MOTOR_IZQUIERDO_ADELANTE_STATE = LOW;
  int MOTOR_IZQUIERDO_ATRAS_STATE = HIGH;

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
*/
  
}
