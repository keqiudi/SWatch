#ifndef BSP_WDOG_H
#define BSP_WDOG_H

#define WDOG_EN_PORT GPIOB
#define WDOG_EN_PIN GPIO_PIN_1

#define WDI_PORT GPIOB
#define WDI_PIN GPIO_PIN_2

void wdog_init(void);
void wdog_feed(void);
void wdog_enable(void);
void wdog_disable(void);

#endif // BSP_WDOG_H