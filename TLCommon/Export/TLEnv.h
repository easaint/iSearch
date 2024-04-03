/***
 * �����ǳ��򻷾���Ϣ��ص��࣬ͨ��������Ի�ȡ�������е�PC��Ϣ
 */
#ifndef _TL_ENV_H
#define _TL_ENV_H
#include <string>
#include <list>

#include "TLCommonBase.h"

/**
 * ������Ҫ����һЩ��̬�������ɣ���ȡ����������Ϣ
 */
class TL_COMMON_API_ CTLEnv {

public:
	/**
	 * ����˵����
	 *   ��ȡ��ǰ�������еĹ���Ŀ¼
	 * ����ֵ��
	 *   ����Ŀ¼
	 */
	static std::string currentRunDir();

	/**
	 * ����˵����
	 *   ��ȡ�������ڵ�Ŀ¼
	 * ����ֵ��
	 *   ����Ŀ¼
	 */
	static std::string exePositonPath();

	/**
	 * ����˵����
	 *   ��ȡ���г��������
	 * ����ֵ��
	 *   ���س�������
	 */
	static std::string exeName();

	/**
	 * ����˵����
	 *   ��ȡ��ǰ�������¼���û���
	 * ����ֵ��
	 *   ���ص�ǰ��¼���û���
	 */
	static std::string pcLoginUserName();

	/***
	 * ����˵����
	 *   ��ȡ������ж����߼����̣�������U�̺͹���
	 * ����ֵ��
	 *   �����߼��̣��磺C:/D:/E:
	 */
	static std::list<std::string> pcLogicalDrivers();

#ifdef _WINDOWS

	/**
	 * ����˵����
	 *   ��windows�����У���ȡ��ǰ������û�������·����ֻ����windows������ʹ��
	 * ����ֵ��
	 *   ��������·��
	 */
	static std::string pcUserDesktopPathForWindowsService();

	/**
	 * ����˵����
	 *   ��windows�����У���ȡ��ǰ������û����ĵ�·����ֻ����windows������ʹ��
	 * ����ֵ��
	 *   �����ĵ�·��
	 */
	static std::string pcUserDocumentPathForWindowsService();

	/**
	 * ����˵����
	 *   ��windows�����У���ȡ��ǰ������û��İ�װ��������·����ֻ����windows������ʹ��
	 * ����ֵ��
	 *   ���س�������·��
	 */
	static std::string pcUserAppDataPathForWindowsService();

	/**
	 * ����˵����
	 *   ��windows�����У���ȡ��ǰ������û����ղؼ�·����ֻ����windows������ʹ��
	 * ����ֵ��
	 *   �����ղؼ�·��
	 */
	static std::string pcUserFavoritesPathForWindowsService();

	/**
	 * ����˵����
	 *   ��windows�����У���ȡ��ǰ������û���ͼƬ·����ֻ����windows������ʹ��
	 * ����ֵ��
	 *   ����ͼƬ·��
	 */
	static std::string pcUserPicturePathForWindowsService();

	/**
	 * ����˵����
	 *   ��windows�����У���ȡ��ǰ������û�������·����ֻ����windows������ʹ��
	 * ����ֵ��
	 *   ��������·��
	 */
	static std::string pcUserMusicPathForWindowsService();

	/**
	 * ����˵����
	 *   ��windows�����У���ȡ��ǰ������û�����Ƶ·����ֻ����windows������ʹ��
	 * ����ֵ��
	 *   ������Ƶ·��
	 */
	static std::string pcUserVideoPathForWindowsService();

	/**
	 * ����˵��
	 *   ��ȡ�û�����༭���ĵ��б�������windows������ʹ��
	 * ����ֵ
	 *   �û��༭���ĵ��б�
	 */
	static std::list<std::string> pcUserRecentDocs();

	/**
	 * ����˵��
	 *   ��ȡ��ǰϵͳĿ¼�� ProgramData Ŀ¼
	 * ����ֵ
	 *   ����ProgramDataĿ¼���磺C:\\ProgramData
	 */
	static std::string pcProgramDataPath();
#endif
};

#endif