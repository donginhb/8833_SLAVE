/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_par_com.c
 * ժ  Ҫ  �����ø���ģʽ���еı���
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 

#include	<string.h>
#include    <stdio.h>
#include 	<math.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include 	"MC14094.h"
#include    "cs99xx_result.h"

void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit);
// void init_offset_par(UN_STRUCT *p);
// static DATA_INFO*  server_combination_key(const uint16_t keyvalue);


/*
 * ��������set_contrast
 * ����  ������Һ�����ĶԱȶ�
 * ����  ��const int8_t gear �Աȶȵĵ�λѡ��
 * ���  ����
 */
void set_contrast(const int8_t gear)
{
	uint8_t data = 0xbb;
	MC14094_CMD_RA(data+4*gear);
	MC14094_Updata_RA();
}
/*
 * ��������define_modes
 * ����  ��ȷ��ģʽ ��ģʽ����Ŀ
 * ����  ����
 * ���  ��mode_buf ��ģʽ�� flag �Ŷ�Ӧmode_buf��ģʽ���� kinds ����ģʽ����Ŀ
 * ����  ����
 */
uint16_t define_modes(const uint8_t **mode_buf, uint8_t *flag, int16_t *kinds)
{
	int32_t k = 0;
	
	if(MODEL_EN & __ACW)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][ACW];
		flag[k] = ACW;
	}
	if(MODEL_EN & __DCW)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][DCW];
		flag[k] = DCW;
	}
	if(MODEL_EN & __IR)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][IR];
		flag[k] = IR;
	}
	if(MODEL_EN & __GR)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][GR];
		flag[k] = GR;
	}
	if(MODEL_EN & __CC)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][CC];
		flag[k] = CC;
	}
	if(MODEL_EN & __BBD)
	{
		mode_buf[++k] = mode_pool[GR_VOL_DROP_EN][BBD];
		flag[k] = BBD;
	}
    
	if(kinds != NULL)
	{
		*kinds = k;
	}
	
	mode_buf[0]=mode_buf[1];
	flag[0] = flag[1];
    
	return k;
}
uint16_t define_g_modes(const uint8_t **mode_buf, uint8_t *flag, int16_t *kinds)
{
	int32_t k = 0;
	if(MODEL_EN & __ACW)
	{
		mode_buf[++k]="ACW";
		flag[k] = ACW;
	}
	if(MODEL_EN & __DCW)
	{
		mode_buf[++k]="DCW";
		flag[k] = DCW;
	}
	if(kinds != NULL)
	{
		*kinds = k;
	}
	mode_buf[0]=mode_buf[1];
	flag[0] = flag[1];
	return k;
}
void init_mode(NODE_STEP *p)
{
	switch(p->one_step.com.mode)
	{
		case ACW:
			init_acw_step(p);
			break;
		case DCW:
			init_dcw_step(p);
			break;
		case IR:
			init_ir_step(p);
			break;
		case GR:
			init_gr_step(p);
			break;
		case BBD:
			init_bbd_step(p);
			break;
        case CC:
            init_cc_step(p);
            break;
	}
	
	init_par_info_list(&cur_node);
	init_offset_par(p);/* ģʽ���޸����Ժ�Ҫ���ƫ�Ʋ�����Ϣ */
}

void updata_title_step(UN_STRUCT *p)
{
}

void updata_step(UN_STRUCT *p)
{
}

void serve_numkey(const uint16_t keyvalue)
{
	uint8_t flag = 0;

	switch(keyvalue)
	{
		case KEY_0:
			change_data(0);
			break;
		case KEY_1:
			change_data(1);
			break;
		case KEY_2:
			change_data(2);
			break;
		case KEY_3:
			change_data(3);
			break;
		case KEY_4:
			change_data(4);
			break;
		case KEY_5:
			change_data(5);
			break;
		case KEY_6:
			change_data(6);
			break;
		case KEY_7:
			change_data(7);
			break;
		case KEY_8:
			change_data(8);
			break;
		case KEY_9:
			change_data(9);
			break;
		default:
			flag = 1;
			break;
	}

	if(flag == 0)
	{
		update_data(pmenu_data);
		shift_cursor(pmenu_data);
	}
}

DATA_INFO* find_menu(uint8_t n)
{
	uint32_t i = 0;
	DATA_INFO* p = par_info_list.head;
	
	if(n > par_info_list.tail->page)
	{
		return 0;
	}
	
	for(i=1; i < 20; i++)
	{
		if(p->page == n)
		{
			return p;
		}
		p = p->next;
	}
    
	return NULL;
}
DATA_INFO*  server_combination_key(const uint16_t keyvalue)
{
	DATA_INFO* temp = NULL;
	switch(keyvalue)
	{
		case KEY_0&KEY_1:
			app_flag.goto_menu_page = 1;
			break;
		case KEY_0&KEY_2:
			app_flag.goto_menu_page = 2;
			break;
		case KEY_0&KEY_3:
			app_flag.goto_menu_page = 3;
			break;
		case KEY_0&KEY_4:
			app_flag.goto_menu_page = 4;
			break;
		case KEY_0&KEY_5:
			app_flag.goto_menu_page = 5;
			break;
		case KEY_0&KEY_6:
			app_flag.goto_menu_page = 6;
			break;
		default:
			app_flag.goto_menu_page = 0;
			g_skip_page = 0;
			break;
	}
    
	if(app_flag.goto_menu_page)
	{
		g_skip_page = 1;
	}
    
	return temp;
}




/*
 * ��������set_step
 * ����  ����ʾһ���˵����ֵ
 * ����  ��p_node ָ���м�����ṹ�����û���Ҫ��������ʱ�Ż���µ����Բ��Ĳ�����
 * ���  ����
 * ����  ����
 */
void set_step(void *p_node)
{
}



void set_mode(void *p_node)
{
}


void init_offset_par(NODE_STEP *p)
{
	
    if(p == NULL)
    {
        return;
    }
    
	switch(p->one_step.com.mode)
	{
		case ACW:
			p->one_step.acw.offset_cur = 0;/* ƫ�Ƶ��� */
			p->one_step.acw.offset_real = 0;/* ƫ����ʵ���� */
			p->one_step.acw.offset_result = 0;/* ƫ�Ƶ���������� 0ʧ�� 1�ϸ� */

			break;
		case DCW:
			p->one_step.dcw.offset_cur = 0;/* ƫ�Ƶ��� */
			p->one_step.dcw.offset_result = 0;/* ƫ�Ƶ���������� 0ʧ�� 1�ϸ� */
			break;
		case GR:
			p->one_step.gr.offset_res = 0;/* ƫ�Ƶ��� */
			p->one_step.gr.offset_result = 0;/* ƫ�Ƶ���������� 0ʧ�� 1�ϸ� */
			break;
		default:
			break;
	}
}

void set_voltage(void *p_node)
{
}

void set_all_ports(void *p_node)
{
}

void set_test_time(void *p_node)
{
}

void set_either_or_par(void *p_node)
{
}

void set_float_par(void *p_node)
{
}

void set_float_par_for_syn(void *p_node)
{
}


void set_frequency(void *p_node)
{
}

void set_option_par(void *p_node)
{
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
