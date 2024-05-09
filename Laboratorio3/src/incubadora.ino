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

// Falta funcion de los switches, funcion de comunicacion serial y configuracion del controlador PID

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

// Controlador PID
float setpoint; // Temperatura deseada en C, punto de operacion
float input; // Temperatura actual en C
float output; // Salida del controlador PID, senal de control



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


// Funcion lectura de  potenciometro
float leer_pot(){
  // Se debe convertir la tension medida a una temperatura en C
  // Temperatura de operacion en el rango [20, 80] C
  float lectura = analogRead(potenciometro); // Leer la tension del potenciometro
  // Se normiliza la tension a un rango de [0, 1]
  float lectura_maxima = 1022.0; // Valor maximo de la lectura
  float lectura_normalizada = lectura/lectura_maxima; // Normalizar la lectura
  float setpoint_min = 20.0; // Temperatura minima de operacion
  float setpoint_max = 80.0; // Temperatura maxima de operacion
  float setpoint = setpoint_min + (setpoint_max - setpoint_min)*lectura_normalizada; // setpoint en el rango [20, 80] C
  
  return setpoint;
}

// Funcion de encedido de luces LED segun la temperatura sensada en el rango [30,42] C
void encedido_leds(float input){ 
  if(input<30){ // Temperatura menor a 30 C
    digitalWrite(led_azul, HIGH); // Encender el led azul
    digitalWrite(led_verde, LOW); 
    digitalWrite(led_rojo, LOW);
  }

  else if(input>=30 && input<=42){ // Temperatura en el rango [30,42] C
    digitalWrite(led_azul, LOW);
    digitalWrite(led_verde, HIGH); // Encender el led verde
    digitalWrite(led_rojo, LOW);
  }

  else if(input>42){ // Temperatura mayor a 42 C
    digitalWrite(led_azul, LOW);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_rojo, HIGH); // Encender el led rojo
  }
  
  else{ // Apagar todos los leds
    digitalWrite(led_azul, LOW);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_rojo, LOW);
  }
}

// Funcion para mostrar la informacion en la pantalla LCD
void pantalla_lcd(int setpoint, float input, float output) {
  lcd.setCursor(0, 0); // Escribe en la linea 0
  lcd.print("SP: "); // Temperatura de operaci´on
  lcd.print(setpoint);


  lcd.setCursor(0, 1); // Escribe en la linea 1
  lcd.print("T: "); // Temperatura sensada
  lcd.print(input);

  lcd.setCursor(0, 2); // Escribe en la linea 2
  lcd.print("PID: "); // Salida del controlador PID
  lcd.print(output);
}

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
  //leer_potenciometro = analogRead(potenciometro); // Leer el valor del potenciometro
  leer_switch_pantalla = analogRead(switch_pantalla); // Leer el estado del switch de la pantalla
  leer_switch_serial   = analogRead(switch_serial); // Leer el estado del switch de la comunicacion serial
  
  // Valores de prueba
  setpoint = leer_pot(); // Temperatura deseada en C
  input = 35; // Temperatura actual en C
  output = 100; // Salida del controlador PID en W
  
  // Se prueban las funciones
  encedido_leds(setpoint);
  pantalla_lcd(setpoint, input, output);
}
