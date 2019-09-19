/**
 * Función de lectura de los sensores de pista.
 * @param  canalMux Canal del Multiplexador correspondiente al sensor que se desea leer.
 * @return [int]    Lectura analógica del sensor indicado.
 */
 /*
<<<<<<< HEAD
int mux_analog_read(byte canalMux) {

  
=======
int mux_analog_read(byte canalMux) {  
>>>>>>> 9611907516e54e0bc31fcff79f61d1f16d25989a
  digitalWrite(MUX_CANAL_0, bitRead(canalMux, 0));
  digitalWrite(MUX_CANAL_1, bitRead(canalMux, 1));
  digitalWrite(MUX_CANAL_2, bitRead(canalMux, 2));
  digitalWrite(MUX_CANAL_3, bitRead(canalMux, 3));
  return analogRead(MUX_LECTURA);
}
*/
/**
 * Función de lectura del sensor indicado mapeando los máximos y mínimos del mismo
 * @param  canalMux Canal del Multiplexador correspondiente al sensor que se desea leer.
 * @param  sensor 	Índice del array de sensores correspondiente al sensor leído actualmente.
 * @return [int]    Lectura analógica del sensor indicado.
 */
<<<<<<< HEAD
 /*
=======
 
>>>>>>> 9611907516e54e0bc31fcff79f61d1f16d25989a
int mux_analog_read_map(byte canalMux, int sensor) {
  digitalWrite(EMITTER_PIN, HIGH);
  int valorRaw = analogRead(canalMux); // SE MODIFICÓ LA FUNCIÓN POR SOLO ANALOGREAD
  valoresSensoresRaw[sensor] = valorRaw;
  return map(valorRaw, valoresCalibracionMinimos[sensor], valoresCalibracionMaximos[sensor], valorCalibradoMinimo, valorCalibradoMaximo);
<<<<<<< HEAD
}*/
=======
  
}
>>>>>>> 9611907516e54e0bc31fcff79f61d1f16d25989a

/**
 * Función de lectura de todos los sensores, mapeando el resultado a los valores máximos y mínimos de la calibración.
 */
void lectura_sensores_calibrados() {
  for (int sensor = 0; sensor < NUMERO_SENSORES; sensor++) {
    valoresSensores[sensor] = mux_analog_read_map(pinesSensores[sensor], sensor);
    if (valoresSensores[sensor] >= /* umbralesCalibracionSensores[sensor] */3000) {
      valoresSensores[sensor] = valorCalibradoMaximo;
    } else if (valoresSensores[sensor] < /* umbralesCalibracionSensores[sensor] */1000) {
      valoresSensores[sensor] = valorCalibradoMinimo;
    }
    if (LINEA == LINEA_BLANCA) {
      valoresSensores[sensor] = valorCalibradoMaximo - valoresSensores[sensor];
    }
  }
}

/**
 * Función de lectura del botón, pasando el valor a lógica positiva.
 * @return [bool]		Indica si el botón está pulsado.
 */
bool btn_pulsado() {
  return digitalRead(BTN);
}
