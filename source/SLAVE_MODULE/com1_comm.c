/**
  ******************************************************************************
  * @file    com1_comm.c
  * @author  ����
  * @version V1.0.0
  * @date    2017.7.13
  * @brief   ��1·���ڵĽṹ��Ϣ��ʼ������
  ******************************************************************************
  */
  


#include "module_manage.h"
#include "USART3.H"
#include "com_comm.h"


static uint8_t *get_com1_frame(COM_STRUCT *com);
static uint32_t get_com1_frame_len(COM_STRUCT *com);
static void get_com1_send_frame(COM_STRUCT *com, uint8_t *data, uint32_t len);
static void get_com1_resend_frame(COM_STRUCT *com);
static uint8_t get_com1_send_status(COM_STRUCT *com);
//static void com1_wait_ack_timeout(void);
static uint8_t get_com1_receive_over_flag(COM_STRUCT *com);

/**
  * @brief  ��1·���ڽṹ��Ϣ
  */
COM_STRUCT com1={
    MODULE_COMM_RECEIVE,///<����״̬
    get_com1_send_frame,///<���ͺ���
    get_com1_resend_frame,///<�ط�����
    receive_over_dispose,///<������ɺ�Ĵ�����
    get_com1_receive_over_flag,///<��ȡ������ɱ�־
    get_com1_frame,///<��ȡ��������֡
    get_com1_frame_len,///<��ȡ��������֡����
    get_com1_send_status,///<��ȡ����ͨ�ŵķ���״̬
};
/**
  * @brief  ��1·���ڷ������ݺ���,ֱ�ӵ��ô���������ķ��ͺ���
  * @param  [in] com ������ָ��
  * @param  [in] data ����
  * @param  [in] len ���ݳ���
  * @retval None
  */
static void get_com1_send_frame(COM_STRUCT *com, uint8_t *data, uint32_t len)
{
    usart3_send_data(data, len);
}

/**
  * @brief  ��1·�����ط�����,ֱ�ӵ��ô����������ṩ���ط�����
  * @param  [in] com ������ָ��
  * @retval None
  */
static void get_com1_resend_frame(COM_STRUCT *com)
{
    usart3_resend_data();
}
/**
  * @brief  ��ȡ��1·���ڽ��յ�������
  * @param  [in] com ������ָ��
  * @retval ���յ�������֡
  */
static uint8_t *get_com1_frame(COM_STRUCT *com)
{
    return usart3_get_receive_data();
}

/**
  * @brief  ��ȡ��1·���ڽ��յ������ݳ���
  * @param  [in] com ������ָ��
  * @retval ���յ�������֡����
  */
static uint32_t get_com1_frame_len(COM_STRUCT *com)
{
    uint32_t len = 0;
    
    len = usart3_get_receive_data_count();
    usart3_clear_receive_data_count();
    
    return len;
}

/**
  * @brief  ��ȡ���ڽ�����ɱ�־
  * @param  [in] com ������ָ��
  * @retval ���ڽ������״̬ 0 δ��� 1 �������
  */
static uint8_t get_com1_receive_over_flag(COM_STRUCT *com)
{
    uint8_t flag = get_usart3_receive_over_flag();
    
    if(flag)
    {
        clear_usart3_receive_over_flag();//���������ɱ�־
    }
    
    return flag;
}
/**
  * @brief  ��ȡ��1·���ڷ���״̬
  * @param  [in] com ������ָ��
  * @retval ���ڷ���״̬ 0 δ��� 1 �������
  */
static uint8_t get_com1_send_status(COM_STRUCT *com)
{
    return usart3_get_send_status();
}
/************************ (C) COPYRIGHT 2017 ��ʢ���� *****END OF FILE****/
