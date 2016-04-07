#include <HC_SR04.h>
#include <ORC.h>
// Ejemplo Librería para el kit de robótica de la ORC 2.16 Organizada por Makers UPV
// http://www.makersupv.com
// Incluye la librería de Ultrasonidos HC-SR04: http://playground.arduino.cc/Code/SR04
// Codificado por Guillermo Orts y por Germán Torres

//Límites de motores
const int MOT_L_MIN=-255;
const int MOT_L_MAX=255;
const int MOT_R_MIN=-255;
const int MOT_R_MAX=255;

const int limite=512; //Límite para distinguir línea de blanco

#define tipo_sensor 0//0: Colores/1: IR Lineas analógico/2: Líneas digital/3: Ultrasonidos

void setup() {
  //Inicializa la librería
  inicializar();
  Serial.println("Calibrado de Sensores");
}

void loop() {
switch(tipo_sensor)
{
case 0: //Colores
int* colores;
colores=lee_color();
Serial.print("R: ");
Serial.print(colores[0]);
Serial.print(" G: ");
Serial.print(colores[1]);
Serial.print(" B: ");
Serial.print(colores[2]);
break;

case 1: //Linea analógico
int* linea;
linea=lee_linea();
Serial.print("Sensor L:");
Serial.print(linea[0]);
Serial.print(" Sensor R:");
Serial.print(linea[1]);
break;

case 2:  //Linea digital
bool* linea_b;
linea_b=lee_linea_bin(limite);
Serial.print("Sensor L:");
Serial.print(linea_b[0]);
Serial.print(" Sensor R:");
Serial.print(linea_b[1]);
break;

case 3://Distancia
int distancia=0;
distancia=lee_distancia();
Serial.print("Distancia: ")
Serial.print(distancia);
Serial.print(" cm");
break;
}
Serial.println();
delay(1000); //Espera 1 segundo
}
