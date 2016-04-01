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

void setup() {
  //Inicializa la librería
  inicializar();
}

void loop() {
espera();

}
