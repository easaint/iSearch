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

	//�����Կ���Ϣ
	struct TLGraphicsCard {
		std::wstring name;     //�Կ�����
		std::wstring deviceId; //�Կ�id
	};

	//����Ӳ����Ϣ
	struct TLDiskDrive {
		std::wstring name;          //��������
		std::wstring deviceId;      //�豸id
		std::wstring serialNumber;   //����re��
		unsigned long long size = 0;//��С �ֽ�
	};

	//����������Ϣ
	struct TLBaseBoard {
		std::wstring name;          //����
		std::wstring manufacturer;  //��������
		std::wstring product;       //��Ʒ
		std::wstring version;       //�汾
		std::wstring serialNumber;  //���к�
	};

	//BIOS WIN32_BIOS
	struct TLBios {
		std::wstring manufacturer;  //��������
		std::wstring releaseDate;   //��������
		std::wstring serialNumber; //���к�
	};

	//�����ڴ���Ϣ
	struct TLPhysicalMemory {
		std::wstring name;         //����
		std::wstring manufacturer; //��������
		std::wstring serialNumber; //���к�
		unsigned int speed;        //Ƶ��
		unsigned long long size;   //���� GB
	};

	//CPU ��Ϣ
	struct TLProcessor {
		std::wstring name;         //����
		std::wstring desc;         //����
		std::wstring manufacturer; //��������
		unsigned int numberOfCores;//CPU��������
		std::wstring processId;    //������id
		unsigned int threadCount;  //�߳�����
		double maxClockSpeed;      //�����Ƶ
	};

	//������Ϣ
	struct TLNetworkAdapter {
		std::wstring name;        //����
		std::wstring desc;        //����
		std::wstring manufacturer;//��������
		std::wstring macAddress;  //mac��ַ
		std::wstring adapterType; //����
	};

	//ϵͳ��Ϣ
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
	//����WMINC������
	struct WMICContext
	{
		HRESULT hResult = NULL;
		IWbemLocator* pLoc = NULL;
		IWbemServices* pSvc = NULL;
		std::stringstream message;
	};
#endif

	//�����쳣
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
	 * ��ģ���ṩӲ����Ϣ��ȡ��windowsƽ̨��Ҫ��ʹ��wmi�ӿ�
	 */
	class TL_COMMON_API_ CTLHardware {
	public:
		CTLHardware();
		/*���ⲿ����HRESULT�������ͻ*/
		CTLHardware(HRESULT* hRet);
		~CTLHardware();
	public:
		/**
		 * ����˵��
		 *   ��ȡϵͳ��Ϣ���������׳�HardwareException�쳣
		 * ����ֵ
		 *   ������Ϣ�ṹ
		 */
		TLOSInfo osInfo();

		/**
		 * ����˵��
		 *   ��ȡ�Կ���Ϣ���������׳�HardwareException�쳣
		 * ����ֵ
		 *   �����Կ���Ϣ�����ܴ��ڶ����Կ�
		 */
		std::vector<TLGraphicsCard> graphicsCard();

		/**
		 * ����˵��
		 *   ��ȡӲ���������������׳�HardwareException�쳣
		 * ����ֵ
		 *   ����Ӳ����Ϣ�������ж���Ӳ��
		 */
		std::vector<TLDiskDrive> diskDrive();

		/**
		 * ����˵��
		 *   ��ȡ������Ϣ���������׳�HardwareException�쳣
		 * ����ֵ
		 *   ����������Ϣ
		 */
		TLBaseBoard baseBoard();

		/**
		 * ����˵��
		 *   ��ȡbios��Ϣ���������׳�HardwareException�쳣
		 * ����ֵ
		 *   ����bios��Ϣ
		 */
		TLBios bois();

		/**
		 * ����˵��
		 *   ��ȡ�����ڴ���Ϣ���������׳�HardwareException�쳣
		 * ����ֵ
		 *   �����ڴ���Ϣ
		 */
		std::vector<TLPhysicalMemory> physicalMemory();

		/**
		 * ����˵��
		 *   ��ȡ��������Ϣ���������׳�HardwareException�쳣
		 * ����ֵ
		 *   ���ش�������Ϣ
		 */
		std::vector<TLProcessor> processor();

		/**
		 * ����˵��
		 *   ��ȡ������Ϣ���������׳�HardwareException�쳣
		 * ����ֵ
		 *   ����������Ϣ
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