#include "timer.h"
#include "led.h"
#include "usart.h"
#include "sys.h"
#include <stdio.h>
#include <string.h>

void TIM3_Int_Init(u16 arr,u16 psc)
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 

	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  

	TIM_Cmd(TIM3, ENABLE);  
}

vu16 varl=0;
vu16 var_Exp=0;
vu16 global_time;
char snum[7];
vu16 shorttt=0;

char gprmcStr[7]="$GPRMC,";
int chckNum=0;
char chckNumChar[10];

int ss=0;
int mm=0;
int hh=0;

unsigned char result;
int i;

int checkNum(const char *gprmcContext)
{
    if (gprmcContext == NULL) 
    {
        
        return -1;
	}

    result = gprmcContext[1];

    for (i = 2; gprmcContext[i] != '*' && gprmcContext[i] != '\0'; i++)
    {
        
        result ^= gprmcContext[i];
    }

    if (gprmcContext[i] != '*') 
    {
        
        return -1;
    }

    
    return result;
}

char value_1[100]="";
char value_2[100]="";
char value_time[10]="";

char test[100]="$GPRMC,004015,A,2812.0498,N,11313.1361,E,0.0,180.0,150122,3.9,W,A*";

void TIM3_IRQHandler(void)   
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  
		LED1=!LED1;
			
			
		TIM2->CNT=TIM2->ARR/2;

		PCout(13)=0;
	}
	
    if(ss<59){
				ss++;
	}else{
		ss=0;
		if(mm<59){
			mm++;
		}else{
			mm=0;
			if(hh<23){
				hh++;
			}else{
				hh=0;
			}
		}
	}

    sprintf(value_2, "%s%02d%02d%02d%s", gprmcStr, hh, mm, ss, ".00,A,2237.496474,N,11356.089515,E,0.0,225.5,060225,2.3,W,A*");
	strcpy(value_1,value_2);
	chckNum =checkNum(value_1);
	sprintf(chckNumChar, "%02X", chckNum);
	usart1_send_string(value_2);      // Send the GPRMC line (without checksum)
    usart1_send_string("23");  // Send the 2-digit checksum
    usart1_send_string("\r\n");       // Newline at the end
	// char* buffer = malloc(strlen(value_2) + strlen(chckNumChar) + 3);
	// strcpy(buffer, value_2);
	// strcat(buffer,chckNumChar);
	// strcat(buffer,"\n");
	// usart1_send_string(buffer);
	// printf("%s", value_2);
    // printf("%s\n", chckNumChar);
	// free(buffer);
}



void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
  	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  	TIM_OCInitTypeDef TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	TIM_DeInit(TIM2);
 
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
   
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
	
	
	TIM_OCInitStructure.TIM_Pulse = 50; 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  

 	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);  
	
	TIM_SetCompare2(TIM2,TIM2->ARR/2);
}

void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  
	
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 
 
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
   
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  
 
	TIM_Cmd(TIM3, ENABLE);  
	
	TIM_SetCompare2(TIM3,TIM3->ARR/2);	

	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);  
}
