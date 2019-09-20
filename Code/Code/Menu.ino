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
