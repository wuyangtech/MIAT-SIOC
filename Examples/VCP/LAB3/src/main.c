/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : main.c
* Author             : MIAT
* Version            : V1.0
* Date               : 03/19/2011
* Description        : Virtual Com Port Demo main file	: printf hello, world!
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "hw_config.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void Delay(vu32 nCount);
/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
  //int a,b;

  Set_System();
    
  while(1)
    {

		char name[10];               
    	float w,h;

		getchar();

        	printf("\n\r");   
	        printf("Please input your name:");
    	    scanf("%s", &name);
    		
	    	printf("\n\r");
    	    printf("Please input your Weight:");
    		scanf("%f", &w);
    
	    	printf("\n\r");
    		printf("Please input your Height:");
    		scanf("%f", &h);
    
    		if ((w/(h*h))<18.5){
        		printf("\n\r");
        		printf("hi %s, your BMI is %f, too light!", name,(w/(h*h)));
        		printf("\n\r");
        	}
        	else if ((w/(h*h))>=18.5&&(w/(h*h))<24){
             	printf("\n\r");
    	     	printf("hi %s, your BMI is %f, perfect!", name,(w/(h*h)));
    	     	printf("\n\r");
        	}
        	else{
            	printf("\n\r");
    	    	printf("hi %s, your BMI is %f, overweight!", name,(w/(h*h)));
    	    	printf("\n\r");
        	}

	}
}  
/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}	
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
