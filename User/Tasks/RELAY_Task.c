//
// Created by 1 on 2024/3/30.
//
#include "RELAY_Task.h"

#include "KEY_Task.h"


void RELAY_Set(uint8_t channel, uint8_t operand, uint8_t state){
	switch ( (channel << 1 ) | operand ) {
		case 0b00:
			if (state ) REL_A_1_SET;
			else	REL_A_1_CLR;
			break;
		case 0b01:
			if (state ) REL_A_2_SET;
			else	REL_A_2_CLR;
			break;
		case 0b10:
			if (state ) REL_B_1_SET;
			else	REL_B_1_CLR;
			break;
		case 0b11:
			if (state ) REL_B_2_SET;
			else	REL_B_2_CLR;
			break;
	}
}

void RELAY_Ctrl(){

	for ( int i = 0; i < 2; ++i ) {
		RELAY_Set(i, 0, Oscillograph.coupler[i]);
		RELAY_Set(i, 1, Oscillograph.attenuator[i]);
	}

}