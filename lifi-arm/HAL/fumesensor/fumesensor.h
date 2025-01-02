#ifndef FUMESENSOR_H_
#define FUMESENSOR_H_

#include "../../MCAL/gpio/gpio.h"
#include "../../std_types.h"

#define FUMESENSOR_PIN PIN3_ID
#define FUMESENSOR_PORT PORTA_ID

#define FUMESENSOR_PIN_DIGITAL PIN2_ID
#define FUMESENSOR_PORT_DIGITAL PORTB_ID

void FUMESENSOR_init(void);

uint8 FUMESENSOR_read(void);

uint8 FUMESENSOR_readAdc(void);

#endif
