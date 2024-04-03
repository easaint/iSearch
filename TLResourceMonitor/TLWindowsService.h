/***
 * windows����ģ��
 */
#ifndef _TL_WINDOWS_SERVICE_H
#define _TL_WINDOWS_SERVICE_H
#include <Windows.h>
#include <tchar.h>

/**
 * ����˵����
 *   �����ʼ��
 */
void Init();

/***
 * ����˵����
 *   �������Ƿ񱻰�װ
 * ����ֵ��
 *   TRUE���Ѱ�װ��FALSE��δ��װ
 */
BOOL IsInstalled();

/**
 * ����˵����
 *   ��װ����
 * ����ֵ��
 *   TRUE���ɹ���FALSE��ʧ��
 */
BOOL Install();

/***
 * ����˵����
 *   ж�ط���
 * ����ֵ��
 *   TRUE���ɹ���FALSE��ʧ��
 */
BOOL Uninstall();

/***
 * ����˵����
 *   ע�����
 * ����ֵ��
 *   TRUE���ɹ���FALSE��ʧ��
 */
BOOL RegistService();

/***
 * ����˵����
 *   ��¼�����¼�
 * ������
 *   pszFormat���¼���Ϣ
 */
void LogEvent(LPCTSTR pszFormat, ...);

/***
 * ����˵����
 *   ������ں��������������ܸı䣬windows�������Ҹú�����
 */
void WINAPI ServiceMain(DWORD argc, LPTSTR* argv);

/***
 * ����˵����
 *   ע�����״̬���ƺ����������ֹͣ���������У���ͣ�Ȼ�ص��ú���
 */
void WINAPI ServiceStrl(DWORD dwControl);

#endif