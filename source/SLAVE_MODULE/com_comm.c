/**
  ******************************************************************************
  * @file    com_comm.h
  * @author  ����
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   ���ڵĽṹ��Ϣ
  ******************************************************************************
  */

#include "module_manage.h"
#include "string.h"
#include "crc.h"
#include "USART3.H"
#include "Timer5_Config.h"

/**
  * @brief  ������ɵĴ�����
  * @param  [in] com ��������ָ��
  * @retval ��
  */
void receive_over_dispose(COM_STRUCT *com)
{
    uint8_t *frame;
    uint32_t len;
    
    frame = com->get_com_frame(com);
    len = com->get_com_frame_len(com);
    
    com_receive_dispose(com, frame, len);
}

/**
  * @brief  ����ͨ��״̬��
  * @param  None
  * @retval None
  */
void com_comm_status_machine(COM_STRUCT *com)
{
    switch(com->status)
    {
        case MODULE_COMM_RECEIVE:
            if(com->get_receive_over_flag(com))
            {
                com->receive_dispose_fun(com);
            }
            break;
        case MODULE_COMM_SEND:
            if(com->get_com_send_status(com))
            {
                com->status = MODULE_COMM_RECEIVE;//�������״̬
            }
            break;
    }
}

/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
