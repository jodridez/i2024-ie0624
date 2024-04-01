/*  
Universidad de Costa Rica
Escuela de Ingenieria Electrica

IE-0624 Laboratorio de Microcontroladores

diceCode.c

Autor: Jonathan Rodriguez Hernandez <jonathan.rodriguezhernandez@ucr.ac.cr>.
Carnet: B76490.
Fecha: 30/3/2024.

Descripcion:
  Contiene el codigo necesario para programar un simulador de dado
  en un controlador PIC12f683. El programa recibe una senal, genera un numero 
  aleatorio y enciende los pines necesarios para encender los LEDs que simularan
  el dado.
*/


//Includes
#include <pic14/pic12f683.h> //Archivo de cabecera especifico del microcontrolador
#include <funciones.c>       //Incluye la declaracion y definicios de funciones


/*
Configuracion:
  _WDTE_OFF: Desactiva el watchdog Timer para evitar interrupciones inesperadas.
*/
typedef unsigned int word;
word __at 0x2007 __CONFIG = (_WDTE_OFF);


//main
void main (void){
  //Configuracion de PINES
  TRISIO  = 0b00100000;   //Configura el PIN GP5  como entrada y el resto como salidas
  GPIO    = 0x00;         //Se ponen todas las salidas en bajo
  
  //Variables
  unsigned int time = 100;  //Tiempo de retardo
  unsigned int resultado;   //Almacena el resultado del lanzamiento del dado

  //Loop forever
  while ( 1 ) {
    //Si se recibe señal por el PIN GP5
    if(GP5){
      //Genera un numero aleatorio del 1 al 6
      resultado = rollDice(GP5);

      switch (resultado){
        case 1:
          //Enciende un led por un instante luego lo apaga
          GP0 = 1; //1 LEDs
          delay(time); 
          GP0 = 0;
        break;

        case 2:
          //Enciende 2 led por un instante luego los apaga
          GP1 = 1; //2 LEDs
          delay(time); 
          GP1 = 0; 
        break;

        case 3:
          //Enciende 3 led por un instante luego los apaga
          GP0 = 1; //1 LEDs
          GP1 = 1; //2 LEDs
          delay(time); 
          GP0 = 0;
          GP1 = 0;
        break;
        
        case 4:
          //Enciende 4 led por un instante luego los apaga
          GP1 = 1; //2 LEDs
          GP2 = 1; //2 LEDs
          delay(time); 
          GP1 = 0;
          GP2 = 0;
        break;

        case 5:
          //Enciende 5 led por un instante luego los apaga
          GP0 = 1;  //1 LEDs
          GP1 = 1;  //2 LEDs
          GP2 = 1;  //2 LEDs
          delay(time); 
          GP0 = 0;
          GP1 = 0;
          GP2 = 0;
        break;

        case 6:
          //Enciende 6 led por un instante luego los apaga
          GP1 = 1;  //2 LEDs
          GP2 = 1;  //2 LEDs
          GP3 = 1;  //2 LEDs
          delay(time); 
          GP1 = 0;
          GP2 = 0;
          GP3 = 0;
        break;

        default:
          //Por defecto los leds estan apagados
          GPIO = 0x00;
        break;
      }
    }
    else{
      //Por defecto los leds estan apagados
      GPIO = 0x00;
    }
  }
}