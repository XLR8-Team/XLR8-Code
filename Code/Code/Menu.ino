/**
 * Función para modificar la velocidad y la velocidad de succión en función del botón pulsado.
 */



if(millis() - t > periodo)
  {
    t = millis();
    leer_pulsadores();

    switch(estado) {
      case INICIALIZADO:
        if(b1 == HIGH)
        {
          for(int i=0; i<num_sensores; i++)
          {
            negro[i] = 0;
            blanco[i] = 1023;
          }
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
          analogWrite(PWM_IZQ,0);
          analogWrite(PWM_DER,0);
          integral = 0;
          led_g_blink = 1;
        }
        break;
    }

    switch (estado) {
      case CALIBRANDO_SENSORES:
        static int var = 0;

        leer_sensores_linea(ADC_linea);
        var = 0;

        if(despreciar_primeras_lecturas > 0)
          despreciar_primeras_lecturas--;
        else
        {
          for(int i=0; i<num_sensores; i++)
          {
            if(ADC_linea[i] > negro[i])
            {
              negro[i] = ADC_linea[i];
              umbral[i] = (negro[i] + blanco[i])/2;
            }
            if(ADC_linea[i] < blanco[i])
            {
              blanco[i] = ADC_linea[i];
              umbral[i] = (negro[i] + blanco[i])/2;
            }
  
            if(negro[i]-blanco[i]> 100)
              var++;
          }
  
          if(var == num_sensores)
              SET_LED_G_HIGH;
        }

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



 
 /*
void btn_cruceta() {
  bool btnPulsado = true;
  int CRUCETA = calcular_btn_cruceta(analogRead(BTN_CRUCETA));
  switch (CRUCETA) {
  case CRUCETA_ARRIBA:
    velocidad_menu++;
    break;
  case CRUCETA_ABAJO:
    velocidad_menu--;
    break;
  case CRUCETA_DERECHA:
    velocidad_succion_menu++;
    break;
  case CRUCETA_IZQUIERDA:
    velocidad_succion_menu--;
    break;
  default:
    btnPulsado = false;
    break;
  }
  velocidad_menu = constrain(velocidad_menu, 0, NUMERO_VELOCIDADES - 1);
  velocidad_succion_menu = constrain(velocidad_succion_menu, 0, NUMERO_VELOCIDADES_SUCCION - 1);
  if (btnPulsado) {
    switch (CRUCETA) {
    case CRUCETA_ARRIBA:
    case CRUCETA_ABAJO:
      switch (velocidad_menu) {
      case 0:
        set_color_RGB(0, 255, 0);
        velocidadMsIdealBase = 1.0;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      case 1:
        set_color_RGB(63, 189, 0);
        velocidadMsIdealBase = 1.25;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      case 2:
        set_color_RGB(126, 126, 0);
        velocidadMsIdealBase = 1.5;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      case 3:
        set_color_RGB(189, 63, 0);
        velocidadMsIdealBase = 1.75;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      case 4:
        set_color_RGB(255, 0, 0);
        velocidadMsIdealBase = 2.0;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      case 5:
        set_color_RGB(255, 0, 63);
        velocidadMsIdealBase = 2.5;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      case 6:
        set_color_RGB(255, 0, 126);
        velocidadMsIdealBase = 2.75;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      case 7:
        set_color_RGB(255, 0, 189);
        velocidadMsIdealBase = 3.0;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      case 8:
        set_color_RGB(255, 0, 255);
        velocidadMsIdealBase = 3.25;
        kp = 0.15f;
        ki = 0;
        kd = 20.0f;
        break;
      }
      break;
    case CRUCETA_DERECHA:
    case CRUCETA_IZQUIERDA:
      switch (velocidad_succion_menu) {
      case 0:
        velocidadSuccionBase = LIPO == LIPO_2S ? 0 : 0;
        set_color_RGB(0, 255, 0);
        break;
      case 1:
        velocidadSuccionBase = LIPO == LIPO_2S ? 0 : 0;
        set_color_RGB(63, 189, 0);
        break;
      case 2:
        velocidadSuccionBase = LIPO == LIPO_2S ? 25 : 17;
        set_color_RGB(126, 126, 0);
        break;
      case 3:
        velocidadSuccionBase = LIPO == LIPO_2S ? 30 : 20;
        set_color_RGB(189, 63, 0);
        break;
      case 4:
        velocidadSuccionBase = LIPO == LIPO_2S ? 50 : 34;
        set_color_RGB(255, 0, 0);
        break;
      case 5:
        velocidadSuccionBase = LIPO == LIPO_2S ? 80 : 54;
        set_color_RGB(255, 0, 63);
        break;
      case 6:
        velocidadSuccionBase = LIPO == LIPO_2S ? 95 : 64;
        set_color_RGB(255, 0, 126);
        break;
      case 7:
        velocidadSuccionBase = LIPO == LIPO_2S ? 110 : 74;
        set_color_RGB(255, 0, 189);
        break;
      case 8:
        velocidadSuccionBase = LIPO == LIPO_2S ? 130 : 87;
        set_color_RGB(255, 0, 255);
        break;
      }
      break;
    }
    do {
      delay(150);
    } while (calcular_btn_cruceta(analogRead(BTN_CRUCETA)) > 0);
    set_color_RGB(0, 0, 0);
  }
}

/**
 * Función para encontrar el botón pulsado de la cruceta con un +-50 de error en las mediciones

int calcular_btn_cruceta(int valorBtn) {
  for (int combinacion = 0; combinacion < NUMERO_COMBINACIONES; combinacion++) {
    if (valorBtn > (crucetaCombinaciones[combinacion] - 50) && valorBtn < (crucetaCombinaciones[combinacion] + 50)) {
      return crucetaCombinaciones[combinacion];
    }
  }
  return 0;
}

*/
