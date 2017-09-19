#include "stm32f10x_lib.h"

void init_PWM(void);
void PWM_output(u16 DutyCycle,u32 Frequency);
void TIM_Configuration (void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
