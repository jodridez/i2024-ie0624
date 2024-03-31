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
//Archivo de cabecera especifico del microcontrolador
#include <pic14/pic12f683.h>


/*
Configuracion:

*/
typedef unsigned int word;
//word __at 0x2007 __CONFIG = ();


//Declaracion de funciones
void main (void);
void delay (unsigned int tiempo);


//Definicion de funciones
void main (void){

}

void delay (unsigned int tiempo){
  
}
