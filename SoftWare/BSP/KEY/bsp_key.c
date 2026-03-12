#include "bsp_key.h"
#include "user_task_init.h"

void key_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin : PA5 */
  /* 原理图：key1没按下前悬空, 按下后为低电平*/
  GPIO_InitStruct.Pin = KEY1_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP; // 上拉电阻，确保按键未按下时为高电平
  HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  /* 原理图：key2没按下前wake引脚为低电平，按下后wake引脚被电源拉为高电平*/
  GPIO_InitStruct.Pin = KEY2_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(KEY2_PORT, &GPIO_InitStruct);
	
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
	
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

uint8_t key_scan(uint8_t mode)
{
	static uint8_t key_press_wait = 1; // 1: wait for key press, 0: wait for key release
	static uint8_t key_pressed_id = 0; // 0: no key, 1: key1, 2: key2
	uint8_t key_value = 0;

	if(mode) // mode=1 手动重置扫描器
	{
		key_press_wait = 1;
		key_pressed_id = 0;
	}

	// 按键按下：key1按下时为低电平，key2按下时为高电平
	if( key_press_wait && ((!KEY1) || KEY2))
	{
		osDelay(pdMS_TO_TICKS(5));// 按键消抖5ms
		if(!KEY1) 
			key_pressed_id = 1;
		if(KEY2)
			key_pressed_id = 2;
		if(key_pressed_id) 
			key_press_wait = 0; // 按键已按下，等待释放
	}

	// 按键释放：key1 = 1: key1释放, key2 = 0: key2释放
	if ( key_pressed_id && (KEY1 && (!KEY2)) )
	{
		osDelay(pdMS_TO_TICKS(5));// 按键消抖5ms
		
		if(KEY1 && (!KEY2)) 
		{
			key_press_wait = 1; // 按键已释放，等待下一次按下
			key_value = key_pressed_id; // 返回按键ID，1或2
			key_pressed_id = 0;
		}
	}

	return key_value;
}

