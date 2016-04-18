#include "ORC.h"
HC_SR04 ultrasonido(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO, 0,500);
Servo servoRobot;

//Colores calibrados
volatile int ROJO_CALIBRADO[10];
volatile int VERDE_CALIBRADO[10];
volatile int AZUL_CALIBRADO[10];
volatile int BLANCO_CALIBRADO[10];
volatile int NEGRO_CALIBRADO[10];

void inicializar()
{//Inicializa librerías y servo
  ultrasonido.begin();
  Serial.begin(57600);
  servoRobot.attach(PIN_SERVO);
  servoRobot.write(90);
  //Mapeo de pines
  pinMode(PIN_A_L,OUTPUT);
  pinMode(PIN_A_R,OUTPUT);
  pinMode(PIN_B_L,OUTPUT);
  pinMode(PIN_B_R,OUTPUT);
  pinMode(PIN_COLOR_R,OUTPUT);
  pinMode(PIN_COLOR_G,OUTPUT);
  pinMode(PIN_COLOR_B,OUTPUT);
  pinMode(PIN_LED_1,OUTPUT);
  pinMode(PIN_LED_2,OUTPUT);
  pinMode(PIN_LED_3,OUTPUT);
  pinMode(PIN_COLOR_SENSE,INPUT);
  pinMode(PIN_LINE_L,INPUT);
  pinMode(PIN_LINE_R,INPUT);
  pinMode(PIN_PULSADOR,INPUT_PULLUP);
  pinMode(PIN_FC1,INPUT_PULLUP);
  pinMode(PIN_FC2,INPUT_PULLUP);
//conversor A/D rápido
  #if FASTADC
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  #endif
  //Enciende el LED rojo
LEDs(true,false,false);
}

void LEDs(bool R, bool Y, bool G)
{//Autoexplanatorio
digitalWrite(PIN_LED_1,R);
digitalWrite(PIN_LED_2,Y);
digitalWrite(PIN_LED_3,G);
}

void RGB(bool R, bool G, bool B)
{//Autoexplanatorio
digitalWrite(PIN_COLOR_R,R);
digitalWrite(PIN_COLOR_G,G);
digitalWrite(PIN_COLOR_B,B);
}


void motores(int mot_L, int mot_R, int* limites)
{
  //Restringe el rango de la entrada
  mot_L=constrain(mot_L,-255,255);
  mot_R=constrain(mot_R,-255,255);
  //Mapea la entrada a los rangos máximos de los motores para asegurar proporcionalidad
  int MOT_L_MIN=limites[0];
  int MOT_L_MAX=limites[1];
  int MOT_R_MIN=limites[2];
  int MOT_R_MAX=limites[3];

  mot_L=map(mot_L,-255,255,MOT_L_MIN,MOT_L_MAX);
  mot_R=map(mot_R,-255,255,MOT_R_MIN,MOT_R_MAX);
  //Lógica de decisión:
  if(mot_L>0)
  {
    analogWrite(PIN_A_L,mot_L);
    analogWrite(PIN_A_R,0);
  }
  else if(mot_L==0)
  {
    analogWrite(PIN_A_L,0);
    analogWrite(PIN_A_R,0);
  }
  else
  {
    analogWrite(PIN_A_L,0);
    analogWrite(PIN_A_R,-mot_L);
  }

  if(mot_R>0)
  {
    analogWrite(PIN_B_L,mot_R);
    analogWrite(PIN_B_R,0);
  }
  else if(mot_R==0)
  {
    analogWrite(PIN_B_L,0);
    analogWrite(PIN_B_R,0);
  }
  else
  {
    analogWrite(PIN_B_L,0);
    analogWrite(PIN_B_R,-mot_R);
  }
}


void espera()
{//Se espera a que el usuario pulse el botón
  while(digitalRead(PIN_PULSADOR));
  //Parpadea el LED amarillo 3 veces
LEDs(false,true,false);
delay(500);
LEDs(false,false,false);
delay(500);
LEDs(false,true,false);
delay(500);
LEDs(false,false,false);
delay(500);
LEDs(false,true,false);
delay(500);
LEDs(false,false,false);
delay(500);
//Enciende el LED verde y vuelve al programa
LEDs(false,false,true);
}

int* lee_color()
{/*
  La función se usa de la siguiente manera:
  int* vector_de_colores;
  vector_de_colores=lee_color();
  //A partir de aquí en vector_de_colores están disponibles los valores.
  vector_de_colores[0];//Rojo (entero 0-1023)
  vector_de_colores[1];//Verde (entero 0-1023)
  vector_de_colores[2];//Azul (entero 0-1023)
  */
  //Autoexplanatorio
  static int valores_color[3];
  int retardo_colores=50;
  RGB(true,false,false);
  delay(retardo_colores);
valores_color[0]=analogRead(PIN_COLOR_SENSE);
  RGB(false,true,false);
  delay(retardo_colores);
valores_color[1]=analogRead(PIN_COLOR_SENSE);
  RGB(false,false,true);
  delay(retardo_colores);
valores_color[2]=analogRead(PIN_COLOR_SENSE);
  RGB(false,false,false);
  return valores_color;
}

int* lee_linea()
{ /*
  La función se usa de la siguiente manera:
  int* vector_de_lineas;
  vector_de_lineas=lee_linea();
  //A partir de aquí en vector_de_lineas están disponibles los valores.
  vector_de_lineas[0];//Izquierda (entero 0-1023)
  vector_de_lineas[1];//Derecha (entero 0-1023)
  */
    //Autoexplanatorio
 static int valores_linea[2];
  valores_linea[0]= analogRead(PIN_LINE_L);
  valores_linea[1]= analogRead(PIN_LINE_R);
  return valores_linea;
}
bool* lee_linea_bin(int limite)
{/*
  La función se usa de la siguiente manera:
  bool* vector_de_lineas;
  vector_de_lineas=lee_linea_bin();
  //A partir de aquí en vector_de_lineas están disponibles los valores.
  vector_de_lineas[0];//Izquierda (lógico)
  vector_de_lineas[1];//Derecha (lógico)
  */
    //Autoexplanatorio
  static int valores_linea_bin[2];
  static bool salida_bin[2];
  valores_linea_bin[0]= analogRead(PIN_LINE_L);
  valores_linea_bin[1]= analogRead(PIN_LINE_R);
  if(valores_linea_bin[0]>limite)
  {
    salida_bin[0]=false;
  }
  else{
    salida_bin[0]=true;
  }
  if(valores_linea_bin[1]>limite)
  {
    salida_bin[1]=false;
  }
  else{
    salida_bin[1]=true;
  }
  return salida_bin;
}

int lee_distancia()
{ //Autoexplanatorio
  static float dist_sonar;
ultrasonido.start();
  while(!ultrasonido.isFinished());
      dist_sonar = ultrasonido.getRange();

      return constrain(int(dist_sonar),0,150);
}

int* barrido(int semicono, int muestras)
{ /*
  La función se usa de la siguiente manera:
  int* vector_barrido;
  vector_barrido=barrido(semicono, muestras);
  "semicono" es el ángulo, medido hacia un sólo lado, que barre el servo
  está restringido de 0 a 90º
  "muestras" es el número de medidas que hace en ese ángulo, si se indica 1
  sólo lee un punto hacia delante. Está restringido de 1 a 4.
  //A partir de aquí en vector_barrido están disponibles los valores en el formato:
  vector_barrido[0]= posición en -semicono;
  vector_barrido[1]=distancia en posición -semicono;
  .
  .
  .
  vector_barrido[2*(2*(muestras-1))-2]= posición en +semicono;
  vector_barrido[2*(2*(muestras-1))-1]= distancia en posición -semicono;

  NOTA: el vector que devuelve la función siempre es de tamaño máximo, por lo que
  es posible acceder a las posiciones más allá de las pedidas a la función.
  */
  //Retardos para el programa
  int retardos=200;
  int retardo_inicial=750;
  int retardo_final=350;
  //Límite de valores
  semicono=constrain(semicono,0,90);
  muestras=constrain(muestras,1,4);
  int angulo;
  bool primero=true;
  int k=0;
  static int salida_barrido[14]; //Primer elemento, ángulo, segundo distancia
  //Si sólo se pide una muestra:
if(muestras==1)
{
  servoRobot.write(90);
  delay(retardos);
  salida_barrido[0]=0;
  salida_barrido[1]=lee_distancia();
}
else //Para más de una muestra
{
  for(int i=-(muestras-1);i<muestras;i++)
  {
    angulo=int(90+i*(float)(semicono/(muestras-1)));
    //Debugging
    if(DEBUG_BARRIDO)
    {
      Serial.print("Angulo: ");
      Serial.println(angulo);
    }
    servoRobot.write(angulo);
    //Si es el primero, espera distinto
    if(primero)
    {
      delay(retardo_inicial);
      primero=false;
    }else if(i==(muestras-1))     //Si es el último, espera distinto
    {
      delay(retardo_final);
    }
    else{
    delay(retardos);
  }
    salida_barrido[k]=angulo-90;
    k++;
    salida_barrido[k]=lee_distancia();
    k++;
  }
}
//Vuelve a poner el servo mirando hacia delante
servoRobot.write(90);
  return salida_barrido;
}

int* extremos()
{/*
  La función se usa de la siguiente manera:
  int* vector_extremos;
  vector_extremos=extremos();
  //A partir de aquí en vector_extremos están disponibles los valores en el formato:
  vector_barrido[0]= posición del mínimo;
  vector_barrido[1]=distancia mínima;
  vector_barrido[2]= posición del máximo;
  vector_barrido[3]=distancia máxima;
  */
   static int salida_extremos[4];
  int* valores;
  valores=barrido(90,4);
  int min=500;
  volatile int i_min=0;
  int max=0;
  volatile int i_max=0;
  //Recorremos el barrido
  for(int i=1;i<14;i+=2)
  {
    if(DEBUG_EXTREMOS)
    {
      Serial.println(valores[i]);
    }
    if((valores[i]>max)&&(valores[i]!=-1))
    {
      max=valores[i];
      i_max=i;
    }
   else if((valores[i]<min)&&(valores[i]!=-1))
    {
      min=valores[i];
      i_min=i;
    }
  }
  //Debugging
  if(DEBUG_EXTREMOS)
  {
    Serial.print("I_min: ");
    Serial.print(i_min);
    Serial.print("  I_max: ");
    Serial.println(i_max);
  }
  salida_extremos[0]=valores[i_min-1];
  salida_extremos[1]=min;
  salida_extremos[2]=valores[i_max-1];
  salida_extremos[3]=max;
  return salida_extremos;
}
void inicializar_calibracion_color(int* valores_r,int* valores_g,int* valores_b,int* valores_w,int* valores_black)
{
  ROJO_CALIBRADO[0]=valores_r[0];
  ROJO_CALIBRADO[1]=valores_r[1];
  ROJO_CALIBRADO[2]=valores_r[2];
  VERDE_CALIBRADO[0]=valores_g[0];
  VERDE_CALIBRADO[1]=valores_g[1];
  VERDE_CALIBRADO[2]=valores_g[2];
  AZUL_CALIBRADO[0]=valores_b[0];
  AZUL_CALIBRADO[1]=valores_b[1];
  AZUL_CALIBRADO[2]=valores_b[2];
  BLANCO_CALIBRADO[0]=valores_w[0];
  BLANCO_CALIBRADO[1]=valores_w[1];
  BLANCO_CALIBRADO[2]=valores_w[2];
  NEGRO_CALIBRADO[0]=valores_black[0];
  NEGRO_CALIBRADO[1]=valores_black[1];
  NEGRO_CALIBRADO[2]=valores_black[2];
  //Calculo de estadisticos de calibracion
//Medias
ROJO_CALIBRADO[4]=(ROJO_CALIBRADO[0]+ROJO_CALIBRADO[1]+ROJO_CALIBRADO[2])/3;
VERDE_CALIBRADO[4]=(VERDE_CALIBRADO[0]+VERDE_CALIBRADO[1]+VERDE_CALIBRADO[2])/3;
AZUL_CALIBRADO[4]=(AZUL_CALIBRADO[0]+AZUL_CALIBRADO[1]+AZUL_CALIBRADO[2])/3;
BLANCO_CALIBRADO[4]=(BLANCO_CALIBRADO[0]+BLANCO_CALIBRADO[1]+BLANCO_CALIBRADO[2])/3;
NEGRO_CALIBRADO[4]=(NEGRO_CALIBRADO[0]+NEGRO_CALIBRADO[1]+NEGRO_CALIBRADO[2])/3;
//Maximos
ROJO_CALIBRADO[5]=max(max(ROJO_CALIBRADO[0],ROJO_CALIBRADO[1]),ROJO_CALIBRADO[2]);
VERDE_CALIBRADO[5]=max(max(VERDE_CALIBRADO[0],VERDE_CALIBRADO[1]),VERDE_CALIBRADO[2]);
AZUL_CALIBRADO[5]=max(max(AZUL_CALIBRADO[0],AZUL_CALIBRADO[1]),AZUL_CALIBRADO[2]);
BLANCO_CALIBRADO[5]=max(max(BLANCO_CALIBRADO[0],BLANCO_CALIBRADO[1]),BLANCO_CALIBRADO[2]);
NEGRO_CALIBRADO[5]=max(max(NEGRO_CALIBRADO[0],NEGRO_CALIBRADO[1]),NEGRO_CALIBRADO[2]);
//Minimos
ROJO_CALIBRADO[6]=min(min(ROJO_CALIBRADO[0],ROJO_CALIBRADO[1]),ROJO_CALIBRADO[2]);
VERDE_CALIBRADO[6]=min(min(VERDE_CALIBRADO[0],VERDE_CALIBRADO[1]),VERDE_CALIBRADO[2]);
AZUL_CALIBRADO[6]=min(min(AZUL_CALIBRADO[0],AZUL_CALIBRADO[1]),AZUL_CALIBRADO[2]);
BLANCO_CALIBRADO[6]=min(min(BLANCO_CALIBRADO[0],BLANCO_CALIBRADO[1]),BLANCO_CALIBRADO[2]);
NEGRO_CALIBRADO[6]=min(min(NEGRO_CALIBRADO[0],NEGRO_CALIBRADO[1]),NEGRO_CALIBRADO[2]);
//Rangos
ROJO_CALIBRADO[7]=ROJO_CALIBRADO[5]-ROJO_CALIBRADO[6];
VERDE_CALIBRADO[7]=VERDE_CALIBRADO[5]-VERDE_CALIBRADO[6];
AZUL_CALIBRADO[7]=AZUL_CALIBRADO[5]-AZUL_CALIBRADO[6];
BLANCO_CALIBRADO[7]=BLANCO_CALIBRADO[5]-BLANCO_CALIBRADO[6];
NEGRO_CALIBRADO[7]=NEGRO_CALIBRADO[5]-NEGRO_CALIBRADO[6];
//Desviacion maximo
ROJO_CALIBRADO[8]=ROJO_CALIBRADO[5]-ROJO_CALIBRADO[4];
VERDE_CALIBRADO[8]=VERDE_CALIBRADO[5]-VERDE_CALIBRADO[4];
AZUL_CALIBRADO[8]=AZUL_CALIBRADO[5]-AZUL_CALIBRADO[4];
BLANCO_CALIBRADO[8]=BLANCO_CALIBRADO[5]-BLANCO_CALIBRADO[4];
NEGRO_CALIBRADO[8]=NEGRO_CALIBRADO[5]-NEGRO_CALIBRADO[4];
//Desviacion minima
ROJO_CALIBRADO[9]=ROJO_CALIBRADO[4]-ROJO_CALIBRADO[6];
VERDE_CALIBRADO[9]=VERDE_CALIBRADO[4]-VERDE_CALIBRADO[6];
AZUL_CALIBRADO[9]=AZUL_CALIBRADO[4]-AZUL_CALIBRADO[6];
BLANCO_CALIBRADO[9]=BLANCO_CALIBRADO[4]-BLANCO_CALIBRADO[6];
NEGRO_CALIBRADO[9]=NEGRO_CALIBRADO[4]-NEGRO_CALIBRADO[6];
//Debugging
if(DEBUG_ESTADISTICAS)
{
  Serial.println("Medias");
  Serial.println("R:   G:   B:   W:   K:   ");
  Serial.print(ROJO_CALIBRADO[4]);
  Serial.print("  ");
}
}

int lee_color_calibrado(int tolerancia_color)
{
  int* colores_leidos;
  colores_leidos=lee_color();
  int media=(colores_leidos[0]+colores_leidos[1]+colores_leidos[2])/3;
  int maximo=max(max(colores_leidos[0],colores_leidos[1]),colores_leidos[2]);
  int minimo=min(min(colores_leidos[0],colores_leidos[1]),colores_leidos[2]);
  int rango=maximo-minimo;
  int desv_minimo=media-minimo;
  int desv_maximo=maximo-media;
  char* nombre_colores[5];
  nombre_colores[0]="Rojo" ;
  nombre_colores[1]="Verde" ;
  nombre_colores[2]="Azul" ;
  nombre_colores[3]="Blanco" ;
  nombre_colores[4]="Negro" ;
  int color_salida=-1;
  if(DEBUG_ESTADISTICAS1)
  {
    Serial.print("Media: ");
    Serial.println(media);
    Serial.print("Maximo: ");
    Serial.println(maximo);
    Serial.print("Minimo: ");
    Serial.println(minimo);
    Serial.print("Rango: ");
    Serial.println(rango);
    Serial.print("Desv-min: ");
    Serial.println(desv_minimo);
    Serial.print("Desv-max: ");
    Serial.println(desv_maximo);
    Serial.println("");
  }
  int color_maximo=0;
  for(int i=0;i<3;i++)
  {
    if(maximo==colores_leidos[i])
    {
color_maximo=i;
    }
  }
  if(DEBUG_ESTADISTICAS1)
  {
    Serial.print("El color maximo es: ");
    Serial.println(nombre_colores[color_maximo]);
  }

  //Decisión del color
if((abs(media-ROJO_CALIBRADO[4])<tolerancia_color)&&(abs(maximo-ROJO_CALIBRADO[5])<tolerancia_color)&&(abs(minimo-ROJO_CALIBRADO[6])<tolerancia_color)&&(abs(rango-ROJO_CALIBRADO[7])<tolerancia_color)&&(abs(desv_maximo-ROJO_CALIBRADO[8])<tolerancia_color)&&(abs(desv_minimo-ROJO_CALIBRADO[9])<tolerancia_color))
{
color_salida=0;
}
if((abs(media-VERDE_CALIBRADO[4])<tolerancia_color)&&(abs(maximo-VERDE_CALIBRADO[5])<tolerancia_color)&&(abs(minimo-VERDE_CALIBRADO[6])<tolerancia_color)&&(abs(rango-VERDE_CALIBRADO[7])<tolerancia_color)&&(abs(desv_maximo-VERDE_CALIBRADO[8])<tolerancia_color)&&(abs(desv_minimo-VERDE_CALIBRADO[9])<tolerancia_color))
{
color_salida=1;
}
if((abs(media-AZUL_CALIBRADO[4])<tolerancia_color)&&(abs(maximo-AZUL_CALIBRADO[5])<tolerancia_color)&&(abs(minimo-AZUL_CALIBRADO[6])<tolerancia_color)&&(abs(rango-AZUL_CALIBRADO[7])<tolerancia_color)&&(abs(desv_maximo-AZUL_CALIBRADO[8])<tolerancia_color)&&(abs(desv_minimo-AZUL_CALIBRADO[9])<tolerancia_color))
{
color_salida=2;
}
if((abs(media-BLANCO_CALIBRADO[4])<tolerancia_color)&&(abs(maximo-BLANCO_CALIBRADO[5])<tolerancia_color)&&(abs(minimo-BLANCO_CALIBRADO[6])<tolerancia_color)&&(abs(rango-BLANCO_CALIBRADO[7])<tolerancia_color)&&(abs(desv_maximo-BLANCO_CALIBRADO[8])<tolerancia_color)&&(abs(desv_minimo-BLANCO_CALIBRADO[9])<tolerancia_color))
{
color_salida=3;
}
if((abs(media-NEGRO_CALIBRADO[4])<tolerancia_color)&&(abs(maximo-NEGRO_CALIBRADO[5])<tolerancia_color)&&(abs(minimo-NEGRO_CALIBRADO[6])<tolerancia_color)&&(abs(rango-NEGRO_CALIBRADO[7])<tolerancia_color)&&(abs(desv_maximo-NEGRO_CALIBRADO[8])<tolerancia_color)&&(abs(desv_minimo-NEGRO_CALIBRADO[9])<tolerancia_color))
{
color_salida=4;
}
if(DEBUG_ESTADISTICAS2)
{
  if(color_salida>-1)
  {
  Serial.print("El color es: ");
  Serial.println(nombre_colores[color_salida]);
  }
  else
  {
  Serial.println("No se reconoce el color");
  }
}
return color_salida;
}
