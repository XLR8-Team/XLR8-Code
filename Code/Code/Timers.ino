/**
 * Función para configurar el Timer 2 para cálculo de posición y PID
 */
 /*
void inicia_timer_PID() {
  Timer1.initializate(500);
  Timer1.attachInterrupt(handler_timer_PID());
}*/

/**
 * Función a la que llama el Timer 2 para realizar el cálculo de posición y PID
 */
void handler_timer_PID() {
  if (timerPID_pause) {
    return;
  }
  posicionActual = calcular_posicion(posicionActual);
  correccion = calcular_PID(posicionActual);
  dar_velocidad(correccion);
}

/**
 * Función para pausar el Timer encargado del PID
 */
void pausa_timer_PID() {
  timerPID_pause = true;
  errorAnterior = 0;
  correccion = 0;
  integralErrores = 0;
  velocidad = 0;
  
  /*
  velocidadMs = 0;
  velocidadMsIdealBase = 0;
  velocidadMsIdeal = 0;
  */
  esc.writeMicroseconds(1000);
  dar_velocidad(0);
}
