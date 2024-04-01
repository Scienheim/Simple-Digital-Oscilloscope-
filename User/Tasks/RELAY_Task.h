//
// Created by 1 on 2024/3/30.
//

#ifndef DIGTAL_OSCILLOGRAPH_RELAY_TASK_H
#define DIGTAL_OSCILLOGRAPH_RELAY_TASK_H

#include "main.h"


#define	REL_A_1_SET  RELAY_A_1_GPIO_Port->BSRR = RELAY_A_1_Pin
#define	REL_A_2_SET  RELAY_A_2_GPIO_Port->BSRR = RELAY_A_2_Pin
#define	REL_B_1_SET  RELAY_B_1_GPIO_Port->BSRR = RELAY_B_1_Pin
#define	REL_B_2_SET  RELAY_B_2_GPIO_Port->BSRR = RELAY_B_2_Pin

#define	REL_A_1_CLR  RELAY_A_1_GPIO_Port->BSRR = RELAY_A_1_Pin << 16U
#define	REL_A_2_CLR  RELAY_A_2_GPIO_Port->BSRR = RELAY_A_2_Pin << 16U
#define	REL_B_1_CLR  RELAY_B_1_GPIO_Port->BSRR = RELAY_B_1_Pin << 16U
#define	REL_B_2_CLR  RELAY_B_2_GPIO_Port->BSRR = RELAY_B_2_Pin << 16U


void RELAY_Set(uint8_t channel, uint8_t operand, uint8_t state);

#endif //DIGTAL_OSCILLOGRAPH_RELAY_TASK_H
