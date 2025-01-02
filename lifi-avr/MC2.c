
#include "uart.h"
#include "lcd.h"
#include "gpio.h"
#include "avr/io.h"
#include "util/delay.h"

#define FUME_MASK 1UL
#define ULTRASONIC_MASK 2UL
#define MAGNETIC_MASK 4UL
#define MUTE_CODE 169UL

// lamb, buzzer, lcd, button
// pc0 , pc5   		, pd2

// message[0-2] 0:fume, 1:ultrasonic, 2:intrusion

void startAlert(void)
{
	GPIO_writePin(PORTC_ID,PIN0_ID,LOGIC_HIGH); 		// flash led
	GPIO_writePin(PORTC_ID,PIN5_ID,LOGIC_HIGH); 			// start buzzer
}

void stopAlert(void)
{
	GPIO_writePin(PORTC_ID,PIN0_ID,LOGIC_LOW); 			// turn off led
	GPIO_writePin(PORTC_ID,PIN5_ID,LOGIC_LOW); 			// turn off buzzer
}

void displayString(char * str)
{
	LCD_clearScreen();
	LCD_moveCursor(0,0);
	LCD_displayString(str);
}

int main(void)
{
	UART_init(9600);
	LCD_init();

	UART_sendByte(1);
	LCD_displayString("waiting signal..");

	GPIO_setupPinDirection(PORTC_ID,PIN0_ID,PIN_OUTPUT);     // setup led direction
	GPIO_writePin(PORTC_ID,PIN0_ID,LOGIC_LOW);				// initialize it with low
	
	GPIO_setupPinDirection(PORTC_ID,PIN5_ID,PIN_OUTPUT);     // setup buzzer direction
	GPIO_writePin(PORTC_ID,PIN5_ID,LOGIC_LOW);				// initialize it with low

	// GPIO_setupPinDirection(PORTD_ID,PIN0_ID,PIN_INPUT);     // setup mute button direction

	// if(!(PIND & (1<<2)))       // check if the push button at PD2 is pressed or not

uint8 fumeADC=0;
uint8 fumeDigital=0;
uint8 magneticData=0;
uint8 ultraSonicData=0;

	uint8 alert=0;
	
    while(1)
    {

		// fumeADC=UART_recieveByte();
		LCD_clearScreen();
		fumeDigital=UART_recieveByte();
		magneticData=UART_recieveByte();
		ultraSonicData=UART_recieveByte();

		if(fumeDigital)
		{
			LCD_displayStringRowColumn(0,0,"fume ");
			alert=1;
		}
		
		if(ultraSonicData)
		{
			LCD_displayStringRowColumn(0,5,"movement ");
			alert=1;
		}
		
		if(magneticData)
		{
			LCD_displayStringRowColumn(1,0,"intrusion");
			alert=1;
		}	

		if(alert)	startAlert();
		
		while (alert)
		{
			if(UART_recieveByte()==MUTE_CODE)
			{
				alert=0;
				stopAlert();
				LCD_clearScreen();
				_delay_ms(4000);
			}
		}
	}
}
