#pragma once
#include <Windows.h>

/**********************************************************************************************
 * 系统环境操作类
 *   
 *********************************************************************************************/
class SystemEven
{
public:
	SystemEven(void);
	~SystemEven(void);

public:
	/***
	 * 函数说明
	 *  获取屏幕大小
	 **/
	static void GetScreenSize(RECT & rect);
};