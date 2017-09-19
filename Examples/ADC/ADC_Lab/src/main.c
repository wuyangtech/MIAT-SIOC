/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : main.c
* Author             : MIAT
* Version            : V1.0
* Date               : 04/12/2011
* Description        : A/D conveter.
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
#include "hw_config.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_adc.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address    ((u32)0x4001244C)
vu16 ADCConvertedValue;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void DMA_Configuration(void);
void ADC_Configuration(void); 
void Count_Temperature(void);
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
  /* System USB configuration  */
  Set_System();
  
  /*pause*/
  getchar();

  /* System clocks configuration  */
  RCC_Configuration();

 /* GPIO configuration */
  GPIO_Configuration();

  /* DMA1 channel1 configuration */
  DMA_Configuration();

  /* ADC1 configuration*/
  ADC_Configuration(); 

  printf("Configuration finish\n\r");
  printf("Please presee DFU Button to get Temperature\n\r");

  while(1)
  {
  	/*presee DFU Button to get Temperature*/
    if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==1)
	{
  		Count_Temperature();
	} 	
 }
  
	
 
}  
/*******************************************************************************
* Function Name  : RCC Configuration
* Description    : configuration the RCC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* Enable ADC1 clock */								   
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE);
  /* Enable GPIOB clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
}

/*******************************************************************************
* Function Name  : GPIO Configuration
* Description    : configuration the GPIO
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : DMA1 chagnel1 Configuration
* Description    : configuration the DMA
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void DMA_Configuration(void)
{

  DMA_InitTypeDef DMA_InitStructure;
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);

  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);
}

/*******************************************************************************
* Function Name  : ADC1_Configuration
* Description    : configuration the ADC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfChannel = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel16 configuration */ 
    //insert your code
 
  /* Temperature Enable */
  //insert your code

  /* Enable ADC1 DMA */
  //insert your code

  /* Enable ADC1 */
 //insert your code

  /* Enable ADC1 reset calibaration register */   
  //insert your code


  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);

  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));

  /* Start ADC1 Software Conversion */ 
  //insert your code

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

/*******************************************************************************
* Function Name  : Count_Temperature
* Description    : Count_Temperature
* Input          : None.
* Output         : None
* Return         : None
*******************************************************************************/
void Count_Temperature(void)
{

	//キА16Ω ADCConvertedValue ㄓ璸衡(ゑ耕非絋)
	//放ゲ斗丁筳叫ㄏノDelay();
	//放=	( 把σщ紇そΑ)
	//瞷放

}

