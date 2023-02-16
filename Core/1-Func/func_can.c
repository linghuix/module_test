
/**
 * @Name		func_can.c
 *
 * @Date    Created on: Mar 14, 2020
 * @Author: lhx
 */

#include "func_can.h"


/**
 * @brief  send a CAN message
 * @para   phcan - pointer to CAN Handle structure 
					 msg - pointer to CAN send structure 
					 pmailbox - DON'T NEED TO PASS A VALUE
											pointer to a variable where the function will return the TxMailbox used to store the Tx message.
          						This parameter can be a value of @arg CAN_Tx_Mailboxes. CAN_TX_MAILBOX0|CAN_TX_MAILBOX1|CAN_TX_MAILBOX2
 * @Return error: HAL_ERROR / HAL_OK 
 * @Author: lhx
 */
Error MX_CANx_send(CAN_HandleTypeDef *phcan, CanTxMsg *msg, MAIL pmailbox)
{
	Error error;
	error = HAL_CAN_AddTxMessage(phcan, (CAN_TxHeaderTypeDef *)& (msg->head), msg->Data, & pmailbox);
	return error;
}



/**
 * @brief  receive a CAN message
 * @para   phcan - pointer to CAN Handle structure 
					 msg -  DON'T NEED TO PASS A VALUE 
									pointer to the received message 
					 FIFO - 	  Fifo number of the received message to be read. @arg CAN_receive_FIFO_number. CAN_RX_FIFO0|CAN_RX_FIFO1
 * @Return error: HAL_ERROR / HAL_OK 
 * @Author: lhx
 */
Error MX_CANx_get(CAN_HandleTypeDef *phcan, CanRxMsg *msg, uint32_t FIFO)
{
	Error error;
	error = HAL_CAN_GetRxMessage(phcan, FIFO, (CAN_RxHeaderTypeDef *)& (msg->head), (msg->Data));
	return error;
}

/**
 * @brief  start CAN hardware
 * @para   phcan - pointer to CAN Handle structure 

 * @Return none
 * @Author:lhx
 */
void CAN_Start(CAN_HandleTypeDef *phcan)
{
	MSG_BSTART("CAN", "start");
	if(HAL_CAN_Start(phcan) != HAL_OK){
		Error_Handler()
	}
	MSG_ASTART("CAN", "start");
}


/**
  * @brief  	Print the CAN frame your just received through interruption.
  * 
  * @param    None
  * @Note			Must enable RX IT. 
              And received message in FIFO1, FIFO2 all will active this function 
  * @retval	 	None
	* @Author   lhx
  */ 

void CAN1_RX0_IRQHandler(void)
{
	MSG(" CR - ");
	HAL_CAN_IRQHandler(&hcan1);
}


void USB_HP_CAN1_TX_IRQHandler(void)
{
	MSG(" CT\r\n");
	HAL_CAN_IRQHandler(&hcan1);
}


/**
  ******************************************************************************
  * @section    Test
  * @author  xlh
  * @brief   
  ******************************************************************************
  */
/*@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ TEST @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@*/

//#define CAN_TEST

#ifdef CAN_TEST



/**
  * @brief  	To print the CAM receive message data which is sending. 
							IF you need to receive CAN messages and use them to do something. Add it in this function
  * 
  * @param  	CAN_HandleTypeDef: can structure Handler
  * @Note			Must enable RX IT, and use HAL_CAN_IRQHandler function in stm32f10x_it.c
  * @retval	 	None
	* @Author   lhx
  */ 

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CanRxMsg rxmsg;
	if (HAL_CAN_GetRxFifoFillLevel(&hcan1, CAN_RX_FIFO0)>0) {
		MX_CANx_get(&hcan1, &rxmsg, CAN_RX_FIFO0);
		printf("CAN receive: %ld ",rxmsg.head.StdId);
		for(int i=0;i<8;i++){
			printf("-%d",rxmsg.Data[i]);
		}
		printf("\r\n");
		
		// IF you need to receive CAN messages and use them to do something. Add it in this function
		// CAN_RX_Callback(rxmsg)
		
	}
}

/**
 * @author lhx
 *
 * @brief : CAN frame send test
 * 
 */
CanTxMsg txmsg; // CAN_TX_MAILBOX0|CAN_TX_MAILBOX1|CAN_TX_MAILBOX2;
CanRxMsg rxmsg;
int Flag_receive;
void CAN_Send_test(void)
{
	printf("CAN test beginning ...\r\n");

	uint32_t mailbox = 0;
	uint32_t msg[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};

	//MX_CAN1_Test_Init(CAN_MODE_LOOPBACK);
	MX_CAN1_Test_Init(CAN_MODE_NORMAL);
	CAN_Start(&hcan1);

	txmsg.head.StdId = 0x002;
	txmsg.head.DLC = 8;
	txmsg.head.RTR = CAN_RTR_DATA;
	txmsg.head.IDE = CAN_ID_STD;

	for(int i = 0;i < 8;i++){
		txmsg.Data[i] = msg[i];
	}
	
	//int canerror;
	int i;
	while(1)
	{
		if(i++ > 500000){//周期 0.133s 的样子
			i = 0;
			
			printf("send ");
			for(int i = 0;i < 8;i++){
				txmsg.Data[i] = txmsg.Data[i]+1;
				printf("0x%X ",txmsg.Data[i]);
			}
			printf("\r\n");
			
			mailbox = (mailbox+1)%3;
			MX_CANx_send(&hcan1, &txmsg, mailbox);
		}
	}
}


/**
 * @author lhx
 *
 * @brief : CAN frame receive test
 * 
 */
TEST CAN_Rcv_test(void)
{
	printf("CAN receive test beginning ...\r\n");

	//MX_CAN1_Test_Init(CAN_MODE_LOOPBACK);
	MX_CAN1_Test_Init(CAN_MODE_NORMAL);
	CAN_Start(&hcan1);
	while(1){
	}
}

#endif
