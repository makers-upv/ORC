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
int limites_motores[4];

void setup() {
  //Inicializa la librería
  inicializar();
  limites_motores[0]=MOT_L_MIN;  //Metemos en el vector de límites los límites de los motores;
  limites_motores[1]=MOT_L_MAX;
  limites_motores[2]=MOT_R_MIN;
  limites_motores[3]=MOT_R_MAX;
}

void loop() {
espera();
while(1)
{
//Código aquí:
}
}
