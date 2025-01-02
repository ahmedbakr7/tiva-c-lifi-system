
#include "hal/ultrasonic/ultrasonic.h" 
#include "hal/magneticsensor/magneticsensor.h" 
#include "hal/fumesensor/fumesensor.h" 
//#include "hal/bluetooth/bluetooth.h"
#include "MCAL/uart/uart.h"
#include "MCAL/gpio/gpio.h"
#include "MCAL/adc/adc.h"
#include "std_types.h"
#include "common_macros.h"

// SW1 pf4,SW2 pf4	
#define F_CPU 16000000UL              // Frequency 16Mhz
#define MINIMUM_DISTANCE 70UL              
#define SWITCH_0 0UL              
#define SWITCH_1 4UL              
#define MUTE_CODE 169UL

#define Enable_Exceptions()    __asm(" CPSIE I ")
#define Enable_Faults()        __asm(" CPSIE F ")

uint8 volatile systemState=1;			// system state	1: active, 0: stop
uint8 volatile flagSwitch0=0;
uint8 volatile muteState=0;	
uint8 delayFlag=0;

void _delay_ms(uint32 ms)
{
	uint32 delay= ms*(F_CPU/1000)/3-1;
	uint32 i=0;
	for ( i=0; i<delay; i++);
}

void readSwitch0(void)
{
	if(!GPIO_ReadPin(PORTF_ID, SWITCH_0))		// toggle switch0
	{
		if(flagSwitch0 == 0)
		{
			TOGGLE_BIT(systemState, 0);
			flagSwitch0 = 1;
		}
	}
	else 
	{
		flagSwitch0=0;
	}
}

void GPIOF_Handler(void)
{	
	if (GPIO_PORTF_MIS_R & 0x10) /* check if interrupt causes by PF4/SW1*/
    {   
		muteState=1;
      	GPIO_PORTF_ICR_R |= 0x10; /* clear the interrupt flag */
	} 
    else if (GPIO_PORTF_MIS_R & 0x01) /* check if interrupt causes by PF0/SW2 */
    {   
		readSwitch0();
		GPIO_PORTF_ICR_R |= 0x01; /* clear the interrupt flag */
    }
}


char* itoa(int num, char* str, int base);

void _gerToString(uint8 data,uint8 *buff)
{
   itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
}

void readSwitch0(void);

		// mute





void SW2_Init(void)
{
    GPIO_PORTF_LOCK_R   = 0x4C4F434B;   /* Unlock the GPIO_PORTF_CR_REG */
    GPIO_PORTF_CR_R    |= (1<<0)  | (1<<4);       /* Enable changes on PF0 */
    GPIO_PORTF_AMSEL_R &= ~(1<<0) & ~(1<<4);      /* Disable Analog on PF0 */
    GPIO_PORTF_PCTL_R  &= 0xFFFFFFF0;   /* Clear PMCx bits for PF0 to use it as GPIO pin */
    GPIO_PORTF_DIR_R   &= ~(1<<0) & ~(1<<4);      /* Configure PF0 as input pin */
    GPIO_PORTF_AFSEL_R &= ~(1<<0) & ~(1<<4);      /* Disable alternative function on PF0 */
    GPIO_PORTF_PUR_R   |= (1<<0) | (1<<4);       /* Enable pull-up on PF0 */
    GPIO_PORTF_DEN_R   |= (1<<0) |(1<<4);       /* Enable Digital I/O on PF0 */
    GPIO_PORTF_IS_R    &= ~(1<<0)& ~(1<<4);      /* PF0 detect edges */
    GPIO_PORTF_IBE_R   &= ~(1<<0)& ~(1<<4);      /* PF0 will detect a certain edge */
    GPIO_PORTF_IEV_R   &= ~(1<<0)& ~(1<<4);      /* PF0 will detect a falling edge */
    GPIO_PORTF_ICR_R   |= (1<<0)| (1<<4);       /* Clear Trigger flag for PF0 (Interrupt Flag) */
    GPIO_PORTF_IM_R    |= (1<<0)|(1<<4);       /* Enable Interrupt on PF0 pin */
    /* Set GPIO PORTF priority as 2 by set Bit number 21, 22 and 23 with value 2 */
    NVIC_EN0_R         |= 0x40000000;   /* Enable NVIC Interrupt for GPIO PORTF by set bit number 30 in EN0 Register */
}


int main(void)
{
	GPIO_Init(5, SWITCH_0, PIN_INPUT, 1);
	GPIO_Init(5, SWITCH_1, PIN_INPUT, 1);
    SYSCTL_RCGCGPIO_R |= 0x20;
    while(!(SYSCTL_PRGPIO_R & 0x20));
    SW2_Init();

    Enable_Exceptions();
    Enable_Faults();
	// ADC_Config configurations = {ADC0, CHANNEL_0, SS3};
	// ADC_init(&configurations);
	// 
	// uint16 adc_value;

	UART_ConfigType uartLifi= {1,9600};
	UART_ConfigType uartBluetooth= {5,9600};
	UART_Init(&uartLifi);   					// lifi uart init()
	UART_Init(&uartBluetooth);   					// bluetooth uart init()

	ULTRASONIC_init();
	FUMESENSOR_init();
	MAGNETIC_init();

	uint8 fumeData=0;						// read data from fume sensor
	uint8 fumeDataADC=0;						// read data from fume sensor
	uint8 magneticData=0;					// read data from magnetic sensor
	uint8 ultraSonicData=0;					// read data from ultrasonic sensor
	uint8 fumeMessageAlert[]="Smoke Alert!!!\n";			// message to be displayed on lcd
	uint8 fumeMessageIdle[]="no smoke detected.\n";			// message to be displayed on lcd
	uint8 magneticMessageAlert[]="Breach alert!!!\n";			// message to be displayed on lcd
	uint8 magneticMessageIdle[]="no breach.\n";			// message to be displayed on lcd
	uint8 ultraSonicMessageAlert[]="Distance = ";			// message to be displayed on lcd
	uint8 buff[16]; /* String to hold the ascii result */
	
/* 
ultrasonc: Distance = .... cm

smoke ==> (...)No Smoke Detected!! / (...)Smoke Alert!!!

Magnetic ==> No breach found / Breach alert!!!
 */


	readSwitch0();


	for(;;)
	{
		if(muteState)     // read mute button
		{
			UART_SendByte(&uartLifi, MUTE_CODE);
			muteState=0;
			// _delay_ms(1000);
		}
		while(!systemState)
		{
			readSwitch0();
			UART_SendByte(&uartLifi, MUTE_CODE);
		}

		// fumeData=ADC_readChannel(ADC0, CHANNEL_0, SS3);
		fumeData=FUMESENSOR_read();
		magneticData=MAGNETIC_read();
		ultraSonicData=ULTRASONIC_getDistance();

		if (ultraSonicData < MINIMUM_DISTANCE)
			ultraSonicData=0;
		else
			ultraSonicData=1;


		if (fumeData|magneticData|ultraSonicData) 	// if any sensor detected something
		{
			/* delayStart();					// start delay
			while(delayFlag==0);				// wait until delay is finished
			delayFlag=0; */					// reset delay flag
			_delay_ms(500);
			// fumeData=ADC_readChannel(ADC0, CHANNEL_0, SS3);
			fumeData=FUMESENSOR_read();
			magneticData=MAGNETIC_read();
			ultraSonicData=ULTRASONIC_getDistance();
		}

		if(fumeData||magneticData||ultraSonicData)
		{
			// UART_SendByte(&uartLifi,FUMESENSOR_readAdc());			// transmit FUME ADC value through lifi
			UART_SendByte(&uartLifi,fumeData);				// transmit bit stating whether the fume outputs digital high or low
			UART_SendByte(&uartLifi,magneticData);			// transmit data through lifi

			UART_SendByte(&uartLifi,ultraSonicData);			// transmit data through lifi
			/*********************************** transmit ultrasonic readings **************************/			
			UART_SendString(&uartBluetooth,ultraSonicMessageAlert);			// transmit data through bluetooth
			_gerToString(ultraSonicData, buff);			// convert data to string
			buff[3]='c';
			buff[4]='m';
			buff[4]='\n';
			buff[5]='\0';
			UART_SendString(&uartBluetooth,buff);			// transmit data through bluetooth
			/* ************************************************************************************************* */
			/*********************************** transmit fume readings **************************///fume ==> (...)No Smoke Detected!! / (...)Smoke Alert!!!
			_gerToString(FUMESENSOR_read(), buff);			// send fume adc value
			UART_SendString(&uartBluetooth,buff);			// transmit data through bluetooth
			UART_SendString(&uartBluetooth,fumeData?fumeMessageAlert:fumeMessageIdle); // decide whether to send alert or idle message based on fume digital output
			/* ************************************************************************************************* */
			/*********************************** transmit magnetic readings **************************///No breach found / Breach alert!!!
			UART_SendString(&uartBluetooth,magneticData?magneticMessageAlert:magneticMessageIdle);			// transmit data through bluetooth
			/* ************************************************************************************************* */
			fumeData=0;magneticData=0;ultraSonicData=0;
			
		}
	}
}





void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}
// Implementation of citoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    uint8 isNegative = 0;
 
    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
    str[i] = '\0'; // Append string terminator
    // Reverse the string
    reverse(str, i);
    return str;
}
