
#ifndef GPIO_H_
#define GPIO_H_

#include "../../tm4c123gh6pm.h"
#include "../../std_types.h"
#include "../../common_macros.h"

#define PORTA_ID               0
#define PORTB_ID               1
#define PORTC_ID               2
#define PORTD_ID               3
#define PORTE_ID               4
#define PORTF_ID               5

#define PIN0_ID                0
#define PIN1_ID                1
#define PIN2_ID                2
#define PIN3_ID                3
#define PIN4_ID                4
#define PIN5_ID                5
#define PIN6_ID                6
#define PIN7_ID                7

typedef enum
{
	PIN_INPUT,PIN_OUTPUT
}GPIO_PinDirectionType;


void GPIO_Init(uint8 portNumber,uint8 pinNumber,GPIO_PinDirectionType direction,uint8 pullup );

void GPIO_writePin(uint8 portNumber,uint8 pinNumber,uint8 value);

// void GPIO_setPinDirection(uint8 portNumber,uint8 pinNumber,uint8 value);

void GPIO_writePort(uint8 portNumber,uint8 value);

uint8 GPIO_ReadPin(uint8 portNumber,uint8 pinNumber);

uint8 GPIO_ReadPort(uint8 portNumber);


#endif

