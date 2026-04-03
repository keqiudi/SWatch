#include "hw_rtc_interface.h"
#include "rtc.h"


static void hw_rtc_set_time(uint8_t hours,uint8_t minutes,uint8_t seconds)
{
    my_rtc_set_time(hours, minutes, seconds);
}

static void hw_rtc_set_date(uint8_t year,uint8_t month,uint8_t date)
{
    my_rtc_set_date(year, month, date);
}

static uint8_t hw_rtc_get_time_date(hw_date_time_t* date_time)
{

    if(date_time == NULL)
    {
        return 1;
    }

    RTC_TimeTypeDef now_time = {0};
    RTC_DateTypeDef now_date = {0};
    HAL_RTC_GetTime(&hrtc, &now_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &now_date, RTC_FORMAT_BIN);
    date_time->hours = now_time.Hours; 
    date_time->minutes = now_time.Minutes;
    date_time->seconds = now_time.Seconds;
    date_time->year = now_date.Year;
    date_time->month = now_date.Month;
    date_time->date = now_date.Date;
    date_time->weekday = my_rtc_cal_weekday(date_time->year, date_time->month, date_time->date, 20); // 这里的世纪数c为20，表示2000年以后的年份

    return 0;
}

static uint8_t hw_rtc_calculate_weekday(uint8_t year, uint8_t month, uint8_t day, uint8_t century)
{
    return my_rtc_cal_weekday(year, month, day, century);
}

hw_rtc_interface_t hw_rtc_interface = {
    .set_time = hw_rtc_set_time, 
    .set_date = hw_rtc_set_date, 
    .get_time_date = hw_rtc_get_time_date, 
    .calculate_weekday = hw_rtc_calculate_weekday, 
};
