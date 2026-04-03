/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  if(HAL_RTCEx_BKUPRead(&hrtc,RTC_BKP_DR0)!=0X5050) // 根据是否写入标志数据，来决定是否需要重新初始化
  {
  /* USER CODE END Check_RTC_BKUP */

    /** Initialize RTC and set the Time and Date
    */
    sTime.Hours = 0x11;
    sTime.Minutes = 0x59;
    sTime.Seconds = 0x55;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;
    if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
    {
      Error_Handler();
    }
    sDate.WeekDay = RTC_WEEKDAY_SUNDAY;
    sDate.Month = RTC_MONTH_MAY;
    sDate.Date = 0x21;
    sDate.Year = 0x23;

    if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
    {
      Error_Handler();
    }

    /** Enable the WakeUp
    */
    if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2048, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
    {
      Error_Handler();
    }
    /* USER CODE BEGIN RTC_Init 2 */
    HAL_RTCEx_BKUPWrite(&hrtc,RTC_BKP_DR0,0X5050); // 写入备份寄存器，用于判断是否第一次进入
  }
    /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */


/*
  * 蔡勒公式计算星期几
  * 蔡勒公式： w = (y + [y/4] + [c/4] - 2c + [26(m+1)/10] + d - 1) mod 7
  * 参数：
  * y: 年份的后两位数
  * m: 月份
  * d: 日期
  * c: 世纪数，即年份的前两位数
  * 返回值：0-6分别对应星期日到星期六
  * 说明：如果月份是1月或2月，计算时需将1、2月视为上一年的13、14月,即 m = m + 12, y = y - 1
  * 
*/

uint8_t my_rtc_cal_weekday(int y,int m,int d,int c)
{
    int w; // w为计算结果
    if(m == 1 || m == 2) 
    {
        m += 12; 
        y -= 1;
    }

    w = (y + y/4 + c/4 - 2*c + 26*(m+1)/10 + d - 1) % 7;
    while(w < 0) // 确保结果为非负数
    {
        w += 7;
    }

    w = w % 7; 
    return ((w==0) ? 7 : w); // 将0转换为7，表示星期日, 结果为1-7分别对应星期一到星期日
}

void my_rtc_set_time(uint8_t hours,uint8_t minutes,uint8_t seconds)
{
  RTC_TimeTypeDef sTime = {0};
  sTime.Hours = hours;
  sTime.Minutes = minutes;
  sTime.Seconds = seconds;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

void my_rtc_set_date(uint8_t year,uint8_t month,uint8_t date)
{
  RTC_DateTypeDef sDate = {0};
  sDate.Month = month;
  sDate.Date = date;
  sDate.Year = year;

  sDate.WeekDay = my_rtc_cal_weekday(year, month, date, 20); // 这里的世纪数c为20，表示2000年以后的年份

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE END 1 */

