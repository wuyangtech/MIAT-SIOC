/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************

* File Name          : main.c

* Author             : MIAT

* Version            : V1.0

* Date               : 03/19/2011

* Description        : SPI _ CRC

********************************************************************************

* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/

#include "stm32f10x.h"

#include "stdio.h"

#include "hw_config.h"

#include "stm32_eval.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BufferSize  32

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
SPI_InitTypeDef  SPI_InitStructure;


uint16_t  SPI2_Buffer_Rx[BufferSize]={55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55,55, 55,55,55,55,55,55,55,55};
uint32_t  SPI2_Buffer_Data[BufferSize/2]={55,55,55,55,55,55,55,55,55,55,55,55,55,55};
uint32_t TxIdx = 0, RxIdx = 0;
__IO uint16_t CRC1Value = 0, CRC2Value = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;

/* Private functions ---------------------------------------------------------*/

void Delay(vu32 nCount);
void RCC_Configuration(void);
void GPIO_Configuration(void);
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);

/*******************************************************************************

* Function Name  : main.

* Description    : Main routine.

* Input          : None.

* Output         : None.

* Return         : None.

*******************************************************************************/

int main(void)

{

  int a,b;

  SPI_I2S_DeInit(SPI2);
  Set_System();
  
  /* System clocks configuration ---------------------------------------------*/
  RCC_Configuration();

  /* GPIO configuration ------------------------------------------------------*/
  GPIO_Configuration();

  /* SPI1 configuration ------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;

  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
 // SPI_InitStructure.SPI_CRCPolynomial = 7;


  /* SPI2 configuration ------------------------------------------------------*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
  SPI_Init(SPI2, &SPI_InitStructure);

  

  
  /* Enable SPI2 */
    SPI_Cmd(SPI2, ENABLE);

  
  /* Transfer procedure */
  while (RxIdx< BufferSize )
  {
    
    
    /* Wait for SPI2 data reception */
   while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);
    /* Read SPI2 received data */
     SPI2_Buffer_Rx[RxIdx] = SPI_I2S_ReceiveData(SPI2);
	 
	 if(RxIdx % 2 == 0)
		  SPI2_Buffer_Data[RxIdx/2] = SPI2_Buffer_Rx[RxIdx] << 16;
	 else
    	   SPI2_Buffer_Data[RxIdx/2] += SPI2_Buffer_Rx[RxIdx] ;
	RxIdx++;
		   
  }

 

  
  

  
    

  while (1)

  { 
  								   
	getchar();
	
	

	a = 0;
	while (a <  BufferSize/2) 
	{
	 printf("SPI2 RX%d : %d\n\r",a,SPI2_Buffer_Data[a]); 
	 a++;
	}
	

       

    

  }

}  
 /**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* PCLK2 = HCLK/2 */
  RCC_PCLK2Config(RCC_HCLK_Div2);

  /* Enable peripheral clocks --------------------------------------------------*/
  /* GPIOA, GPIOB and SPI1 clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_SPI1, ENABLE);

  /* SPI2 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure SPI1 pins: SCK, MISO and MOSI ---------------------------------*/
  /* Confugure SCK and MOSI pins as Alternate Function Push Pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  /* Confugure MISO pin as Input Floating  */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* Configure SPI2 pins: SCK, MISO and MOSI ---------------------------------*/
  /* Confugure SCK and MOSI pins as Input Floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Confugure MISO pin as Alternate Function Push Pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
 
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer1 identical to pBuffer2
  *   FAILED: pBuffer1 differs from pBuffer2
  */
TestStatus Buffercmp(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }

    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;
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

