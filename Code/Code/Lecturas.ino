/**
 * Función de lectura de los sensores de pista.
 * @param  canalMux Canal del Multiplexador correspondiente al sensor que se desea leer.
 * @return [int]    Lectura analógica del sensor indicado.
 */
 
int mux_analog_read_map(byte canalMux, int sensor) {  
  int valorRaw = analogRead(canalMux); // SE MODIFICÓ LA FUNCIÓN POR SOLO ANALOGREAD... mux_analog_read En este caso leemos el pin del sensor
  valoresSensoresRaw[sensor] = valorRaw;
  return map(valorRaw, valoresCalibracionMinimos[sensor], valoresCalibracionMaximos[sensor], valorCalibradoMinimo, valorCalibradoMaximo);
}

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
