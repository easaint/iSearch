#pragma once
#include <Windows.h>

/**********************************************************************************************
 * ϵͳ����������
 *   
 *********************************************************************************************/
class SystemEven
{
public:
	SystemEven(void);
	~SystemEven(void);

public:
	/***
	 * ����˵��
	 *  ��ȡ��Ļ��С
	 **/
	static void GetScreenSize(RECT & rect);
};