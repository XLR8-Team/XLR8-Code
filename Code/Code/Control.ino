
/**
 * Función para calcular la corrección a aplicar a los motores para mantenerse en la posición deseada de la pista.
 * @param  position Posición actual sobre la pista.
 * @return [int]          Corrección que se debe aplicar al control de la velocidad.
 */
 /*
 int calcular_PID(int posicionActual) {
 
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
  
}*/

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
  /*
  int velocidadIzquierda = velocidad - correccion;
  int velocidadDerecha = velocidad + correccion;

  // Nunca menor que 0 ni mayor que 255
  velocidadIzquierda = min(max(0, velocidadIzquierda), 255);
  velocidadDerecha = min(max(0, velocidadDerecha), 255);
  analogWrite(MOTOR_IZQUIERDO_PWM, velocidadIzquierda);
  analogWrite(MOTOR_DERECHO_PWM, velocidadDerecha);  */
      int velD = velocidad - correccion;
      int velI = velocidad + correccion;

        //limitamos desbordamientos
       velD = constrain(velD, -255, 255);
       velI = constrain(velI, -255, 255);

      //asiganmos valores a la rueda derecha teniendo en cuenta de que si el valor es negativo va hacia atras
      if (velD >= 0) {
        digitalWrite(MOTOR_DERECHO_ADELANTE, HIGH);
        digitalWrite(MOTOR_DERECHO_ATRAS, LOW);
        analogWrite(MOTOR_DERECHO_PWM, velD);
      } else {
        digitalWrite(MOTOR_DERECHO_ADELANTE, LOW);
        digitalWrite(MOTOR_DERECHO_ATRAS, HIGH);
        analogWrite(MOTOR_DERECHO_PWM, abs(velD));        
      }

      //asiganmos valores a la rueda izquierda teniendo en cuenta de que si el valor es negativo va hacia atras
      if (velI >= 0) {
        digitalWrite(MOTOR_IZQUIERDO_ADELANTE, HIGH);
        digitalWrite(MOTOR_IZQUIERDO_ATRAS, LOW);
        analogWrite(MOTOR_IZQUIERDO_PWM, velI);
      } else {
        digitalWrite(MOTOR_IZQUIERDO_ADELANTE, LOW);
        digitalWrite(MOTOR_IZQUIERDO_ATRAS, HIGH);
        analogWrite(MOTOR_IZQUIERDO_PWM, abs(velI));        
      }
}
