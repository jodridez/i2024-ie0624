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
#include <util/delay.h>

 int press_boton = 0; //Indica si se presiono un boton
 int medio_segundo = 0; //Lleva la cuenta de los medios segundos

void configurarPines() {
    DDRB |= 0b00001111; // Configura los pines B0, B1, B2 y B3 como salida, el resto como entrada
    PORTB |= 0b00000010; // Configura el pin B1 en alto, y el resto en bajo.
}

void configurarInterrupciones() {
    sei(); // Habilita las interrupciones globales
    GIMSK |= (1 << INT0); // Se habilita la interrupción por INT0 (pin D2)
    MCUCR |= (1 << ISC01) | (1 << ISC00); // Configura para que INT0 se dispare por flanco positivo

   // Configura Timer/Counter1 en modo CTC (Clear Timer on Compare Match)
    TCCR1B |= (1 << WGM12); // Modo CTC
    OCR1A = 976; // Valor de comparación para generar una interrupción cada 0.5 segundos (FCPU = 1MHz, prescaler = 1024)
    // Configura el prescaler a 1024
    TCCR1B |= (1 << CS12) | (1 << CS10); //prescaler = 1024
    // Habilita la interrupción por Comparación de salida de Timer/Counter1 (Output Compare Match A)
    TIMSK |= (1 << OCIE1A);
}

ISR(INT0_vect) {
    press_boton = 1; // Se establece la bandera del botón
}

ISR(TIMER1_COMPA_vect) {
    medio_segundo++; // Incrementa el contador de medio segundo
}

int main() {
    configurarPines();
    configurarInterrupciones();

    while (1) {
        PORTB |= (1 << PB1); // Enciende luz verde
        if (medio_segundo >= 4) {
            PORTB |= (1 << PB0); // Enciende luz roja de abajo
             // Reinicia el contador de medio segundo
            _delay_ms(500); // Espera 500 ms
            PORTB &= ~(1 << PB0); // Apaga luz roja de abajo
            //medio_segundo = 0;
        }
        if (press_boton == 1) { // Si se presionó el botón
            PORTB &= ~(1 << PB0); // Apaga la luz roja de abajo
            _delay_ms(500); // Espera 500 ms
            press_boton = 0; // Reinicia la bandera del botón
        }
    }

    return 0;
}
