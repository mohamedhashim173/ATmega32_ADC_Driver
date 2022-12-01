/*
 * ADC_interface.h
 *
 *  Created on: Oct 25, 2022
 *      Author: MSAEED99
 */

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_



// Initialize ADC
void ADC_voidADCInit(void);

// Get the channel read (Be careful, the channel value is from 0 to 7)

/* Input arguments:
 * copy_u8Channel			: Channel pin number of ADC (from 0 to 7) [ex.: ADC0 = PinA0]
 * copy_pvNotificationFunc	: Pointer to function that user want to execute (pointer to ISR [Callback]).
 * copy_pu8ADCRead			: Pointer to user variable that holds ADC read.
 */
u8 ADC_u8GetChannelReadSync(u8 copy_u8Channel);
u8 ADC_u8GetChannelReadAsync(u8 copy_u8Channel, void (*copy_pvNotificationFunc)(void), u8 *copy_pu8ADCRead);


#endif /* ADC_INTERFACE_H_ */

