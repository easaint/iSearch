#include "SystemEven.h"


SystemEven::SystemEven(void)
{
}


SystemEven::~SystemEven(void)
{
}

/***
 * 函数说明
 *  获取屏幕大小
 **/
void SystemEven::GetScreenSize(RECT & rect)
{
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
}