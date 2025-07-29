
#include "delay.h"
#include "sys.h"


/** 
 * ����freeRTOS���Զ���ʼ��systick������һ����Բ�����
 *
 **/
void delay_init(void)
{
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //����systickʱ��Դ
		HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq)); // ����systick�ж�Ϊ1ms
}


void delay_us(u32 us)
{
	
	u32 tick_old=0,tick_now=0,tick_cnt=0;
  u32 need_ticks=0; // ÿus����Ҫ����Ticks
	u32 auto_reload=0; //SysTick��װ��ֵ
	
	auto_reload = SysTick->LOAD; // ��ȡ��װ��ֵ
	
// SysTick��ʱ��������ϵͳ��Ƶ���������õ�100MHZ��1��tick(1/100MHZ) = 1us / 100������1us = 100 tick
	u32 ticks_per_us = SystemCoreClock / 1000000;//ÿ1us����Ҫ��tick��Ϊ100
	need_ticks = us * ticks_per_us; //��ʱָ��us��Ҫ����Tick��
	
	tick_old = SysTick->VAL; // ��ʼ��ʱ��SysTick�ļ�����ֵ
	
	while(tick_cnt < need_ticks) //��ÿ��ѭ�����߹���tick�ۼӵ�tick_cnt�У�ֱ��tick_cnt������Ҫ��ָ��us��tick��
	{
		tick_now = SysTick->VAL; // ���ڵ�SysTick�ļ���ֵ
		
		if(tick_now != tick_old)
		{
			 if(tick_now<tick_old) // ������δ��װ��
				  tick_cnt += tick_old - tick_now;
			 else  // �����������װ��ʱ�߹���tick
					tick_cnt = tick_old + (auto_reload - tick_now);
			 
			 tick_old = tick_now;
		}
	}
	/*SysTick �� tick ÿ����һ��ʱ�����ھͼ���һ�Σ�������ƵΪ 100MHz ʱ��SysTick ÿ 0.01 ΢���һ������
	 *�� while ѭ����ִ��һ�Σ�CPU ��Ҫ�������ָ����ȡ�Ĵ������жϡ��ۼӵȣ�����ͨ��Ҫ���ļ�ʮ�����ϰٸ�ʱ������(tick)��*/
}



void delay_ms(u32 ms)
{
	for(u32 i=0;i<ms;i++)
	{
		delay_us(1000);
	}
	
}