#ifndef _TL_UI_MSG_H
#define _TL_UI_MSG_H
#include <Windows.h>
/**
 * ��ģ�鶨���û���Ϣ��������UI�̴߳��巢����Ϣ
 */

//���ü������֪ͨ
#define WM_USER_CONF_LOAD_FINISHED     (WM_USER + 100)
//�汾���µ���Ϣ֪ͨ
#define WM_USER_VERSION_UPDATE         (WM_USER + 101)
//������������Ϣ֪ͨ
#define WM_USER_PLUGIN_LOAD_FINISHED   (WM_USER + 102)
//��������༭���ĵ���Ϣ֪ͨ
#define WM_USER_LOAD_RECENT_ACCESS_DOC (WM_USER + 103)
//���ر��ؼ����ĵ��ɹҵ���Ϣ֪ͨ
#define WM_USER_LOAD_LOCAL_SEARCH_RESULT_SUCCUESS  (WM_USER + 104)

//�û�sessionʧЧ����Ϣ֪ͨ
#define WM_USER_SESSION_INVALID		   (WM_USER + 105)

#endif