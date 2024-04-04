/*  
Universidad de Costa Rica
Escuela de Ingenieria Electrica

IE-0624 Laboratorio de Microcontroladores

main.c

Autor: Jonathan Rodriguez Hernandez <jonathan.rodriguezhernandez@ucr.ac.cr>.
Carnet: B76490.
Fecha: 3/4/2024.

Descripcion:
  Contiene el codigo necesario para programar un simulador de dado
  en un controlador PIC12f683. El programa recibe una senal, genera un numero 
  aleatorio por software mediante contadores y enciende los pines necesarios para encender los LEDs 
  que simulan la cara del dado resultante. Los pines encienden la siguiente cantidad de LEDS:
  GPO: 1 LED
  GP1: 2 LEDs
  GP1: 2 LEDs
  GP4: 1 LED
*/


//Includes
#include <pic14/pic12f683.h> //Archivo de cabecera especifico del microcontrolador


/*Declaracion de funciones:
Como solo se uso una funcion se pondra en este mismo archivo.
delay: Funcion facilitada por el profesor en el ejemplo 
de hola PIC, genera un retardo.
*/
void delay (unsigned int tiempo);

/*
Configuracion:
  _WDTE_OFF: Desactiva el watchdog Timer para evitar interrupciones inesperadas.
*/
typedef unsigned int word;
word __at 0x2007 __CONFIG = (_WDTE_OFF);


//main
void main (void)
{
  //Configuracion de PINES
  //El orden de los pines es el siguiente: 0b00543210
  TRISIO  = 0b00100000;   //Configura el PIN GP5  como entrada y el resto como salidas
  GPIO    = 0x00;         //Se ponen todas las salidas en bajo
  
  //Variables
  unsigned int time       = 400;  //Tiempo de retardo
  unsigned int resultado  = 0;    //Almacena el resultado del lanzamiento del dado
  unsigned int caras      = 6;    //Almacena el numero de caras del dado
  
  //Loop forever
  while ( 1 )
  {
    //Si se recibe señal por el PIN GP5. Este es nuestro boton de entrada
    if(GP5)
    { 
      /*
      Este segmento genera el numero aleatorio, se usa el metodo por sofware mediante contadores.
      Segun el tiempo que se mantenga presionado el boton de entrada, ira contando hasta que 
      se deje de presionar
      */
      while (GP5 == 1)
      {
        resultado++;
        if (resultado>caras)
        {
          resultado = 0;
        }
      }
      
      /*
      Este segmento controla el encendido de los LEDs segun el resultado
      */
      switch (resultado)
      {
        case 1:
          //Enciende un LED por un instante luego lo apaga
          GP0 = 1; //1 LEDs
          delay(time); 
          GP0 = 0;
        break;

        case 2:
          //Enciende 2 LEDs por un instante luego los apaga
          GP1 = 1; //2 LEDs
          delay(time); 
          GP1 = 0; 
        break;

        case 3:
          //Enciende 3 LEDs por un instante luego los apaga
          //Activa los pines GP0 y GP1
          GPIO = 0b00000011;
          delay(time); 
          GPIO = 0x00;
        break;
        
        case 4:
          //Enciende 4 LEDs por un instante luego los apaga
          //Activa los pines GP1 y GP2
          GPIO = 0b00000110;
          delay(time); 
          GPIO = 0x00;
        break;

        case 5:
          //Enciende 5 LEDs por un instante luego los apaga
          //Activa los pines GP0, GP1 y GP2
          GPIO = 0b00000111;
          delay(time); 
          GPIO = 0x00;
        break;

        case 6:
          //Enciende 6 LEDs por un instante luego los apaga
          //Activa los pines GP0, GP1, GP2, GP4
          GPIO = 0b00010111;
          delay(time); 
          GPIO = 0x00;
        break;

        default:
          //Por defecto los LEDs estan apagados
          GPIO = 0x00;
        break;
      }
    }
    else
    {
      //Por defecto los LEDss estan apagados
      GPIO = 0x00;
    }
  }
}



//Defincion de funciones
void delay (unsigned int tiempo){
  unsigned int
  i ;
  unsigned int j ;
  for( i =0; i <tiempo ; i ++)
  for( j =0; j <1275; j ++);
}