



#include "bsp_aht20.h"
#include "hw_aht20_interface.h"
#include	"SEGGER_RTT.h"

static int hw_aht20_init()
{
	int ret = AHT20_init();
	if(ret == 1)
	{
		//SEGGER_RTT_printf(0,"aht20 init fail!");
		return ERR_FAILED;
	}
	
	return ERR_SUCCESS;
}


static int hw_aht20_read(float* temperature,float* humidity)
{
	int ret = AHT20_read(temperature,humidity);
	if(ret == 1)
	{
		//SEGGER_RTT_printf(0,"AHT20 read error!\n");
		return ERR_FAILED;
	}
	
	return ERR_SUCCESS;
}
	

hw_aht20_interface_t hw_aht20_interface = {
		.state = DEVICE_STATUS_UNINIT,
		.temperature = 0,
		.humidity = 0,
		.init = hw_aht20_init,
		.read = hw_aht20_read,
};