/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�GUI_Draw.c
 * ժ  Ҫ  ����������������Ҫ����
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
 
 /********************* ����ͷ�ļ� ***********************/
#define	 MENU_GLOBAL


#include	<stdio.h>
#include	<string.h>
#include    "stm32f10x.h"
#include	"keyboard.h"
#include 	"app.h"
#include 	"serve_test.h"
#include	"cs99xx_type.h"
#include    "GUI_Draw.h"
#include    "math.h"
#include    "mb_cmd_define.h"



/************************* ȫ�ֱ�����ʼ�� ******************/
const uint8_t *unit_pool[]=
{
	"","mV","V","kV","~A","mA","A","m��","��","M��","G��","pF","nF","~F",
};
const uint8_t *unit_pool_[]=
{
	"","mV","V","kV","uA","mA","A","m��","��","M��","G��","pF","nF","uF",
};
const uint8_t *mode_pool[][10]=
{
	{"", "ACW", "DCW", "IR ", "GR ", "BBD", "CC "},
	{"", "ACW", "DCW", "IR ", "DV ", "BBD", "CC "},
};

const uint8_t *opt_pool[]=
{
    "","��ѹ", "����", "����", "����",
};

const char *work_mode_buf[]=
{
	"G", "N",
};//G_MODE

const char *offset_res_buf[4]=
{
	" ", "F", "P", "-"
};

const char* status_str[][2] = 
{
	{"�ȴ�����", "TestWait"},
	{"��ѹ����", "  Rise  "},
	{"���ڲ���", "Testing "},
	{"��ѹ�½�", "  Fall  "},
	{"��ѹ����", "Change  "},
	{"����ȴ�", "Interval"},
	{"���Ժϸ�", "  PASS  "},
	{"����ʧ��", "TestFail"},
	{"���ڳ��", " Charge "},
	{"��ѹʱ��", "Sta.Time"},
	{"���ڷŵ�", "Discharg"},
	{"�ŵ����", "Disch.OK"},
	{"�����ʱ", "Out.Dly"},
};

const char* except_buf[][3]=
{
	{"","",""},
	{"���ޱ���", "High Fail", "High"   },
	{"���ޱ���", "Low Fail" , "Low"    },
	{"��ʵ����", "Real Fail", "Real"   },
	{"��籨��", "Char Fail", "Charge" },
	{"��·����", "Open Fail", "Open"   },
	{"��·����", "ShortFail", "Short"  },
	{"ARC����" , "ARC Fail" , "ARC"    },
	{"GFI����" , "GFI Fail" , "GFI"    },
	{"���ű���", "AMP Fail" , "AMP"    },
	{"��λ����", "GEAR Fail", "GEAR"   },
	{"��ѹ�쳣", "VOL. Fail", "VOL."   },
	{"δ����"  , "Undefine ", "Undef." },
};
const char* sys_sw_status_buf[][2] =
{
	{"GFI:DISABLE"	,"GFI:ENABLE "},
	{"ARC:DISABLE" 	,"ARC:ENABLE "},
	{" HV:ZERO"		," HV:HIGH   "},
};
/* IR��ʾ����ʱ����ʱʹ�õ� */
const char* res_infinite_str[]=
{
    "-----","-.---","--.--","---.-","-.---","--.--","---.-"
};

MORE_OPTIONS_STR cc_cur_gear_option_pool[]=
{
    {{"��", "High"},PX,0,CC_CUR_HIGH,NULL,NULL},
    {{"��", "Mid."},PX,0,CC_CUR_MID ,NULL,NULL},
    {{"��", "Low "},PX,0,CC_CUR_LOW ,NULL,NULL},
};

MORE_OPTIONS_STR ir_filtering_deep_option_pool[]=
{
    {{"��", "Fast"},PX,0,IR_FILTERING_DEEP_FAST,NULL,NULL},
    {{"��", "Mid."},PX,0,IR_FILTERING_DEEP_MID ,NULL,NULL},
    {{"��", "slow "},PX,0,IR_FILTERING_DEEP_SLOW ,NULL,NULL},
};

MORE_SEL_STR mor_sel_menu[]=
{
    {0, PX/*x*/,0/*y*/,{0, 50, 50 + 40, 0, 50, 50 + 40, 0}},/* ACW������λ */
    {0, PX/*x*/,0/*y*/,{0, 35, 40 + 35, 0, 35, 40 + 35, 0}},/* DCW������λ */
    {0, PX/*x*/,0/*y*/,{0, 35, 40 + 35, 0, 35, 40 + 35, 0}},/* GR������λ */
    {0, PX/*x*/,0/*y*/,{0, 28, 28 + 28, 28 + 28 + 23, (28 + 28 + 23 + 23), 0}},/* ģʽ���� */
    {0, SX/*x*/,0/*y*/,{0, SOX, 0, SOX, 0, SOX, 0, SOX, 0}},/* ϵͳʧ��ģʽ���� */
};

const uint8_t* fail_mode_pool[]=
{
    "STOP"      ,
    "HALT"      ,
    "CON."      ,
    "RESTART"   ,
    "NEXT"      ,
    "FPDFC"     ,
};

MORE_OPTIONS_STR plc_signal_option_pool[]=
{
    {{"����", "EachStep"},SX,0,EACH_STEP,NULL,NULL},
    {{"�ܲ�", "AllSteps"},SX,0,ALL_STEP ,NULL,NULL},
    {{"����", "Hold"    },SX,0,TEST_HOLD,NULL,NULL},
};


MORE_OPTIONS_STR amp_type_option_pool[]=
{
    {{"8833", "8833"} ,SX,0,AMP_8833,NULL,NULL},
    {{"Old", "Old"  } ,SX,0,AMP_OLD ,NULL,NULL},
};
MORE_OPTIONS_STR par_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, PAR_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, PAR_MEDIUM_FLASH ,NULL,NULL},
};
MORE_OPTIONS_STR res_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, RES_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, RES_MEDIUM_FLASH ,NULL,NULL},
};
MORE_OPTIONS_STR cal_medium_option_pool[]=
{
    {{"eep", "eep"      } ,SX,0, CAL_MEDIUM_EEP,NULL,NULL},
    {{"flash", "flash"  } ,SX,0, CAL_MEDIUM_FLASH ,NULL,NULL},
};


const OUTPUT_IMPEDAN_OPTS_T output_impedan_opt[]=
{
    {
        {
            "0 - 0.00k��",
            "1 - 22.4k��",
            "2 - 7.60k��",
            "3 - 4.00k��",
        },
        4,
    },
    {
        {
            "0 - 4.40k��",
            "1 - 2.80k��",
            "2 - 1.10k��",
        },
        3,
    },
};

DATA_INFO menu_par[]=
{
	/************* COM ************/
	{// 1   ���Բ���
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,4/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		1,0,{"���Բ���","Step"},{"","",""},set_step,0/*no_dis_bits*/,GUI_COM_STEP,UDSB,"99  ","01  ",
	},
	
	{// 2   ����ģʽ
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		1,0,{"����ģʽ","Mode"},{"","ACW","DCW","IR","GR","BBD"},set_mode,0/*no_dis_bits*/,GUI_COM_MODE,UDRB,
	},
	
	{// 3   ��ʱʱ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		9999,3,{"��ʱʱ��","Delay"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_DELAY_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 4   ����ʱ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		9999,3,{"����ʱ��","Rise"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_RISE_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
    
	{// 5   ��ѹʱ��
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,3,{"��ѹʱ��","Stabili."},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_STAB_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 6   ����ʱ��
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,3,{"����ʱ��","Test"},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_TEST_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 7	�½�ʱ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		9999,3,{"�½�ʱ��","Fall"},{"s",""},set_test_time,0/*no_dis_bits*/,GUI_COM_FALL_T,UDRB,"999.9s","0,0.3s",0,0,1/*rang_flag*/,
	},
	
	{// 8	���ʱ�� \ ����ʱ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		9999,0,{"���ʱ��","Interval"},{"s","",""},set_test_time,0/*no_dis_bits*/,GUI_COM_INTER_T,UDRB,"999.9s","000.0s",0,0,1/*rang_flag*/,
	},
	
	{// 9 ����PASS
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,EITHER_OR/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,1,{"����PASS","StepPass"},{"��","��","Yes","No"},set_either_or_par,0/*no_dis_bits*/,GUI_COM_STEPS_PASS,UDRB,"","",35,3,
	},
	
	{// 10 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,EITHER_OR/*type*/,5/*lon*/,1/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,1,{"��������","StepSer."},{"��","��","Yes","No"},set_either_or_par,0/*no_dis_bits*/,GUI_COM_STEPS_CONT,UDRB,"","",35,3,
	},
	
	{// 11 ���Ƶ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,1/*bits*/,0,
		0,10,{"���Ƶ��","Freq."},{"","Hz",""},set_frequency,0/*no_dis_bits*/,GUI_COM_OUTPUT_FREQ,UDRB,"   9  ","   1  ",0,0,1/*rang_flag*/,
	},
	
	{// 12	���Զ˿�
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,STRUCT_T/*type*/,0/*lon*/,0/*decs*/,8/*total*/,1/*cur*/,0/*ch_bit*/,6/*page*/,4/*bits*/,0,
		0,10,{"���Զ˿�","Port"},{"",""},set_all_ports,0/*no_dis_bits*/,GUI_COM_PORT,UDRB,"","",40,3
	},
    
	{// 13 ����ƫ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,0,{"����ƫ��","Offset"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_COM_CUR_OFFSET,UDRB,
	},
	
	{// 14 ����ƫ��
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,0,{"����ƫ��","Offset"},{"m��",""},set_float_par,0/*no_dis_bits*/, GUI_COM_RES_OFFSET,UDRB,
	},
	
	/************* ACW ************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_ACW_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 ������λ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"������λ","Range"},{"",""},set_ac_cur_gear,0/*no_dis_bits*/,GUI_ACW_CUR_GEAR,UDRB,
	},
	
	{// 3 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","High"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_CUR_H,UDRB,
	},
	
	{// 4 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_CUR_L,UDRB,
	},
	
	{// 5 ��ʵ����
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"��ʵ����","Real"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_REAL_CUR,UDRB,
	},
	
	{// 6 �绡���
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"�绡���","Arc"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_ACW_ARC_SUR,UDRB,
	},
    
	/************* DCW ************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_DCW_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 ������λ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"������λ","Range"},{"",""},set_dcw_cur_gear,0/*no_dis_bits*/, GUI_DCW_CUR_GEAR,UDRB,
	},
	
	{// 3 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","High"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CUR_H,UDRB,
	},
	
	{// 4 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CUR_L,UDRB,
	},
	
	{// 6 ������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"������","Charge_I"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_CHARGE_CUR,UDRB,
	},
    
	{// 7 �绡���
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"�绡���","Arc"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_DCW_ARC_SUR,UDRB,
	},
    
	{// 8 ����迹
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		3,0,{"����迹","Impedan"},{"","0 - 0.00k��","1 - 22.4k��","2 - 7.60k��","3 - 4.00k��"},set_float_par,0/*no_dis_bits*/, GUI_DCW_OUTPUT_IMPEDANCE,UDRB,"","",0,0,2/*rang_flag*/,
	},
    
	/************* IR ************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_IR_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 �Զ�����
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"�Զ�����","Auto.Lev"},{"��","��","YES","NO"},set_either_or_par,0/*no_dis_bits*/, GUI_IR_AUTO_GEAR,UDRB,"","",35,3,
	},
	
	{// 3 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,2/*bits*/,0,
		9999,0,{"��������","High"},{"M��","M��"},set_float_par,0/*no_dis_bits*/, GUI_IR_RES_H,UDRB,"09999M��","00000M��",
	},
	
	{// 4 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,0/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,2/*bits*/,0,
		9999,1,{"��������","Low"},{"M��","M��"},set_float_par,0/*no_dis_bits*/, GUI_IR_RES_L,UDRB,"09999M��","00000M��",0,0,0/*rang_flag*/,
	},
    
	/************* GR ************/
	{// 1 ��ѹ��λ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,OPTION_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"��ѹ��λ","Gear"},{"","","20mV","20mV","200mV","200mV","2000mV""2000mV","12V","12V"},set_gr_vol_gear,0/*no_dis_bits*/,GUI_GR_VOL_GEAR,UDRB,
	},
	
	{// 2 �������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		1,0,{"�������","Cur."},{"A","mA",""},set_gr_output_cur,1/*no_dis_bits*/,GUI_GR_OUTPUT_CUR,UDRB,
	},
	
	{// 3 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"��������","High"},{"m��","��",""},set_gr_res_upper,0/*no_dis_bits*/,GUI_GR_RES_H,UDRB,"510.0m��","001.0m��",
	},
	
	{// 4 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"��������","Low"},{"m��","��",""},set_gr_res_lower,0/*no_dis_bits*/,GUI_GR_RES_L,UDRB,
	},
	
	{// 5 ��ѹ����
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"��ѹ����","High"},{"mV","V",""},set_gr_vol_upper,0/*no_dis_bits*/,GUI_GR_VOL_H,UDRB,"00.00V","00.00V",
	},
	
	{// 6 ��ѹ����
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,1,{"��ѹ����","Low"},{"mV","v",""},set_gr_vol_lower,0/*no_dis_bits*/,GUI_GR_VOL_L,UDRB,"00.00V","00.00V",
	},
	
	{// 7 ����ģʽ
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,OPTION_T/*type*/,1/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,0/*bits*/,0,
		1,0,{"���Է���","Method"},{"����","����","Cont.","Pulse"},set_gr_test_method,0/*no_dis_bits*/,GUI_GR_TEST_METHOD,UDRB,"","",40,3
	},
    
	/************* BBD************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_BBD_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 ��·���
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,3/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		100,10,{"��·���","Open"},{"%","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_OPEN_CHECK,UDRB,"  100%","  10%",0,0,1/*rang_flag*/,
	},
	
	{// 3 ��·���
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,INT_T/*type*/,3/*lon*/,0/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"��·���","Short"},{"%","v",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_SHORT_CHECK,UDRB," 500%","0,100%",0,0,1/*rang_flag*/,
	},
	
	{// 4 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"��������","High"},{"nF","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_CAP_H,UDRB,"0.000~F","0.000~F",0,0,0/*rang_flag*/,
	},
	
	{// 5 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		500,100,{"��������","Low"},{"nF","",""},set_float_par,0/*no_dis_bits*/,GUI_BBD_CAP_L,UDRB,"0.000~F","0.000~F",0,0,0/*rang_flag*/,
	},
	
	/************* CC ************/
	{// 1 ���Ե�ѹ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,1/*bits*/,0,
		10,0,{"�����ѹ","Vol."},{"kV","",""},set_voltage,0/*no_dis_bits*/,GUI_CC_OUTPUT_VOL,UDRB,"0.050kV","5.000kV"
	},
	
	{// 2 ������λ
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,INT_T/*type*/,0/*lon*/,0/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"������λ","Range"},{"",""},set_ac_cur_gear,0/*no_dis_bits*/,GUI_CC_CUR_GEAR,UDRB,
	},
	
	{// 3 ��������
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","High"},{"",""},set_float_par,0/*no_dis_bits*/, GUI_CC_CUR_H,UDRB,
	},
	
	{// 4 ��������
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		0,10,{"��������","Low"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_CC_CUR_L,UDRB,
	},
	
	{// 5 ��ʵ����
		(void*)0,/*pData*/PX/*x*/,PY/*y*/,FLOAT_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"��ʵ����","Real"},{"mA",""},set_float_par,0/*no_dis_bits*/, GUI_CC_REAL_CUR,UDRB,
	},
	
	{// 6 ����ǿ��
		(void*)0/*pData*/,PX/*x*/,PY/*y*/,OPTION_T/*type*/,5/*lon*/,2/*decs*/,3/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,1/*bits*/,0,
		0,10,{"����ǿ��","CurInten."},{"��","Mid"},set_option_par,0/*no_dis_bits*/, GUI_CC_CUR_INTENSITY,UDRB,
	},
    
	/************* SYS ************/
	{//	1 Һ���Աȶ�
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		9,1,{"Һ���Աȶ�","Contrast"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_CONTRAST,UDRB,"(1-9)",""
	},
	
	{//	2 ������ʾ
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		9,0,{"������ʾ","RemainHint"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_REMAIN_HINT,UDRB,"(0-9)",""
	},
	
	{//	3 �������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		0,10,{"�������","ResultSave"},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_RES_SAVE,UDRB,"","",SOX,SOY,
	},
	
	{//	4 �������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,1/*page*/,0/*bits*/,0,
		0,10,{"�������","OverCover"},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_VOER_COVER,UDRB,"","",SOX,SOY,
	},
	
	{//	5 GFI����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"GFI ����","GFI Prot."},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_GFI_PROT,UDRB,"","",SOX,SOY,
	},
	
	{//	6 �Լ�����
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"�Լ�����","SelfCheck"},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_SELF_CHECK,UDRB,"","",SOX,SOY,
	},
	
	{//	7 ����ѡ��
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"����ѡ��","Language"},{"����","Ӣ��","Chinese","English",},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_LANGUAGE,UDRB,"","",SOX,SOY,
	},
	
	{//	8 ʧ�ܼ���
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"ʧ�ܼ���","FailCont."},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_FAIL_CONT,UDRB,"","",SOX,SOY,
	},
	
	{//	9 �б���ʾ
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"�б���ʾ","ListDisplay"},{"��","��","Yes","No"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_LIST_DIS,UDRB,"","",SOX,SOY,
	},
	
	{//	10 ���Զ˿�
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,2/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"���Զ˿�","TestPort"},{"�ӵ�","����","GND","Float"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_TEST_PORT,UDRB,"","",SOX,SOY,
	},
    
	{//	11 �����ź�
		(void*)0,/*pData*/SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,3/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"�����ź�","TestSignal"},{"��ƽ","����","Level","Point"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_TEST_SIGNAL,UDRB,"","",SOX,SOY,
	},
	
	{//	12 ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,3/*page*/,0/*bits*/,0,
		0,10,{"��������","Buzzer SW."},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_BUZZER_SW,UDRB,"","",SOX,SOY,
	},
	
	{//	13  ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,STRING_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,3/*bits*/,0,
		0,10,{"��������","CheckDate"},{"2015-1-1 12:12:12",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_CHECK_DATE,UDRB,"","",SOX,SOY,
	},
	
	{//	14  ��Ź���
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,INT_T/*type*/,1/*lon*/,0/*decs*/,4/*total*/,1/*cur*/,0/*ch_bit*/,4/*page*/,0/*bits*/,0,
		2,0,{"��Ź���","NO.Rule"},{"",""},set_int_sys_par,0/*no_dis_bits*/, GUI_SYS_NO_RULE,UDRB,"(0-2)","",SOX,SOY,
	},
	
	{//	15 ʧ��ģʽ
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"ʧ��ģʽ","FailMode"},{"STOP","STOP","Yes","No"},set_option_sys_par,0/*no_dis_bits*/, GUI_SYS_FAIL_MODE,UDRB,"","",SOX,SOY,
	},
    
	{//	16 ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"��������","Silent SW."},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_SYS_SILENT_SW,UDRB,"","",SOX,SOY,
	},
    
	{//	17 PLC �ź�
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"PLC �ź�","PLC Signal"},{"����","�ܲ�","EachStep","AllSteps"},set_plc_option_sys_par,0/*no_dis_bits*/, GUI_SYS_PLC_SIGNAL,UDRB,"","",SOX,SOY,
	},
    
	{//	18 �����ʱ
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,FLOAT_T/*type*/,5/*lon*/,1/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,0,{"�����ʱ","Out.Delay"},{"s (0.0-999.9s)","s (0.0-999.9s)"},set_float_sys_par,0/*no_dis_bits*/, GUI_SYS_OUTPUT_DELAY,UDRB,"(0.0-999.9s)","",SOX,SOY,
	},
    
	{//	19 ��λ����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,FLOAT_T/*type*/,5/*lon*/,3/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,1/*bits*/,0,
		9999,200,{"IR��λ����","IRGearHold"},{"s (0.2-9.999s)","s (0.2-9.999s)"},set_float_sys_par,0/*no_dis_bits*/, GUI_SYS_IR_GEAR_HOLD,UDRB,"(0.2-9.999s)","",SOX,SOY,
	},
    
	{//	20 IR �����ٶ�
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,5/*lon*/,3/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		9999,200,{"IR�����ٶ�","IR SPEED"},{"",""},set_option_par,0/*no_dis_bits*/, GUI_SYS_IR_SPEED,UDRB,"(0.2-9.999s)","",SOX,SOY,
	},
    
	/************* CUSTOM(���Ʋ���) ************/
	{//	1 ����ѡ��
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"����ѡ��","8833AMP"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_AMP_SELECT,UDRB,"","",SOX,SOY,
	},
	{//	2 ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"��������","AMP_TYPE"},{"PWM","LINE","PWM","LINE"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_AMP_TYPE,UDRB,"","",SOX,SOY,
	},
	{//	3 �����洢����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"�����洢","PAR_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_PAR_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	4 ����洢����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"����洢","RES_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_RES_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	5 У׼�洢����
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,OPTION_T/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"У׼�洢","CAL_MEDIUM"},{""},set_custom_options_par,0/*no_dis_bits*/, GUI_CUSTOM_CAL_MEDIUM,UDRB,"","",SOX,SOY,
	},
	{//	6  ������ʹ��
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"������ʹ��","BUZZER-EN"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_BUZZER_EN,UDRB,"","",SOX,SOY,
	},
	{//	7  ��������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"��������","MUTE-SW"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_MUTE_SW,UDRB,"","",SOX,SOY,
	},
	{//	8  ���뵼��
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"���뵼��","LeadingSW"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_LEADING_SW,UDRB,"","",SOX,SOY,
	},
	{//	9  ������ʱ
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"IR��������","IR_DELAY"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_IR_G_HOLD_SW,UDRB,"","",SOX,SOY,
	},
	{//	10  IR�����ٶ�
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"IR�����ٶ�","IR_SPEED"},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_IR_SPEED_SW,UDRB,"","",SOX,SOY,
	},
	
	{//	11  ƫ������
		(void*)0/*pData*/,SX/*x*/,SY/*y*/,EITHER_OR/*type*/,0/*lon*/,0/*decs*/,4/*total*/,4/*cur*/,0/*ch_bit*/,2/*page*/,0/*bits*/,0,
		0,10,{"ƫ������","OFFSET_SW."},{"��","�ر�","ON","OFF"},set_either_or_sys_par,0/*no_dis_bits*/, GUI_CUSTOM_OFFSET_SW,UDRB,"","",SOX,SOY,
	},
};


void more_options_sel_gui(uint8_t options, uint8_t n, MORE_SEL_STR *mor_sel_info);
/*************************** �������� *************************/
uint8_t * mystrcat(uint8_t *buf, uint8_t *str1, uint8_t *str2)
{
    uint8_t t_buf1[20] = {0};
    uint8_t t_buf2[20] = {0};
    
    strcpy((char*)t_buf1, (const char*)str1);
    strcpy((char*)t_buf2, (const char*)str2);
    buf[0] = 0;
    
    strcat((char*)buf, (const char*)t_buf1);
    strcat((char*)buf, (const char*)t_buf2);
    
    return buf;
}

void front_add_space_at_string(uint8_t *buf, uint8_t c)
{
}

void init_test_gui_area_info(void)
{
}
/*
 * ��������draw_colon
 * ����  ��������ҳ���·���ʾð��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void draw_colon(void)
{
}

/*
 * ��������save_par_to_array
 * ����  ��������ֵ���µ�һ��������
 * ����  ��data_info �˵������ݽṹ
 *         value �и��µ�����
 *         array_num �����Ա�ĸ���
 * ���  ����
 * ����  ����
 */
void save_par_to_array(DATA_INFO *data_info, uint32_t value, uint8_t array_num)
{
}
/*
 * ��������save_par_to_ram
 * ����  ��������ֵ���µ��ڴ�
 * ����  ��data_info �˵������ݽṹ
 *         value �и��µ�����
 * ���  ����
 * ����  ����
 */
void save_par_to_ram(DATA_INFO *data_info, uint32_t value)
{
}

/*
 * ��������draw_one_par_info
 * ����  ����ʾһ���˵����ֵ
 * ����  ��p �˵������ݽṹ
 *         pdata ��ʾ������
 *         unit  ��λ
 * ���  ����
 * ����  ����
 */
void draw_one_par_info(DATA_INFO* p, void *pdata, const uint8_t unit)
{
}

/*
 * ��������read_par_to_buf
 * ����  ������ָ���˵��������
 * ����  ��data_info
 * ���  ��value �˵������� ���ΪNULL�Ͳ���ֵ
 * ����  ���˵�������
 */
uint32_t read_par_to_buf(DATA_INFO* data_info, uint32_t *value)
{
    return 0;
}

/*
 * ��������load_this_page_info
 * ����  �����ص�ǰ����ҳ�����������Ϣ
 * ����  ����
 * ���  ��data_info
 * ����  ����
 */
void load_this_page_info(DATA_INFO* data_info[])
{
}
/*
 * ��������load_this_page_value
 * ����  ���Ѽ��ص��ڴ��������Ϣ��ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void load_this_page_value(void)
{
}

/*
 *ȥ��ǰ����
 */
char* div_str_pre_zero(char *str)
{
    uint8_t len = strlen(str);
    uint32_t i = 0;
    char *p = str;
    
    for(i = 0; i < len - 1; i++)
    {
        if(p[0] == '0' && p[1] != '.')
        {
            p++;
        }
        else
        {
            break;
        }
    }
    
    return p;
}


/*
 * ��������ten_power
 * ����  ��10����
 * ����  ����
 * ���  ����
 * ����  ����
 */
uint32_t ten_power(u8 n)
{
	u8 i;
	uint32_t temp = 1;
	for(i = 0; i < n; i++)
	{
		temp *= 10;
	}
	return temp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//������mysprintf                                                                                /
//����type :153 ��ʾ�����Ǹ����� 1��ǰ����0����ǰ���� ��󳤶�Ϊ5����С���㣬3λС�� ��󳤶Ȳ��ܳ���5��С��λ���ܳ���3 /
//    unit:Ϊ��������׷�ӵĵ�λ�ַ���                                                            /
//    value:Ҫת��Ϊ�����͵���������                                                             /
//////////////////////////////////////////////////////////////////////////////////////////////////
void mysprintf(uint8_t *buf, const uint8_t* unit, uint8_t type, uint32_t value)
{
    uint8_t decs_n = (type / 1) % 10;/* С��λ */
    uint8_t lon    = (type / 10) % 10;/* ���� */
    uint8_t app_0  = (type / 100) % 10;/* ǰ�����־ */
    uint8_t int_n = 0;
    uint8_t temp = 0;
    uint8_t temp_l = 0;
    int32_t i = 0;
    int32_t j = 0;
    
    if(buf == NULL)
    {
        return;
    }
    
    if(lon > 7 || decs_n > 3)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    
    if(lon == 4 && decs_n > 2)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    if(lon == 3 && decs_n > 1)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    if(lon == 2 && decs_n > 0)
    {
        strcpy((char*)buf, "0.000");
        strcat((char*)buf, (const char*)unit);
        return;
    }
    
    int_n =  lon - decs_n - (decs_n > 0);
    
    for(i = 0, j = 0; i < int_n; i++)
    {
        if(decs_n > 0)
        {
            temp = (value / ten_power(lon - 2 - i)) % 10;
        }
        else
        {
            temp = (value / ten_power(lon - 1 - i)) % 10;
        }
        
        /* ȥ��ǰ���� */
        if(app_0 == 0)
        {
            /* ���һ������λ */
            if(int_n - i == 1)
            {
                buf[j++] = temp + '0';
            }
            /* �������һ������λ */
            else
            {
                if(temp || j)
                {
                    buf[j++] = temp + '0';
                }
            }
        }
        /* ��ȥ��ǰ���� */
        else
        {
            buf[j++] = temp + '0';
        }
    }
    
    if(decs_n)
    {
        buf[j++] = '.';
    }
    
    temp_l = j;
    
    /* С������ */
    for(i = 0; i < decs_n; i++)
    {
        temp = (value / ten_power(decs_n - i - 1)) % 10;
        buf[i + temp_l] = temp + '0';
    }
    
    buf[i + temp_l] = 0;
    
    /* ׷�Ӻ�׺��Ϣ */
    if(unit != NULL)
    {
        strcat((char*)buf, (const char*)unit);
    }
}
/*
 * ��������cursor_pos
 * ����  ����ʾ��Ļ��� ���������һ����ʾ�Ĺ��
 * ����  ���������� �������궨��Ϊ�з�������Ϊ�˿��Դ��븺ֵ���꣬���ǽ�����Ƴ���Ļ
 * ���  ����
 * ����  ����
 */
void cursor_pos(int16_t x, int16_t y)
{
}
void dis_cursor_pos(void)
{
}
void cursor_pos_n(int16_t x, int16_t y)
{
}
/*
 * ��������shift_cursor_l
 * ����  �����������λ�� ����ʾ����
 * ����  ��p ָ��ǰ���ò�������Ϣ�ṹ
 * ���  ����
 * ����  ����
 */
void dis_cursor(DATA_INFO * p)
{
}
/*
 * ��������shift_cursor
 * ����  �����ѭ������
 * ����  ��p ָ��ǰ���ò�������Ϣ�ṹ
 * ���  ����
 * ����  ����
 */
void shift_cursor (DATA_INFO * p)
{
}

/*
 * ��������shift_cursor_l
 * ����  �����ѭ������
 * ����  ��p ָ��ǰ���ò�������Ϣ�ṹ
 * ���  ����
 * ����  ����
 */
void shift_cursor_l (DATA_INFO * p)
{
}

/*
 * ��������change_data
 * ����  ����������
 * ����  ��base ���ڲ�����λ������ֵ0-9
 * ���  ����
 * ����  ����
 */
void change_data(uint8_t base)
{
}
/*
 * ��������my_sprintf
 * ����  ���������ݵ���ʾ��
 * ����  ��p ָ��ǰ�������ݵ���Ϣ�ṹ
 * ���  ����
 * ����  ����
 */
void my_sprintf(char *buf, DATA_INFO * p, uint32_t data)
{
	int32_t i = 0;
	int32_t j = 0;
    uint8_t t = 0;
    
    uint8_t temp1 = p->lon;
    uint8_t temp2 = p->decs;
    
    if(p->decs != 0)
    {
        temp1 = p->lon - p->decs - 1;
    }
    
    //��������
    for (i = 0; i < temp1; i++)
    {
        t = (data / ten_power(temp1 + temp2 - i - 1)) % 10;
        buf[i] = '0' + t;
    }
    //С����
    if(temp2)
    {
        buf[i++] = '.';
    }
    //С������
    for(j = 0; j < temp2; j++)
    {
        t = (data / ten_power(temp2 - j - 1)) % 10;
        buf[i++] = '0' + t;
    }
    //��λ
    strcat(buf, (const char*)p->unit[0]);
}
/*
 * ��������update_data
 * ����  ���������ݵ���ʾ��
 * ����  ��p ָ��ǰ�������ݵ���Ϣ�ṹ
 * ���  ����
 * ����  ����
 */
void update_data(DATA_INFO * p)
{
}
/*
 * ��������clear_
 * ����  ��ֻ��������ʾ��� ��ֹ��Ļ��˸
 * ����  ����
 * ���  ����
 * ����  ����
 */
void clear_(void)
{
}

/*
 * ��������clear_menu
 * ����  �������Ļ�Ҳ�Ĳ˵��� ֻ��������ʾ��� ��ֹ��Ļ��˸
 * ����  ����
 * ���  ����
 * ����  ����
 */
void clear_menu(void)
{
}
void clear_menu_n(uint8_t n)
{
}

///////////////////////////////////////////////////////////////////


/*
 * ��������gui_par_inval
 * ����  ��˵������������Ч
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_par_inval(void)
{
}

/*
 * ��������gui_par_inval
 * ����  ����gģʽʱ�еĲ���������Ҫ�����ض�����²������õ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_g_mode_inval(void)
{
}
/*
 * ��������ratio_broken
 * ����  ��У׼ϵ����ʧ��������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void ratio_broken(void)
{
}
/*
 * ��������cal_warning
 * ����  ��У׼���ش򿪺���ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void cal_warning(void)
{
}
/*
 * ��������auto_cal_pass
 * ����  ���Զ�У׼ͨ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void auto_cal_pass(void)
{
}
/*
 * ��������rate_of_advance
 * ����  ����ɽ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void rate_of_advance(void)
{
}
/*
 * ��������type_mismatch
 * ����  �����Ͳ�ƥ������ʾ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void type_mismatch(void)
{
}
/*
 * ��������dis_set_port
 * ����  �����ý�����ʾ�˿�״̬
 * ����  ��val ״ֵ̬ n λ�� ports �˿���Ŀ 4 8
 * ���  ����
 * ����  ����
 */
void dis_set_port(const uint8_t val,const uint8_t n, const uint8_t ports)
{
}
/*
 * ��������ports_cursor_pos
 * ����  �����ý������ö˿�ʱ������ʾλ��
 * ����  ��n λ�� ports �˿���Ŀ 4 8
 * ���  ����
 * ����  ����
 */
void ports_cursor_pos(uint8_t n, uint8_t ports)
{
}
void gui_dis_float_at(const float v, const uint8_t decs, const uint8_t x, const uint8_t y)
{
}
/*
 * ��������dcw_cur_sel_gui
 * ����  ��dcw ������λѡ���õĽ���������
 * ����  ��kinds ������λ������ n �ڼ���������
 * ���  ����
 * ����  ����
 */
void dcw_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
}

/*
 * ��������acw_gr_hz_sel_gui
 * ����  ��Ƶ������ʱ�� ��ʾ�������
 * ����  ��kinds Ƶ�ʵ����� n �ڼ����� gear_buf ��λ�����ƻ���
 * ���  ����
 * ����  ����
 */
void acw_gr_hz_sel_gui(uint8_t kinds, uint8_t n, const uint8_t **gear_buf)
{
}
/*
 * ��������acw_cur_sel_gui
 * ����  ��acw ����ѡ�����������
 * ����  ��kinds ������λ������ n �ڼ���������
 * ���  ����
 * ����  ����
 */
void ac_cur_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
}
/*
 * ��������sys_option_sel_gui
 * ����  ��acw ����ѡ�����������
 * ����  ��kinds ������λ������ n �ڼ���������
 * ���  ����
 * ����  ����
 */
void sys_option_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n)
{
}

void more_options_sel_gui(uint8_t options, uint8_t n, MORE_SEL_STR *mor_sel_info)
{
}

void draw_bitmap_circle(uint8_t x, uint8_t y, uint8_t st)
{
}
/*
 * ��������draw_more_options_gui
 * ����  ����ʾ��ѡ��Լ���ǰѡ����
 * ����  ��list ��ѡ�������
 *         iteration ��ǰ��ѡ��
 * ���  ����
 * ����  ����
 */
void draw_more_options_gui(MORE_OPTIONS_LIST *list, MORE_OPTIONS_STR *iteration)
{
}

/*
 * ��������gr_vol_sel_gui
 * ����  ��GR ֱ��ѹ��ģʽ�� ѡ���ѹ��λ
 * ����  ��kinds ������λ������ n �ڼ����絵
 * ���  ����
 * ����  ����
 */
void gr_vol_sel_gui(uint8_t options, const uint8_t** opt_buf, uint8_t n)
{
}

/*
 * ��������mode_sel_gui
 * ����  ��ѡ��ģʽ������ʾ�������
 * ����  ��kinds ģʽ������ n λ��
 * ���  ����
 * ����  ����
 */
void mode_sel_gui(uint8_t kinds, const uint8_t** gear_buf, uint8_t n)
{
}
/*
 * ��������dcgr_commuincation_err
 * ����  ��ֱ��gr����ͨ��ʧ����ʾ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void dcgr_commuincation_err(void)
{
}
/*
 * ��������cal_sw_on_warning
 * ����  ��У׼���ش򿪺�ͱ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void cal_sw_on_warning(void)
{
}

void draw_wait_usb_connect_gui(void)
{
}
void draw_auto_calibration_gui(void)
{
}
void draw_no_connect_usb_gui(void)
{
}
/*
 * ��������GUI_Draw_build_connect_to_board
 * ����  ����������ͨ�ŵ���ʾ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_build_connect_to_board(void)
{
}
void my_refresh_lcd24064(void)
{
}
/*
 * ��������GUI_Draw_build_connect_fail
 * ����  ��ͨ������ʧ����ʾ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_build_connect_fail(void)
{
}
/*
 * ��������gui_draw_dc_gr_module_comm_fail
 * ����  ��ֱ��ģ��ͨ���쳣
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_dc_gr_module_comm_fail(void)
{
}

void gui_draw_dc_gr_module_being_in_comm(void)
{
}


/*
 * ��������GUI_Draw_self_check
 * ����  �������Լ�
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_self_check(void)
{
}

/*
 * ��������GUI_Draw_main
 * ����  ������������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_main(void)
{
}

/*
 * ��������MenuTitle
 * ����  �����±���������Ϣ ���ý���ʹ��
 * ����  ��which ģʽ
 * ���  ����
 * ����  ����
 */
void MenuTitle(u8 which)
{
}
/*
 * ��������syn_test_menu_title
 * ����  �����±���������Ϣ ���Խ���ʹ��
 * ����  ��which ģʽ
 * ���  ����
 * ����  ����
 */
void syn_test_menu_title(void)
{
}

/*
 * ��������draw_port_for_set
 * ����  �������˿�������Ϣ ���ý���ʹ��
 * ����  ��which ģʽ
 * ���  ����
 * ����  ����
 */
void draw_port_for_set(TEST_PORT *p)
{
}

/*
 * ��������dis_port
 * ����  �����¶˿�״̬��Ϣ ���Խ���ʹ��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void dis_port(void)
{
}

/*
 * ��������gui_draw_set_sys_cur_limit
 * ����  ������֧·����ʱ˵��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_set_sys_cur_limit(void)
{
}
/*
 * ��������draw_short_cut_bm
 * ����  ���ұ߿�ݼ��� ͼ����ʾ����
 * ����  ��type ���ݲ�ͬ�����;�����ʾ��Щͼ��
 * ���  ����
 * ����  ����
 */
void draw_short_cut_bm(uint8_t type)
{
}

/*
 * ��������draw_short_cut_str
 * ����  ���ұ߿�ݼ��� ������ʾ
 * ����  ��str1 str2 str3 str4 �ֱ����4�������
 * ���  ����
 * ����  ����
 */
void draw_short_cut_str(uint8_t *str1, uint8_t *str2, uint8_t *str3, uint8_t *str4)
{
}

/*
 * ��������draw_menu_frame_line
 * ����  �������˵������ ��������һ��ֱ�� ����ͨ����������
 * ����  ��x1 x2 y ָ������ֱ�ߵ����� ��� x1>x2�����ٻ�������
 * ���  ����
 * ����  ����
 */
void draw_menu_frame_line(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2)
{
}
/*
 * ��������gui_draw_menu_sys
 * ����  ���˵����ý����²˵���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_menu_sys(MENU_INFO* p_sys_menu, uint8_t lon)
{
}

/*
 * ��������gui_draw_menu
 * ����  ����ʾһ��������Ľ����� cs99xx_type.c��
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_menu(void)
{
}

void draw_custom_type_gui(void)
{
}
/*
 * ��������gui_draw_help_1
 * ����  �������˵�1
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_help_1(void)
{
}

/*
 * ��������gui_draw_help_2
 * ����  �������˵�1
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_help_2(void)
{
}

/*
 * ��������gui_draw_help_3
 * ����  �������˵�1
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_help_3 (void)
{
}


/*
 * ��������gui_draw_file_management
 * ����  ���ļ��������
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_file_management(void)
{
}

/*
 * ��������gui_draw_communication_interface
 * ����  ��ͨ�Ž���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_communication_interface(MENU_INFO* pmenu_info)
{
}

/*
 * ��������gui_draw_communication_plc
 * ����  ��PLCͨ�ſ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_communication_plc(void)
{
}
/*
 * ��������gui_draw_communication_agreement
 * ����  ��ͨ�ſ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_communication_agreement(void)
{
}

/*
 * ��������gui_draw_communication_agreement
 * ����  ��ͨ�ſ���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_password_manage(void)
{
}

/*
 * ��������gui_draw_time_manage
 * ����  ��ϵͳʱ�����ý���
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_time_manage(void)
{
}

/*
 * ��������gui_draw_offset
 * ����  ��ƫ�Ʋ��Խ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_offset(void)
{
}
/*
 * ��������gui_draw_bbd
 * ����  ��BBD���Խ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_bbd(void)
{
}

/*
 * ��������gui_draw_test_result
 * ����  �����Խ����ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_test_result(void)
{
}

/*
 * ��������gui_draw_keyboard_lock_manage
 * ����  ����������ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_keyboard_lock_manage(void)
{
}

/*
 * ��������gui_draw_recover
 * ����  ���ָ�Ĭ��ֵ��ʾ����
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_recover(void)
{
}

/*
 * ��������gui_draw_test
 * ����  �����Խ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_draw_test (void)
{
}

/*
 * ��������gui_drow_test_syn
 * ����  ����·ͬ�����Խ�����ʾ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void gui_drow_test_syn (void)
{
}



/*
 * ��������prompt_recover_pw
 * ����  �����û�����ENTER+1��ϼ�ʱ��ʾ�û�ϵͳ�����Ѿ���ԭΪĬ��ֵ �ú�����help_subtask,main_task����
 * ����  ��uint8_t s �����ڲ�ͬ״̬�¸��û���һЩ��ʾ��Ϣ
 * ���  ����
 * ���ú�����GUI_SetFont��GUI_DispStringAt��GUI_DrawLine��GUI_ClearRect��
 *			OSQPend��GUI_DrawBitmap
 * ����������test_subtask
 */
void prompt_recover_pw(void)
{
}

/*
 * ��������dis_program_info
 * ����  ����ʾ��¼�ļ���ʱ����Ϣ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void dis_program_info(void)
{
}

void dis_interface_info(void)
{
}

/*
 * ��������dis_scan_modules_gui
 * ����  ����ʾɨ��ģ�������Ϣ
 * ����  ����
 * ���  ����
 * ����  ����
 */
void dis_scan_modules_gui(void)
{
}

/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
