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

// PULSADORES
#define BUTTON_1      EXP_P7
#define GET_B1        get_exp_value(BUTTON_1)
#define BUTTON_2      EXP_P6
#define GET_B2        get_exp_value(BUTTON_2)
char b1 = 0;
char b2 = 0;

void leer_pulsadores(void) {
  static unsigned int cont_b1 = 0;
  static unsigned int cont_b2 = 0;
  
  if(GET_B1 == LOW)
  {
    if(cont_b1 >= 20)
      b1 = HIGH;
    else
      cont_b1 += periodo;
  }
  else
  {
    b1 = LOW;
    cont_b1 = 0;
  }

  if(GET_B2 == LOW)
  {
    if(cont_b2 >= 20)
      b2 = HIGH;
    else
      cont_b2 += periodo;
  }
  else
  {
    b2 = LOW;
    cont_b2 = 0;
  }
}
