#include "hr_algorithm.h"
#include "string.h"

Queue datas; // EM7028的PPG光电容积信号原始采样波形数据
Queue times; // 与采样值对应的时间戳窗口(ms)
Queue HR_List; // 近期心率估计值窗口，用于均值滤波

void HR_AlgoInit(void)
{
	initQueue(&times);
	initQueue(&datas);
	initQueue(&HR_List);
}

// 简单的均值滤波函数，输入为心率列表和列表长度，输出为平均心率值
uint8_t Hr_Ave_Filter(uint32_t *HrList, uint8_t lenth)
{
	uint32_t ave = 0;
	uint8_t i;
	for(i = 0;i<lenth;i++)
	{
		ave += HrList[i];
	}
	ave /= lenth;

	return (uint8_t)ave;
}

uint16_t HR_Calculate(uint16_t present_dat,uint32_t present_time)
{

	static uint16_t peaks_time[]={0,0}; // 最近两个峰值的时间戳，初始为0
	static uint8_t HR=0; // 作为心率结果输出
 
	// 维持固定长度的滑动窗口
	if(isQueueFull(&datas))
	{dequeue(&datas);}
	if(isQueueFull(&times))
	{dequeue(&times);}
	if(isQueueFull(&HR_List))
	{dequeue(&HR_List);}

	enqueue(&datas,present_dat);
	enqueue(&times,present_time);

	if((datas.data[3]>=datas.data[2]) && (datas.data[3]>=datas.data[1]) && (datas.data[3]>datas.data[0]) 
		&& (datas.data[3]>=datas.data[4]) && (datas.data[3]>=datas.data[5]) && (datas.data[3]>datas.data[6]))
	{
			if((times.data[3]-peaks_time[0])>425) // 两个峰之间至少425ms，过滤过快的伪峰(最大心率约 141 bpm）。
			{
					peaks_time[1] = peaks_time[0]; // 更新上一个峰的时间戳
					peaks_time[0] = times.data[3]; // 更新当前峰的时间戳
					enqueue(&HR_List,60000/(peaks_time[0]-peaks_time[1])); // 60000为1分钟的毫秒数，时间差为两个峰值的差值，对应一次心率
					if(HR_List.data[6]!=0)
					{
						HR = Hr_Ave_Filter(HR_List.data,7);
					}
			}
	}
	return HR;
}



