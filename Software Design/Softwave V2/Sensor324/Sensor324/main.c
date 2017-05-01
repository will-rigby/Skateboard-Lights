/*
 * Sensor324.c
 *
 * Created: 12/02/2017 16:53:57
 * Author : William
 */ 

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

volatile uint8_t pulses = 0;
volatile uint16_t ticks = 0;

void enable_pcint(void){
	PCICR |= (1<<PCIE1); // turn on Pin Change Interrupt Control Register 1 (PORTC)
	PCMSK1 |= (1<<PCINT8); // enable the mask on PORTC0
}

void enable_timer(void){
	/* Generating interrupt every 1ms */
	TCCR0A = (1<<WGM01); // CTC Mode, Normal Port Operation
	TCCR0B = (1<<CS01)|(1<<CS00); // Clock Prescaler /64
	TCNT0 = 0; // set timer value to 0
	OCR0A = 125; // set the output compare register to flag at 125
	TIMSK0 = (1<<OCIE0A); // enable the output compare match A interrupt
}

int main(void)
{
	/* Set PORTB to be output */
	DDRB = 0xFF;
	PORTB = 0x00;
	/* Set PORTC as input and enable pull up resistor */
	DDRC = (0<<PORTC0);
	PORTC = (1<<PORTC0);
	DDRD = (1<<PORTD2);
	
	enable_pcint();
	enable_timer();
	
	sei(); // enable interrupts
    while (1) 
    {
		if(ticks >= 1000){
			PORTB = pulses;
			pulses = 0;
			ticks = 0;
			PORTD = ~(PORTD && 0b100) && 0b100;
		}
    }
}

ISR(PCINT1_vect){
	if(!(PINC & (1<<PORTC0))){
		/* detect falling edge */
		pulses++;
	}
}

ISR(TIMER0_COMPA_vect){
	ticks++;
}