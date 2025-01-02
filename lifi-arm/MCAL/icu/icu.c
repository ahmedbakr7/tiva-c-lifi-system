#include "icu.h"
#include "../../HAL/ultrasonic/ultrasonic.h"
#include "../gpio/gpio.h"
#include "../../tm4c123gh6pm.h"


#define TIMER0A_PIN     PIN6_ID
#define TIMER0A_PORT    PORTB_ID


/* Timer0A initialization function */
/* Initialize Timer0A in input-edge time mode with up-count mode */
void ICU_init(void)
{
    SYSCTL_RCGCTIMER_R |= 1;     /* enable clock to gpt0 */
    SYSCTL_RCGCGPIO_R |= ULTRASONIC_TRIGGER_PORT;      /* enable clock to trigger port */
    while(!(SYSCTL_PRTIMER_R  & 1));
    while(!(SYSCTL_PRGPIO_R & ULTRASONIC_TRIGGER_PORT));
    
    CLEAR_BIT(GPIO_PORTB_DIR_R,TIMER0A_PIN);        // make timer0a pin input
    SET_BIT(GPIO_PORTB_DEN_R,TIMER0A_PIN);           /* make PB6 as digital pin */
    SET_BIT(GPIO_PORTB_AFSEL_R,TIMER0A_PIN);           /* make PB6 as digital pin */
    GPIO_PORTB_PCTL_R &= ~0x0F000000;  /* configure PB6 for T0CCP0 */
    GPIO_PORTB_PCTL_R |= 0x07000000;
    
    CLEAR_BIT(TIMER0_CTL_R,0);          /* disable timer0A during setup */
    TIMER0_CFG_R=4;                     /* 16-bit timer mode */
    TIMER0_TAMR_R=0x17;                 /* up-count, edge-time, capture mode */
    TIMER0_CTL_R |=0x0C;                /* capture the rising edge */
    TIMER0_CTL_R |= (1<<0);             /* enable timer0A */
}

