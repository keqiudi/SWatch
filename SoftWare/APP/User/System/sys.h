#ifndef __SYS_H__
#define __SYS_H__


/**
 * sys.h: 
 * 嵌入式中常用于集中放置全局性的配置、数据类型和开关宏等，由用户自己定义。
 * 任何地方只需 #include "sys.h" 即可用全局类型和宏，减少头文件依赖混乱。
 * 统一代码风格，提升可读性和可维护性。
 * 工程配置一目了然，便于团队协作。
 *  
 */

#include "stm32f4xx.h"                  // Device header




typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;





#endif

