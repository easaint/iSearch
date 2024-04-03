#ifndef _TL_HARDWARE_H
#define _TL_HARDWARE_H
#include "TLCommonBase.h"
#include <string>
#include <exception>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <iomanip>

#ifdef _WINDOWS
#include <Windows.h>
#include <comdef.h>
#include <Wbemidl.h>
#endif

namespace tl {

	//定义显卡信息
	struct TLGraphicsCard {
		std::wstring name;     //显卡名称
		std::wstring deviceId; //显卡id
	};

	//定义硬盘信息
	struct TLDiskDrive {
		std::wstring name;          //磁盘名称
		std::wstring deviceId;      //设备id
		std::wstring serialNumber;   //序列re号
		unsigned long long size = 0;//大小 字节
	};

	//定义主板信息
	struct TLBaseBoard {
		std::wstring name;          //名称
		std::wstring manufacturer;  //生产厂商
		std::wstring product;       //产品
		std::wstring version;       //版本
		std::wstring serialNumber;  //序列号
	};

	//BIOS WIN32_BIOS
	struct TLBios {
		std::wstring manufacturer;  //生产厂商
		std::wstring releaseDate;   //发布日期
		std::wstring serialNumber; //序列号
	};

	//物理内存信息
	struct TLPhysicalMemory {
		std::wstring name;         //名称
		std::wstring manufacturer; //生产厂商
		std::wstring serialNumber; //序列号
		unsigned int speed;        //频率
		unsigned long long size;   //容量 GB
	};

	//CPU 信息
	struct TLProcessor {
		std::wstring name;         //名称
		std::wstring desc;         //描述
		std::wstring manufacturer; //生产厂商
		unsigned int numberOfCores;//CPU核心数量
		std::wstring processId;    //处理器id
		unsigned int threadCount;  //线程数量
		double maxClockSpeed;      //最大主频
	};

	//网卡信息
	struct TLNetworkAdapter {
		std::wstring name;        //名称
		std::wstring desc;        //描述
		std::wstring manufacturer;//生产厂商
		std::wstring macAddress;  //mac地址
		std::wstring adapterType; //类型
	};

	//系统信息
	struct TLOSInfo {
		std::wstring name;
		std::wstring buildNumber;
		std::wstring version;
		std::wstring installDate;
		std::wstring osArchitecture;
		std::wstring registerdUser;
		std::wstring serialNumber;
	};

#ifdef _WINDOWS
	//定义WMINC上下文
	struct WMICContext
	{
		HRESULT hResult = NULL;
		IWbemLocator* pLoc = NULL;
		IWbemServices* pSvc = NULL;
		std::stringstream message;
	};
#endif

	//定义异常
	class HardwareException : public std::exception {
	public:
		HardwareException(const std::string& message) : message_(message) {}

		const char* what() const override {
			return message_.c_str();
		}

	private:
		std::string message_;
	};

	/**
	 * 该模块提供硬件信息获取，windows平台主要是使用wmi接口
	 */
	class TL_COMMON_API_ CTLHardware {
	public:
		CTLHardware();
		/*从外部传入HRESULT，解决冲突*/
		CTLHardware(HRESULT* hRet);
		~CTLHardware();
	public:
		/**
		 * 函数说明
		 *   获取系统信息，错误则抛出HardwareException异常
		 * 返回值
		 *   返回信息结构
		 */
		TLOSInfo osInfo();

		/**
		 * 函数说明
		 *   获取显卡信息，错误则抛出HardwareException异常
		 * 返回值
		 *   返回显卡信息，可能存在多张显卡
		 */
		std::vector<TLGraphicsCard> graphicsCard();

		/**
		 * 函数说明
		 *   获取硬盘驱动，错误则抛出HardwareException异常
		 * 返回值
		 *   返回硬盘信息，可能有多张硬盘
		 */
		std::vector<TLDiskDrive> diskDrive();

		/**
		 * 函数说明
		 *   获取主板信息，错误则抛出HardwareException异常
		 * 返回值
		 *   返回主板信息
		 */
		TLBaseBoard baseBoard();

		/**
		 * 函数说明
		 *   获取bios信息，错误则抛出HardwareException异常
		 * 返回值
		 *   返回bios信息
		 */
		TLBios bois();

		/**
		 * 函数说明
		 *   获取物理内存信息，错误则抛出HardwareException异常
		 * 返回值
		 *   返回内存信息
		 */
		std::vector<TLPhysicalMemory> physicalMemory();

		/**
		 * 函数说明
		 *   获取处理器信息，错误则抛出HardwareException异常
		 * 返回值
		 *   返回处理器信息
		 */
		std::vector<TLProcessor> processor();

		/**
		 * 函数说明
		 *   获取网卡信息，错误则抛出HardwareException异常
		 * 返回值
		 *   返回网卡信息
		 */
		std::vector<TLNetworkAdapter> networkAdapter();

	private:
#ifdef _WINDOWS
		IEnumWbemClassObject* ExecQuery(const std::wstring className);
		void initContext();
#endif

	private:
#ifdef _WINDOWS
		WMICContext* m_pCtx;
		HRESULT* m_phRet;
#endif
	};
}


#endif