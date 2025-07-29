
#include "delay.h"
#include "sys.h"


/** 
 * 由于freeRTOS会自动初始化systick，所以一般可以不调用
 *
 **/
void delay_init(void)
{
		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK); //设置systick时钟源
		HAL_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq)); // 设置systick中断为1ms
}


void delay_us(u32 us)
{
	
	u32 tick_old=0,tick_now=0,tick_cnt=0;
  u32 need_ticks=0; // 每us所需要的总Ticks
	u32 auto_reload=0; //SysTick重装载值
	
	auto_reload = SysTick->LOAD; // 获取重装载值
	
// SysTick的时钟来自于系统主频，我们设置的100MHZ，1个tick(1/100MHZ) = 1us / 100，所以1us = 100 tick
	u32 ticks_per_us = SystemCoreClock / 1000000;//每1us所需要的tick数为100
	need_ticks = us * ticks_per_us; //延时指定us需要的总Tick数
	
	tick_old = SysTick->VAL; // 开始计时的SysTick的计数器值
	
	while(tick_cnt < need_ticks) //将每次循环将走过的tick累加到tick_cnt中，直到tick_cnt等于需要的指定us的tick数
	{
		tick_now = SysTick->VAL; // 现在的SysTick的计数值
		
		if(tick_now != tick_old)
		{
			 if(tick_now<tick_old) // 计数器未重装载
				  tick_cnt += tick_old - tick_now;
			 else  // 处理计数器重装载时走过的tick
					tick_cnt = tick_old + (auto_reload - tick_now);
			 
			 tick_old = tick_now;
		}
	}
	/*SysTick 的 tick 每经过一个时钟周期就计数一次，比如主频为 100MHz 时，SysTick 每 0.01 微秒计一次数。
	 *而 while 循环体执行一次，CPU 需要处理多条指令（如读取寄存器、判断、累加等），这通常要消耗几十甚至上百个时钟周期(tick)。*/
}



void delay_ms(u32 ms)
{
	for(u32 i=0;i<ms;i++)
	{
		delay_us(1000);
	}
	
}