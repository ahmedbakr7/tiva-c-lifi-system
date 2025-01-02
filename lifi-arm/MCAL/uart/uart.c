
#include "uart.h"
#include "../../tm4c123gh6pm.h"

#define F_CPU 16000000UL
/*******************************************************************************
 *                         Private Functions Definitions                       *
 *******************************************************************************/

static void GPIO_SetupUARTPins(const UART_ConfigType * uart)
{
    switch (uart->uartNumber)
    {
        case 0:
            SET_BIT(SYSCTL_RCGCGPIO_R,U0RX_PORT);            /* Enable clock for GPIO PORTA */
            while(!(SYSCTL_PRGPIO_R & (1<<U0RX_PORT)));                       /* Wait until GPIO PORTA clock is activated and it is ready for access*/
            GPIO_PORTA_AMSEL_R &= 0xFC;         /* Disable Analog on PA0 & PA1 */
            GPIO_PORTA_DIR_R   &= 0xFE;         /* Configure PA0 as input pin */
            GPIO_PORTA_DIR_R   |= 0x02;         /* Configure PA1 as output pin */
            GPIO_PORTA_AFSEL_R |= 0x03;         /* Enable alternative function on PA0 & PA1 */
            /* Set PMCx bits for PA0 & PA1 with value 1 to use PA0 as UART0 RX pin and PA1 as UART0 Tx pin */
            SET_BIT(GPIO_PORTA_PCTL_R,U0RX_PIN*4); 
            SET_BIT(GPIO_PORTA_PCTL_R,(U0RX_PIN+1)*4); 
            GPIO_PORTA_DEN_R   |= 0x03;         /* Enable Digital I/O on PA0 & PA1 */
            break;

        case 1:
            SET_BIT(SYSCTL_RCGCGPIO_R,U1RX_PORT);            /* Enable clock for GPIO PORTA */
            while(!(SYSCTL_PRGPIO_R & (1<<U1RX_PORT)));                       /* Wait until GPIO PORTA clock is activated and it is ready for access*/
            GPIO_PORTB_AMSEL_R &= 0xFC;         /* Disable Analog on PB0 & PB1 */
            GPIO_PORTB_DIR_R   &= ~ 0x1;         /* Configure PB0 as input pin */
            GPIO_PORTB_DIR_R   |= 0x02;         /* Configure PB1 as output pin */
            GPIO_PORTB_AFSEL_R |= 0x03;         /* Enable alternative function on PA0 & PA1 */
            /* Set PMCx bits for PA0 & PA1 with value 1 to use PA0 as UART0 RX pin and PA1 as UART0 Tx pin */
            SET_BIT(GPIO_PORTB_PCTL_R,U1RX_PIN*4); 
            SET_BIT(GPIO_PORTB_PCTL_R,(U1RX_PIN+1)*4); 
            GPIO_PORTB_DEN_R   |= 0x03;         /* Enable Digital I/O on PA0 & PA1 */
            break;

        case 5:
            SET_BIT(SYSCTL_RCGCGPIO_R,U5RX_PORT);            /* Enable clock for GPIO PORTA */
            while(!(SYSCTL_PRGPIO_R &  (1<<U5RX_PORT)));                       /* Wait until GPIO PORTA clock is activated and it is ready for access*/
            GPIO_PORTE_AMSEL_R &= ~ (0b11<<U5RX_PIN);         /* Disable Analog on PE4 & PE5 */
            GPIO_PORTE_DIR_R   &= ~ (1   <<U5RX_PIN);         /* Configure PE0 as input pin */
            SET_BIT(GPIO_PORTE_DIR_R,U5RX_PIN+1);         /* Configure PA1 as output pin */
            GPIO_PORTE_AFSEL_R |= (0b11<<U5RX_PIN);         /* Enable alternative function on PA0 & PA1 */
            /* Set PMCx bits for PA0 & PA1 with value 1 to use PA0 as UART0 RX pin and PA1 as UART0 Tx pin */
            SET_BIT(GPIO_PORTE_PCTL_R,U5RX_PIN*4); 
            SET_BIT(GPIO_PORTE_PCTL_R,(U5RX_PIN+1)*4); 
            GPIO_PORTE_DEN_R   |= (0b11<<U5RX_PIN);         /* Enable Digital I/O on PA0 & PA1 */
            break;

        default:
            break;
    }
}

/*******************************************************************************
 *                         Public Functions Definitions                        *
 *******************************************************************************/

void UART_Init(const UART_ConfigType * uart)
{
    GPIO_SetupUARTPins(uart);                                 /* Setup UART0 pins PA0 --> U0RX & PA1 --> U0TX */
    SET_BIT(SYSCTL_RCGCUART_R,uart->uartNumber);                /* Enable clock for UART0 */
    while(!(SYSCTL_PRUART_R & (1<<uart->uartNumber)));          /* Wait until UART0 clock is activated and it is ready for access*/
    float32 baudRate= (F_CPU)/(uart->baudRate*8);
    switch (uart->uartNumber)
    {
        case 0:
            UART0_CTL_R = 0;                    /* Disable UART0 at the beginning */
            UART0_CC_R  = 0;                    /* Use System Clock*/
            /* To Configure UART0 with Baud Rate 9600 */
            UART0_IBRD_R = (uint32) baudRate;
            UART0_FBRD_R = (uint32)((float32)(baudRate-(uint32) baudRate)*64+0.5);
            /* UART Line Control Rister Settings
            * BRK = 0 Normal Use
            * PEN = 0 Disable Parity
            * EPS = 0 No affect as the parity is disabled
            * STP2 = 0 1-stop bit at end of the frame
            * FEN = 0 FIFOs are disabled
            * WLEN = 0x3 8-bits data frame
            * SPS = 0 no stick parity
            */
            UART0_LCRH_R = (UART_DATA_8BITS << UART_LCRH_WLEN_BITS_POS);
            /* UART Control Rister Settings
            * RXE = 1 Enable UART Receive
            * TXE = 1 Enable UART Transmit
            * HSE = 1 The UART is clocked using the system clock divided by 8
            * UARTEN = 1 Enable UART
            */
            UART0_CTL_R = UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK | UART_CTL_RXE_MASK| (1<<5);
            break;
        
        case 1:
            UART1_CTL_R = 0;                    /* Disable UART0 at the beginning */
            UART1_CC_R  = 0;                    /* Use System Clock*/
            UART1_IBRD_R = (uint32) baudRate;
            UART1_FBRD_R = (uint32)((float32)(baudRate-(uint32) baudRate)*64+0.5);
            UART1_LCRH_R = (UART_DATA_8BITS << UART_LCRH_WLEN_BITS_POS);
            UART1_CTL_R = UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK | UART_CTL_RXE_MASK| (1<<5);
            break;
        
        case 5:
            UART5_CTL_R = 0;                    /* Disable UART0 at the beginning */
            UART5_CC_R  = 0;                    /* Use System Clock*/
            UART5_IBRD_R = (uint32) baudRate;
            UART5_FBRD_R = (uint32)((float32)(baudRate-(uint32) baudRate)*64+0.5);
            UART5_LCRH_R = (UART_DATA_8BITS << UART_LCRH_WLEN_BITS_POS);
            UART5_CTL_R = UART_CTL_UARTEN_MASK | UART_CTL_TXE_MASK | UART_CTL_RXE_MASK| (1<<5);
            break;
        
    default:
        break;
    }
}

void UART_SendByte(const UART_ConfigType * uart,uint8 data)
{
    switch (uart->uartNumber)
    {
        case 0:
            while(!(UART0_FR_R & UART_FR_TXFE_MASK)); /* Wait until the transmit FIFO is empty */
            UART0_DR_R = data; /* Send the byte */
        break;
            
        case 1:
            while(!(UART1_FR_R & UART_FR_TXFE_MASK)); /* Wait until the transmit FIFO is empty */
            UART1_DR_R = data; /* Send the byte */
        break;
            
        case 5:
            while(!(UART5_FR_R & UART_FR_TXFE_MASK)); /* Wait until the transmit FIFO is empty */
            UART5_DR_R = data; /* Send the byte */
        break;
    
    default:
        break;
    }
}

uint8 UART_ReceiveByte(const UART_ConfigType * uart)
{
    switch (uart->uartNumber)
    {
        case 0:
            while(UART0_FR_R & UART_FR_RXFE_MASK); /* Wait until the receive FIFO is not empty */
            return UART0_DR_R; /* Read the byte */
        break;
            
        case 1:
            while(UART1_FR_R & UART_FR_RXFE_MASK); /* Wait until the receive FIFO is not empty */
            return UART1_DR_R; /* Read the byte */
        break;
            
        case 5:
            while(UART5_FR_R & UART_FR_RXFE_MASK); /* Wait until the receive FIFO is not empty */
            return UART5_DR_R; /* Read the byte */
        break;
    
    default:
    return 0;
        break;
    }
}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_SendString(const UART_ConfigType * uart,const uint8 *pData)
{
    uint32 uCounter =0;
	/* Transmit the whole string */
    while(pData[uCounter] != '\0')
    {
        UART_SendByte(uart,pData[uCounter]); /* Send the byte */
        uCounter++; /* increment the counter to the next byte */
    }
}

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_ReceiveString(const UART_ConfigType * uart,uint8 *pData)
{
    uint32 uCounter =0;

    pData[uCounter] = UART_ReceiveByte(uart); /* Receive the first byte */
	
	/* Receive the whole string until the '#' */
    while(pData[uCounter] != '#')
    {
		uCounter++;
        pData[uCounter] = UART_ReceiveByte(uart);
    }
	/* After receiving the whole string plus the '#', replace the '#' with '\0' */
    pData[uCounter] = '\0';
}

// /*
//  * Description :
//  * Send the required number of bytes through UART to the other UART device.
//  */
// void UART_SendData(const uint8 *pData, uint32 uSize)
// {
//     /* Transmit the number of bytes requested on the UART port */
//     while(uSize--)
//     {
        
//         UART0_SendByte(*pData); /* Send the byte */
//         pData++; /* increment pointer to the next byte */
//     }
// }

// /*
//  * Description :
//  * receive the required number of bytes through UART from the other UART device.
//  */
// void UART_ReceiveData(uint8 *pData, uint32 uSize)
// {
//     /* Receive the number of bytes requested on the UART port */
//     while(uSize--)
//     {
//         *pData = UART0_ReceiveByte(); /* receive a byte from the UART */
//          pData++; /* increment pointer to receive the next byte in the next location */
//     }
// }
