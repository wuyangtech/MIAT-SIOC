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
		int i, checksum;
    	char str[10];
    	char string[]="ABCDEFGHJKLMNPQRSTUVXYWZIO"; //10~35


		getchar();

    		printf("\n\renter your ID\n\r");
        	scanf("%s",&str);   
    		if ( strlen(str) == 10 && (str[1] == '1' ||str[1] == '2'))    
	    	{    
    		    i = strchr(string, str[0]) - string + 10;       
        	    checksum = i / 10 + i % 10 * 9;
    	    	for(i = 1; i < 9; ++i) 
    	        	checksum += (str[i] - '0') * (9 - i);
    	    	checksum += (str[9] - '0');
            	if (checksum % 10 == 0)
               		printf("\n\rCorrect\n\r");
            	else
                	printf("\n\rError\n\r");
         	}
			else
				printf("\n\rError\n\r");	
     
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
