

#include "bsp_wdog.h"
#include "main.h"

void wdog_init(void)
{
        GPIO_InitTypeDef  GPIO_InitStructure = {0};
        
        __HAL_RCC_GPIOB_CLK_ENABLE();
        
        GPIO_InitStructure.Pin = WDOG_EN_PIN;
        GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStructure.Pull = GPIO_PULLUP;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW; // 看门狗使能引脚
        HAL_GPIO_Init(WDOG_EN_PORT,&GPIO_InitStructure);
        GPIO_InitStructure.Pin = WDI_PIN;
        GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH; // 看门狗喂狗引脚，要求快速响应
        HAL_GPIO_Init(WDI_PORT,&GPIO_InitStructure);
}


void wdog_enable(void)
{
    HAL_GPIO_WritePin(WDOG_EN_PORT, WDOG_EN_PIN, GPIO_PIN_RESET); //BL1551B A2与B通 使能看门狗
}

void wdog_disable(void)
{
    HAL_GPIO_WritePin(WDOG_EN_PORT, WDOG_EN_PIN, GPIO_PIN_SET); // BL1551B A1与B通 关闭看门狗
}

void wdog_feed(void)
{
    HAL_GPIO_TogglePin(WDI_PORT,WDI_PIN); // 如果 WDI 保持高电平或低电平的时间超过超时周期，则会触发复位。
}