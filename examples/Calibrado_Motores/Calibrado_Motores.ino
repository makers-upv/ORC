#include <HC_SR04.h>
#include <ORC.h>
// Ejemplo Librería para el kit de robótica de la ORC 2.16 Organizada por Makers UPV
// http://www.makersupv.com
// Incluye la librería de Ultrasonidos HC-SR04: http://playground.arduino.cc/Code/SR04
// Codificado por Guillermo Orts y por Germán Torres

//Límites de motores
#define MOT_L_MIN -255
#define MOT_L_MAX 255
#define MOT_R_MIN -255
#define MOT_R_MAX 255
char recibido;
/*
  Esta función espera por puerto serie una cadena de forma:
  FX,Y //Mover hacia delante
  BX,Y //Mover hacia detrás
  dónde X e Y son los valores de los motores izquiero y derecho
  respectivamente para calibrar que el robot vaya recto.
  Los valores se guardan y codifican en las constantes MOT_L_MIN, MOT_L_MAX,
  MOT_R_MIN y MOT_R_MAX.
*/
void setup() {
  //Inicializa la librería
  inicializar();
  Serial.setTimeout(1);//Ponemos el límite de tiempo para el timeout bajo
}

void loop() {
  //Espera a que le llegue algo al puerto serie
  while (!Serial.available());
  recibido = Serial.read(); //Lee el primer caracter

  if (bufferSerie[0] == 'F') //Si detecta "F"
  { int L;
    int R;
    L = Serial.parseInt(','); //Lee primer valor
    R = Serial.parseInt(); //Lee segundo valor
    motores(L, R);
  }
  else if (bufferSerie[0] == 'B')
  {
    int L;
    int R;
    L = Serial.parseInt(','); //Lee primer valor
    R = Serial.parseInt(); //Lee segundo valor
    motores(-L, -R);
  }
}
