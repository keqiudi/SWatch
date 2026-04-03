#ifndef HW_RTC_INTERFACE_H
#define HW_RTC_INTERFACE_H

#include "device.h"

typedef struct
{
    uint8_t hours;    // 小时，RTC_HourFormat_12格式下取值范围：1-12，RTC_HourFormat_24格式下取值范围：0-23

    uint8_t minutes;  // 分钟，取值范围：0-59

    uint8_t seconds;  // 秒，取值范围：0-59

    uint8_t year;     // 年份，通常为两位数（如2026年表示为26）

    uint8_t month;    // 月份，取值范围：1-12

    uint8_t date;     // 日期，取值范围：1-31

    uint8_t weekday;  // 星期，1-7分别对应星期一到星期日
}hw_date_time_t;


typedef struct{
 
    void (*set_time)(uint8_t hours,uint8_t minutes,uint8_t seconds);
    void (*set_date)(uint8_t year,uint8_t month,uint8_t date);
    uint8_t (*get_time_date)(hw_date_time_t* date_time);
    uint8_t (*calculate_weekday)(uint8_t year, uint8_t month, uint8_t day, uint8_t century);
}hw_rtc_interface_t;

extern hw_rtc_interface_t hw_rtc_interface;

#endif // HW_RTC_INTERFACE_H