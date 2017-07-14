/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�general_test.h
 * ժ  Ҫ  ��������
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

#include    "cs99xx_vref.h"
#include 	<math.h>
#include	<string.h>
#include	<stdio.h>
#include	"keyboard.h"
#include 	"MC14094.h"
#include 	"app.h"
#include    "cs99xx_result.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_relay_motion.h"
#include	"cs99xx_type.h"
#include	"cs99xx_it.h"
#include 	"mb_server.h"
#include 	"SysTemConfig.h"
#include    "cs99xx_plc.h"
#include    "cs99xx_led.h"
#include    "cs99xx_collect.h"



void update_info(void)
{
    clear_();
	load_data();
	gui_draw_test();
	dis_test_ui();	/* ������ǰ���Ĳ������� */
}

void dis_gear_up_limit(float f, uint16_t x, uint16_t y, uint8_t dec)
{
}

/* ����������� */
uint8_t judge_ir_rang(void)
{
    uint8_t res_buf[10] = {0};
    
    if(res_ave > cur_ir_rang_h)
    {
        if(cur_ir_rang_h < 100 * 1000)
        {
            mysprintf(res_buf, NULL, 41, cur_ir_rang_h / 1000 * 10);
        }
        else
        {
            mysprintf(res_buf, NULL, 40, cur_ir_rang_h / 1000);
        }
        
        sprintf((char*)loop_buf, ">%s", res_buf);
        return 1;
    }
    
    return 0;
}

void tr_ir_gear_up_limit_to_string(void)
{
    uint8_t res_buf[20] = {0};
    
    if(cur_gear == cur_ir_dis_gear_1)
    {
        switch(cur_ir_dis_gear_1)
        {
            case IR_10MOHM:
                mysprintf(res_buf, NULL, 41, gear_up_limit * 10);
                break;
            case IR_100MOHM:// > 10.0M
                mysprintf(res_buf, NULL, 41, gear_up_limit * 10);
                break;
            case IR_1GOHM:// > 100M
                mysprintf(res_buf, NULL, 40, gear_up_limit);//>100M
                break;
            case IR_10GOHM:
                if(cur_gear == IR_1GOHM)
                {
                    mysprintf(res_buf, NULL, 40, gear_up_limit);//>1000M
                }
                else
                {
                    mysprintf(res_buf, NULL, 42, gear_up_limit * 100/1000.0);//>1.00G
                }
                break;
            case IR_100GOHM:
                mysprintf(res_buf, NULL, 41, gear_up_limit/1000 * 10);//>10.0G
                break;
            default:
                mysprintf((uint8_t*)loop_buf, NULL, 40, gear_up_limit/1000);//>100G
                break;
        }
        
        sprintf((char*)loop_buf, ">%s", res_buf);
        res_ave_comm = 9999;
    }
    else
    {
        switch(cur_gear)
        {
            case IR_10MOHM:
                mysprintf(res_buf, NULL, 41, gear_up_limit * 10);
                break;
            case IR_100MOHM:
                mysprintf(res_buf, NULL, 40, gear_up_limit);//>100M
                break;
            case IR_1GOHM:
                mysprintf(res_buf, NULL, 40, gear_up_limit);//>1000M
                break;
            case IR_10GOHM:
                mysprintf(res_buf, NULL, 41, gear_up_limit/1000 * 10);//>10.0G
                break;
            case IR_100GOHM:
                mysprintf(res_buf, NULL, 40, gear_up_limit/1000);//>100G
                break;
            default:
                mysprintf(res_buf, NULL, 40, gear_up_limit/1000);//>100G
                break;
        }
        
        sprintf((char*)loop_buf, ">%s", res_buf);
        res_ave_comm = 9999;
    }
}

void transform_test_ir_res_loop(void)
{
    uint8_t flag = 0;
    uint8_t res_buf[10] = {0};
    
	if(app_flag.absorptance)
	{
        mysprintf(real_buf, NULL, 51, ir_absorptance * 10);
	}
	else
	{
		memset(real_buf, 0, sizeof(real_buf));
	}

    if(test_flag.gradation == STAGE_READY || TERMINATE)
    {
        if(cur_gear < 4)
        {
            mysprintf(loop_buf, NULL, ir_gear[cur_gear].decs + 50, res_ave * pow(10, ir_gear[cur_gear].decs));
        }
        else if(cur_gear == 4)
        {
            if(res_ave < 10 * 1000)
            {
                mysprintf(loop_buf, NULL, ir_gear[cur_gear].decs + 50, res_ave);
            }
            else
            {
                mysprintf(loop_buf, NULL, ir_gear[cur_gear].decs + 50 - 1, res_ave / 10);
            }
        }
        else
        {
            mysprintf(loop_buf, NULL, ir_gear[cur_gear].decs + 50, res_ave / 10);
        }
		return;
    }
    
	/* ��ѹ�����׶β���ʾ����ֵ */
	if(test_flag.gradation == STAGE_RISE)
	{
        strcpy((char*)loop_buf, res_infinite_str[cur_gear]);
		return;
	}
    
    if(ERR_NUM == ERR_SHORT)
    {
        mysprintf(res_buf, NULL, 41, type_spe.ir_res_l * 10);
        sprintf((char*)loop_buf, "<%s", res_buf);
        return;
    }
    
	/*
	��· 
		�����ǣ�1 ������С
				2 ��ѹ�������Ĳ�����Χ��+-10v
	*/
    if(cur_ave <= 1.0 && (((vol_ave + 10) > cur_vol) || (vol_ave < (cur_vol + 10))))
	{
        if(cur_auto == AUTO_SHIFT_OFF)
        {
            tr_ir_gear_up_limit_to_string();
        }
        else
        {
            if(cur_gear >= IR_10GOHM)
            {
                /* ����������� */
                judge_ir_rang();
            }
            else
            {
                sprintf((char*)loop_buf, "%s", res_infinite_str[cur_gear]);
            }
        }
	}
	else
	{
        if(cur_auto == AUTO_SHIFT_OFF)
        {
            if(res_ave > gear_up_limit)
            {
                tr_ir_gear_up_limit_to_string();
                flag = 1;
            }
        }
        
        if(flag == 0)
		switch(cur_gear)
		{
			case IR_10MOHM:/* 1-10M */
			{
                if(res_ave < 10)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 53, res_ave * 1000);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 52, res_ave * 100);
                }
				res_ave_comm = res_ave * 1000;
				break;
			}
			case IR_100MOHM:/* 10-100M */
			{
                if(res_ave < 100)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 152, res_ave * 100);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 151, res_ave * 10);
                }
                
                res_ave_comm = res_ave * 100;
				break;
			}
			case IR_1GOHM:/* 100-1000M */
			{
                if(res_ave < 1000)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 51, res_ave * 10);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 50, res_ave);
                }
                
                res_ave_comm = res_ave * 10;
				break;
			}
			case IR_10GOHM:/* 1-10G */
			{
                /* ����������� */
                if(judge_ir_rang())
                {
                    break;
                }
                
                if(res_ave < 10 * 1000)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, ir_gear[cur_gear].decs + 50, res_ave);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 52, res_ave / 10);
                }
                
                res_ave_comm = res_ave / 1;
				break;
			}
			case IR_100GOHM:
			{
                /* ����������� */
                if(judge_ir_rang())
                {
                    break;
                }
                
                if(res_ave < 1000 * 1000)
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 52, res_ave / 10);
                }
                else
                {
                    mysprintf((uint8_t*)loop_buf, NULL, 52, res_ave / 10);
                }
                
                res_ave_comm = res_ave / 100;
				break;
			}/* end case */
		}//end switch(cur_gear)
	}//end else
}




void dis_gr_vol(float gr_vol)
{
}
void dis_test_gr_cur(void)
{
}

void dis_test_gr_vol(void)
{
}
void transform_test_gr_vol(void)
{
	/* �������ǰ0.2s����ʾ���� */
    if(test_flag.gradation == STAGE_TEST)
    {
        if(g_dis_time < 2 && !GR_OVER)
        {
            if(GR_VOL_DROP_EN && type_spe.gr_dis_opt & GR_DIS_RES)
            {
                strcpy((char*)loop_buf, "---.-");
            }
            return;
        }
    }
    
	if(GR_VOL_DROP_EN)
	{
		/* ��ʾ��ѹ */
		if(type_spe.gr_dis_opt & GR_DIS_VOL)
		{
			/* ѹ��ģʽ�򿪺���ʾ��ѹ */
			if(GR_VOL_DROP_EN)
			{
				switch(cur_gear)
				{
					case DC_GR_20mV:
					{
                        mysprintf(loop_buf, NULL, 152, vol_ave * 100);
						break;
					}
					case DC_GR_200mV:
					{
                        mysprintf(loop_buf, NULL, 151, vol_ave * 100);
						break;
					}
					case DC_GR_2000mV:
					{
                        mysprintf(loop_buf, NULL, 150, vol_ave * 100);
						break;
					}
					case DC_GR_12V:
					case DC_GR_12V_100mA:
					case DC_GR_12V_1000mA:
					{
                        mysprintf(loop_buf, NULL, 150, vol_ave / 1000 * 100);
						break;
					}
				}
			}
		}
		/* ��ʾ���� */
		else if(type_spe.gr_dis_opt & GR_DIS_RES)
		{
            mysprintf(loop_buf, NULL, 151, res_ave);
		}
	}
	else
	{
        if(ERR_NUM == ERR_OPEN)
        {
            strcpy((char*)loop_buf, "> 510");
        }
        else
        {
            mysprintf((uint8_t*)loop_buf, NULL, 151/*"%.1f"*/, (uint32_t)res_ave);
        }
	}
}

void dis_pass(void)
{
//    uint8_t flag = 0;
    
    relay_motion();/* ��λ�ŵ� */
    
	if(steps_pass)
	{
        led_cmd(LED_ST_PASS);
    }
	
	if(g_comm_status == LOCAL_COMM)
	{
		on_keyboard();/* �򿪼��� */
	}
	
    /* ����ݲ��Էŵ���ɺ��ѹ��������ʾ0 */
    if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
    {
        cur_ave = 0;
        vol_ave = 0;
        transform_test_vol_string();
        transform_test_loop_string();
    }
    
	if(!OFFSET_BBD)
	{
		test_dis();/* �����ɼ���������ʾ���� */
	}
    
}
void dis_fail(void)
{
    relay_motion();/* ��λ�ŵ� */
    
    LED_ALL = LED_OFF;
	LED_FAIL = LED_ON;
	STOP_COMM = 0;
	cur_status = ST_ERR_FAIL;
	
    plc_signal_cmd(PLC_SIG_FAIL);
    
	/* ���������� */
    if(BUZZER_EN>0)
    {
        BUZZER = BUZZER_ON;
    }
    
    /* ����ݲ��Էŵ���ɺ��ѹ��������ʾ0 */
    if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
    {
        cur_ave = 0;
        vol_ave = 0;
        transform_test_vol_string();
        transform_test_loop_string();
    }
    
    test_dis();
    
    
	while(STOP_PIN && !STOP_COMM)
    {
        if(TERMINATE)
        {
            break;
        }
    }
	
	if(g_comm_status == LOCAL_COMM)
	{
		on_keyboard();/* �򿪼��� */
	}
	
    load_steps_to_list(1);//���ص�һ�� 2017.5.20 
	g_cur_step = list_99xx.head;
	load_data();
	dis_test_ui(); /* ������ǰ���Ĳ������� */
	
	FAIL = 0;
	LED_FAIL = LED_OFF;
	
    if(BUZZER_EN>0)
    {
        BUZZER = BUZZER_OFF;
    }
	
	g_cur_step = list_99xx.head;
}
void dis_discharge_over(void)
{
}

void dis_stop(void)
{
	serve_count(KEY_STOP_COUNT);
	STOP = 0;
	PASS = 0;
	TEST = 0;
	LED_TEST = 0;
	BUZZER = BUZZER_OFF;
	
	g_return = 0;
	
	stop_test();
    plc_signal_cmd(PLC_SIG_RESET);/* plc��λ */
    test_reset();/* ������Ծ���״̬ */
    
    if(sys_par.fail_mode != FAIL_MODE_HALT || cur_step == g_cur_file->total)
    {
        load_steps_to_list(1);
        g_cur_step = list_99xx.head;
        load_data();
    }
    
	test_flag.gradation = STAGE_READY;
	
	if(g_comm_status == LOCAL_COMM)
	{
		on_keyboard();/* �򿪼��� */
	}
	
    /* ����ݲ��Էŵ���ɺ��ѹ��������ʾ0 */
    if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
    {
        cur_ave = 0;
        vol_ave = 0;
        transform_test_vol_string();
        transform_test_loop_string();
    }
}

void stop_server_for_comm(void)
{
	g_dis_time = tes_t;
	vol_ave = cur_vol;
	cur_ave = cur_high;
	
	if(cur_mode == GR)
	{
		cur_ave_comm = cur_vol;
		res_ave_comm = cur_high;
	}
    else if(cur_mode == IR)
	{
		res_ave_comm = cur_low;
		
		if(res_ave_comm < 10)
		{
			res_ave_comm *= 1000;
		}
		else if(res_ave_comm < 100)
		{
			res_ave_comm *= 100;
		}
		else if(res_ave_comm < 1000)
		{
			res_ave_comm *= 10;
		}
		else if(res_ave_comm < 10*1000)
		{
			res_ave_comm *= 1;
		}
		else if(res_ave_comm < 100*1000)
		{
			res_ave_comm /= 10;
		}
		else
		{
			res_ave_comm /= 100;
		}
	}
	
	cur_status = ST_WAIT;
}

int32_t dis_charge_remain_vol(void)
{
    uint32_t temp_delay = 0;
    int32_t res = 0;
    
    g_test_time = 0;
    test_flag.dis_status = TEST_DISCHARGE_INDEX;/* ��ǰ����ʾ״̬���� */
    
    test_flag.vol_change_flag = 1;/* ������ʾ������ */
    set_sample_rate(20);
    
    on_sample_task();
    
    if(vol_ave < 50)
    {
        return 0;
    }
    
    CUR_FAIL = 0;
    TERMINATE = 0;
    cur_mode = DCW;
    off_schedule_task();
    open_test_timer();/* ����ʱ�� */
    resume_sample_task();
    
    while(1)
    {
        /* ������ʾֵ */
        if(test_flag.sample_task_en)
        {
            count_dis_value();
        }
        
        g_dis_time = g_test_time;
        
        test_status_dis();/* ��ʾ����״̬ */
        
        test_dis();/* ����ϸ�� */
        
        if(STOP_PIN == RESET)
        {
            res = -1;
        }
        
        OSTimeDlyHMSM(0,0,0,10);
        
        dcw_big_cap_execute_dis_charge();
        
        updata_time(U_TEST_TIME, g_dis_time);/* ����ʱ�� */
        
        if(test_flag.dis_charge_end == 1)
        {
            if(++temp_delay > 5)
            {
                disable_sample_task();
                close_test_timer();
                dis_discharge_over();
                return res;
            }
        }
    }
}


void cur_cylinder_ctrl_over(void)
{
    CYLINDER_CTRL(2000);
}

void clear_test_falg(void)
{
    memset((void*)&test_flag, 0, sizeof test_flag);/* ��ձ�־λ */
}

static void g_mode_next_step(void)
{
    test_flag.judge_err_en = 0;
    
    close_test_timer();/* �ض�ʱ�� */
    save_cur_result(&cur_result);
    test_pass();
    
    count_vol_step_value();/* �����ѹ����ֵ */
    load_steps_to_list(g_cur_step->one_step.com.step);
    g_cur_step = list_99xx.head->next;/* ������һ�� */
    
    /* ������ǰ���Ĳ������� */
    load_data();
    dis_test_ui(); /* ������ǰ���Ĳ������� */
    
    shift_gear(cur_mode);/* �л���λ�̵��� */
    OSTimeDlyHMSM(0,0,0,50);
    exit_sw();/* ���ⲿ�ж� */
    test_vref(cur_high);/* �������׼ */
    save_result(&cur_result, INIT_RESULT);/* ��ʼ�����沽 */
    
    test_flag.save_only_one = 0;/* �������һ�α�־ */
    g_test_time = zeo_t;/* ������1�׶� */
    test_flag.gradation = STAGE_CHANGE;/* �����ѹ����׶� �������Ҫ */
    test_flag.dis_status = TEST_TEST_INDEX;
    DIS_SPEED = 0;/* ��ʾ�ٶȿ��Ʊ�־���� */
    LED_PASS = LED_OFF;/* �ر�LED�� */
    g_dis_time = 0;
    test_flag.judge_err_en = 1;
    test_flag.pass_only_one = 0;
    test_flag.vol_rise_step_t = 0;
    open_test_timer();/* ����ʱ�� */
}

void re_draw_test_ui(void)
{
    init_test_ui_info(0);
    
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        if(cur_mode == DCW)
        {
            dcw_big_cap_output_Impedan_sel(cur_output_impedan);
        }
    }
    
	clear_menu();
	update_info();
	
	cur_status = ST_WAIT;
	
    if(cur_mode == DCW)
    if(type_spe.dcw_big_cap_en == ENABLE)
    {
        set_sample_rate(50);/* ���ò����� */
        on_sample_task();
        off_schedule_task();
    }
}

/*
 * ��������transform_test_time_string
 * ����  ��������ʱ��ת��Ϊ�ַ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
// void transform_test_time_string(uint16_t time)
// {
//     mysprintf((uint8_t*)time_buf, NULL, 1 + 150, time);
// }
/*
 * ��������into_output_delay
 * ����  �������ʱ
 * ����  ����
 * ���  ����
 * ����  ����
 */
static int32_t into_output_delay(void)
{
    return 0;
}
enum{
    TEST_NULL,/* ���� */
    TEST_RE_DRAW,/* �ػ���� */
    TEST_PREV_STEP,
    TEST_NEXT_STEP,
    TEST_SHORTCUT,/* ��ݼ� */
    TEST_RESET_UI,
    TEST_STOP_UI,
    TEST_INTO_GETC,/* �����ȡ��ֵ���棬���Զ����в��� */
    TEST_INTO_GETO,/* �����ȡƫ�ƽ��棬���Զ����в��� */
    
    TEST_NULL_CONTROL,/* ���� */
    TEST_NEXT_CONTROL,
    TEST_PASS_CONTROL,
    TEST_STOP_CONTROL,
    TEST_CON_CONTROL,
    TEST_START_CONTROL,/* �������������Կ�ʼ */
    TEST_TESTING_CONTROL,/* ���ڲ��� */
    TEST_EXCEPTION_CONTROL,
    TEST_QUIT_CONTROL,
    TEST_FAIL_RE_START_CONTROL,
    TEST_CONT_TEST_CONTROL,
    
};

enum{
EXIT_SHORT,
EXIT_ARC,
EXIT_STOP,
EXIT_GFI,
EXIT_RTC,
};
/*
 * ��������app_ctrl_exit_sw
 * ����  ��Ӧ�ò� ���ڿ����ⲿ�жϿ���
 * ����  ��witch ָ�����ⲿ�ж�
 *         st  �ж�״̬ ENABLE DISABLE
 * ���  ����
 * ����  ����
 */
void app_ctrl_exit_sw(uint8_t witch, uint8_t st)
{
    switch(witch)
    {
        case EXIT_SHORT:
            SHORT_INT(st);
            break;
        case EXIT_ARC:
            ARC_INT(st);
            break;
        case EXIT_STOP:
            STOP_INT(st);
            break;
        case EXIT_GFI:
            GFI_INT(st);
            break;
        case EXIT_RTC:
            RTC_INI(st);
            break;
    }
}

void uninstall_test_irq_fun(void)
{
	test_irq_fun = NULL;
	cs99xx_test_fun = NULL;
}
/*
 * ��������testing_process_control
 * ����  �����Թ��̿���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void testing_process_control(uint8_t *st)
{
    uint8_t state = 0;
    
    if(st == NULL)
    {
        return;
    }
    
    state = *st;
    *st = TEST_NULL_CONTROL;
    
    /* �˳�״̬�� */
    if(STOP)
    {
        *st = TEST_STOP_CONTROL;//TEST_QUIT_CONTROL;
    }
    
    switch(state)
    {
        case TEST_START_CONTROL:/* �������� */
        {
			cur_status = ST_TESTING;
            serve_count(KEY_START_COUNT);/* ���ż��� */
            
            /* �����һ�������λ�ͼ����µĲ��Բ� */
            if(NEXT)
            {
                /* �����ǰ���������һ�� �ͼ�����һ�� */
                if(g_cur_step->next != NULL)
                {
                    load_steps_to_list(g_cur_step->next->one_step.com.step);
                    g_cur_step = list_99xx.head;
                }
                /* ����ͼ��ص�һ�� */
                else
                {
                    load_steps_to_list(1);
                    g_cur_step = list_99xx.head;
                }
            }
            
            /* ����Gģʽʱÿ�β���ֻ�ܴӵ�һ����ʼ */
            if(g_cur_file->work_mode == G_MODE)
            {
                load_steps_to_list(1);
                g_cur_step = list_99xx.head;
            }
            
            /* ������ǰ���Ĳ������� */
            load_data();
            dis_test_ui(); /* ������ǰ���Ĳ������� */
            
            /* ����Ǵ���ݲ�����Ҫ�Ƚ��зŵ� */
            if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW)
            {
                /* ����ŵ�����а����˸�λ����Ҫֹͣ���� */
                if(-1 == dis_charge_remain_vol())
                {
                   *st = TEST_QUIT_CONTROL;
                    return;
                }
            }
            
            LCD_REFRESH();
            
			uninstall_test_irq_fun();/* ж�ز���״̬�� */
            /* ��ǰ���ǵ�һ�����������ʱʱ�䲻Ϊ�� */
            if(cur_step == 1 && sys_par.output_delay > 0)
            {
                test_flag.dis_status = TEST_OUTPUT_DELAY;
                g_test_time = 0;
                open_test_timer();/* ����ʱ�� */
                cur_status = ST_OUTPUT_DELAY;
				plc_signal_cmd(PLC_SIG_TESTING);
            
                if(-1 == into_output_delay())/* ���������ʱ */
				{
					*st = TEST_QUIT_CONTROL;
                    cur_status = ST_WAIT;
                    close_test_timer();/* �ض�ʱ�� */
                    plc_signal_cmd(PLC_SIG_RESET);
					return;
				}
                close_test_timer();/* �ض�ʱ�� */
            }
            
            install_test_irq_fun();/* ��װ����״̬�� */
            
            /* ����������Ϊ�վ��˳� */
            if(cs99xx_test_fun == NULL)
            {
                return;
            }
            
            startup();/* �������� */
            *st = TEST_TESTING_CONTROL;
            break;
        }
        case TEST_TESTING_CONTROL:/* ���Կ��� */
        {
            /* ������ʾֵ */
            if(test_flag.sample_task_en && ERR_NUM == ERR_NONE)
            {
                count_dis_value();
            }
            
            /* ÿ��ʧ��ֻ����һ�� && !CUR_FAIL */
            if(ERR_NUM && !CUR_FAIL)
            {
                test_flag.vol_change_flag = 1;/* ȡ������test_dis()�ܳɹ�ˢ������ */
                
                /* ѹ�������� */
                if(GR_VOL_DROP_EN)
                {
                    if(g_dis_time >= 4)
                    {
                        exception_handling(ERR_NUM);
                    }
                }
                else
                {
                    exception_handling(ERR_NUM);
                }
            }
            else
            {
                cs99xx_test_fun();/* ������Գ��� */
            }
            
            /* ����ʱ�䵽 */ // read_stop_pin()
            if(test_flag.gradation == STAGE_TEST)
            if(tes_t && g_dis_time == tes_t && test_flag.err_once == 0)
            {
                test_flag.err_once = 1;/* ȷ��ʱ�䵽����ִֻ��һ�� */
                /* ����ʱ�䵽�˺��ٴμ��������� */
                ERR_NUM = judge_err(); 
                /* ����ʱ�䵽����쳣 ��־ */
                if(ERR_NUM_OVER != ERR_NONE)
                {
                    exception_handling(ERR_NUM_OVER);
                }
                else if(ERR_NUM != ERR_NONE)
                {
                    exception_handling(ERR_NUM);
                }
                
                open_test_timer();//��ʱ���ڲ���ʱ�䵽ʱ��irq����ر�����Ҫ�ٴδ򿪼�������Ĳ���
            }
            
            /* ���Խ����� ֪ͨͨ�ſ� �ź�Ҫ����һ��ʱ�䱣֤���ӿڰ��ȡ�� */
            if(OVER)
            {
                cur_cylinder_ctrl_over();
            }
            /* �ڼ���ʧ�ܲ�  && ���в�������� && �ۺ��жϴ� && ʧ�ܼ�������
               ��ʾ����ʧ��
            */
            if( FAIL && OVER && sys_par.plc_signal != EACH_STEP
                && (sys_par.fail_mode == FAIL_MODE_CON 
                || sys_par.fail_mode == FAIL_MODE_HALT
                || sys_par.fail_mode == FAIL_MODE_CON
                || sys_par.fail_mode == FAIL_MODE_RESTART
                || sys_par.fail_mode == FAIL_MODE_NEXT
                || sys_par.fail_mode == FAIL_MODE_FPDFC
                    ))
            {
                *st = TEST_EXCEPTION_CONTROL;
            }
            /* (��ǰ������ ���� ����Ҫ��������) ���� (���в�������� ���� ������������������) 
               ���������������Ҫֹͣ���� 
            */
            else if((CUR_OVER && !CONT) || (OVER && !(CONT && int_t)))
            {
                /* ��ǰ��PASS ���� δ�����쳣 */
                if(!CUR_FAIL && !ERR_NUM_OVER)
                {
                    /* δ���¸�λ�� */
                    if(!TERMINATE)
                    {
                        PASS = 1;/* �����ٽ��в���ʱ�ͽ�pass��־д1 */
                        *st = TEST_PASS_CONTROL;
                    }
                    /* �����˸�λ�� */
                    else
                    {
                        STOP = 1;
                        *st = TEST_STOP_CONTROL;
                    }
                }
                /* �����쳣 */
                else
                {
                    *st = TEST_EXCEPTION_CONTROL;
                }
            }
            /* Ҫ���˳����� */
            else if(STOP)
            {
                *st = TEST_STOP_CONTROL;
            }
            /* ʧ�ܺ�������������������������ǿ��Դӵ�һ����ʼ������ */
            else if(FAIL_RE_START)
            {
                *st = TEST_FAIL_RE_START_CONTROL;
            }
            /* ������һ������ */
            else if(CONT)
            {
                *st = TEST_CONT_TEST_CONTROL;
            }
            /* ����ݲ��Բ��ҷŵ���� */
            else if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW
                    && test_flag.dis_charge_end == 1 && tes_t == 0)
            {
                *st = TEST_STOP_CONTROL;//TEST_QUIT_CONTROL;
            }
            /* ��ͨ���� */
            else
            {
                *st = TEST_TESTING_CONTROL;
            }
            break;
        }
        case TEST_FAIL_RE_START_CONTROL:/* ����ʧ���������� */
        {
            switch(FAIL_RE_START)
            {
                /* �����ӵ�һ����ʼ */
                case FAIL_RE_START_FIRST:
                {
                    load_steps_to_list(1);
                    g_cur_step = list_99xx.head;
                    clear_test_falg();/* ��ձ�־λ */
                    *st = TEST_START_CONTROL;
                    break;
                }
                /* ��������һ����ʼ */
                case FAIL_RE_START_NEXT:
                {
                    NEXT = 1;
                    *st = TEST_START_CONTROL;
                    break;
                }
                default:
                {
                    *st = TEST_QUIT_CONTROL;
                    break;
                }
            }
            break;
        }
        case TEST_EXCEPTION_CONTROL:/* �쳣���� */
        {
            save_cur_result(&cur_result);
            
            /* ����ݲ��Եȴ����ݷ������ */
            if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW
                && test_flag.dis_charge_end == 0)
            {
                dis_charge_remain_vol();
            }
            
            app_ctrl_exit_sw(EXIT_STOP, DISABLE);/* �رո�λ�ж� */
            dis_fail();
            clear_keyboard();
            *st = TEST_QUIT_CONTROL;
//             else if(CONT)
//             {
//                 CONT = 0;
//                 *st = TEST_TESTING_CONTROL;
//             }
            break;
        }
        case TEST_PASS_CONTROL:
        {
            test_pass();
            save_cur_result(&cur_result);
            cur_status = ST_PASS;/* ��ǰ״̬Ϊpass */
            dis_pass();
            
            NEXT = 1;
            
            if(sys_par.fail_mode == FAIL_MODE_FPDFC && cur_step == 1)
            {
                NEXT = 0;
            }
            
            /* �˳����� */
            if(test_flag.g_stop)
            {
                dis_stop();
                LCD_REFRESH();
                app_ctrl_exit_sw(EXIT_STOP, DISABLE);	/* �رո�λ�ж� */
                clear_keyboard();
                *st = TEST_QUIT_CONTROL;
            }
            /* �������� */
            else if(test_flag.g_start)
            {
                clear_keyboard();
                *st = TEST_START_CONTROL;
            }
			/* �˳����� */
            else
            {
//                 dis_stop();
//                 LCD_REFRESH();
//                 app_ctrl_exit_sw(EXIT_STOP, DISABLE);	/* �رո�λ�ж� */
//                 clear_keyboard();
                *st = TEST_QUIT_CONTROL;
            }
            break;
        }
        case TEST_STOP_CONTROL:
        {
            plc_signal_cmd(PLC_SIG_RESET);
            
            /* ����ݲ��Եȴ����ݷ������ */
            if(type_spe.dcw_big_cap_en == ENABLE && cur_mode == DCW
                && test_flag.dis_charge_end == 0)
            {
                STOP = 0;
                
				disable_stop_exit();/* �رո�λ�ж� */
                g_test_time = for_t + 1;
                on_sample_task();
                open_test_timer();/* ����ʱ�� */
                enable_stop_exit();
                ERR_NUM = ERR_NONE;
                test_flag.judge_err_en = 0;
                *st = TEST_TESTING_CONTROL;
            }
            /* ��������ֹͣ */
            else
            {
                app_ctrl_exit_sw(EXIT_STOP, DISABLE);	/* �رո�λ�ж� */
                stop_server_for_comm();
                dis_stop();
                clear_keyboard();
                TERMINATE = 0;
                *st = TEST_QUIT_CONTROL;/* �˳����� */
            }
            break;
        }
        case TEST_CONT_TEST_CONTROL:
        {
            /* ��������������һ�� Gģʽ û��ֹͣ���� */
            if(CONT && g_cur_file->work_mode == G_MODE && !STOP)
            {
                CONT = 0;
                g_mode_next_step();
                *st = TEST_TESTING_CONTROL;
            }
            /* ��������������һ�� Nģʽ û��ֹͣ���� */
            else if(CONT && g_cur_file->work_mode == N_MODE && !STOP)
            {
                CONT = 0;
                save_cur_result(&cur_result);
                stop_test();/* ֹͣ���� */
                
                /* ��ǰ���������һ�� */
                if(g_cur_step->next != NULL)
                {
                    /* ��ǰ��û�з����쳣 */
                    if(!CUR_FAIL)
                    {
                        test_pass();
                    }
                    
                    /* ������һ�� */
                    load_steps_to_list(g_cur_step->one_step.com.step + 1);
                    g_cur_step = list_99xx.head;
                }
                /* ��ǰ�������һ�� */
                else
                {
                    /* ���ʱ��Ϊ0 */
                    if(int_t == 0)
                    {
                        /* ��ǰ��û�з����쳣 ���������˳� */
                        if(!CUR_FAIL)
                        {
                            PASS = 1;/* �����ٽ��в���ʱ�ͽ�pass��־д1 */
                            *st = TEST_PASS_CONTROL;
                            break;
                        }
                    }
                    /* ���ص�һ�� */
                    load_steps_to_list(1);
                    g_cur_step = list_99xx.head;
                }
                
                load_data();/* �������� */
                update_info();/* ���½��� */
                
                OSTimeDlyHMSM(0,0,0,5);/* ��ʱ */
                
                /* ��������˸�λ����ֹͣ���� */
                if(TERMINATE)
                {
                    *st = TEST_STOP_CONTROL;
                }
                /* �������� */
                else
                {
                    *st = TEST_START_CONTROL;
                }
            }
            break;
        }
    }
}
/*
 * ��������testing_gui_control
 * ����  �����Գ���������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void testing_gui_control(uint8_t *st)
{
    uint8_t state = 0;
    
    if(st == NULL)
    {
        return;
    }
    
    state = *st;
    *st = TEST_NULL;
    
    switch(state)
    {
        case TEST_RE_DRAW:
        {
            re_draw_test_ui();
            break;
        }
        
        case TEST_INTO_GETO:
        {
            sys_offset(0);
            OFFSET_BBD =TEST_NORMAL;
            app_flag.into_get_offset = 0;
            *st = TEST_RE_DRAW;
            clear_test_falg();/* ��ձ�־λ */
            load_steps_to_list(1);
            g_cur_step = list_99xx.head;
            break;
        }
        case TEST_INTO_GETC:
        {
            if(type_spe.mode_en & __BBD)
            {
                sys_bbd(0);
                OFFSET_BBD = TEST_NORMAL;
                app_flag.into_get_cap = 0;
                *st = TEST_RE_DRAW;
                clear_test_falg();/* ��ձ�־λ */
                load_steps_to_list(1);
                g_cur_step = list_99xx.head;
            }
            break;
        }
        case TEST_PREV_STEP:
        {
            clear_test_falg();/* ��ձ�־λ */
            
            if(g_cur_step->one_step.com.step > 1)
            {
                load_steps_to_list(g_cur_step->one_step.com.step - 1);
                g_cur_step = list_99xx.head;
            }
            else
            {
                load_steps_to_list(g_cur_file->total);
                g_cur_step = list_99xx.head;
            }
            
            update_info();
            break;
        }
        case TEST_NEXT_STEP:
        {
            clear_test_falg();/* ��ձ�־λ */
            
            if(g_cur_step->next != NULL)
            {
                load_steps_to_list(g_cur_step->next->one_step.com.step);
                g_cur_step = list_99xx.head;
            }
            else
            {
                load_steps_to_list(1);
                g_cur_step = list_99xx.head;
            }
            
            update_info();
            break;
        }
        case TEST_SHORTCUT:
        {
            clear_test_falg();/* ��ձ�־λ */
            de_reset();
			plc_signal_cmd(PLC_SIG_EXIT);/* PLC״̬����Ϊδ���� */
            serve_test_shortcut();
            
            if(g_exit)
            {
                return;
            }
            
            if(g_return)
            {
                g_return = 0;
                test_reset();
                *st = TEST_RE_DRAW;
                break;
            }
            
            test_reset();
            
            /* ����ݲ�������迹���� */
            if(type_spe.dcw_big_cap_en == ENABLE)
            {
                if(cur_mode == DCW)
                {
                    dcw_big_cap_output_Impedan_sel(cur_output_impedan);
                }
            }
            
            *st = TEST_RE_DRAW;
            break;
        }
        case TEST_RESET_UI:
        {
            if(sys_par.fail_mode != FAIL_MODE_HALT || cur_step == g_cur_file->total)
            {
                clear_test_falg();/* ��ձ�־λ */
            }
            
			dis_stop();
            LED_ALL = LED_OFF;
			LCD_REFRESH();
            break;
        }
        case TEST_STOP_UI:
        {
			stop_server_for_comm();
			dis_stop();
            break;
        }
        default:
        {
            off_schedule_task();/* �رյ����� */
            break;
        }
    }
}
/*
 * ������:test_running
 * ����  :
 * ����  :��
 * ���  :��
 * ����  :��
 */
void test_running(void)
{
    uint8_t test_state = TEST_START_CONTROL;
    
    off_schedule_task();
    
    while(1)
    {
        testing_process_control(&test_state);
        OSTimeDlyHMSM(0,0,0,5);/* ������Ȩ���� */
        
        /* �˳����� */
        if(TEST_QUIT_CONTROL == test_state)
        {
            break;
        }
    }
}
enum{
    GENERAL_TEST_UI,/* ��ͨ���Խ��� */
    OFFSEET_TEST_UI,/* ƫ�Ʋ��Խ��� */
};
/*
 * ������:serve_test
 * ����  :
 * ����  :��
 * ���  :��
 * ����  :��
 */
void serve_test(void)
{
	uint16_t* pkey_value = NULL;
	INT8U err = 0;
    uint8_t test_state = TEST_RE_DRAW;
    
    if(g_comm_status != LOCAL_COMM)
    {
        off_keyboard();/*Զ�̿��� �رռ��� */
    }
	
    init_test_ui_info(GENERAL_TEST_UI);/* ��ʼ�����Խ���������Ϣ */
    load_steps_to_list(1);/* ���ص�һ���������� */
    g_cur_step = list_99xx.head;
    load_data();/* ���ص�һ�� */
	
	install_test_irq_fun();/* ��װ����״̬�� */
	exit_off();
    clear_test_falg();/* ��ձ�־λ */
	test_reset();/* �л��̵��� */
    
	clear_();/* ���� */
    
	while(1)
	{
		/* ���Խ������ */
        testing_gui_control(&test_state);
        
        if(app_flag.into_get_cap == 1)
        {
            app_flag.into_get_cap = 2;
            test_state = TEST_INTO_GETC;
        }
        
        if(app_flag.into_get_offset == 1)
        {
            app_flag.into_get_offset = 2;
            test_state = TEST_INTO_GETO;
        }
        
        /* �˵������� */
		if(g_exit)
		{
			goto l_exit;
		}
        /* �¼��˵���������Ҫ�ػ���� */
		if(g_return)
		{
			g_return = 0;
			test_state = TEST_RE_DRAW;
		}
		/* ����ֹͣ ��ʾ�ȴ����Խ��� */
		if(STOP)
		{
            test_state = TEST_STOP_UI;
		}
		/* ���¸�λ���� ��ʾ�ȴ����Լ��� ������ͨ�ſ��� */
		if(RESET == STOP_PIN || TERMINATE/* && CUR_OVER*/)
		{
            test_state = TEST_RESET_UI;
		}
        
        /* ��λ��������������Ҫˢ����ʾ */
		if(test_flag.refresh_for_comm == 1)
		{
			test_flag.refresh_for_comm = 0;
			test_state = TEST_RE_DRAW;
		}
        
        /* �Ӱ�������ȡ��ֵ */
		pkey_value = OSQPend(KeyboardQSem,2,&err);
        
        if(pkey_value == NULL)
        {
            continue;
        }
        
		switch(*pkey_value)
		{
			case KEY_UP:
                test_state = TEST_PREV_STEP;
				break;
			case KEY_DOWN:
                test_state = TEST_NEXT_STEP;
				break;
			case KEY_SHORTCUT:
                test_state = TEST_SHORTCUT;
				break;
			case KEY_BACK:
			case KEY_EXIT:
				de_reset();
				g_exit = 1;
				break;
            case KEY_STOP:
                cur_cylinder_ctrl_stop();
                break;
			case KEY_START:
				if(STOP_PIN == RESET)
				{
					break;
				}
                
                test_running();/* ������Գ��� */
				break;
		}//end switch()
	}
	
l_exit:
	plc_signal_cmd(PLC_SIG_EXIT);
}
/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/