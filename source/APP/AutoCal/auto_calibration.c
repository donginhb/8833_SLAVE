/*
 * Copyright(c) 2016,�Ͼ���ʢ����
 * All rights reserved
 * �ļ����ƣ�auto_calibration.c
 * ժ  Ҫ  ���Զ�У׼ϵͳ
 * ��ǰ�汾��V0.0����д�ߣ�����
 * ��ʷ�汾��
 * �޸ļ�¼��
 *
 */
#define AUTO_CALIBRATE_GLOBALS

#include "stm32f10x.h"
#include "GUI_Draw.h"
#include "auto_calibration.h"
#include "calibration.h"
#include "serve_test.h"
#include "cs99xx_type.h"
#include "cs99xx_mem_api.h"
#include "DAC_VREF.h"
#include "app.h"
#include "keyboard.h"
#include "auto_cal_server.h"
#include <string.h>
#include "auto_cal_comm.h"
#include "MC14094.h"
#include "dc_module.h"
#include "cs99xx_collect.h"


