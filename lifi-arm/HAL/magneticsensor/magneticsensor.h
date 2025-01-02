#ifndef MAGNETIC_H_
#define MAGNETIC_H_

#include "../../MCAL/gpio/gpio.h"
#include "../../std_types.h"

#define MAGNETIC_PIN PIN6_ID
#define MAGNETIC_PORT PORTD_ID


void MAGNETIC_init(void);

uint8 MAGNETIC_read(void);


#endif
