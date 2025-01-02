#include "gpio.h"


void GPIO_Init(uint8 portNumber,uint8 pinNumber,GPIO_PinDirectionType direction,uint8 pullup )
{
    SYSCTL_RCGCGPIO_R|=(1<<portNumber);
    while ((SYSCTL_PRGPIO_R & (1<<portNumber))==0);
    switch (portNumber)
    {
    case PORTA_ID:
        GPIO_PORTA_LOCK_R= 0X4C4F434B;
        GPIO_PORTA_CR_R|=(1<<pinNumber);
        GPIO_PORTA_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTA_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTA_DEN_R|=(1<<pinNumber);
        break;
        
    case PORTB_ID:
        GPIO_PORTB_LOCK_R= 0X4C4F434B;
        GPIO_PORTB_CR_R|=(1<<pinNumber);
        GPIO_PORTB_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTB_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTB_DEN_R|=(1<<pinNumber);
        break;
        
    case PORTC_ID:
        if(pinNumber<=3) return;                                       // IF PIN=pc0-pc3
        GPIO_PORTC_LOCK_R= 0X4C4F434B;
        GPIO_PORTC_CR_R|=(1<<pinNumber);
        GPIO_PORTC_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTC_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTC_DEN_R|=(1<<pinNumber);

        break;
        
    case PORTD_ID:
        GPIO_PORTD_LOCK_R= 0X4C4F434B;
        GPIO_PORTD_CR_R|=(1<<pinNumber);
        GPIO_PORTD_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTD_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTD_DEN_R|=(1<<pinNumber);

        break;
        
    case PORTE_ID:
        GPIO_PORTE_LOCK_R= 0X4C4F434B;
        GPIO_PORTE_CR_R|=(1<<pinNumber);
        GPIO_PORTE_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTE_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTE_DEN_R|=(1<<pinNumber);

        break;
        
    case PORTF_ID:
        GPIO_PORTF_LOCK_R= 0X4C4F434B;
        GPIO_PORTF_CR_R|=(1<<pinNumber);
        GPIO_PORTF_DIR_R|=(direction<<pinNumber);
        if(direction==PIN_INPUT) GPIO_PORTF_PUR_R|=(pullup<<pinNumber);
        GPIO_PORTF_DEN_R|=(1<<pinNumber);

        break;
    
    default:
        break;
    }
}

void GPIO_writePin(uint8 portNumber,uint8 pinNumber,uint8 value)
{
    switch (portNumber)
    {
    case PORTA_ID:
        value?SET_BIT(GPIO_PORTA_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTA_DATA_R,pinNumber);
        break;
    case PORTB_ID:
        value?SET_BIT(GPIO_PORTB_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTB_DATA_R,pinNumber);
        break;
    case PORTC_ID:
        value?SET_BIT(GPIO_PORTC_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTC_DATA_R,pinNumber);
        break;
    case PORTD_ID:
        value?SET_BIT(GPIO_PORTD_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTD_DATA_R,pinNumber);
        break;
    case PORTE_ID:
        value?SET_BIT(GPIO_PORTE_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTE_DATA_R,pinNumber);
        break;
    case PORTF_ID:
        value?SET_BIT(GPIO_PORTF_DATA_R,pinNumber):CLEAR_BIT(GPIO_PORTF_DATA_R,pinNumber);
        break;
    
    default:
        break;
    }
}

void GPIO_writePort(uint8 portNumber,uint8 value)
{
    switch (portNumber)
    {
    case PORTA_ID:
        GPIO_PORTA_DATA_R=value;
        break;
    case PORTB_ID:
        GPIO_PORTB_DATA_R=value;
        break;
    case PORTC_ID:
        GPIO_PORTC_DATA_R=value;
        break;
    case PORTD_ID:
        GPIO_PORTD_DATA_R=value;
        break;
    case PORTE_ID:
        GPIO_PORTE_DATA_R=value;
        break;
    case PORTF_ID:
        GPIO_PORTF_DATA_R=value;
        break;
    
    default:
        break;
    }
}

uint8 GPIO_ReadPin(uint8 portNumber,uint8 pinNumber)
{
    switch (portNumber)
    {
    case PORTA_ID:
        return BIT_IS_SET(GPIO_PORTA_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTB_ID:
        return BIT_IS_SET(GPIO_PORTB_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTC_ID:
        return BIT_IS_SET(GPIO_PORTC_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTD_ID:
        return BIT_IS_SET(GPIO_PORTD_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTE_ID:
        return BIT_IS_SET(GPIO_PORTE_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    case PORTF_ID:
        return BIT_IS_SET(GPIO_PORTF_DATA_R,pinNumber)?LOGIC_HIGH:LOGIC_LOW;
        break;
    
    default:
        break;
    }
}

uint8 GPIO_ReadPort(uint8 portNumber)
{
    
    switch (portNumber)
    {
    case PORTA_ID:
        return GPIO_PORTA_DATA_R;
        break;
    case PORTB_ID:
        return GPIO_PORTB_DATA_R;
        break;
    case PORTC_ID:
        return GPIO_PORTC_DATA_R;
        break;
    case PORTD_ID:
        return GPIO_PORTD_DATA_R;
        break;
    case PORTE_ID:
        return GPIO_PORTE_DATA_R;
        break;
    case PORTF_ID:
        return GPIO_PORTF_DATA_R;
        break;
    
    default:
        break;
    }
}