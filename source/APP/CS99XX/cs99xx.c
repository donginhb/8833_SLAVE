/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx.c
 * ժ  Ҫ  ���������������Գ���
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

/****************** ����ͷ�ļ� **************************/
#include "cs99xx_config.h"


#include	"SPI_CPLD.h"
#include    "cs99xx_result.h"
#include 	"serve_test.h"
#include	"cs99xx_collect.h"
#include    "cs99xx_type.h"
#include    "cs99xx_relay_motion.h"
#include    <string.h>

/************************* ��ʼ��ȫ�ֱ�־ *************************/

/************************* �������� *************************/


/*
 * ��������updata_time
 * ����  �����²�����ʾʱ��
 * ����  ��n ��ǵ�ǰ��ƫ�ƻ��ǲ��� 0 ���� 1ƫ��
 * ���  ����
 * ����  ����
 */
void updata_time(const int8_t n, uint16_t time)
{
    transform_test_time_string(time);
}

/*
 * ��������test_dis
 * ����  �������ڲ��Գ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void test_dis(void)
{
    /* IR�Զ����� */
    if(cur_mode == IR)
    {
        ir_auto_find_gear();
    }
    
    /* ���½�� */
    if(test_flag.gradation == STAGE_TEST)
    {
        if(tes_t && ((g_dis_time + 5) > tes_t))
        {
            updata_result(cur_mode);
        }
    }
    
    /* ��ʾ���� */
    if(test_flag.vol_change_flag == 0)
    {
        /* �ڸտ�ʼ��ʱ��Ҫ������ʾ���ݲ������� */
        if(DIS_SPEED < 20)
        {
            DIS_SPEED++;
        }
        /* ������ʾ���ٶ� */
        else
        {
            if(++DIS_SPEED > 38)
            {
                DIS_SPEED = 28;
            }
            else
            {
                return;
            }
        }
    }
    
	test_flag.re_times_per_second++;
    transform_test_vol_string();
    transform_test_loop_string();
	if(ERR_NUM)
	{
        disable_sample_task();/* �رղ������� */
    }
	
	return;
}

/*
 * ��������offset_dis
 * ����  ��������ƫ�Ʋ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
int8_t offset_dis(const int8_t n)
{
	int8_t err_num = 0;
    
    if(OFFSET_BBD == TEST_BBD)
    {
        bbd_auto_select_gear();
    }
    
    transform_test_vol_string();
    transform_test_loop_string();
	return err_num;
}

void dis_test_ui(void)
{
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
