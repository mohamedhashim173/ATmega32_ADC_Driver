/*
 * main.c
 *
 *  Created on: Oct 25, 2022
 *      Author: MSAEED99
 */


// Libraries
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// MCAL
#include "DIO_interface.h"
#include "ADC_interface.h"

// Prototypes
void ADCNotificationFunction(void);

// Global variable
u8 App_u8ADCChannelRead;

/* Use Async function*/

void main(void)
{
	u8 local_u8ErrorCode = OK;

	// Set PortC as output for LEDs
	DIO_voidSetPortDirection(DIO_u8PORTC, DIO_u8PORT_OUTPUT);

	// Set ADC0 channel on PinA0 as input
	DIO_voidSetPinDirection(DIO_u8PORTA, DIO_u8PIN0, DIO_u8PIN_INPUT);

	// Initialize ADC
	ADC_voidADCInit();

	while(1)
	{
		// Update ADC channel read by ADC interrupt (Asynchronous mode)
		local_u8ErrorCode = ADC_u8GetChannelReadAsync(0, &ADCNotificationFunction, &App_u8ADCChannelRead);
	}
}


// Callback function to ISR
void ADCNotificationFunction(void)
{
	// Set channel read as output to 8 LEDs
	DIO_voidSetPortValue(DIO_u8PORTC, App_u8ADCChannelRead);
}


