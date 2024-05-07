/*  
Universidad de Costa Rica
Escuela de Ingenieria Electrica

IE-0624 Laboratorio de Microcontroladores

incubadora.ino

Autores: Jonathan Rodriguez Hernandez <jonathan.rodriguezhernandez@ucr.ac.cr>.
         Diego José Acosta Obando <diego.acostaobando@ucr.ac.cr>
Carnets: B76490.
         C00041.
Fecha: 8/5/2024.

Descripcion:
  Contiene el codigo necesario para desarrollar una incubadora de huevos automatica basada en el Arduino UNO
  Sensa la temperatura cuyo rango deseado de operacion es de [30,42] C
  Visualiza la temperatura en una pantalla LCD PCD8544
  Incluye la funcion de comunicacion con una PC para guardar un registro de estos datos en un archivo de
  texto plano
*/

// Imports
#include <PCD8544.h> // Para utilizar la pantalla LCD PCD8544
#include <PID_v1.h> // Para implementar el controlador PID 

// Declaracion de variables globales
// Pines de entrada analogica
int potenciometro   = 0; // Potenciometro conectado al PIN A0
int switch_pantalla = 4; // Pin para el switch de encendidio de la pantalla conectado al pin A4
int switch_serial   = 5; // Pin para el switch de comunicacion serial conectado al pin A5
// Pines de salida digital
int led_azul  = 8;  // Led azul conectado al pin D8
int led_verde = 12; // Led verde conectado al pin D12
int led_rojo  = 13; // Led rojo conectado al pin D13
// Variables de lectura de los pines
int leer_potenciometro;
int leer_switch_pantalla;
int leer_switch_serial;
// Pantalla LCD
PCD8544 lcd; //Instancia de la pantalla LCD

// Declaracion de funciones propias
/*
∗ Esta funcion simula la planta/proceso
∗ @param Q: Entrada de calor en Watts (o J/s). Para convertir la salida en temperatura del control PID(Output) puede utilizar:
∗ float TempWatts = (int)Output ∗ 20.0 / 255;
∗ @return T: Temperatura de salida en la planta
*/
float simPlanta(float Q) {
  // simula un bloque de aluminio de 1x1x2cm con un calentador y con enfreamiento pasivo
  // float C = 237; // W/mK coeficiente de conductividad termica para el Aluminio
  float h     = 5;    // W/m2K coeficiente de conveccion termica para el Aluminio
  float Cps   = 0.89; // J/gC
  float area  = 1e-4; // m2 area por conveccion
  float masa  = 10 ;  // g
  float Tamb  = 25;   // Temperatura ambiente en C
  static float T = Tamb; // Temperatura en C
  static uint32_t last = 0;
  uint32_t interval = 100; // ms
  if ( millis () - last >= interval) {
    last += interval;
    // 0−transferencia de calor
    T = T + Q * interval / 1000 / masa / Cps - (T - Tamb) * area * h;
  }
  return T;
}


// Falta funcion de impresion en la pantalla lcd,  funcion de comunicacion serial y configuracion del controlador PID

// Inicializacion / Configuracion de perifericos
void setup() {
  // put your setup code here, to run once:
  lcd.begin(); // Iniciar la pantalla LCD

  //Serial.begin(115200); // Open serial communications and wait for port to open:

  // Los pines analogicos se configuran automaticamente como entradas
  // Los pines digitales se configuran con pinmode().
  pinMode(led_azul, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(led_rojo,   OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  leer_potenciometro = analogRead(potenciometro); // Leer el valor del potenciometro
  leer_switch_pantalla = analogRead(switch_pantalla); // Leer el estado del switch de la pantalla
  leer_switch_serial   = analogRead(switch_serial); // Leer el estado del switch de la comunicacion serial
}
