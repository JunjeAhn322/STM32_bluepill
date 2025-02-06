#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"

extern vu16 var_Exp;
// int main(void)
// {
// 	delay_init();
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
// 	uart_init(9600);
//  	LED_Init();		
// 	TIM2_PWM_Init(999,7199);      
//  	TIM3_PWM_Init(9999,7199);	
	

// 	while(1)
// 	{

// 	}
// }

const char *string = "hello";

int main(void)
{
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(9600);
    LED_Init();		
    TIM2_PWM_Init(999,7199);      
    TIM3_PWM_Init(9999,7199);	
	
    while (1)
    {
		// usart1_send_string(string);
		// delay_ms(500);
    }
}
