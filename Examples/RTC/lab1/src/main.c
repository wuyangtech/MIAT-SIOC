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
#include "stm32f10x_bkp.h"
#include "stm32f10x_rtc.h"
#include "stm32f10x.h "
#include "stm32f10x_pwr.h"

/* Private typedef -----------------------------------------------------------*/

/*#define BKP_DR1                           ((uint16_t)0x0004)
#define RTC_IT_ALR           ((uint16_t)0x0002)  
#define RTC_IT_SEC           ((uint16_t)0x0001)  
#define RTC_IRQChannel               ((u8)0x03)  
*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
vu32 TimeDisplay = 0;

/* Private functions ---------------------------------------------------------*/
void RTC_Configuration(void);
void NVIC_Configuration(void);

u32 Time_Regulate(void);
void Time_Adjust(void);
void Time_Show(void);
void Time_Display(u32 TimeVar);
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

  Set_System();
  
  /*pause*/
  getchar();

  printf("\r\n **********Set System**********");
  /* NVIC configuration */
  NVIC_Configuration();
  printf("\r\n **********NVIC Configuration**********");
    
 
   if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A9)
  {
    
	/* Backup data register value is not correct or not yet programmed (when
       the first time the program is executed) */

    printf("\r\n\n RTC not yet configured....");

    /* RTC Configuration */
    RTC_Configuration();

    printf("\r\n RTC configured....");

    /* Adjust time by values entred by the user on the hyperterminal */
    Time_Adjust();

    BKP_WriteBackupRegister(BKP_DR1, 0xA5A9);
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if(RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET) printf("\r\n\n Power On Reset occurred....");
	else if(RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET) printf("\r\n\n External Reset occurred....");	    

	printf("\r\n No need to configure RTC....");
 
  
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

	/* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
	

  }

  /* Clear reset flags */
  RCC_ClearFlag();


  while(1)
  	Time_Show();
	
 
}  
/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  //NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_SetVectorTable(0x08003000, 0x0);
#endif

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* Enable the RTC Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn; //RTC_IRQChannel;
  //RTC_IRQn 
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/*******************************************************************************
* Function Name  : RTC_Configuration
* Description    : Configures the RTC.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RTC_Configuration(void)
{
  /* Enable PWR and BKP clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  /* Allow access to BKP Domain */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset Backup Domain */
  BKP_DeInit();

  /* Enable LSE */
  RCC_LSEConfig(RCC_LSE_ON);
  /* Wait till LSE(low speed external) is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /* Select LSE as RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

  /* Enable RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC registers synchronization */
  RTC_WaitForSynchro();

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();

  /* Enable the RTC Second */
  RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();


  /* Set RTC prescaler: set RTC period to 1sec */
  RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/*******************************************************************************
* Function Name  : Time_Regulate
* Description    : Returns the time entered by user, using Hyperterminal.
* Input          : None
* Output         : None
* Return         : Current time RTC counter value
*******************************************************************************/
u32 Time_Regulate(void)
{
  u32 Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

    Tmp_HH =0;
   
    Tmp_MM = 0;

    Tmp_SS = 0;	
	

  /* Return the value to store in RTC counter register */
  return((Tmp_HH*3600 + Tmp_MM*60 + Tmp_SS));
 
}

/*******************************************************************************
* Function Name  : Time_Adjust
* Description    : Adjusts time.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Time_Adjust(void)
{

  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
  /* Change the current time */
  RTC_SetCounter(Time_Regulate());
  /* Wait until last write operation on RTC registers has finished */
  RTC_WaitForLastTask();
}

/*******************************************************************************
* Function Name  : Time_Display
* Description    : Displays the current time.
* Input          : - TimeVar: RTC counter value.
* Output         : None
* Return         : None
*******************************************************************************/
void Time_Display(u32 TimeVar)
{
  u32 THH = 0, TMM = 0, TSS = 0;

  /* Compute  hours */
  THH = TimeVar / 3600;
  /* Compute minutes */
  TMM = (TimeVar % 3600) / 60;
  /* Compute seconds */
  TSS = (TimeVar % 3600) % 60;

  printf("Time: %0.2d:%0.2d:%0.2d\r", THH, TMM, TSS);
}

/*******************************************************************************
* Function Name  : Time_Show
* Description    : Shows the current time (HH:MM:SS) on the Hyperterminal.
* Input          : None
* Output         : None
* Return         : None
******************************************************************************/
void Time_Show(void)
{
  printf("\n\r");

  /* Infinite loop */
  while (1)
  {
    /* If 1s has paased */
    if (TimeDisplay == 1)
    {
      /* Display current time */
      Time_Display(RTC_GetCounter());
      TimeDisplay = 0;
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
