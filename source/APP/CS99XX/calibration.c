/*
 * Copyright(c) 2014,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�calibration.c
 * ժ  Ҫ  ������У׼����
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */

/******************************* �����ļ� *********************************/
#define CALIBRATE_GLOBALS
#include    <string.h>
#include 	<math.h>
#include 	"GUI_Draw.h"
#include	"keyboard.h"
#include 	"MC14094.h"
#include 	"DAC_VREF.h"
#include	"SPI_CPLD.h"
#include	"ADC_DMA.h"
#include 	"app.h"
#include    "cs99xx_mem_api.h"
#include 	"serve_test.h"
#include	"cs99xx_collect.h"
#include	"cs99xx_relay_motion.h"
#include	"cs99xx_type.h"
#include	"calibration.h"
#include    "dc_module.h"

#define CALIB_JUDGE_ERR(m) if(-1 == c_judge_err(m)) { return;}


RATIO_DC_GR (*ratio_dc_gr)[5] = (RATIO_DC_GR (*)[5])&ratio_gr;

const VOL_CAL acw_vol_cal[][2]=
{
    /* ���Թ��� */
    {
        /* ��һ�ε�ѹ */
        {
            {
                {0},                                                                     //500V
                {{"10V"     ,"0.010kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//1kV
                {{"20V"     ,"0.020kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//5kV
                {{0}},                                                                   //10kV
                {{0}},                                                                   //20kV     
                {{0}},                                                                   //30kV     
            },
            {
                {0},                                               //500V
                {{25    ,10     },{550  ,200    },{1375 ,500    }},//1kV
                {{112   ,20     },{1140 ,200    },{2828 ,500    }},//5kV
                {{0}},                                             //10kV
                {{0}},                                             //20kV
                {{0}},                                             //30kV
            },
        },
        /* �ڶ��ε�ѹ */
        /* 10kV���ϲ��ֶ� */
        {
            {
                {{"50V"     ,"0.050kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//500V
                {{"500V"    ,"0.500kV"}, {"700V"    ,"0.700kV"}, {"1.0kV"   ,"1.000kV"}},//1kV
                {{"500V"    ,"0.500kV"}, {"2.0kV"   ,"2.000kV"}, {"5.0kV"   ,"5.000kV"}},//5kV
                {{"500V"    ,"0.500kV"}, {"5.0kV"   ,"5.000kV"}, {"9.0kV"   ,"9.000kV"}},//10kV
                {{"1000V"    ,"1.000kV"}, {"10kV"   ,"10.000kV"}, {"15kV"   ,"15.000kV"}},//20kV
                {{"3000V"    ,"3.000kV"}, {"10kV"   ,"10.000kV"}, {"15kV"   ,"15.000kV"}},//30kV
            },
            {
                {{240	 ,50    },{1000	 ,200   },{2800 ,500    }},//500V
                {{1400	 ,500   },{2100	 ,700   },{2828 ,1000   }},//1kV
                {{56*5	 ,500   },{1140	 ,2000  },{2828 ,5000   }},//5kV
                {{125    ,500   },{1300  ,5000  },{2500 ,9000   }},//10kV
                {{125    ,1000   },{1300  ,10000  },{1800 ,15000   }},//20kV
                {{250    ,3000   },{850   ,10000  },{1200 ,15000   }},//30kV
            },
        },
    },
    /* PWM���� *//* pwm�����ڱ�ѹ���˷ֶ� */
    {
        /* ��һ�ε�ѹ */
        {0},
        /* �ڶ��ε�ѹ */
        {
            {
                {0},                                                                    //500V
                {{"10V"     ,"0.010kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//1kV
                {{"10V"     ,"0.010kV"}, {"200V"    ,"0.200kV"}, {"500V"    ,"0.500kV"}},//5kv
                {{"500V"    ,"0.500kV"}, {"5.0kV"   ,"5.000kV"}, {"9.0kV"   ,"9.000kV"}},//10kV
                {{"500V"    ," 0.50kV"}, {"5.0kV"   ," 5.00kV"}, {" 20kV"   ,"20.00kV"}},//20kV
            },
            {
                {0},                                               //500V
                {{25	,10     },{550	,200    },{1375 ,500    }},//1kV
                {{56	,10     },{1140	,200    },{2828 ,500    }},//5kV
                {{175	,500    },{1691	,5000   },{2700 ,9000   }},//10kV
                {{175	,500    },{1691	,1000   },{3050 ,2000   }},//20kV
            },
        },
    }
};

const VOL_CAL dcw_vol_cal[][2]=
{
    /* ���Թ��� */
    {
        /* ��һ�ε�ѹ */
        {
            {
                {0},                                                                        //500V
                {{"10V"     , "0.010kV"}, {"200V"    , "0.200kV"}, {"500V"    , "0.500kV"}},//2kV
                {{"10V"     , "0.010kV"}, {"200V"    , "0.200kV"}, {"500V"    , "0.500kV"}},//6kV
                {0},                                                                        //10kV
                {0},                                                                        //20kV
            },
            {
                {0},                                               //500V
                {{56    ,10     },{1270 , 200   },{3700 , 500   }},//2kV
                {{56    ,10     },{1270 , 200   },{3700 , 500   }},//6kV
                {0},                                               //10kV
                {0},                                               //20kV
            },
        },
        /* �ڶ��ε�ѹ */
        /* 10kV���ϲ��ֶ� */
        {
            {
                {{"50V"     , "0.050kV"}, {"200V"    , "0.200kV"}, {"500V"    , "0.500kV"}},//500V
                {{"500V"    , "0.500kV"}, {"1.0kV"   , "1.000kV"}, {"2.0kV"   , "2.000kV"}},//2kV
                {{"500V"    , "0.500kV"}, {"2.0kV"   , "2.000kV"}, {"6.0kV"   , "6.000kV"}},//6kV
                {{"500V"    , "0.500kV"}, {"5.0kV"   , "5.000kV"}, {"9.0kV"   , "9.000kV"}},//10kV
                {{"1000V"	, "1.000kV"}, {"10kV"	 , "10.000kV"}, {"15kV"   ,"15.000kV"}},//20kV
            },
            {
                {{150   ,50     },{600	 ,200   },{1500 , 500   }},//500V
                {{550   ,500    },{1270 , 1000  },{2500 , 2000  }},//2kV
                {{56*5  ,500    },{1270 , 2000  },{3802 , 6000  }},//6kV
                {{139   ,500    },{1582 , 5000  },{2865 , 9000  }},//10kV
                {{175   ,1000   },{1820 , 10000 },{2520 , 15000 }},//20kV
            },
        }
    },
    /* PWM���� *//* pwm�����ڱ�ѹ���˷ֶ� */
    {
        /* ��һ�ε�ѹ */
        {0},
        /* �ڶ��ε�ѹ */
        {
            {
                {0},                                                                      //500V
                {0},                                                                      //2kV
                {{"100V"    , "0.100kV"}, {"2.0kV"  , "2.000kV"}, {"6.0kV"  , "6.000kV"}},//6kV
                {{"500V"    , "0.500kV"}, {"5.0kV"  , "5.000kV"}, {"9.0kV"  , "9.000kV"}},//10kV
                {{"500V"    , " 0.50kV"}, {"5.0kV"  , " 5.00kV"}, {" 20kV"  , "20.00kV"}},//20kV
            },
            {
                {0},                                               //500V
                {0},                                               //2kV
                {{50    ,100    },{1000 , 2000  },{2600 , 6000  }},//6kV
                {{150	,500    },{1300 , 5000  },{2500 , 9000  }},//10kV
                {{56    ,500    },{1270 , 1000  },{3802 , 2000  }},//20kV
            },
        },
    }
};

const VOL_CAL ir_vol_cal[][2]=
{
    /* ���Թ��� */
    {
        /* ��һ�ε�ѹ */
        {
            {
                {{"50V"     , "0.050kV" }, {"200V"  , "0.200kV"}, {"500V"    , "0.500kV"}},//750V
                {{"50V"     , "0.050kV" }, {"200V"  , "0.200kV"}, {"500V"    , "0.500kV"}},/* 1kV */
                {{"500V"    , "0.500kV" }, {"700V"  , "0.700kV"}, {"1.0kV"   , "1.000kV"}},/* 1.5kV */
                {{"50V"     , " 0.050kV"}, {"200V"  , "0.200kV"}, {"500V"    , "0.500kV"}},/* 2.5kV */
                {{"50V"     , " 0.050kV"}, {"200V"  , "0.200kV"}, {"500V"    , "0.500kV"}},/* 5kV */
                {0},                                                                       //10kV
            },
            {
                {{300   , 50     },{1300 , 200   },{3200 , 500   }},//750V
                {{310   , 50     },{1270 , 200   },{3175 , 500   }},//1kV
                {{150	, 500    },{1300 , 5000  },{3000 , 1000  }},//1.5kV
                {{310   , 50     },{1270 , 5000  },{3175 , 500   }},//2.5kV
                {{310   , 50     },{1270 , 5000  },{3175 , 500   }},//5kV
                {0},                                                //10kV
            },
        },
        /* �ڶ��ε�ѹ */
        {
            {
                {{"500V"  , "0.500kV"}, {"700V"     , "0.700kV"}, {"1.0kV" , "1.000kV"}},//750V
                {{"500V"  , "0.500kV"}, {"700V"     , "0.700kV"}, {"1.0kV" , "1.000kV"}},/* 1kV */
                {{0}},                                                                   /* 1.5kV */
                {{"500V"  , "0.500kV"}, {"1.0kV"    , "1.000kV"}, {"2.5kV" , "2.500kV"}},/* 2.5kV */
                {{"500V"  , "0.500kV"}, {"2.0kV"    , "2.000kV"}, {"5.0kV" , "5.000kV"}},/* 5kV */
                {{"500V"  , "0.500kV"}, {"5.0kV"    , "5.000kV"}, {"9.0kV" , "9.000kV"}},//10kV
            },
            {
                {{1500  ,500 },{2100 , 700  },{3000 , 1000  }},//750V
                {{300   ,500 },{430 , 700   },{620  , 1000  }},//1kV
                {0},                                           //1.5kV
                {{300   ,500 },{700 , 1000  },{1500 , 2500  }},//2.5kV
                {{300   ,500 },{1270 , 2000 },{3302 , 5000  }},//5kV
                {{150   ,500 },{1500 , 5000 },{3000 , 9000  }},//10kV
            },
        }
    },
    /* PWM���� *//* pwm�����ڱ�ѹ���˷ֶ� */
    {
        /* ��һ�ε�ѹ */
        {0},
        /* �ڶ��ε�ѹ */
        {
            {
                {0},                                                                     //1kV
                {0},                                                                     //1.5kV
                {0},                                                                     //2.5kV
                {{"500V"    ,"0.500kV"}, {"5.0kV"   ,"5.000kV"}, {"9.0kV"   ,"9.000kV"}},//5kV
                {{"500V"    ,"0.500kV"}, {"5.0kV"   ,"5.000kV"}, {"9.0kV"   ,"9.000kV"}},//10kV
                {{"500V"    ," 0.50kV"}, {"5.0kV"   ," 5.00kV"}, {" 20kV"   ,"20.00kV"}},//20kV
            },
            {
                {0},                                               //1kV
                {0},                                               //1.5kV
                {0},                                               //2.5kV
                {{150	,500    },{1300 , 5000  },{3000 , 9000  }},//5kV
                {{150	,500    },{1300 , 5000  },{3000 , 9000  }},//10kV
                {{56    ,500    },{1270 , 1000  },{3802 , 2000  }},//20kV
            },
        },
    }
};

DATA_INFO par_info =
{
	(void*)0/*pData*/,0/2+10/*x*/,0/*y*/,1/*type*/,5/*lon*/,3/*decs*/,0/*total*/,1/*cur*/,0/*ch_bit*/,0/*page*/,1/*bits*/,
};


const float default_ir_res_k[]={
1.8,18.0,180.0,1800.0,16000
};

const RES_CAL ir_cur_cal=
{
    {
        /* 1.5kV ����10G */
        {
            "1M��"    , "3M��"  , "5M��",
            "8M��"    , "20M��" , "50M��",
            "80M��"   , "200M��", "500M��",
            "800M��"  , "2G��"  , "5G��",
            "8G��"    ,
        },
        /* 1.5kV ����100G */
        {
            "1M��"    , "3M��"  , "5M��",
            "8M��"    , "20M��" , "50M��",
            "80M��"   , "200M��", "500M��",
            "800M��"  , "2G��"  , "5G��",
            "8G��"    , "30G��" , "60G��",
            "80G��"   ,
        },
        /* 5kV ����100G */
        {
            "5M��"  , "7M��"  , "9M��",
            "30M��" , "60M��" , "90M��",
            "300M��", "600M��", "900M��",
            "3G��"  , "6G��"  , "9G��",
            "30G��" , "50G��" , "70G��",
            "90G��" ,
        },
        /* 10kV ����100G */
        {
            "600M��", "1G��"    , "3G��",
            "6G��"  , "10G��"   , "30G��",
            "60G��" , "70G��"   , "90G��",
        },
        /* 1.5kV ����100G */
        {
            "2M��",
            "20M��",
            "200M��",
            "2G��",
            "10G��",
            "10G��",
            "50G��",
        },
    },
    {
        /* 1.5kV ����10G */
        {
            1  , 3   , 5,
            8  , 20  , 50,
            80 , 200 , 500,
            800, 2  * 1000, 5 * 1000, 8 * 1000, 
        },
        /* 1.5kV ����100G */
        {
            1  , 3   , 5,
            8  , 20  , 50,
            80 , 200 , 500,
            800, 2  * 1000, 5 * 1000,
            8 * 1000, 30 * 1000, 60 * 1000, 80 * 1000,
        },
        /* 5kV ����100G */
        {
            5   , 7   , 9,
            30  , 60  , 90,
            300 , 600 , 900,
            3  * 1000, 6  * 1000, 9 * 1000,
            30 * 1000, 50 * 1000, 70 * 1000, 90 * 1000,
        },
        /* 10kV ����100G */
        {
            600        , 1 * 1000    , 3  * 1000,
            6  * 1000  , 10 * 1000   , 30 * 1000,
            60 * 1000  , 70 * 1000   , 90 * 1000,
        },
        /* 1.5kV ����100G */
        {
            2,
            20,
            200,
            2  * 1000,
            10 * 1000,
            10 * 1000,
            50 * 1000,
        },
    },
/* Ӳ����λ:  1       2        3       4        5  */
/* ��������: 1.5k     15k     150k    1.5M     15M */
    {
        /* 1.5kV ����10G */
        {
            1,1,1,
            2,2,2,
            3,3,3,
            4,4,4,
            4,
        },
        {
            1,1,1,
            2,2,2,
            3,3,3,
            4,4,4,
            5,5,5,
            5,
        },
        {
            1,1,1,
            2,2,2,
            3,3,3,
            4,4,4,
            5,5,5,
            5,
        },
        {
            3,3,3,
            4,4,4,
            5,5,5,
        },
        {
            1,
            2,
            3,
            4, 4,
            5, 5,
        },
    },
};



const CUR_CAL gr_cur_cal[]=
{
    /* 0 ���ڽ��� GR */
    {
        {
            {{"3A"}, {"15A"}, {"30A"}},
            {{"3A"}, {"20A"}, {"40A"}},
        },
        {
            #define GR_CAL_BASE
            #define GR_CAL_RES_POINT    100.0 /* 100mohm */
            
            {{200  , GR_CAL_RES_POINT},{1000 , GR_CAL_RES_POINT},{2000 , GR_CAL_RES_POINT}},
            {{200  , GR_CAL_RES_POINT},{1300 , GR_CAL_RES_POINT},{2700 , GR_CAL_RES_POINT}},
            
            #undef GR_CAL_BASE
            #undef GR_CAL_RES_POINT
        },
    },
    
    /* 1 ����ֱ�� GR */
    {
        {
            {{"3A"}, {"20A"}, {"40A"}},/* ����ֱ�� GR */
        },
        {
            #define GR_CAL_BASE
            #define GR_CAL_RES_POINT    100.0 /* 100mohm */
            
            {{370  , GR_CAL_RES_POINT},{2000 , GR_CAL_RES_POINT},{4000 , GR_CAL_RES_POINT}},
            {{220  , GR_CAL_RES_POINT},{1500 , GR_CAL_RES_POINT},{2248 , GR_CAL_RES_POINT}},
            
            #undef GR_CAL_BASE
            #undef GR_CAL_RES_POINT
        },
    },
    
    /*****************************************************************************/
    /* 2 ���� 20mV */
    {
        {
            {{"100mA"}, {"300mA"}, {"500mA"}},/* ����ֱ��ѹ������ */
        },
        {
            #define DV_CAL_BASE 690
            #define DV_CAL_RES_POINT 10.0 /* 10ohm */
            
            {{DV_CAL_BASE	, DV_CAL_RES_POINT},{3 * DV_CAL_BASE	, DV_CAL_RES_POINT},{5 * DV_CAL_BASE , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
    /* 3 ���� 200mV */
    {
        {
            {{"100mA"}, {"300mA"}, {"500mA"}},/* ����ֱ��ѹ������ */
        },
        {
            #define DV_CAL_BASE 690
            #define DV_CAL_RES_POINT 10.0 /* 10ohm */
            
            {{DV_CAL_BASE	, DV_CAL_RES_POINT},{3 * DV_CAL_BASE	, DV_CAL_RES_POINT},{5 * DV_CAL_BASE , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
    /* 4 ���� 2000mV */
    {
        {
            {{"100mA"}, {"300mA"}, {"500mA"}},/* ����ֱ��ѹ������ */
        },
        {
            #define DV_CAL_BASE 690
            #define DV_CAL_RES_POINT 10.0 /* 10ohm */
            
            {{DV_CAL_BASE	, DV_CAL_RES_POINT},{3 * DV_CAL_BASE	, DV_CAL_RES_POINT},{5 * DV_CAL_BASE , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
    /***********************************ѹ��������*************************************************/
    
    /* 5 ����ֱ�� DV ���ֶ� */
    {
        {
            {{"100mA"}, {"300mA"}, {"500mA"}},/* ����ֱ��ѹ������ */
        },
        {
            #define DV_CAL_BASE 690
            #define DV_CAL_RES_POINT 10.0 /* 10ohm */
            
            {{DV_CAL_BASE	, DV_CAL_RES_POINT},{3 * DV_CAL_BASE	, DV_CAL_RES_POINT},{5 * DV_CAL_BASE , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
/***********************************************************************************************/
    /* �����Ƿֶ�У׼ ����ֱ��ѹ������ */
    
    /* 6 ��һ�ε��� */
    {
        {
            {{"10mA"}, {"50mA"}, {"90mA"}},
        },
        {
            #define DV_CAL_BASE         380
            #define DV_CAL_RES_POINT    100.0 /* 100ohm */
            
            {{DV_CAL_BASE * 1  , DV_CAL_RES_POINT},{DV_CAL_BASE * 5 , DV_CAL_RES_POINT},{DV_CAL_BASE * 9 , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    },
    /* 7 �ڶ��ε��� */
    {
        {
            {{"100mA"}, {"500mA"}, {"900mA"}},
        },
        {
            #define DV_CAL_BASE         380
            #define DV_CAL_RES_POINT    10.0 /* 10ohm */
            
            {{DV_CAL_BASE * 1  , DV_CAL_RES_POINT},{DV_CAL_BASE * 5 , DV_CAL_RES_POINT},{DV_CAL_BASE * 9 , DV_CAL_RES_POINT}},
            
            #undef DV_CAL_BASE
            #undef DV_CAL_RES_POINT
        },
    }
};
void confirm_vol_segment_s(void)
{
    uint32_t temp_h = type_spe.vol_segment_point;
    
    if(type_spe.vol_is_segment == VOL_SEGMENT_NO)
    {
        vol_segment = 1;
    }
    else
    {
        if(cur_vol > temp_h)
        {
            vol_segment = 1;
        }
        else
        {
            vol_segment = 0;
        }
    }
}
uint8_t is_acw_segment(void)
{
    uint8_t res1 = 0;
    uint8_t res2 = 0;
    
    res1 = (acw_vol_cal[type_spe.amp_type][0].point_menu[type_spe.acw_vol_range][0][0] != NULL); /* �жϵ�ѹ��1�Ƿ���� */
    res2 = (acw_vol_cal[type_spe.amp_type][1].point_menu[type_spe.acw_vol_range][0][0] != NULL); /* �жϵ�ѹ��2�Ƿ���� */
    
    return (res1 + res2);
}
uint8_t is_dcw_segment(void)
{
    uint8_t res1 = 0;
    uint8_t res2 = 0;
    
    res1 = (dcw_vol_cal[type_spe.amp_type][0].point_menu[type_spe.dcw_vol_range][0][0] != NULL); /* �жϵ�ѹ��1�Ƿ���� */
    res2 = (dcw_vol_cal[type_spe.amp_type][1].point_menu[type_spe.dcw_vol_range][0][0] != NULL); /* �жϵ�ѹ��2�Ƿ���� */
    
    return (res1 + res2);
}
uint8_t is_ir_segment(void)
{
    uint8_t res1 = 0;
    uint8_t res2 = 0;
    
    res1 = (ir_vol_cal[type_spe.amp_type][0].point_menu[type_spe.ir_vol_range][0][0] != NULL); /* �жϵ�ѹ��1�Ƿ���� */
    res2 = (ir_vol_cal[type_spe.amp_type][1].point_menu[type_spe.ir_vol_range][0][0] != NULL); /* �жϵ�ѹ��2�Ƿ���� */
    
    return (res1 + res2);
}
void init_ratio(uint8_t mode)
{
	if(mode == ACW)
	/* ACW */
	{
	ratio_acw.dac_k[0] = 0.5611042 * 10;/* dac��ѹϵ�� */
	ratio_acw.dac_b[0] = -3.072428;
	ratio_acw.adc_v_k[0] = 1.78737;/* ��ѹϵ�� */
	ratio_acw.adc_v_b[0] = 6.882162;
    
	ratio_acw.dac_k[1] = 0.5611042;/* dac��ѹϵ�� */
	ratio_acw.dac_b[1] = -3.072428;
	ratio_acw.adc_v_k[1] = 1.78737;/* ��ѹϵ�� */
	ratio_acw.adc_v_b[1] = 6.882162;
	
	ratio_acw.adc_cur_k[AC_200uA] = 0.6099935;/* 200uA����ϵ�� */
	ratio_acw.adc_cur_k[AC_2mA]   = 1.179548;/* 2mA����ϵ�� */
	ratio_acw.adc_cur_k[AC_10mA]  = 0.593058;/* 10mA����ϵ�� */
	ratio_acw.adc_cur_k[AC_20mA]  = 0.593058;/* 20mA����ϵ�� */
	ratio_acw.adc_cur_k[AC_50mA]  = 0.593058;/* 50mA����ϵ�� */
	ratio_acw.adc_cur_k[AC_100mA] = 0.593058;/* 100mA����ϵ�� */
	ratio_acw.adc_cur_k[AC_200mA] = 0.593058;/* 200mA����ϵ�� */
	
	ratio_acw.arc_k = 1.0;/* �绡ϵ�� */
	ratio_acw.adc_r_k = 1.0;/* ��ʵ����ϵ�� */
	ratio_acw.adc_r_b = 0.0;
	}
	/* DCW */
    else if(mode == DCW)
	{
	ratio_dcw.dac_k[0] = 0.6336942 * 10;/* dac��ѹϵ�� */
	ratio_dcw.dac_b[0] = 7.366048;
	ratio_dcw.adc_v_k[0] = 1.58793;/* ��ѹϵ�� */
	ratio_dcw.adc_v_b[0] = -12.26546;
    
	ratio_dcw.dac_k[1] = 0.6336942;/* dac��ѹϵ�� */
	ratio_dcw.dac_b[1] = 7.366048;
	ratio_dcw.adc_v_k[1] = 1.58793;/* ��ѹϵ�� */
	ratio_dcw.adc_v_b[1] = -12.26546;
    
	ratio_dcw.adc_cur_k[DC_2uA] = 1.0;
	ratio_dcw.adc_cur_k[DC_20uA] = 1.0;
	ratio_dcw.adc_cur_k[DC_200uA] = 1.0;
	ratio_dcw.adc_cur_k[DC_2mA] = 0.5377155;/* 2mA����ϵ�� */
	ratio_dcw.adc_cur_k[DC_10mA] = 0.5382956;/* 10mA����ϵ�� */
	ratio_dcw.adc_cur_k[DC_20mA] = 0.5382956;/* 20mA����ϵ�� */
	ratio_dcw.adc_cur_k[DC_50mA] = 0.5382956;/* 50mA����ϵ�� */
	ratio_dcw.adc_cur_k[DC_100mA] = 0.5382956;/* 100mA����ϵ�� */
	
	ratio_dcw.arc_k = 1.0;/* �绡ϵ�� */
	ratio_dcw.adc_a_b = 0.0;
	ratio_dcw.adc_r_k = 1.0;/* ��ʵ����ϵ�� */
	ratio_dcw.adc_r_b = 0.0;
	}
	/* 	IR */
    else if(mode == IR)
    {
        int32_t i = 0;
        int32_t j = 0;
        
	ratio_ir.dac_k[0] = 0.6336942 * 10;/* dac��ѹϵ�� */
	ratio_ir.dac_b[0] = 7.366048;
	ratio_ir.adc_v_k[0] = 1.58793;/* ��ѹϵ�� */
	ratio_ir.adc_v_b[0] = -12.26546;
    
	ratio_ir.dac_k[1] = 0.6336942;/* dac��ѹϵ�� */
	ratio_ir.dac_b[1] = 7.366048;
	ratio_ir.adc_v_k[1] = 1.58793;/* ��ѹϵ�� */
	ratio_ir.adc_v_b[1] = -12.26546;
    
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < SECT_VOL; j++)
        {
            ratio_ir.res_k[i][j] = default_ir_res_k[0];
        }
    }
    
    for(i = 3; i < 6; i++)
    {
        for(j = 0; j < SECT_VOL; j++)
        {
            ratio_ir.res_k[i][j] = default_ir_res_k[1];
        }
    }
    
    for(i = 6; i < 9; i++)
    {
        for(j = 0; j < SECT_VOL; j++)
        {
            ratio_ir.res_k[i][j] = default_ir_res_k[2];
        }
    }
    
    for(i = 9; i < 12; i++)
    {
        for(j = 0; j < SECT_VOL; j++)
        {
            ratio_ir.res_k[i][j] = default_ir_res_k[3];
        }
    }
    
    for(i = 12; i < CAL_POINTS; i++)
    {
        for(j = 0; j < SECT_VOL; j++)
        {
            ratio_ir.res_k[i][j] = default_ir_res_k[4];
        }
    }
    }
	/* GR DC */
    else if(mode == GR)
	{
		if(DC_GR_EN)
		{
        ratio_gr.dac_k[0]   = 0.9819072;/* dac��ѹϵ�� */
        ratio_gr.dac_b[0]   = 5.779623;
        ratio_gr.adc_v_k[0] = 1.976271;/* ��ѹϵ�� */
        ratio_gr.adc_v_b[0] = -7.708008;
        ratio_gr.adc_i_k[0] = 0.01021449;/* ����ϵ�� */
        ratio_gr.adc_i_b[0] = -0.1292979;
        
        ratio_gr.dac_k[1]   = 0.9819072;/* dac��ѹϵ�� */
        ratio_gr.dac_b[1]   = 5.779623;
        ratio_gr.adc_v_k[1] = 1.976271;/* ��ѹϵ�� */
        ratio_gr.adc_v_b[1] = -7.708008;
        ratio_gr.adc_i_k[1] = 0.01021449;/* ����ϵ�� */
        ratio_gr.adc_i_b[1] = -0.1292979;
        
        ratio_gr.dac_k[2]   = 0.9819072;/* dac��ѹϵ�� */
        ratio_gr.dac_b[2]   = 5.779623;
        ratio_gr.adc_v_k[2] = 1.976271;/* ��ѹϵ�� */
        ratio_gr.adc_v_b[2] = -7.708008;
        ratio_gr.adc_i_k[2] = 0.01021449;/* ����ϵ�� */
        ratio_gr.adc_i_b[2] = -0.1292979;
        
        ratio_gr.dac_k[3]   = 0.9819072;/* dac��ѹϵ�� */
        ratio_gr.dac_b[3]   = 5.779623;
        ratio_gr.adc_v_k[3] = 1.976271;/* ��ѹϵ�� */
        ratio_gr.adc_v_b[3] = -7.708008;
        ratio_gr.adc_i_k[3] = 0.01021449;/* ����ϵ�� */
        ratio_gr.adc_i_b[3] = -0.1292979;
        
        ratio_gr.dac_k[4]   = 1;/* dac��ѹϵ�� */
        ratio_gr.dac_b[4]   = 0;
        ratio_gr.adc_v_k[4] = 1;/* ��ѹϵ�� */
        ratio_gr.adc_v_b[4] = 0;
        ratio_gr.adc_i_k[4] = 1;/* ����ϵ�� */
        ratio_gr.adc_i_b[4] = 0;
        
        ratio_gr.dac_k[5]   = 3.745226;/* dac��ѹϵ�� */
        ratio_gr.dac_b[5]   = -2.949219;
        ratio_gr.adc_v_k[5] = 4.800114;/* ��ѹϵ�� */
        ratio_gr.adc_v_b[5] = 26.48014;
        ratio_gr.adc_i_k[5] = 0.02677111;/* ����ϵ�� */
        ratio_gr.adc_i_b[5] = -0.1443431;
        
        ratio_gr.dac_k[6]   = 0.3643337;/* dac��ѹϵ�� */
        ratio_gr.dac_b[6]   = -4.97933;
        ratio_gr.adc_v_k[6] = 4.80092;/* ��ѹϵ�� */
        ratio_gr.adc_v_b[6] = 24.46875;
        ratio_gr.adc_i_k[6] = 0.2751979;/* ����ϵ�� */
        ratio_gr.adc_i_b[6] = -0.8348796;
		}
		/* GR AC */
		else
		{
        ratio_gr.dac_k[0]   = 1.0;/* dac��ѹϵ�� */
        ratio_gr.dac_b[0]   = 0;
        ratio_gr.adc_v_k[0] = 1.0;/* ��ѹϵ�� */
        ratio_gr.adc_v_b[0] = 0;
        ratio_gr.adc_i_k[0] = 1.0;/* ����ϵ�� */
        ratio_gr.adc_i_b[0] = 0;
		}
	}
}


void init_ratio_all(void)
{
    init_ratio(ACW);
    init_ratio(DCW);
    init_ratio(IR);
    init_ratio(GR);
}

uint8_t get_ir_res_meal(void)
{
    uint8_t temp = 0;
    
    switch(type_spe.transformer_type)
    {
        default:
        case TRANS_750V:
        case TRANS_1kV:
        case TRANS_2kV:
            temp = 0;
            break;
        case TRANS_10kV:
            switch(type_spe.ir_vol_range)
            {
                case IR_1kV:
                case IR_1_5kV:
                    if(type_spe.ir_res_h <= 10*1000)
                    {
                        temp = 0;
                    }
                    else
                    {
                        temp = 1;
                    }
                    break;
                case IR_2_5kV:
                    temp = 2;
                    break;
                case IR_5kV:
                    temp = 2;
                    break;
                case IR_10kV:
                    temp = 3;
                    break;
            }
            break;
        case TRANS_20kV:
            break;
        case TRANS_5kV:
            switch(type_spe.ir_vol_range)
            {
                case IR_1kV:
                case IR_1_5kV:
                    if(type_spe.ir_res_h <= 10*1000) //��λGohm
                    {
                        temp = 0;
                    }
                    else
                    {
                        temp = 1;
                    }
                    break;
                case IR_2_5kV:
                    temp = 2;
                    break;
                case IR_5kV:
                    temp = 2;
                    break;
            }
            break;
    }
    
    /* ����ʹ�� */
    if(g_cur_type->num == DEBUG_TYPE)
    {
        temp = 4;
    }
    
    return temp;
}

uint8_t get_cal_dcw_vol_meal(void)
{
    return type_spe.dcw_vol_range;
}
uint8_t get_cal_acw_vol_meal(void)
{
    return type_spe.acw_vol_range;
}
uint8_t select_gr_cal_menu(void)
{
    uint8_t temp = 0;
    
    if(GR_VOL_DROP_EN > 0)
    {
        temp = 0;
        cur_method = GR_V_CONT_MODEL;/* У׼ʱ������������ģʽ */
        send_dc_module_cmd(DC_MODULE_CMD_GEAR, cur_gear);
        send_dc_module_cmd(DC_MODULE_CMD_METHOD, cur_method);
    }
    else
    {
        /* ֱ�� GR */
        if(DC_GR_EN == ENABLE)
        {
            temp = 0;
        }
        /* ���� GR */
        else
        {
            switch(type_spe.gr_amp_type)
            {
                default:
                case GR_AMP_32A:
                    temp = 0;
                    break;
                case GR_AMP_40A:
                    temp = 1;
                    break;
            }
        }
    }
    
    return temp;
}
uint8_t geat_ir_res_cal_points_num(void)
{
    uint8_t i = 0;
    uint8_t meal = get_ir_res_meal();
    
    for(i = 0; i < 16; i++)
    {
        if(0 == ir_cur_cal.point_da[meal][i])
        {
            break;
        }
    }
    
    return i;
}

void get_acw_vol_cal_point_range(uint32_t *range, uint8_t *name_pool[])
{
	uint32_t i = 0;
	uint32_t b = 0;
	
	if(type_spe.vol_is_segment == VOL_SEGMENT_YES)
	{
		for(i = 0; i < 3; i++)
		{
			range[i] = acw_vol_cal[type_spe.amp_type][0].point_da[type_spe.acw_vol_range][i][1];
            name_pool[i] = acw_vol_cal[type_spe.amp_type][0].point_menu[type_spe.acw_vol_range][i][1];
		}
		
		b = 3;
	}
	
	for(i = 0; i < 3; i++)
	{
		range[b + i] = acw_vol_cal[type_spe.amp_type][1].point_da[type_spe.acw_vol_range][i][1];
        name_pool[b + i] = acw_vol_cal[type_spe.amp_type][0].point_menu[type_spe.acw_vol_range][i][1];
	}
}

void get_dcw_vol_cal_point_range(uint32_t *range, uint8_t *name_pool[])
{
	uint32_t i = 0;
	uint32_t b = 0;
	
	if(type_spe.vol_is_segment == VOL_SEGMENT_YES)
	{
		for(i = 0; i < 3; i++)
		{
			range[i] = dcw_vol_cal[type_spe.amp_type][0].point_da[type_spe.dcw_vol_range][b + i][1];
		}
		
		b = 3;
	}
	
	for(i = 0; i < 3; i++)
	{
		range[b + i] = dcw_vol_cal[type_spe.amp_type][1].point_da[type_spe.dcw_vol_range][i][1];
	}
}
void get_ir_vol_cal_point_range(uint32_t *range, uint8_t *name_pool[])
{
	uint32_t i = 0;
	uint32_t b = 0;
	
    cur_set_meal = get_cal_ir_meal();//��ѹ
	
	if(type_spe.vol_is_segment == VOL_SEGMENT_YES)
	{
		for(i = 0; i < 3; i++)
		{
			range[i] = ir_vol_cal[type_spe.amp_type][0].point_da[cur_set_meal][b + i][1];
		}
		
		b = 3;
	}
	
	for(i = 0; i < 3; i++)
	{
		range[b + i] = ir_vol_cal[type_spe.amp_type][1].point_da[cur_set_meal][i][1];
	}
}

uint8_t get_cal_ir_meal(void)
{
    uint8_t temp = 0;
    
    if(type_spe.transformer_type == TRANS_10kV)
    {
        temp = IR_10kV;
    }
    else
    {
		switch(type_spe.transformer_type)
		{
			case TRANS_5kV:
				switch(type_spe.ir_vol_range)
				{
					case IR_2_5kV:
					case IR_5kV:
						temp = type_spe.ir_vol_range;
						break;
					default:
						temp = IR_1kV;
						break;
				}
				break;
			default:
				temp = type_spe.ir_vol_range;
				break;
		}
    }
    
    return temp;
}
// /*
//  * ��������startup_cal
//  * ����  ������У׼
//  * ����  ��mode ģʽ gear ������λ
//  * ���  ����
//  * ����  ����
//  */
void startup_cal(const int8_t mode, const int8_t gear) 
{
	/* �л���λ�̵��� */
	cur_mode = mode;
	cur_gear = gear;
    clear_test_flag();
    clear_dc_gr_data();
	relay_motion();
    reset_posrts_mc14094_ctrl();
	
    if(!(mode == GR && DC_GR_EN))
    {
		amp_relay_ctrl_on();/* ���ż̵��� */
	}
	MC14094_Updata();// �������������
    OSTimeDlyHMSM(0,0,0,50);
    
    relay_ready();
	
	MC14094_CMD(MC14094_A, MC14094_PLC_TEST, 0);/* �ڸ�λ״̬��PLC�������̵����������� */
	MC14094_CMD(MC14094_A, MC14094_PLC_FAIL, 0);
	MC14094_CMD(MC14094_A, MC14094_PLC_PASS, 0);
	
	/* ����ģʽѡ�� */
	if(sys_par.test_method == GND_MODE)
	{
		MC14094_CMD(MC14094_B, MC14094_GFI_RELAY, 1); /* k8 */
		MC14094_CMD(MC14094_C, MC14094_C_GND_FLOAT, 1);/* �ӵ�ģʽ */
	}
	else
	{
		MC14094_CMD(MC14094_B, MC14094_GFI_RELAY, 0); /* k8 */
		MC14094_CMD(MC14094_C, MC14094_C_GND_FLOAT, 0);/* ����ģʽ */
	}
	
	if(mode == GR)
	{
		MC14094_CMD(MC14094_C, MC14094_C_GR, 1);/* GR */
	}
	else
	{
		MC14094_CMD(MC14094_C, MC14094_C_GR, 0);/* ACW DCW IR */
	}
	
	switch(mode)
	{
		case ACW:
		case GR:
		case BBD:
			open_sine(60);/* 60HZ */
			break;
		case DCW:
			if(type_spe.dcw_big_cap_en)
			{
				dcw_big_cap_cal();
			}
		case IR:
			MC14094_CMD(MC14094_B, MC14094_AC_DC, 1);
			open_sine(400);/* 400HZ */
			break;
	}
	
	MC14094_Updata();// �������������
	
    set_sample_rate(50);
    
    if(mode == GR && DC_GR_EN)
    {
        off_sample_task();
        test_flag.allow_dc_gr_fetch = 1;
        test_flag.uart_next = 0;
		OSTimeDlyHMSM(0,0,1,0);
		amp_relay_ctrl_on();/* ���ż̵��� */
    }
    else
    {
        on_sample_task();
        resume_sample_task();
    }
	
}

// /*
//  * ��������stop_cal
//  * ����  ��ֹͣУ׼
//  * ����  ��mode ģʽ
//  * ���  ����
//  * ����  ����
//  */
void stop_cal(const int8_t mode)
{
	app_flag.dc_gr_module_status = DC_GR_STOPING;
	DAC_Vol[0] = 0;
	DAC_Vol[1] = SHORT_VREF_RESET;
	DAC_Vol[2] = ARC_VREF_RESET;
    disable_sample_task();
    MC14094_CMD(MC14094_C, MC14094_C_GR, 0);/* ACW DCW IR */
	irq_stop_relay_motion();/* �رյ��ӿ��� ��ѹ */
    
	if(mode == GR)
	{
		/* ֱ�� */
		if(DC_GR_EN)
		{
			/* ֹͣ���� */
            send_dc_module_cmd(DC_MODULE_CMD_STOP, 0);
			return;
		}
	}
    
	key_buzzer_time = KEY_DELAY_NORMAL;
    
	LED_TEST = LED_OFF;
}


/******************* (C) COPYRIGHT 2014 ��ʢ���� *****END OF FILE****/
