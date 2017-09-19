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
#include "stm32f10x_i2c.h"
/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define I2C1_SLAVE_ADDRESS7   0x30
#define I2C2_SLAVE_ADDRESS7   0x30
#define BufferSize            10
#define ClockSpeed            200000 // 200 KHz
#define I2C1_EV_IRQChannel           ((u8)0x1F)  /* I2C1 Event Interrupt */
#define I2C1_ER_IRQChannel           ((u8)0x20)  /* I2C1 Error Interrupt */
#define I2C2_EV_IRQChannel           ((u8)0x21)  /* I2C2 Event Interrupt */
#define I2C2_ER_IRQChannel           ((u8)0x22)  /* I2C2 Error Interrupt */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
I2C_InitTypeDef  I2C_InitStructure;
u8 DataSize;
u8 I2C1_Buffer_Tx[BufferSize] = {1,2,3,4,5,6,7,8,9,10};
u8 I2C2_Buffer_Rx[BufferSize];
vu8 Tx_Idx = 0, Rx_Idx = 0, PEC_Value = 0;
volatile TestStatus TransferStatus = FAILED;
/* Private functions ---------------------------------------------------------*/
void Delay(vu32 nCount);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void)
{
  u8 num;
  Set_System();
  getchar();

  NVIC_Configuration();
  GPIO_Configuration();
  printf("How many data do you want to send\r\n");
  scanf("%d", &num);
  
  DataSize = num; 
  printf("DataSize %d\r\n", DataSize);
  /* I2C1 configuration ------------------------------------------------------*/
  
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;				 // I2C家Α
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	//е硉家Α匡兜100KHZΤノ
  I2C_InitStructure.I2C_OwnAddress1 = I2C1_SLAVE_ADDRESS7;	//slave address
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;					//–Ω常穦癳ACK
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;	  // I2C 硉皌竚硂絛ㄒ琌200KHz琌40KHZ400KHZ琌伐ぃê或蔼
  I2C_Init(I2C1, &I2C_InitStructure);
  printf("I2C1 configuration\r\n");

  /* I2C2 configuration ------------------------------------------------------*/
  
  I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE_ADDRESS7;
  I2C_Init(I2C2, &I2C_InitStructure);
  printf("I2C2 configuration\r\n");


  /*----- Transmission Phase -----*/
  /* Send I2C1 START condition */
  
  /* Enable I2C1 and I2C2 event and buffer interrupt */
  I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
  I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, ENABLE);
  /* Enable I2C1 and I2C2 ----------------------------------------------------*/
  I2C_Cmd(I2C1, ENABLE);
  I2C_Cmd(I2C2, ENABLE);

  I2C_GenerateSTART(I2C1, ENABLE);
  printf("Send I2C1 START condition\r\n");
  /* Send data */
  
  
  while(Rx_Idx < (num+1))
  {
	
  }
  
  /* Check the corectness of written data */
  printf("Check the corectness of written data\r\n");
  TransferStatus = Buffercmp(I2C1_Buffer_Tx, I2C2_Buffer_Rx, num);
  printf("Buffercmp Finish\r\n");

  /* TransferStatus = PASSED, if the transmitted and received data are equal */
  /* TransferStatus = FAILED, if the transmitted and received data are different */
 
  /*
  if(TransferStatus) GPIO_Write(GPIOB, GPIO_Pin_8);
  else GPIO_Write(GPIOB, GPIO_Pin_9);    
  */
  
  if(TransferStatus) printf("Transmission SUCESS\r\n");
  else printf("Transmission FAILED\r\n");  
  while(1)
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
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure I2C1 pins: SCL and SDA ----------------------------------------*/
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 |GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  /* Configure I2C2 pins: SCL and SDA ----------------------------------------*/
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  //GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  // Configure PD.08 & PD.09 for LED1,2 
  /*
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  */
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location.
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
  NVIC_SetVectorTable(0x8003000, 0x0);   
#endif

  /* 1 bit for pre-emption priority, 3 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
   
  /* Configure and enable I2C1 interrupt -------------------------------------*/
  NVIC_InitStructure.NVIC_IRQChannel = I2C1_EV_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Configure and enable I2C2 interrupt -------------------------------------*/  
  NVIC_InitStructure.NVIC_IRQChannel = I2C2_EV_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : Buffercmp
* Description    : Compares two buffers.
* Input          : - pBuffer1, pBuffer2: buffers to be compared.
*                : - BufferLength: buffer's length
* Output         : None
* Return         : PASSED: pBuffer1 identical to pBuffer2
*                  FAILED: pBuffer1 differs from pBuffer2
*******************************************************************************/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}

#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif	
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
