#include "SystemEven.h"


SystemEven::SystemEven(void)
{
}


SystemEven::~SystemEven(void)
{
}

/***
 * ����˵��
 *  ��ȡ��Ļ��С
 **/
void SystemEven::GetScreenSize(RECT & rect)
{
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rect, 0);
}