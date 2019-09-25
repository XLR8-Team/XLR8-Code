/**
 * Función a la que llama el Timer 2 para realizar el cálculo de posición y PID
 */
void handler_timer_PID() {
  //if (timerPID_pause) {
  //  return;
  //}
  posicionActual = calcular_posicion(posicionActual);
  correccion = calcular_PID(posicionActual);
  dar_velocidad(correccion);
}
