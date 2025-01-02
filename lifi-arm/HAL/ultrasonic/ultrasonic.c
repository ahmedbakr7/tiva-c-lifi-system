#include "ultrasonic.h"
#include "../../MCAL/timer_ic/timer.h"
#include "../../MCAL/gpio/gpio.h"

#define F_CPU 16000000UL              // Frequency 16Mhz

void delay_ms(uint32 ms)
{
	uint32 delay= ms*(F_CPU/1000)/3-1;
	uint32 i=0;
	for ( i=0; i<delay; i++);
}


void ULTRASONIC_init(void){
    Timer0_init();
    GPIO_Init(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, PIN_OUTPUT,0);
}

void ULTRASONIC_trigger(void){
    GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_LOW);
    delay_ms(10);
    GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_HIGH);
    delay_ms(10);
    GPIO_writePin(ULTRASONIC_TRIGGER_PORT, ULTRASONIC_TRIGGER_PIN, LOGIC_LOW);
}

uint16 ULTRASONIC_getDistance(void){
    ULTRASONIC_trigger();
    uint16 time = Timer0_getPulseDuration();      
    uint16 distance = (time * 10625)/10000000;  // Distance in centimeters
    return distance;
}
