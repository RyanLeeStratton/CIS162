/**
@file main.c
@brief Lab 3 Starter Code
@version .01 
@mainpage Lab 3 Starter Code

@section intro Code Overview
 
@section hw Hardware Pin Out
Port A:
A0 - A3 : Push Buttons
A4 - A7 : Slide Switches 

Port B:
B0 - B3 : SPI (SD Card)
B4		: Nothing
B5		: Audio Out
B6 		: Red Enable 
B7 		: Green Enable

Port C:
C0 - C7 : LED Array (Row)

Port D:
D0 - D1 : Nothing
D2		: Serial RX
D3		: Serial TX
D4 - D7	: Nothing

Port E:
E0 - E2 : LED Array (Column)
E3		: USB (UID)
E4 - E5	: Nothing
E6		: Relay
E7		: Nothing

Port F:
F0 		: ADC Channel 0
F1 		: ADC Channel 1
F2 		: ADC Channel 2
F3 		: ADC Channel 3
F4		: ADC Channel 4 (Audio In)
F5 		: ADC Channel 5 (Accel X Axis)
F6 		: ADC Channel 6 (Accel Y Axis)
F7 		: ADC Channel 7 (Accel Z Axis (if installed))

*/


/** Constants */
#define F_CPU 1000000UL
#define DEBUG 1
#define SERIAL_SPEED 9600
#define BAUD (((F_CPU)/(SERIAL_SPEED*8UL))-1)

#define IDLE 1
#define RUNNING 2

/* fill in the contents of this macro with the necessary values to set the clock to clock/1024. 
Look up CS02 and CS00 in the data sheet.
*/
#define CK1024 ((1 << CS02) | (1 << CS00))

/** Includes */
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include "adc.h"


/** Global Variables */

int state;

/** Functions */

/** The clearArray() function turns off all LEDS on the Wunderboard array. It accepts no inputs and returns nothing*/
void clear_array(void)
{
	PORTC = 0x00;
	PORTB |= (1 << PB6) | (1 << PB7);		/** Enable latches*/
	PORTB &= ~((1 << PB6) | (1 << PB7));	/** Disable latches*/
}

/** The initialize() function initializes all of the Data Direction Registers for the Wunderboard. Before making changes to DDRx registers, ensure that you have read the peripherals section of the Wunderboard user guide.*/
void initialize(void)
{
	/** Port A is the switches and buttons. They should always be inputs. ( 0 = Input and 1 = Output )*/
	DDRA=0b00000000;

	/** Port B has the LED Array color control, SD card, and audio-out on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRB=0b11000111;

	/** Port C is for the 'row' of the LED array. They should always be outputs. ( 0 = Input and 1 = Output )*/
	DDRC=0b11111111;

	/** Port D has the Serial on it. Leave DDRB alone. ( 0 = Input and 1 = Output )*/
	DDRD=0b00000000;

	/** Port E has the LED Array Column control out on it. Leave DDRE alone. ( 0 = Input and 1 = Output )*/
	DDRE=0b00000111;

	/** Port F has the accelerometer and audio-in on it. Leave DDRF alone. ( 0 = Input and 1 = Output )*/
	DDRF=0b00000000;
}

#if DEBUG 
/** This function needs to setup the variables used by the UART to enable the UART and tramsmit at 9600bps. This 
function should always return 0. Remember, by defualt the Wunderboard runs at 1mHz for its system clock.*/
unsigned char initialize_UART()
{
	UBRR1H = (unsigned char) (BAUD>>8);
	UBRR1L = (unsigned char)BAUD;
	
	UCSR1A |= (1<<1);
	
	UCSR1B = ((1 << RXEN1)|(1 << TXEN1));
	
	UCSR1C = ((0 << USBS1)|(3 << UCSZ10));

	return 0;
}

/** This function needs to write a single byte to the UART. It must check that the UART is ready for a new byte 
and return a 1 if the byte was not sent. 
@param [in] data This is the data byte to be sent.
@return The function returns a 1 or error and 0 on successful completion.*/
unsigned char send_byte_UART(uint8_t data)
{

	while(!( UCSR1A & (1<<UDRE1)));

		UDR1 = data;
		return 0;

}

/** This function needs to writes a string to the UART. It must check that the UART is ready for a new byte and 
return a 1 if the string was not sent. 
@param [in] str This is a pointer to the data to be sent.
@return The function returns a 1 or error and 0 on successful completion.*/
unsigned char send_string_UART(char* str)
{

	int Nchar = 0;
	
	while(!(str[Nchar] == '\0')){

	send_byte_UART(str[Nchar]);
	Nchar++;

	}

	return 0;

}
#endif // DEBUG

/** This function needs to setup the variables used by TIMER0 Compare Match (CTC) mode with 
a base clock frequency of clk/1024. This function should return a 1 if it fails and a 0 if it 
does not. Remember, by default the Wunderboard runs at 1mHz for its system clock.
@return This function returns a 1 is unsuccessful, else return 0.*/
unsigned char initialize_TIMER0()
{
			
	/* Set the CTC mode */
	TCCR0A = (1 << WGM01);
	
	/* Set the Clock Frequency */
	TCCR0B = CK1024;
	
	/* Set initial count value */
	OCR0A = 0;

	TIMSK0 |= 0b00000010; 

	return 0;
		
	
}


/* Beginning of Interupts */


ISR(TIMER0_COMPA_vect){
	cli();
	int number = 0;
	char value[5];

	while(number < 1000){
	PORTB = 0b10000000;
	PORTE = 0;
	PORTC = 0xFF;
	number++;
	}

	if(state == RUNNING)
	{
		number = read_adc(5);
		sprintf(value, "%d", number);

		send_string_UART(value);
		send_string_UART("  ");

	}
	sei();	

}


/** This function takes two values, clock and count. The value of count should be copied into OCR0A and the value of clock should be used to set CS02:0. The TCNT0 variable should also be reset to 0 so that the new timer rate starts from 0.  
@param [in] clock Insert Comment
@param [in] count Insert Comment
@return The function returns a 1 or error and 0 on successful completion.*/
unsigned char set_TIMER0(unsigned char clock, unsigned char count)
{
	if (clock & ~7)
	{
		return 1;
	}
	
	OCR0A = count;
	TCCR0B &= ~7;
	TCCR0B |= clock;
	TCNT0 = 0;
	
	return 0;

}

unsigned char USART_recieve()
{
if((UCSR1A & ~(1<<UDRE1))){
 
char x = UDR1;

UCSR1A &= (~(1<<RXC1));
return x;


}
}

/** Main Function */
int main (int argc, char **argv)
{
	/** Local Varibles */
	
	char character;


	initialize();
	clear_array();
	
	//turn on interrupts
	


	// Initialize TIMER/COUNTER0
	initialize_UART();
	initialize_TIMER0();

	// Set TIMER/COUNTER0 period, check for errors
	
	set_TIMER0( CK1024,255);
	sei();

	while (1){
	// While switch A7 is on
	while (PINA & (1 << PA7))
		{
		switch(state){

			case IDLE:
				send_string_UART("");
				character = NULL;
				character = USART_recieve();	
				if(character == 's')
					state = 2;
				break;

			case RUNNING:
				send_string_UART("");
				character = NULL;
				character = USART_recieve();
				if(character == 's')
					state = 1;
				break;

			
			default:
				send_string_UART("");
				character = USART_recieve();
				if(character == 's')
					state = 2;
			
			
			
			// If TIMER/COUNTER0 has elapsed
			// Read accelerometer data, write to UART, check for errors in the ISR
		}
	
	}
}
}
