/*  
Universidad de Costa Rica
Escuela de Ingenieria Electrica

IE-0624 Laboratorio de Microcontroladores

cruce_semaforos.c

Autores: Jonathan Rodriguez Hernandez <jonathan.rodriguezhernandez@ucr.ac.cr>.
         NOMBRE APELLIDO APELLIDO CORREO
Carnets: B76490.
         carnet.
Fecha: 15/4/2024.

Descripcion:
  Contiene el codigo necesario para desarrollar un cruce de semaforos simplificado
  en un controlador tiny4313.
*/
#include <avr/io.h>
#include <avr/interrupt.h>


//Variables;
int press_boton = 0;
int medio_segundo = 0;
int contador = 0;

// Atencion de interrupciones
ISR(INT0_vect) {
    press_boton = 1; // Se establece la bandera del botón
}

ISR(TIMER1_COMPA_vect) {
    medio_segundo++; // Incrementa el contador de medio segundo
}


//Estados:
typedef enum {
    PASO_VEHICULOS,          // Estado idle LDPV ON, LDVD OFF
    ADVERTENCIA_VEHICULOS,  // Se preciona el boton, LDPV parpadea
    DETENCION_VEHICULOS,    // LDPV se apaga, LDVD se enciende
    PASO_PEATONES,          // LDPP se enciende, LDPD se apaga
    ADVERTENCIA_PEATONES,   // Se acaba el tiempo, LDPP parpadea
    DETENCION_PEATONES      // LDPP se apaga, LDPD se enciende
} Estado;
Estado estado;
//Declaracion de funciones
void FMS();

void configurarPines() {
    DDRB |= 0b00001111; // Configura los pines B0, B1, B2 y B3 como salida, el resto como entrada
    PORTB |= 0b0000000; // Configura los pines en bajo.

}

void configurarInterrupciones() {
    sei(); // Habilita las interrupciones globales
    GIMSK |= (1 << INT0); // Se habilita la interrupción por INT0 (pin D2)
    MCUCR |= (1 << ISC00) | (1 << ISC01); // Configura para que INT0 se dispare por flanco positivo

   // Configura Timer/Counter1 en modo CTC (Clear Timer on Compare Match)
    TCCR1B |= (1 << WGM12); // Modo CTC
    OCR1A = 976; // Valor de comparación para generar una interrupción cada 0.5 segundos (FCPU = 1MHz, prescaler = 1024)
    // Configura el prescaler a 1024
    TCCR1B |= (1 << CS12) | (1 << CS10); //prescaler = 1024
    // Habilita la interrupción por Comparación de salida de Timer/Counter1 (Output Compare Match A)
    TIMSK |= (1 << OCIE1A);
}




int main() {
    configurarPines();
    configurarInterrupciones();
    estado = PASO_VEHICULOS;
    press_boton = 0;
    medio_segundo = 0;
    contador = 0;

    while (1) { 
        FMS();
    }
}



//Maquina de estados
void FMS(){
    switch (estado){
        case PASO_VEHICULOS:
            // LDPV y LDPD encendido, los demas apagados
            PORTB |= 0b0001001;
            if(press_boton){
                PORTB &= 0b11110000; //Apaga todos los LEDs
                estado = ADVERTENCIA_VEHICULOS;
                press_boton = 0;
            }
        break;

        case ADVERTENCIA_VEHICULOS:
            //Enciende LDPD y parpadea LDPV
            PORTB |= (1<<PB3); // Se mantiene encendido
            if (medio_segundo % 2 == 0) {
                PORTB ^= (1 << PB0); // Invierte el estado del pin 0
            }
        break;

        case DETENCION_VEHICULOS:
            //Enciende LDVD y LDPP
        break;

        default:
        break;
    }
}


/*
            //PORTB |= 0b00000000;
            
            //PRUEBA DE FUNCION DE PARPADEO
            if(medio_segundo%2==0){
                PORTB = (1<<PB0);
            }
            else
            {
                PORTB = (0<<PB0);
            }
            

        //Prueba Interrupcion boton
        
        if (press_boton){
            PORTB |= (1 << PB0);
            press_boton = 0;
        }
        


        //Prueba
        
        if (press_boton){
            PORTB |= 0b0001001;
            press_boton = 0;
        }
        */