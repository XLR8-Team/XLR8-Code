/* 
Programa: Control de Motor Brushless con Arduino
Autor:  Humberto Higinio
Web: www.humbertohiginio.com
Canal de Youtube: https://www.youtube.com/user/HHSolis
Video Exclusivo para mi canal de Youtube
Todos los Derechos Reservados - 2019
Código de Dominio Público
*/

#include <Servo.h>  //Usamos la Libreria Servo para el control del ESC

Servo esc; //Creamos una clase Servo con nombre esc

void setup()

{

esc.attach(10); //Especificamos el Pin de Arduino al cual se conectara la señal esc, D8

esc.writeMicroseconds(1000); //Inicializamos la señal a 1000

//Serial.begin(9600);

}

void loop()

{

//int val; //Creamos una variable val

//val= analogRead(A0); //Leemos la entrada desde el pin A0 y lo almacenamos en val

//val= map(val, 0, 1023,1000,2000); //Mapeamos val a un minimo y maximo(Realizar cambios si es necesario) 

esc.writeMicroseconds(1200); //Usamos val como la señal para esc

}
