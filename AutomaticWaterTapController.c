#define F_CPU 1000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#define  Trigger_pin	PA0	/* Trigger pin */

int TimerOverflow = 0;
int state = 0;
int degree = 0;

ISR(TIMER1_OVF_vect)
{
	TimerOverflow++;	/* Increment Timer Overflow count */
}


void RotateCW(int degree)
{
	degree = degree / 4;
	int c1=0;
	while(c1 < degree){
		PORTC = 0x01;
		_delay_us(1000);
		PORTC = 0x00;
		_delay_ms(19);
		c1++;
	}
}


void RotateCCW(int degree)
{
	degree = degree / 4;
	int c2 = 0;
	while(c2 < degree){
		PORTC = 0x01;
		_delay_us(1800);
		PORTC = 0x00;

		_delay_us(18200);
		c2++;
	}
}

int main(void)
{
	char string[10];
	long count;
	double distance;
	
	DDRA = 0x0F;
	PORTA = PORTA | 0b00001110;
	/* Make trigger pin as output */
	PORTD = 0xFF;		/* Turn on Pull-up */
	
	//LCD_Init();
	//LCD_String_xy(1, 0, "Ultrasonic");
	DDRC = 0x01;         //Makes RC0 output pin
	PORTC = 0x00;
	_delay_ms(5000);
	
	sei();			/* Enable global interrupt */
	TIMSK = (1 << TOIE1);	/* Enable Timer1 overflow interrupts */
	TCCR1A = 0;		/* Set all bit to zero Normal operation */

	while(1)
	{
		/* Give 10us trigger pulse on trig. pin to HC-SR04 */
		PORTA |= (1 << Trigger_pin);
		_delay_us(100);
		PORTA &= (~(1 << Trigger_pin));
		
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x41;	/* Capture on rising edge, No prescaler*/
		TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */

		/*Calculate width of Echo by Input Capture (ICP) */
		
		while ((TIFR & (1 << ICF1)) == 0);/* Wait for rising edge */
		TCNT1 = 0;	/* Clear Timer counter */
		TCCR1B = 0x01;	/* Capture on falling edge, No prescaler */
		TIFR = 1<<ICF1;	/* Clear ICP flag (Input Capture flag) */
		TIFR = 1<<TOV1;	/* Clear Timer Overflow flag */
		TimerOverflow = 0;/* Clear Timer overflow count */

		while ((TIFR & (1 << ICF1)) == 0);/* Wait for falling edge */
		count = ICR1 + (65535 * TimerOverflow);	/* Take count */
		/* 8MHz Timer freq, sound speed =343 m/s */
		distance = (double)count / 58.3;
		
		if (state == 1)
		{
			_delay_ms(1000);
		}
		
		
		if( distance > 15)
		{
			if(state == 1)
			{
				/*PORTA =0b00000010;
				_delay_ms(2000);
				//PORTA = 0b00000100;
				PORTA = 0x00;*/
				RotateCCW(150);
				_delay_ms(2000);
				state = 0;
				
			}
			else{
				_delay_ms(200);
			}
			
		}
		else{
			
			if(state == 0)
			{
				/*//PORTA =0b0000000;
				PORTA= 0b00000100;
				_delay_ms(2000);
				//PORTA = 0b00000100;
				PORTA = 0x00;*/
				RotateCW(150);
				_delay_ms(2000);
				state = 1;	
			}
			else{
				_delay_ms(200);
			}
			
			 
		}

		/*dtostrf(distance, 2, 2, string);/* distance to string 
		strcat(string, " cm   ");	/* Concat unit i.e.cm 
		LCD_String_xy(2, 0, "Dist = ");
		LCD_String_xy(2, 7, string);	*/
		
	}
}


/*
#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

void RotateCW(int degree)
{
	degree = degree / 4;
	int c1=0;
	while(c1 < degree){
		PORTC = 0x01;
		_delay_us(1000);
		PORTC = 0x00;
		_delay_ms(19);
		c1++;
	}
}


void RotateCCW(int degree)
{
	degree = degree / 4;
	int c2 = 0;
	while(c2 < degree){
		PORTC = 0x01;
		_delay_us(1800);
		PORTC = 0x00;

		_delay_us(18200);
		c2++;
	}
}

int main(void)
{
	
	DDRC = 0x01;         //Makes RC0 output pin
	PORTC = 0x00;
	_delay_ms(3000);
	while(1)
	{
		_delay_ms(1000);
		RotateCW(150);
		_delay_ms(1000);
		RotateCCW(150);
		
	}
}*/