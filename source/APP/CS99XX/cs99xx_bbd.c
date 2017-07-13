/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_bbd.c
 * ժ  Ҫ  ��bbd����
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include	"calibration.h"
/*
 * ��������bbd_dis_step
 * ����  ������һ����������Ϣ
 * ����  ��p ���뵱ǰ����ָ��
 * ���  ����
 * ����  ����
 */
//static void bbd_dis_step(const uint8_t n)
//{
//}


/*
 * ��������offset_manual
 * ����  ����ƫ�Ʋ��Խ���ϸ��˵��
 * ����  ����
 * ���  ����
 * ����  ����
 */
//static void bbd_manual(void)
//{
//}
/*
 * ��������serve_offset_shortcut
 * ����  ��������ƫ�Ʋ��Եĸ���������
 * ����  ����
 * ���  ����
 * ����  ����
 */
//static void serve_bbd_shortcut(void)
//{
//}
/*
 * ��������dis_one_step
 * ����  ������һ����ƫ�Ʋ�����Ϣ
 * ����  ��p ���뵱ǰ����ָ�� �� n λ����Ϣ
 * ���  ����
 * ����  ����
 */
//static void dis_one_bbd_step(const UN_STRUCT *p, const int8_t n)
//{
//}

/*
 * ��������draw_one_page
 * ����  ������һҳ��ƫ�Ʋ�����Ϣ
 * ����  ��p ���뵱ǰҳ�ĵ�һ������ָ��
 * ���  ����
 * ����  ����
 */
//static void draw_bbd_one_page(NODE_STEP *p)
//{
//}
void clear_bbd_ok_flag(void)
{
	NODE_STEP *p_bak = g_cur_step;
	
	for(;g_cur_step;g_cur_step = g_cur_step->next)
	{
		if(g_cur_step->one_step.com.mode == BBD)
		{
			g_cur_step->one_step.bbd.get_cs_ok = 0;
		}
	}
    
	g_cur_step = p_bak;
}

void update_cap_limit(void)
{
	if(cap_ave * 5 < 10)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 1000 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 1000 * 0.8;
	}
	else if(cap_ave * 5 < 100)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 100 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 100 * 0.8;
	}
	else if(cap_ave * 5 < 1000)
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 10 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 10 * 0.8;
	}
	else
	{
		g_cur_step->one_step.bbd.upper_limit = cap_ave * 1 * 1.2;
		g_cur_step->one_step.bbd.lower_limit = cap_ave * 1 * 0.8;
	}
}
/*
 * ��������sys_bbd
 * ����  ����ȡ��·��·������ֵ
 * ����  ��const int8_t w ��ǵ��ú���
 * ���  ����
 * ����  ����
 */
void sys_bbd(uint32_t w)
{
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
