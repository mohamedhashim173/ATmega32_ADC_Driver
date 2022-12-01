/*
 * ADC_program.c
 *
 *  Created on: Oct 25, 2022
 *      Author: MSAEED99
 */


// Libraries
#include "STD_TYPES.h"
#include "BIT_MATH.h"

// MCAL
#include "ADC_register.h"
#include "ADC_config.h"
#include "ADC_private.h"
#include "ADC_interface.h"


// Pointer Function to be used Asynchronous function
void (*ADC_PtrToNotificationFunction)(void) = NULL;

// Pointer to hold the ADC read for Asynchronous function
u8 *ADC_pu8PtrToRead = NULL;

/**********************************************************************/

// Initialize ADC
void ADC_voidADCInit(void)
{
	// Choose AVCC as Reference Voltage (Vref)
	SET_BIT(ADMUX, ADMUX_REFS0);
	CLR_BIT(ADMUX, ADMUX_REFS1);

	// Enable left adjust (8-bit resolution)
	SET_BIT(ADMUX, ADMUX_ADLAR);

	// Set the pre-scaler value (conversion frequency) (Divide by 128)
	SET_BIT(ADCSRA, ADCSRA_ADPS0);
	SET_BIT(ADCSRA, ADCSRA_ADPS1);
	SET_BIT(ADCSRA, ADCSRA_ADPS2);

	// Enable ADC peripheral
	SET_BIT(ADCSRA, ADCSRA_ADEN);
}


// Get the channel read (Be careful, the channel value is from 0 to 7)
u8 ADC_u8GetChannelReadSync(u8 copy_u8Channel)
{
	u32 local_u32Counter = COUNTER_START_VAL;
	// Bit Masking
	// 1. Clear the required bits
	ADMUX &= 0b11100000;
	// 2. Set the needed value to MUX bits
	ADMUX |= copy_u8Channel;

	// Start conversion
	SET_BIT(ADCSRA, ADCSRA_ADSC);

	// Polling (Busy waiting) till the conversion is done or the counter value reaches the timeout
	// Timeout mechanism for sync functions (loop breaks when timer times out)
	while((GET_BIT(ADCSRA, ADCSRA_ADIF) == 0) && (local_u32Counter < ADC_TIMEOUT_VAL))
	{
		// Timer counter
		local_u32Counter++;
	}

	// Action on timeout
	if(local_u32Counter == ADC_TIMEOUT_VAL)
	{
		// Set the error code with timeout error (we haven't implemented error state)
	}
	else
	{
		// Conversion is done before timeout and the flag is risen
		// Clear flag
		SET_BIT(ADCSRA, ADCSRA_ADIF);
	}

	// Return ADC data register ADCH (using left adjustment 8-bit resolution)
	return ADCH;
}


/* Input arguments:
 * copy_u8Channel			: Channel pin number of ADC (from 0 to 7) [example.: ADC0 = PinA0]
 * copy_pvNotificationFunc	: Pointer to function that user want to execute (pointer to ISR [Callback]).
 * copy_pu8ADCRead			: Pointer to user variable that holds ADC read.
 */
u8 ADC_u8GetChannelReadAsync(u8 copy_u8Channel, void (*copy_pvNotificationFunc)(void), u8 *copy_pu8ADCRead)
{
	u8 local_u8ErrorCode = OK;

	if((copy_pvNotificationFunc != NULL) && (copy_pu8ADCRead != NULL))
	{
		ADC_pu8PtrToRead = copy_pu8ADCRead;
		ADC_PtrToNotificationFunction = copy_pvNotificationFunc;

		// Bit Masking to set the required channel
		// 1. Clear the required bits
		ADMUX &= 0b11100000;
		// 2. Set the needed value to MUX bits
		ADMUX |= copy_u8Channel;

		// Start conversion
		SET_BIT(ADCSRA, ADCSRA_ADSC);

		// Enable ADC interrupt [ADC conversion complete (vector_16)]
		SET_BIT(ADCSRA, ADCSRA_ADIE);
	}
	else
	{
		local_u8ErrorCode = NOT_OK;
	}

	return local_u8ErrorCode;
}


// ISR implementation (ADC conversion complete: vector_16)
// When the program execute ISR, that means the conversion is done!
void __vector_16(void) __attribute__((signal));
void __vector_16(void)
{
	// Read ADC result
	*ADC_pu8PtrToRead = ADCH;

	// Invoke notification function
	ADC_PtrToNotificationFunction();

	// Disable ADC interrupt [ADC conversion complete (vector_16)]
	CLR_BIT(ADCSRA, ADCSRA_ADIE);
}
































