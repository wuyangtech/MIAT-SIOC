/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : stm32f10x_it.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Main Interrupt Service Routines.
*                      This file provides template for all exceptions handler
*                      and peripherals interrupt service routine.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "stm32f10x_i2c.h"
#include "usb_lib.h"
#include "usb_istr.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define BufferSize 10
#define I2C2_SLAVE_ADDRESS7   0x30
/* Private variables ---------------------------------------------------------*/
extern vu8 Tx_Idx, Rx_Idx, PEC_Value, DataToTransfer;
extern u8 I2C1_Buffer_Tx[], I2C2_Buffer_Rx[], DataSize;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/*******************************************************************************
* Function Name  : NMI_Handler
* Description    : This function handles NMI exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NMI_Handler(void)
{
}

/*******************************************************************************
* Function Name  : HardFault_Handler
* Description    : This function handles Hard Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : MemManage_Handler
* Description    : This function handles Memory Manage exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : BusFault_Handler
* Description    : This function handles Bus Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : UsageFault_Handler
* Description    : This function handles Usage Fault exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : SVC_Handler
* Description    : This function handles SVCall exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SVC_Handler(void)
{
}

/*******************************************************************************
* Function Name  : DebugMon_Handler
* Description    : This function handles Debug Monitor exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void DebugMon_Handler(void)
{
}

/*******************************************************************************
* Function Name  : PendSV_Handler
* Description    : This function handles PendSVC exception.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PendSV_Handler(void)
{
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : This function handles SysTick Handler.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

#ifndef STM32F10X_CL
/*******************************************************************************
* Function Name  : USB_LP_CAN1_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}
#endif /* STM32F10X_CL */

#if defined (USE_STM3210B_EVAL) || defined (USE_STM3210E_EVAL)
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
  if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  {
    /* Send the received data to the PC Host*/
    USART_To_USB_Send_Data();
  }

  /* If overrun condition occurs, clear the ORE flag and recover communication */
  if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
  {
    (void)USART_ReceiveData(USART1);
  }
}
#endif /* USE_STM3210B_EVAL or USE_STM3210E_EVAL */

#ifdef USE_STM3210C_EVAL
/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
  if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
  {
    /* Send the received data to the PC Host*/
    USART_To_USB_Send_Data();
  }

  /* If overrun condition occurs, clear the ORE flag and recover communication */  
  if (USART_GetFlagStatus(USART2, USART_FLAG_ORE) != RESET)
  {
    (void)USART_ReceiveData(USART2);
  }
}
#endif /* USE_STM3210C_EVAL */

#ifdef STM32F10X_CL
/*******************************************************************************
* Function Name  : OTG_FS_IRQHandler
* Description    : This function handles USB-On-The-Go FS global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void OTG_FS_IRQHandler(void)
{
  STM32_PCD_OTG_ISR_Handler(); 
}
#endif /* STM32F10X_CL */

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/*******************************************************************************
* Function Name  : PPP_IRQHandler
* Description    : This function handles PPP interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*void PPP_IRQHandler(void)
{
}*/
/*******************************************************************************
* Function Name  : I2C1_EV_IRQHandler
* Description    : This function handles I2C1 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_EV_IRQHandler(void)
{
	vu8 num;
	//printf("DataSize222 %d\r\n", DataSize);	
	switch (I2C_GetLastEvent(I2C1))
  {
    /* Test on I2C1 EV5 and clear it */
    case I2C_EVENT_MASTER_MODE_SELECT:               
      /* Send I2C2 slave Address for write */
      I2C_Send7bitAddress(I2C1, I2C2_SLAVE_ADDRESS7, I2C_Direction_Transmitter);
      break;

    /* Test on I2C1 EV6 and first EV8 and clear them */
    case I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED: 
      /* I2C1 and I2C2 PEC Transmission Enable */
      I2C_CalculatePEC(I2C1, ENABLE);
      I2C_CalculatePEC(I2C2, ENABLE);
      /* Send the first data */
	  num = Tx_Idx++; 
      I2C_SendData(I2C1, I2C1_Buffer_Tx[num]);  //EV8 just after EV6
	  printf("Send Data %d...\r\n", I2C1_Buffer_Tx[num]);
      break;

    /* Test on I2C1 EV8 and clear it */
    case I2C_EVENT_MASTER_BYTE_TRANSMITTED:          
      if(Tx_Idx < DataSize)
      {
        /* Send buffer data */
		num = Tx_Idx++;
        I2C_SendData(I2C1, I2C1_Buffer_Tx[num]);
		printf("Send Data %d...\r\n", I2C1_Buffer_Tx[num]);
      }
      else
      {
        /* Disable I2C1 interrupts */
        I2C_ITConfig(I2C1, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
		
        /* Enable Transfer PEC next for I2C1 and I2C2 */
        I2C_TransmitPEC(I2C2, ENABLE);
        I2C_TransmitPEC(I2C1, ENABLE);
      }
      break;

    default:
      break;
  }
 // I2C_ClearITPendingBit(I2C1,I2C_IT_SMBALERT | I2C_IT_TIMEOUT |I2C_IT_PECERR | I2C_IT_OVR | I2C_IT_AF | I2C_IT_ARLO | I2C_IT_BERR);
  //I2C_ClearITPendingBit(I2C1,0xffffffff);
}

/*******************************************************************************
* Function Name  : I2C1_ER_IRQHandler
* Description    : This function handles I2C1 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_ER_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : I2C2_EV_IRQHandler
* Description    : This function handles I2C2 Event interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_EV_IRQHandler(void)
{
  
	switch (I2C_GetLastEvent(I2C2))
  {
    /* Test on I2C2 EV1 and clear it */
    case I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED:   
      break;

    /* Test on I2C2 EV2 and clear it */
    case I2C_EVENT_SLAVE_BYTE_RECEIVED:              
      if (Rx_Idx < DataSize)
      {
        /* Store received data buffer */
        I2C2_Buffer_Rx[Rx_Idx++] = I2C_ReceiveData(I2C2); 
      }
      else
      {
        /* Store received PEC value */
        PEC_Value = I2C_ReceiveData(I2C2);
        Rx_Idx++;
        /* Send I2C1 STOP Condition */
        I2C_GenerateSTOP(I2C1, ENABLE);
      }     
      break; 

    /* Test on I2C2 EV4 and clear it */
    case I2C_EVENT_SLAVE_STOP_DETECTED:              
      /* Clear STOPF flag */
      I2C_ClearFlag(I2C2, I2C_FLAG_STOPF);
      /* Disable I2C2 interrupts */
      I2C_ITConfig(I2C2, I2C_IT_EVT | I2C_IT_BUF, DISABLE);
      break;

    default:
      break;
  }
 // I2C_ClearITPendingBit(I2C2,I2C_IT_SMBALERT | I2C_IT_TIMEOUT |I2C_IT_PECERR | I2C_IT_OVR | I2C_IT_AF | I2C_IT_ARLO | I2C_IT_BERR);
 //I2C_ClearITPendingBit(I2C2,0xffffffff);
}

/*******************************************************************************
* Function Name  : I2C2_ER_IRQHandler
* Description    : This function handles I2C2 Error interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C2_ER_IRQHandler(void)
{
}
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/

