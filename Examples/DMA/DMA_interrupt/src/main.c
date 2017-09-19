/**
  ******************************************************************************
  * @file    DMA/FLASH_RAM/main.c 
  * @author  MCD Application Team
  * @version V3.4.0
  * @date    10/15/2010
  * @brief   Main program body
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "hw_config.h"
#include "stm32f10x_dma.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BufferSize  32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint32_t CurrDataCounterBegin = 0;
uint16_t CurrDataCounterEnd = 0x01;/* This variable should not be initialized to 0 */

const uint32_t SRC_Const_Buffer[BufferSize]= {
                                    0x01020304,0x05060708,0x090A0B0C,0x0D0E0F10,
                                    0x11121314,0x15161718,0x191A1B1C,0x1D1E1F20,
                                    0x21222324,0x25262728,0x292A2B2C,0x2D2E2F30,
                                    0x31323334,0x35363738,0x393A3B3C,0x3D3E3F40,
                                    0x41424344,0x45464748,0x494A4B4C,0x4D4E4F50,
                                    0x51525354,0x55565758,0x595A5B5C,0x5D5E5F60,
                                    0x61626364,0x65666768,0x696A6B6C,0x6D6E6F70,
                                    0x71727374,0x75767778,0x797A7B7C,0x7D7E7F80};
uint32_t DST_Buffer[BufferSize];

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void NVIC_Configuration(void);
void Delay(vu32 nCount);
void DMA_Configuration(void);
void Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength);
    
/* Private functions ---------------------------------------------------------*/

int main(void)
{
    Set_System();

   	/*pause*/
  	getchar();
	
	printf("\r\nDMA1_FLAG_TC6:%d",DMA_GetFlagStatus(DMA1_FLAG_TC6));
	Buffercmp(SRC_Const_Buffer, DST_Buffer, BufferSize);

   	/* System Clocks Configuration */
   	RCC_Configuration();
       
   	/* NVIC configuration */
   	NVIC_Configuration();

   	/* Configure the DMA */
   	DMA_Configuration();

  	/* Enable DMA1 Channel6 Transfer Complete interrupt */
  	DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);

  	/* Get Current Data Counter value before transfer begins */
  	CurrDataCounterBegin = DMA_GetCurrDataCounter(DMA1_Channel6);

    /* Enable DMA1 Channel6 transfer */
    DMA_Cmd(DMA1_Channel6, ENABLE);
	
	
	while (1)
  	{	
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
/*******************************************************************************
* Function Name  : DMA_Configuration
* Description    : Configures the DMA.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DMA_Configuration(void)
{

  	DMA_InitTypeDef DMA_InitStructure;
	
	/* DMA1 channel6 configuration */
  	DMA_DeInit(DMA1_Channel6);
  	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)SRC_Const_Buffer;
  	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DST_Buffer;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  	DMA_InitStructure.DMA_BufferSize = BufferSize;
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;
  	DMA_Init(DMA1_Channel6, &DMA_InitStructure);
  
}
/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
  	/* Enable peripheral clocks ------------------------------------------------*/
  	/* Enable DMA1 clock */
  	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configure the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
  	NVIC_InitTypeDef NVIC_InitStructure;
 	
  	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  	/* Enable DMA1 channel6 IRQ Channel */
  	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *   FAILED: pBuffer differs from pBuffer1
  */
void Buffercmp(const uint32_t* pBuffer, uint32_t* pBuffer1, uint16_t BufferLength)
{
	unsigned short int flag = 0;
  	while(BufferLength--)
  	{
    	if(*pBuffer != *pBuffer1)
    	{
			printf("\r\ntransmitted and received data are not equal.");
      		BufferLength = 0;
			flag = 1;
    	}
    
    	pBuffer++;
    	pBuffer1++;
  	}
	if(flag == 0){
  		printf("\r\ntransmitted and received data are equal");
   	}
}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
