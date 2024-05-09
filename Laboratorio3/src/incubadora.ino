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

#include <PCD8544.h> // Para utilizar la pantalla LCD PCD8544
#include <PID_v1.h>  // Para implementar el controlador PID 
#include <Arduino.h> // Incluido para compatibilidad en compilación fuera del IDE de Arduino

// Declaración de variables globales y pines
int potenciometro = 0;           // Potenciómetro conectado al PIN A0
int switch_pantalla = 4;         // Pin para el switch de encendido de la pantalla, conectado al pin A4
int switch_serial = 5;           // Pin para el switch de comunicación serial, conectado al pin A5
int led_azul = 8;                // LED azul conectado al pin D8
int led_verde = 12;              // LED verde conectado al pin D12
int led_rojo = 13;               // LED rojo conectado al pin D13

// Variables para el controlador PID y pantalla
float setpoint, input, output;   // Variables para el control PID
PCD8544 lcd;                     // Instancia de la pantalla LCD

// Función que simula la planta/proceso
float simPlanta(float Q) {
  float h = 5;           // Coeficiente de convección térmica
  float Cps = 0.89;      // Calor específico
  float area = 1e-4;     // Área en metros cuadrados
  float masa = 10;       // Masa en gramos
  float Tamb = 25;       // Temperatura ambiente en °C
  static float T = Tamb; // Temperatura inicial
  static uint32_t last = 0;
  uint32_t interval = 100; // Intervalo de tiempo en ms
  if (millis() - last >= interval) {
    last += interval;
    T = T + Q * interval / 1000 / masa / Cps - (T - Tamb) * area * h;
  }
  return T;
}

// Función para leer el setpoint del potenciómetro
float leer_pot() {
  float lectura = analogRead(potenciometro);
  float lectura_maxima = 1023.0; // Valor máximo para ADC de 10 bits
  float setpoint_min = 20.0;     // Temperatura mínima de operación
  float setpoint_max = 80.0;     // Temperatura máxima de operación
  return setpoint_min + (setpoint_max - setpoint_min) * (lectura / lectura_maxima);
}

// Función para controlar los LEDs en base a la temperatura
void encedido_leds(float temp) {
  if (temp < 30) {
    digitalWrite(led_azul, HIGH);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_rojo, LOW);
  } else if (temp <= 42) {
    digitalWrite(led_azul, LOW);
    digitalWrite(led_verde, HIGH);
    digitalWrite(led_rojo, LOW);
  } else {
    digitalWrite(led_azul, LOW);
    digitalWrite(led_verde, LOW);
    digitalWrite(led_rojo, HIGH);
  }
}

// Función para mostrar la información en la pantalla LCD
void pantalla_lcd(float setpoint, float input, float output, int modo) {
  if (modo == LOW) { // Modo activado cuando el switch está presionado
    lcd.setPower(HIGH);
    lcd.setCursor(0, 0);
    lcd.print("SP: ");
    lcd.print(setpoint);
    lcd.setCursor(0, 1);
    lcd.print("T: ");
    lcd.print(input);
    lcd.setCursor(0, 2);
    lcd.print("PID: ");
    lcd.print(output);
  } else {
    lcd.setPower(LOW); // Apagar la pantalla LCD
  }
}

// Configuración inicial de pines y comunicación
void setup() {
  pinMode(potenciometro, INPUT);
  pinMode(switch_pantalla, INPUT_PULLUP);
  pinMode(switch_serial, INPUT_PULLUP);
  pinMode(led_azul, OUTPUT);
  pinMode(led_verde, OUTPUT);
  pinMode(led_rojo, OUTPUT);
  lcd.begin();
  Serial.begin(9600);
}

// Loop principal donde se ejecutan todas las operaciones
void loop() {
  int estadoSwitchPantalla = digitalRead(switch_pantalla);
  int estadoSwitchSerial = digitalRead(switch_serial);

  setpoint = leer_pot();   // Actualizar el setpoint a partir del potenciómetro
  input = simPlanta(output); // Obtener la temperatura actual de la simulación
  output = 100;  // Este debería ser calculado por el control PID real

  encedido_leds(input);  // Actualizar el estado de los LEDs
  pantalla_lcd(setpoint, input, output, estadoSwitchPantalla); // Actualizar la pantalla LCD según el switch

  // Comunicación con la PC si el switch está activado
  if (estadoSwitchSerial == LOW) {
    Serial.print("Setpoint: ");
    Serial.print(setpoint);
    Serial.print(", Temperatura Actual: ");
    Serial.print(input);
    Serial.print(", Salida PID: ");
    Serial.println(output);
  }
}
