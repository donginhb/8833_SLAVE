/*
 * FreeModbus Libary: STM32 Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer.c,v 1.60 2013/08/13 15:07:05 Armink $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

#include "BSP_config.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
	uint16_t PrescalerValue = 0;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//====================================ʱ�ӳ�ʼ��===========================
	//ʹ�ܶ�ʱ��4ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//====================================��ʱ����ʼ��===========================
	//��ʱ��ʱ�������˵��
	//HCLKΪ72MHz��APB1����2��ƵΪ36MHz
	//TIM3��ʱ�ӱ�Ƶ��Ϊ72MHz��Ӳ���Զ���Ƶ,�ﵽ���
	//TIM3�ķ�Ƶϵ��Ϊ3599��ʱ���Ƶ��Ϊ72 / (1 + Prescaler) = 20KHz,��׼Ϊ50us
	//TIM������ֵΪusTim1Timerout50u
	
	PrescalerValue = (uint16_t) (SystemCoreClock / 20000) - 1;
	//��ʱ��1��ʼ��
	TIM_TimeBaseStructure.TIM_Period = (uint16_t) usTim1Timerout50us;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//Ԥװ��ʹ��
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	//====================================�жϳ�ʼ��===========================
	//����NVIC���ȼ�����ΪGroup2��0-3��ռʽ���ȼ���0-3����Ӧʽ���ȼ�
// 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM4_IT_PRE_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM4_IT_SUB_PRIO;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//�������жϱ�־λ
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	//��ʱ��4����жϹر�
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	//��ʱ��4����
	TIM_Cmd(TIM4, DISABLE);
	return TRUE;
}

void vMBPortTimersEnable()
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, ENABLE);
}

void vMBPortTimersDisable()
{
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	TIM_SetCounter(TIM4, 0);
	TIM_Cmd(TIM4, DISABLE);
}

void prvvTIMERExpiredISR(void)
{
	(void) pxMBPortCBTimerExpired();
}

void TIM4_IRQHandler(void)
{
	OSIntEnter();
    
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		
		TIM_ClearFlag(TIM4, TIM_FLAG_Update);	     //���жϱ��
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);	 //�����ʱ��T3����жϱ�־λ
		prvvTIMERExpiredISR();
	}
    
	OSIntExit();
}