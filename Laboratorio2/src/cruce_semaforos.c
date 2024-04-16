/*  
Universidad de Costa Rica
Escuela de Ingenieria Electrica

IE-0624 Laboratorio de Microcontroladores

cruce_semaforos.c

Autores: Jonathan Rodriguez Hernandez <jonathan.rodriguezhernandez@ucr.ac.cr>.
         Diego José Acosta Obando <diego.acostaobando@ucr.ac.cr>
Carnets: B76490.
         C00041.
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
    OCR1A = 976; // Valor de comparación para generar una interrupción cada 0.5 segundos (FCPU = 8MHz, prescaler = 1024)
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
            if(press_boton&&medio_segundo >= 150){
                PORTB &= 0b11110000; //Apaga todos los LEDs
                medio_segundo = 0;
                estado = ADVERTENCIA_VEHICULOS;
                press_boton = 0;
            }
        break;

        case ADVERTENCIA_VEHICULOS:
            PORTB |= (1 << PB3); // Enciende el pin 3
        
            // Parpadeo del pin PB0
            if (medio_segundo % 2 == 0) {
                PORTB ^= (1 << PB0); // Invierte el estado del pin 0
            }
            
            //Tiempo para necesario para que se 3 segundos
            if (medio_segundo == 18) {
                PORTB &= 0b11110000; // Apaga LEDs
                medio_segundo = 0;
                estado = DETENCION_VEHICULOS;
            }
        break;

        case DETENCION_VEHICULOS:
            //Enciende LDVD y LDPP
            PORTB |= 0b0001010;
            // Tiempo necesario para que transcurra un segundo
            if(medio_segundo == 15){
                PORTB &= 0b11110000; // Apaga LEDs
                medio_segundo = 0;
                estado = PASO_PEATONES;
            }
        break;

        case PASO_PEATONES:
            // Enciende LDVD y LDPP
            PORTB |= 0b0000110;
            // Tiempo necesario para que transcuran 10s
            if(medio_segundo == 150){
                PORTB &= 0b11110000; // Apaga LEDs
                medio_segundo = 0;
                estado = ADVERTENCIA_PEATONES;
            }
        break;

        case ADVERTENCIA_PEATONES:
            //Enciende LDVD y LDPP parpadea
            PORTB |= (1 << PB1); // Enciende el pin 0
        
            // Parpadeo del pin PB0
            if (medio_segundo % 2 == 0) {
                PORTB ^= (1 << PB2); // Invierte el estado del pin 2
            }
            
            //Tiempo para necesario para que se 3 segundos
            if (medio_segundo == 18) {
                PORTB &= 0b11110000; // Apaga LEDs
                medio_segundo = 0;
                estado = PASO_VEHICULOS;
            }
        break;

        case DETENCION_PEATONES:
            //Enciende LDVD y LDPD
            PORTB |= 0b0001001;
            // Tiempo necesario para que transcurra un segundo
            if(medio_segundo == 15){
                PORTB &= 0b11110000; // Apaga LEDs
                medio_segundo = 0;
                estado = PASO_PEATONES;
            }
        
        default:
            PORTB |= 0b0000000;
        break;
    }
}
