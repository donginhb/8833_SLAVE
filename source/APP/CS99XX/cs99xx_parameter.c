/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�cs99xx_parameter.c
 * ժ  Ҫ  ���������ã����˵�����ȹ���
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

/*************************������ͷ�ļ� ***********************/

#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include 	"mb_server.h"
#include 	"SysTemConfig.h"
#include    "cs99xx_update.h"

/*************************** ȫ�ֱ��� **************************/

GEAR_STR ac_gear[]=
{
    {0},//0
    {"2~A"   , "2.000~A", "0.000~A", CUR_U_uA, 2000, 1, 2000, 0, 3},//2uA
    {"20~A"  , "20.00~A", "00.00~A", CUR_U_uA, 2000, 1, 2000, 0, 2},//20uA
    {"200~A" , "200.0~A", "000.0~A", CUR_U_uA, 2000, 1, 2000, 0, 1},//200uA
    {"2mA"   , "2.000mA", "0.000mA", CUR_U_mA, 2000, 1, 2000, 0, 3},
    {"10mA"  , "10.00mA", "00.00mA", CUR_U_mA, 1000, 1, 1000, 0, 2},
    {"20mA"  , "20.00mA", "00.00mA", CUR_U_mA, 2000, 1, 2000, 0, 2},
    {"50mA"  , "50.00mA", "00.00mA", CUR_U_mA, 5000, 1, 5000, 0, 2},
    {"100mA" , "100.0mA", "000.0mA", CUR_U_mA, 1000, 1, 1000, 0, 1},
    {"200mA" , "200.0mA", "000.0mA", CUR_U_mA, 2000, 1, 2000, 0, 1},
};

GEAR_STR dc_gear[]=
{
    {0},//0
    {"2~A"   , "2.000~A", "0.000~A", CUR_U_uA, 2000, 1, 2000, 0, 3},//2uA
    {"20~A"  , "20.00~A", "00.00~A", CUR_U_uA, 2000, 1, 2000, 0, 2},//20uA
    {"200~A" , "200.0~A", "000.0~A", CUR_U_uA, 2000, 1, 2000, 0, 1},//200uA
    {"2mA"   , "2.000mA", "0.000mA", CUR_U_mA, 2000, 1, 2000, 0, 3},
    {"10mA"  , "10.00mA", "00.00mA", CUR_U_mA, 1000, 1, 1000, 0, 2},
    {"20mA"  , "20.00mA", "00.00mA", CUR_U_mA, 2000, 1, 2000, 0, 2},
    {"50mA"  , "50.00mA", "00.00mA", CUR_U_mA, 5000, 1, 5000, 0, 2},
    {"100mA" , "100.0mA", "000.0mA", CUR_U_mA, 1000, 1, 1000, 0, 1},
};

GEAR_STR ir_gear[]=
{
    {0},//0
    {"" , "", "", RES_U_MOHM, 0, 0, 0, 0, 3},//IR_10MOHM
    {"" , "", "", RES_U_MOHM, 0, 0, 0, 0, 2},//IR_100MOHM
    {"" , "", "", RES_U_MOHM, 0, 0, 0, 0, 1},//IR_1GOHM
    {"" , "", "", RES_U_GOHM, 0, 0, 0, 0, 3},//IR_10GOHM
    {"" , "", "", RES_U_GOHM, 0, 0, 0, 0, 2},//IR_100GOHM
};

VOL_RANGE_INFO acw_vol_info[]=
{
    {ACW_1kV    , VOL_U_kV, 3},
    {ACW_5kV    , VOL_U_kV, 3},
    {ACW_10kV   , VOL_U_kV, 3},
    {ACW_20kV   , VOL_U_kV, 2},
    {ACW_30kV   , VOL_U_kV, 2},
};

VOL_RANGE_INFO dcw_vol_info[]=
{
    {DCW_6kV    , VOL_U_kV, 3},
    {DCW_10kV   , VOL_U_kV, 3},
    {DCW_20kV   , VOL_U_kV, 2},
};

VOL_RANGE_INFO ir_vol_info[]=
{
    {IR_1kV     , VOL_U_kV, 3},
    {IR_1_5kV   , VOL_U_kV, 3},
    {IR_2_5kV   , VOL_U_kV, 3},
    {IR_5kV     , VOL_U_kV, 3},
    {IR_10kV    , VOL_U_kV, 3},
};



/*************************** �������� *************************/

void init_gr_info(void)
{
    if(0 != type_spe.gr_cur_decs)
    {
        menu_par[GUI_GR_OUTPUT_CUR].decs = type_spe.gr_cur_decs;
    }
}

void init_acw_info(void)
{
	switch(type_spe.acw_vol_range)
	{
		case ACW_10kV:
			menu_par[GUI_ACW_OUTPUT_VOL].decs = 3;
			break;
		case ACW_20kV:
		case ACW_30kV:
			menu_par[GUI_ACW_OUTPUT_VOL].decs = 2;
			break;
	}
}

void init_dcw_info(void)
{
	switch(type_spe.dcw_vol_range)
	{
		case DCW_10kV:
			menu_par[GUI_DCW_OUTPUT_VOL].decs = 3;
			break;
		case DCW_20kV:
			menu_par[GUI_DCW_OUTPUT_VOL].decs = 2;
			break;
	}
}

void init_ir_info(void)
{
	menu_par[GUI_IR_OUTPUT_VOL].decs = 3;
}

void init_com_info(void)
{
    if(MAX_STEPS < 100)
    {
        menu_par[GUI_COM_STEP].lon = 2;
    }
    else
    {
        menu_par[GUI_COM_STEP].lon = 4;
    }
}

void init_test_ui_info(uint8_t whitch)
{
    if(whitch == 0)
    {
        /* ����1�Ľ�����Ϣ ����������ʾ��ѹ������ */
        test_ui_ele_set[TEST_ELE_1].x   = TEST_ELE_1_X;
        test_ui_ele_set[TEST_ELE_1].y   = TEST_ELE_1_Y;
        test_ui_ele_set[TEST_ELE_1].u_x = TEST_ELE_1_U_X(9);
        test_ui_ele_set[TEST_ELE_1].o_y = CH15;
        test_ui_ele_set[TEST_ELE_1].cw  = CW15;
        test_ui_ele_set[TEST_ELE_1].str = output_buf;
        /* ����2����Ϣ ����������ʾ��������� */
        test_ui_ele_set[TEST_ELE_2].x   = TEST_ELE_2_X;
        test_ui_ele_set[TEST_ELE_2].y   = TEST_ELE_2_Y;
        test_ui_ele_set[TEST_ELE_2].u_x = TEST_ELE_2_U_X(9);
        test_ui_ele_set[TEST_ELE_2].o_y = CH15;
        test_ui_ele_set[TEST_ELE_2].cw  = CW15;
        test_ui_ele_set[TEST_ELE_2].str = loop_buf;
        /* ����3����Ϣ����������ʾ��ѹ ��ʵ�������ݵ� */
        test_ui_ele_set[TEST_ELE_3].x   = TEST_ELE_3_X;
        test_ui_ele_set[TEST_ELE_3].y   = TEST_ELE_3_Y;
        test_ui_ele_set[TEST_ELE_3].u_x = TEST_ELE_3_U_X(9);
        test_ui_ele_set[TEST_ELE_3].o_y = CH15;
        test_ui_ele_set[TEST_ELE_3].cw  = CW15;
        test_ui_ele_set[TEST_ELE_3].str = real_buf;
        /* ����4����Ϣ����������ʾ����ʱ��� */
        test_ui_ele_set[TEST_ELE_4].x   = TEST_ELE_4_X;
        test_ui_ele_set[TEST_ELE_4].y   = TEST_ELE_4_Y;
        test_ui_ele_set[TEST_ELE_4].u_x = TEST_ELE_4_U_X(9);
        test_ui_ele_set[TEST_ELE_4].o_y = CH15;
        test_ui_ele_set[TEST_ELE_4].cw  = CW15;
        test_ui_ele_set[TEST_ELE_4].str = time_buf;
        
        /* ����5����Ϣ����������ʾ����״̬�� */
        test_ui_ele_set[TEST_ELE_5].x   = TEST_ELE_5_X;
        test_ui_ele_set[TEST_ELE_5].y   = TEST_ELE_5_Y;
        test_ui_ele_set[TEST_ELE_5].u_x = TEST_ELE_5_U_X(9);
        test_ui_ele_set[TEST_ELE_5].o_y = GUI_GEN_STATUS_CH;
        test_ui_ele_set[TEST_ELE_5].cw  = GUI_GEN_STATUS_CW;
        test_ui_ele_set[TEST_ELE_5].str = NULL;
        
        /* ����6����Ϣ����������ʾ����״̬�� */
        test_ui_ele_set[TEST_ELE_6].x   = TEST_ELE_6_X;
        test_ui_ele_set[TEST_ELE_6].y   = TEST_ELE_6_Y;
        test_ui_ele_set[TEST_ELE_6].u_x = TEST_ELE_6_U_X(9);
        test_ui_ele_set[TEST_ELE_6].o_y = GUI_GEN_STATUS_CH;
        test_ui_ele_set[TEST_ELE_6].cw  = GUI_GEN_STATUS_CW;
        test_ui_ele_set[TEST_ELE_6].str = NULL;
    }
    else if(whitch == 1)
    {
        /* ����1�Ľ�����Ϣ ����������ʾ��ѹ������ */
        test_ui_ele_set[OFFSET_ELE_1].x   = OFFSET_ELE_1_X;
        test_ui_ele_set[OFFSET_ELE_1].y   = OFFSET_ELE_1_Y;
        test_ui_ele_set[OFFSET_ELE_1].u_x = OFFSET_ELE_1_U_X(6);
        test_ui_ele_set[OFFSET_ELE_1].o_y = CH12;
        test_ui_ele_set[OFFSET_ELE_1].cw  = CW12;
        test_ui_ele_set[OFFSET_ELE_1].str = output_buf;
        /* ����2����Ϣ ����������ʾ��������� */
        test_ui_ele_set[OFFSET_ELE_2].x   = OFFSET_ELE_2_X;
        test_ui_ele_set[OFFSET_ELE_2].y   = OFFSET_ELE_2_Y;
        test_ui_ele_set[OFFSET_ELE_2].u_x = OFFSET_ELE_2_U_X(6);
        test_ui_ele_set[OFFSET_ELE_2].o_y = CH12;
        test_ui_ele_set[OFFSET_ELE_2].cw  = CW12;
        test_ui_ele_set[OFFSET_ELE_2].str = loop_buf;
        /* ����3����Ϣ����������ʾ��ѹ ��ʵ�������ݵ� */
        test_ui_ele_set[OFFSET_ELE_3].x   = OFFSET_ELE_3_X;
        test_ui_ele_set[OFFSET_ELE_3].y   = OFFSET_ELE_3_Y;
        test_ui_ele_set[OFFSET_ELE_3].u_x = OFFSET_ELE_3_U_X(6);
        test_ui_ele_set[OFFSET_ELE_3].o_y = CH12;
        test_ui_ele_set[OFFSET_ELE_3].cw  = CW12;
        test_ui_ele_set[OFFSET_ELE_3].str = real_buf;
        /* ����4����Ϣ����������ʾ����ʱ��� */
        test_ui_ele_set[OFFSET_ELE_4].x   = OFFSET_ELE_4_X;
        test_ui_ele_set[OFFSET_ELE_4].y   = OFFSET_ELE_4_Y;
        test_ui_ele_set[OFFSET_ELE_4].u_x = OFFSET_ELE_4_U_X(6);
        test_ui_ele_set[OFFSET_ELE_4].o_y = CH12;
        test_ui_ele_set[OFFSET_ELE_4].cw  = CW12;
        test_ui_ele_set[OFFSET_ELE_4].str = time_buf;
        /* ����5����Ϣ����������ʾ����״̬�� */
        test_ui_ele_set[OFFSET_ELE_5].x   = OFFSET_ELE_5_X;
        test_ui_ele_set[OFFSET_ELE_5].y   = OFFSET_ELE_5_Y;
        test_ui_ele_set[OFFSET_ELE_5].u_x = OFFSET_ELE_5_U_X(6);
        test_ui_ele_set[OFFSET_ELE_5].o_y = CH12;
        test_ui_ele_set[OFFSET_ELE_5].cw  = CW12;
        test_ui_ele_set[OFFSET_ELE_5].str = NULL;
    }
}

void init_par_info(void)
{
    init_com_info();
	init_acw_info();
	init_dcw_info();
	init_ir_info();
	init_gr_info();
	/* ��ͨ�Ű屨�汾������״̬ */
	updata_comm_flag = UPDATE_PLC_SW | UPDATE_COMM_SW |	UPDATE_COMM_BPS;
    init_test_ui_info(0);
}

/*
 * ��������update_affect_par_info
 * ����  �����������ò���ʱ��̬������ݺϷ��ԣ�����������
 * ����  ��mode
 * ���  ����
 * ����  ��uint32_t ���ߵ������Ƿ�Ҫ����һ������ �������Ҫ����
 */
uint32_t update_affect_par_info(uint8_t mode)
{
    uint32_t flag = 0;/* ��������Ƿ��и��� */
    
	switch(mode)
	{
		case ACW:
			menu_par[GUI_ACW_CUR_L].upper		= read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL);
			menu_par[GUI_ACW_REAL_CUR].upper 	= read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL);
			menu_par[GUI_COM_CUR_OFFSET].upper 	= read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL);
			if(read_par_to_buf(&menu_par[GUI_ACW_CUR_L], NULL) > read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL))
			{
                save_par_to_ram(&menu_par[GUI_ACW_CUR_L], read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL));
                flag++;
			}
			
			if(read_par_to_buf(&menu_par[GUI_ACW_REAL_CUR], NULL) > read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL))
			{
                save_par_to_ram(&menu_par[GUI_ACW_REAL_CUR], read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL));
                flag++;
			}
			
			if(read_par_to_buf(&menu_par[GUI_COM_CUR_OFFSET], NULL) > read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL))
			{
                save_par_to_ram(&menu_par[GUI_COM_CUR_OFFSET], read_par_to_buf(&menu_par[GUI_ACW_CUR_H], NULL));
                flag++;
			}
			break;
		case DCW:
			menu_par[GUI_DCW_CUR_L].upper 		= read_par_to_buf(&menu_par[GUI_DCW_CUR_H], NULL);
			menu_par[GUI_DCW_CHARGE_CUR].upper 	= read_par_to_buf(&menu_par[GUI_DCW_CUR_H], NULL);
			menu_par[GUI_COM_CUR_OFFSET].upper 	= read_par_to_buf(&menu_par[GUI_DCW_CUR_H], NULL);
			if(read_par_to_buf(&menu_par[GUI_DCW_CUR_L], NULL) > read_par_to_buf(&menu_par[GUI_DCW_CUR_H], NULL))
			{
                save_par_to_ram(&menu_par[GUI_DCW_CUR_L], read_par_to_buf(&menu_par[GUI_DCW_CUR_H], NULL));
                flag++;
			}
			if(read_par_to_buf(&menu_par[GUI_COM_CUR_OFFSET], NULL) > read_par_to_buf(&menu_par[GUI_DCW_CUR_H], NULL))
			{
                save_par_to_ram(&menu_par[GUI_COM_CUR_OFFSET], read_par_to_buf(&menu_par[GUI_DCW_CUR_H], NULL));
                flag++;
			}
			break;
		case IR:
			//��ѹ����1.5kV ����������СֵΪ5M
			if(read_par_to_buf(&menu_par[GUI_IR_OUTPUT_VOL], NULL) > 1500)
			{
				menu_par[GUI_IR_RES_L].lower = 5;
			}
			//��ѹС��1.5kV ����������СֵΪ IR_RES_L
			else
			{
				menu_par[GUI_IR_RES_L].lower = IR_RES_L;
			}
			
			if(read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL) < menu_par[GUI_IR_RES_L].lower)
			{
				if(read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL) > 0)
				{
					save_par_to_ram(&menu_par[GUI_IR_RES_H], menu_par[GUI_IR_RES_L].lower + 1);
				}
			}
			
			if(read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL) != 0)
			{
				menu_par[GUI_IR_RES_L].upper = read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL);
			}
			else
			{
				menu_par[GUI_IR_RES_L].upper = IR_RES_H;
			}
			
            /* ������޲�Ϊ0����С�����޵���Сֵ����ô��������ֵ����������Сֵ��1 */
			if((read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL) != 0) 
               && (read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL) < menu_par[GUI_IR_RES_L].lower))
            {
                save_par_to_ram(&menu_par[GUI_IR_RES_H], menu_par[GUI_IR_RES_L].lower + 1);
                flag++;
            }
            
            /* �������ֵ��Ϊ0��������ֵ��������ֵ����ô��������ֵ��������ֵ */
			if((read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL) != 0) 
				&& (read_par_to_buf(&menu_par[GUI_IR_RES_L], NULL) > read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL)))
			{
                save_par_to_ram(&menu_par[GUI_IR_RES_L], read_par_to_buf(&menu_par[GUI_IR_RES_H], NULL));
                flag++;
			}
			/* �����������С����Сֵ��ֱ�ӵ�����Сֵ */
			if(read_par_to_buf(&menu_par[GUI_IR_RES_L], NULL) < menu_par[GUI_IR_RES_L].lower)
			{
                save_par_to_ram(&menu_par[GUI_IR_RES_L], menu_par[GUI_IR_RES_L].lower);
                flag++;
			}
			break;
		case GR:
            if(read_par_to_buf(&menu_par[GUI_GR_OUTPUT_CUR], NULL) > menu_par[GUI_GR_OUTPUT_CUR].upper)
            {
                save_par_to_ram(&menu_par[GUI_GR_OUTPUT_CUR], menu_par[GUI_GR_OUTPUT_CUR].upper);
                flag++;
            }
            else if(read_par_to_buf(&menu_par[GUI_GR_OUTPUT_CUR], NULL) < menu_par[GUI_GR_OUTPUT_CUR].lower)
            {
                save_par_to_ram(&menu_par[GUI_GR_OUTPUT_CUR], menu_par[GUI_GR_OUTPUT_CUR].lower);
                flag++;
            }
            /* �������������������������ֵ */
            if(read_par_to_buf(&menu_par[GUI_GR_RES_H], NULL) > GR_RES_H(read_par_to_buf(&menu_par[GUI_GR_OUTPUT_CUR], NULL)))
            {
                save_par_to_ram(&menu_par[GUI_GR_RES_H], GR_RES_H(read_par_to_buf(&menu_par[GUI_GR_OUTPUT_CUR], NULL)));
                flag++;
            }
            
            if(read_par_to_buf(&menu_par[GUI_GR_RES_L], NULL) > read_par_to_buf(&menu_par[GUI_GR_RES_H], NULL))
            {
                save_par_to_ram(&menu_par[GUI_GR_RES_L], read_par_to_buf(&menu_par[GUI_GR_RES_H], NULL));
                flag++;
            }
			
			menu_par[GUI_COM_RES_OFFSET].upper 	= read_par_to_buf(&menu_par[GUI_GR_RES_H], NULL);
            if(read_par_to_buf(&menu_par[GUI_COM_RES_OFFSET], NULL) > read_par_to_buf(&menu_par[GUI_GR_RES_H], NULL))
            {
                save_par_to_ram(&menu_par[GUI_COM_RES_OFFSET], read_par_to_buf(&menu_par[GUI_GR_RES_H], NULL));
                flag++;
            }
			break;
		case BBD:
			break;
		case CC:
            break;
	}
    
    return flag;
}

/*
 * ��������set_subtask
 * ����  �����ò��Բ���������
 * ����  ��w �����˭������set_subtask ���ݲ�ͬ�ĵ��ú�����������
 * ���  ����
 * ����  ����
 */
void set_subtask(const int8_t w)
{
}

/*
 * ��������menu_subtask
 * ����  ��ϵͳ�������õ������� ��main_task()����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void menu_subtask(void)
{
}

/*
 * ��������test_subtask
 * ����  ���������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void test_subtask(void)
{
    serve_test();
}

void set_custom_par_info(void)
{
}
#include "stdio.h"
#include "module.h"
#include "dc_module.h"
void dis_soft_version(void)
{
}

/*
 * ��������help_subtask
 * ����  �������˵������� ��ν��������ʵ������һ������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void help_subtask(void)
{
}



/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE*******************/
