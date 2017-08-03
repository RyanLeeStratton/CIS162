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
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "adc.h"


/** Global Variables */

//Score light variables

int Score_one =0;
int Score_two =0;
int Score_three =0;
int Score_four =0;
int Score_five =0;
int Score_six =0;
int Score_seven =0;
int Score_eight =0;
int Score_nine = 0;
int Score_ten = 0;
int Score_eleven = 0;
int Score_twelve = 0;
int Score_thirteen = 0;
int Score_fourteen = 0;
int Score_fifteen = 0;
int Score_sixteen = 0;
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

	return 0;

	
}

/** This function checks if TIMER0 has elapsed. 
@return This function should return a 1 if the timer has elapsed, else return 0*/
unsigned char check_TIMER0()
{
	if (TIFR0 & (1 << OCF0A))
	{
		TIFR0 |= (1 << OCF0A);
		return 1;
	}
	else
	{
		return 0;
	}

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

//function that keeps track of the LED score
void Score()
{
	if(Score_one == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b10000000;
		PORTE = 7;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_one == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b10000000;
		PORTE = 7;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_one == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b10000000;
		PORTE = 7;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_two == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b10000000;
		PORTE = 6;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_two == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b10000000;
		PORTE = 6;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_two == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b10000000;
		PORTE = 6;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_three == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b10000000;
		PORTE = 5;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_three == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b10000000;
		PORTE = 5;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_three == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b10000000;
		PORTE = 5;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_four == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b10000000;
		PORTE = 4;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_four == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b10000000;
		PORTE = 4;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_four == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b10000000;
		PORTE = 4;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_five == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b10000000;
		PORTE = 3;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_five == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b10000000;
		PORTE = 3;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_five == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b10000000;
		PORTE = 3;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_six == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b10000000;
		PORTE = 2;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_six == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b10000000;
		PORTE = 2;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_six == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b10000000;
		PORTE = 2;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_seven == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b10000000;
		PORTE = 1;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_seven == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b10000000;
		PORTE = 1;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_seven == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b10000000;
		PORTE = 1;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_eight == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b10000000;
		PORTE = 0;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_eight == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b10000000;
		PORTE = 0;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_eight == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b10000000;
		PORTE = 0;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_nine == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b01000000;
		PORTE = 7;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_nine == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b01000000;
		PORTE = 7;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_nine == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b01000000;
		PORTE = 7;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_ten == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b01000000;
		PORTE = 6;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_ten == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b01000000;
		PORTE = 6;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_ten == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b01000000;
		PORTE = 6;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_eleven == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b01000000;
		PORTE = 5;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_eleven == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b01000000;
		PORTE = 5;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_eleven == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b01000000;
		PORTE = 5;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_twelve == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b01000000;
		PORTE = 4;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_twelve == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b01000000;
		PORTE = 4;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_twelve == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b01000000;
		PORTE = 4;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_thirteen == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b01000000;
		PORTE = 3;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_thirteen == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b01000000;
		PORTE = 3;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_thirteen == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b01000000;
		PORTE = 3;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_fourteen == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b01000000;
		PORTE = 2;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_fourteen == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b01000000;
		PORTE = 2;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_fourteen == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b01000000;
		PORTE = 2;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_fifteen == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b01000000;
		PORTE = 1;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_fifteen == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b01000000;
		PORTE = 1;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_fifteen == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b01000000;
		PORTE = 1;
		_delay_ms(.1);
		clear_array();
	}

	if(Score_sixteen == 0)
	{
		PORTB = 0b11000000;
		PORTC = 0b01000000;
		PORTE = 0;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_sixteen == 1)
	{
		PORTB = 0b10000000;
		PORTC = 0b01000000;
		PORTE = 0;
		_delay_ms(.1);
		clear_array();
	}
	else if(Score_sixteen == 2)
	{
		PORTB = 0b01000000;
		PORTC = 0b01000000;
		PORTE = 0;
		_delay_ms(.1);
		clear_array();
	}

}

//functions for the time remaining LEDs

void Ten()
{
	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();
}

void Twenty()
{
	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();
}

void Thirty()
{
	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();
}

void Forty()
{
	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();
}

void Fifty()
{
	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();
}

void Sixty()
{
	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();
}

void Seventy()
{
	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();
}

void Number_nine()
{
	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111100;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100000;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

void Number_eight()
{
	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111100;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

void Number_seven()
{
	PORTB = 0b01000000;
	PORTC = 0b00111110;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100000;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00010000;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

void Number_six()
{
	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00000010;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011110;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

void Number_five()
{
	PORTB = 0b01000000;
	PORTC = 0b00111110;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00000010;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011110;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100000;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100000;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

void Number_four()
{
	PORTB = 0b01000000;
	PORTC = 0b00010000;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011000;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00010100;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00010010;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111110;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00010000;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00010000;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

void Number_three()
{
	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100000;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011000;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100000;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

void Number_two()
{
	PORTB = 0b01000000;
	PORTC = 0b00011100;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100010;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00100000;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00010000;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00000100;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111110;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

void Number_one()
{
	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001100;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00001000;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111110;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

}

//function that handles out of time for LEDs

void End()
{
	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 1;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 2;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 3;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 4;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 5;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 6;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 7;
	_delay_ms(.1);
	clear_array();

	PORTB = 0b01000000;
	PORTC = 0b00111111;
	PORTE = 8;
	_delay_ms(.1);
	clear_array();

}
 
/** Main Function */
int main (int argc, char **argv)
{
	/** Local Varibles */
	
	int Problem = 1;
	char answer[5];
	int foo = 0;
	int Game_score = 0;
	int check = 0;


	initialize();
	clear_array();
	
	// Initialize TIMER/COUNTER0
	
	initialize_UART();
	initialize_TIMER0();

	// Set TIMER/COUNTER0 period, check for errors
	set_TIMER0( CK1024,255);
	
	while (1){
	
	// While switch A7 is on
	while (PINA & (1 << PA7))
		{
			//questions
			if(Problem == 1)
			{

					//Displays the question with each possible answer
					if(check == 0)
					{
					send_string_UART("Scholars or intellectuals are commonly said to reside in what kind of tower?\n \r");
					send_string_UART("a.)Clock\n \r");
					send_string_UART("b.)Radio\n \r");
					send_string_UART("c.)Ivory\n \r");
					send_string_UART("d.)Water\n \n \r");
					check = 1;
					}


					//Pin inputs for answers
					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_one = 2;
							foo = 0;
							Problem = 2;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_one = 2;
							foo = 0;
							Problem = 2;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Correct!\n \n \r");
							Score_one = 1;
							foo = 0;
							Game_score += 1;
							Problem = 2;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is C\n \r");
							Score_one = 2;
							foo = 0;
							Problem = 2;
							check = 0;
							_delay_ms(1024);
						}

				}

			//same basic system repeats for all 16 questions
			if(Problem == 2)
			{
					if(check == 0)
					{
					send_string_UART("According to the proverb, what is the pot calling the kettle? \n \n \r");
					send_string_UART("a.)Hot\n \r");
					send_string_UART("b.)Noisy\n \r");
					send_string_UART("c.)Black\n \r");
					send_string_UART("d.)My hero\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_two = 2;
							foo = 0;
							Problem = 3;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_two = 2;
							foo = 0;
							Problem = 3;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Correct!\n \n \r");
							Score_two = 1;
							foo = 0;
							Game_score += 1;
							Problem = 3;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_two = 2;
							foo = 0;
							Problem = 3;
							check = 0;
							_delay_ms(1024);
						}

				}

			if(Problem == 3)
			{
					if(check == 0)
					{
					send_string_UART("According to a common saying, if something is really cheap, it's \"a dime a\" what?\n \n \r");
					send_string_UART("a.)Score\n \r");
					send_string_UART("b.)Dozen\n \r");
					send_string_UART("c.)Hundred\n \r");
					send_string_UART("d.)Too many\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_three = 2;
							foo = 0;
							Problem = 4;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Correct!\n \n \r");
							Score_three = 1;
							foo = 0;
							Game_score += 1;
							Problem = 4;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_three = 2;
							foo = 0;
							Problem = 4;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_three = 2;
							foo = 0;
							Problem = 4;
							check = 0;
							_delay_ms(1024);
						}

				}

			if(Problem == 4)
			{
					if(check == 0)
					{
					send_string_UART("What type of salute is traditionally given to U.S. war heroes at their funerals?\n \n \r");
					send_string_UART("a.)3-ring\n \r");
					send_string_UART("b.)14-karat\n \r");
					send_string_UART("c.)21-gun\n \r");
					send_string_UART("d.)76-trombone\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_four = 2;
							foo = 0;
							Problem = 5;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_four = 2;
							foo = 0;
							Problem = 5;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Correct!\n \n \r");
							Score_four = 1;
							foo = 0;
							Game_score += 1;
							Problem = 5;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_four = 2;
							foo = 0;
							Problem = 5;
							check = 0;
							_delay_ms(1024);
						}

				}

				if(Problem == 5)
			{
					if(check == 0)
					{
					send_string_UART("In most U.S. states, which of these crimes is a capital offense?\n \n \r");
					send_string_UART("a.)Murder\n \r");
					send_string_UART("b.)Money-laundering\n \r");
					send_string_UART("c.)Embezzlement\n \r");
					send_string_UART("d.)Mooning the Senate\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Correct!\n \n \r");
							Score_five = 1;
							foo = 0;
							Game_score += 1;
							Problem = 6;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_five = 2;
							foo = 0;
							Problem = 6;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_five = 2;
							foo = 0;
							Problem = 6;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_five = 2;
							foo = 0;
							Problem = 6;
							check = 0;
							_delay_ms(1024);
						}

				}

				if(Problem == 6)
			{
					if(check == 0)
					{
					send_string_UART("According to a popular expression, what is in the eye of the beholder?\n \n \r");
					send_string_UART("a.)Art\n \r");
					send_string_UART("b.)Beauty\n \r");
					send_string_UART("c.)Magic\n \r");
					send_string_UART("d.)Dust\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_six = 2;
							foo = 0;
							Problem = 7;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Correct!\n \n \r");
							Score_six = 1;
							foo = 0;
							Game_score += 1;
							Problem = 7;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_six = 2;
							foo = 0;
							Problem = 7;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_six = 2;
							foo = 0;
							Problem = 7;
							check = 0;
							_delay_ms(1024);
						}

				}

				if(Problem == 7)
			{
					if(check == 0)
					{
					send_string_UART("People who sever ties and leave no way to return are said to be \"burning their\" what?\n \n \r");
					send_string_UART("a.)Bridges\n \r");
					send_string_UART("b.)Houses\n \r");
					send_string_UART("c.)Dinner\n \r");
					send_string_UART("d.)Lifelines\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Correct!\n \n \r");
							Score_seven = 1;
							foo = 0;
							Game_score += 1;
							Problem = 8;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_seven = 2;
							foo = 0;
							Problem = 8;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_seven = 2;
							foo = 0;
							Problem = 8;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_seven = 2;
							foo = 0;
							Problem = 8;
							check = 0;
							_delay_ms(1024);
						}

				}

				if(Problem == 8)
			{
					if(check == 0)
					{
					send_string_UART("What city is home to the famous Madison Square Garden arena?\n \n \r");
					send_string_UART("a.)New York City\n \r");
					send_string_UART("b.)Philadelphia\n \r");
					send_string_UART("c.)Los Angeles\n \r");
					send_string_UART("d.)Dallas\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Correct!\n \n \r");
							Score_eight = 1;
							foo = 0;
							Game_score += 1;
							Problem = 9;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_eight = 2;
							foo = 0;
							Problem = 9;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_eight = 2;
							foo = 0;
							Problem = 9;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_eight = 2;
							foo = 0;
							Problem = 9;
							check = 0;
							_delay_ms(1024);
						}

				}

				if(Problem == 9)
			{
					if(check == 0)
					{
					send_string_UART("In military communications, what word is used to represent the letter C? \n \n \r");
					send_string_UART("a.)Caper\n \r");
					send_string_UART("b.)Charlie\n \r");
					send_string_UART("c.)Crook\n \r");
					send_string_UART("d.)Captain\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_nine = 2;
							foo = 0;
							Problem = 10;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Correct!\n \n \r");
							Score_nine = 1;
							foo = 0;
							Game_score += 1;
							Problem = 10;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_nine = 2;
							foo = 0;
							Problem = 10;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_nine = 2;
							foo = 0;
							Problem = 10;
							check = 0;
							_delay_ms(1024);
						}

				}

				if(Problem == 10)
			{
					if(check == 0)
					{
					send_string_UART("What kind of animal is the Dr. Seuss character named Horton? \n \n \r");
					send_string_UART("a.)Cat\n \r");
					send_string_UART("b.)Gerbil\n \r");
					send_string_UART("c.)Walrus\n \r");
					send_string_UART("d.)Elephant\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_ten = 2;
							foo = 0;
							Problem = 11;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_ten = 2;
							foo = 0;
							Problem = 11;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_ten = 2;
							foo = 0;
							Problem = 11;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Correct!\n \n \r");
							Score_ten = 1;
							foo = 0;
							Game_score += 1;
							Problem = 11;
							check = 0;
							_delay_ms(1024);
						}

				}

				if(Problem == 11)
			{
					if(check == 0)
					{
					send_string_UART("Which two countries currently account for more than a third of Earth's population? \n \n \r");
					send_string_UART("a.)China and Russia\n \r");
					send_string_UART("b.)India and China\n \r");
					send_string_UART("c.)India and Pakistan\n \r");
					send_string_UART("d.)China and Brazil\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_eleven = 2;
							foo = 0;
							Problem = 12;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Correct!\n \n \r");
							Score_eleven = 1;
							foo = 0;
							Game_score += 1;
							Problem = 12;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_eleven = 2;
							foo = 0;
							Problem = 12;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is B\n \n \r");
							Score_eleven = 2;
							foo = 0;
							Problem = 12;
							check = 0;
							_delay_ms(1024);
						}

				}

				if(Problem == 12)
			{
					if(check == 0)
					{
					send_string_UART("From 1954 to 1976, what was the capital of North Vietnam? \n \n \r");
					send_string_UART("a.)Hanoi\n \r");
					send_string_UART("b.)Saigon\n \r");
					send_string_UART("c.)Da Nang\n \r");
					send_string_UART("d.)Haiphong\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Correct!\n \n \r");
							Score_twelve = 1;
							foo = 0;
							Game_score += 1;
							Problem = 13;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_twelve = 2;
							foo = 0;
							Problem = 13;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_twelve = 2;
							foo = 0;
							Problem = 13;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_twelve = 2;
							foo = 0;
							Problem = 13;
							check = 0;
							_delay_ms(1024);
						}

				}

					if(Problem == 13)
			{
					if(check == 0)
					{
					send_string_UART("By definition, a person with rickets is deficient in which of these nutrients? \n \n \r");
					send_string_UART("a.Vitamin D\n \r");
					send_string_UART("b.)Zinc\n \r");
					send_string_UART("c.)Iron\n \r");
					send_string_UART("d.)Vitamin E\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Correct!\n \n \r");
							Score_thirteen = 1;
							foo = 0;
							Game_score += 1;
							Problem = 14;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_thirteen = 2;
							foo = 0;
							Problem = 14;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_thirteen = 2;
							foo = 0;
							Problem = 14;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is A\n \n \r");
							Score_thirteen = 2;
							foo = 0;
							Problem = 14;
							check = 0;
							_delay_ms(1024);
						}

				}

					if(Problem == 14)
			{
					if(check == 0)
					{
					send_string_UART("What two-letter suffix is used to designate a German Internet address?  \n \n \r");
					send_string_UART("a.).gr\n \r");
					send_string_UART("b.).gm\n \r");
					send_string_UART("c.) .de\n \r");
					send_string_UART("d.).dl\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_fourteen = 2;
							foo = 0;
							Problem = 15;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_fourteen = 2;
							foo = 0;
							Problem = 15;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Correct!\n \n \r");
							Score_fourteen = 1;
							foo = 0;
							Game_score += 1;
							Problem = 15;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Incorrect, Correct answer is C\n \n \r");
							Score_fourteen = 2;
							foo = 0;
							Problem = 15;
							check = 0;
							_delay_ms(1024);
						}

				}

					if(Problem == 15)
			{
					if(check == 0)
					{
					send_string_UART("The ancient temple of Angkor Wat is located in what country? \n \n \r");
					send_string_UART("a.)Tibet\n \r");
					send_string_UART("b.)Thailand\n \r");
					send_string_UART("c.)Myanmar\n \r");
					send_string_UART("d.)Cambodia\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_fifteen = 2;
							foo = 0;
							Problem = 16;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_fifteen = 2;
							foo = 0;
							Problem = 16;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_fifteen = 2;
							foo = 0;
							Problem = 16;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Correct!\n \n \r");
							Score_fifteen = 1;
							foo = 0;
							Game_score += 1;
							Problem = 16;
							check = 0;
							_delay_ms(1024);
						}

				}

					if(Problem == 16)
			{
					if(check == 0)
					{
					send_string_UART("Shortly after WWII, a famous war crimes tribunal was held where? \n \n \r");
					send_string_UART("a.)Dresden\n \r");
					send_string_UART("b.)Bremen\n \r");
					send_string_UART("c.)Potsdam\n \r");
					send_string_UART("d.)Nuremberg\n \n \r");
					check = 1;
					}


					if(PINA & (1 << PA0))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_sixteen = 2;
							foo = 0;
							Problem = 17;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA1))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_sixteen = 2;
							foo = 0;
							Problem = 17;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA2))
						{
							send_string_UART("Incorrect, Correct answer is D\n \n \r");
							Score_sixteen = 2;
							foo = 0;
							Problem = 17;
							check = 0;
							_delay_ms(1024);
						}
					if(PINA & (1 << PA3))
						{
							send_string_UART("Correct!\n \n \r");
							Score_sixteen = 1;
							foo = 0;
							Game_score += 1;
							Problem = 17;
							check = 0;
							_delay_ms(1024);
						}

				}

				
				if(Problem == 17)
			{
					if(check == 0)
					{
					send_string_UART("Your score for the game is: \n \n \r");
					sprintf(answer, "%d", Game_score);
					send_string_UART(answer);
					send_string_UART(" out of 16");
					check = 1;

					}
			}

			//timing
			if (check_TIMER0())
			{
				foo += 1;
			}

			Score();

			//first row of lights turn on for 10 seconds past
			if((foo >= 40) && foo < 284)
			{
				Ten();	
			}
			
			//second row of lights turn on for 20seconds past
			if((foo >= 80) && foo < 284)
			{
				Twenty();
			}

			if((foo >= 120) && foo < 284)
			{
				Thirty();
			}

			if((foo >= 160) && foo < 284)
			{
				Forty();
			}

			if((foo >= 200) && foo < 284)
			{
				Fifty();
			}

			if((foo >= 240) && foo < 284)
			{
				Sixty();
			}

			if((foo >= 280) && foo < 284)
			{
				Seventy();
			}

			if((foo >= 284) && foo < 288)
			{
				Number_nine();		
			}

			if((foo >= 288) && foo < 292)
			{
				Number_eight();
			}

			if((foo >= 292) && foo < 296)
			{
				Number_seven();
			}

			if((foo >= 296) && foo < 300)
			{
				Number_six();
			}

			if((foo >= 300) && foo < 304)
			{
				Number_five();
			}

			if((foo >= 304) && foo < 308)
			{
				Number_four();
			}	

			if((foo >= 308) && foo < 312)
			{
				Number_three();
			}

			if((foo >= 312) && foo < 316)
			{
				Number_two();
			}

			if((foo >= 316) && foo < 320)
			{
				Number_one();
			}


			//out of time, turn all LEDs red and go to the next problem
			if(foo >= 320)
			{
				End();
				send_string_UART("Time is up! \n \n \r");
				if(Problem == 1)
				{
					Score_one = 2;
					send_string_UART("Correct answer is C\n \n \r");
				}

				if(Problem == 2)
				{
					Score_two = 2;
					send_string_UART("Correct answer is C\n \n \r");
				}

				if(Problem == 3)
				{
					Score_three = 2;
					send_string_UART("Correct answer is B\n \n \r");
				}

				if(Problem == 4)
				{
					Score_four = 2;
					send_string_UART("Correct answer is C\n \n \r");
				}

				if(Problem == 5)
				{
					Score_five = 2;
					send_string_UART("Correct answer is A\n \n \r");
				}

				if(Problem == 6)
				{
					Score_six = 2;
					send_string_UART("Correct answer is B\n \n \r");
				}

				if(Problem == 7)
				{
					Score_seven = 2;
					send_string_UART("Correct answer is A\n \n \r");
				}

				if(Problem == 8)
				{
					Score_eight = 2;
					send_string_UART("Correct answer is A\n \n \r");
				}

				if(Problem == 9)
				{
					Score_nine = 2;
					send_string_UART("Correct answer is B\n \n \r");
				}

				if(Problem == 10)
				{
					Score_ten = 2;
					send_string_UART("Correct answer is D\n \n \r");
				}

				if(Problem == 11)
				{
					Score_eleven = 2;
					send_string_UART("Correct answer is B\n \n \r");
				}

				if(Problem == 12)
				{
					Score_twelve = 2;
					send_string_UART("Correct answer is A\n \n \r");
				}

				if(Problem == 13)
				{
					Score_thirteen = 2;
					send_string_UART("Correct answer is A\n \n \r");
				}

				if(Problem == 14)
				{
					Score_fourteen = 2;
					send_string_UART("Correct answer is C\n \n \r");
				}

				if(Problem == 15)
				{
					Score_fifteen = 2;
					send_string_UART("Correct answer is D\n \n \r");
				}

				if(Problem == 16)
				{
					Score_sixteen = 2;
					send_string_UART("Correct answer is D\n \n \r");
				}

				Problem += 1;
				foo = 0;
				check = 0;
			}
		}
		}

	return 0;
}

