#include "bsp_power.h"
#include "adc.h"
#include "delay.h"

#define INTERNAL_RES 0.128
#define CHARGING_CUR 1

static void power_pins_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(POWER_PORT, POWER_PIN, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = POWER_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(POWER_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : PA2 */
  GPIO_InitStruct.Pin = CHARGE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING; //charge引脚设置为上升沿和下降沿都触发中断
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(CHARGE_PORT, &GPIO_InitStruct);

  HAL_NVIC_EnableIRQ(EXTI2_IRQn);
  HAL_NVIC_SetPriority(EXTI2_IRQn, 0, 0);
	
}

void power_enable(void)
{
	HAL_GPIO_WritePin(POWER_PORT,POWER_PIN,GPIO_PIN_SET);
}

void power_disEnable(void)
{
	HAL_GPIO_WritePin(POWER_PORT,POWER_PIN,GPIO_PIN_RESET);
}

uint8_t charge_check(void) //1:charging
{
	return HAL_GPIO_ReadPin(CHARGE_PORT,CHARGE_PIN);
}

float bat_check(void)
{
	uint16_t dat;
	float BatVoltage;
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1,5);
	dat = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);
	BatVoltage = dat *2 *3.3 /4096;
	return BatVoltage;
}

float bat_check_8times(void)
{
	uint32_t dat=0;
	uint8_t i;
	float BatVoltage;
	for(i=0;i<8;i++)
	{
		HAL_ADC_Start(&hadc1);
		HAL_ADC_PollForConversion(&hadc1,5);
		dat += HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		delay_ms(1);
	}
	dat = dat>>3; // 取平均值
	BatVoltage = dat *2 *3.3 /4096;
	return BatVoltage;
}

uint8_t power_calculate(void)
{
	uint8_t power;
	float voltage;
	const float v_min = 3.45f;
	const float v_max = 4.20f;
	voltage = bat_check_8times(); 
	
	// if(charge_check())
	// {voltage -= INTERNAL_RES * CHARGING_CUR;} // 如果正在充电，电压会有一个内阻压降，进行补偿以更准确地估算剩余电量

	if(voltage <= v_min)
	{
		power = 0;
	}
	else if(voltage >= v_max)
	{
		power = 100;
	}
	else
	{
		float ratio = (voltage - v_min) / (v_max - v_min); // 计算电压在范围内的位置比例
		uint8_t percent = (uint8_t)(ratio * 100.0f + 0.5f); // 将比例转换为百分比，并四舍五入到最近的整数
		power = percent;
	}
	return power;
    
	// 这样有个问题：电量显示波动快，可以使用电压低通滤波和电量死区解决？
}

void power_init(void)
{
	power_pins_init();
	power_enable();
}


