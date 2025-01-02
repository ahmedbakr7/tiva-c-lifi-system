#include "magneticsensor.h"

void MAGNETIC_init(void)
{
    GPIO_Init(MAGNETIC_PORT,MAGNETIC_PIN,PIN_INPUT,1);
}

uint8 MAGNETIC_read(void)
{
    return GPIO_ReadPin(MAGNETIC_PORT,MAGNETIC_PIN);
}
