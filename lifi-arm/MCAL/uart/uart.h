
#ifndef UART_H_
#define UART_H_

#include "../../std_types.h"
#include "../../common_macros.h"
#include "../gpio/gpio.h"

/*******************************************************************************
 *                             PreProcessor Macros                             *
 *******************************************************************************/

#define UART_DATA_5BITS          0x0
#define UART_DATA_6BITS          0x1
#define UART_DATA_7BITS          0x2
#define UART_DATA_8BITS          0x3
#define UART_LCRH_WLEN_BITS_POS  5
#define UART_CTL_UARTEN_MASK     0x00000001
#define UART_CTL_TXE_MASK        0x00000100
#define UART_CTL_RXE_MASK        0x00000200
#define UART_FR_TXFE_MASK        0x00000080
#define UART_FR_RXFE_MASK        0x00000010


#define U0RX_PIN    PIN0_ID
#define U0RX_PORT     PORTA_ID

#define U1RX_PIN    PIN0_ID
#define U1RX_PORT     PORTB_ID

// #define U2RX_PIN    PIN6_ID
// #define U2RX_PORT     PORTD_ID

#define U3RX_PIN    PIN6_ID
#define U3RX_PORT     PORTC_ID

#define U4RX_PIN    PIN4_ID
#define U4RX_PORT     PORTC_ID

#define U5RX_PIN    PIN4_ID
#define U5RX_PORT     PORTE_ID

#define U6RX_PIN    PIN4_ID
#define U6RX_PORT     PORTD_ID

#define U7RX_PIN    PIN0_ID
#define U7RX_PORT     PORTE_ID



/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/


typedef struct {
	uint8 uartNumber;
	uint32 baudRate;
	// uint16 parityMode;
	// uint8 numberOfBits;
	// uint8 stopBit;
}UART_ConfigType;



void UART_Init(const UART_ConfigType * uart);

void UART_SendByte(const UART_ConfigType * uart,uint8 data);

uint8 UART_ReceiveByte(const UART_ConfigType * uart);

void UART_SendString(const UART_ConfigType * uart,const uint8 *pData);

void UART_ReceiveString(const UART_ConfigType * uart,uint8 *pData);

#endif
