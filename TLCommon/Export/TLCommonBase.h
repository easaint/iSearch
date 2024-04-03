#ifndef _TL_COMMON_BASE_H
#define _TL_COMMON_BASE_H

#ifndef _IN_
#define _IN_  //�����������
#endif

#ifndef _OUT_
#define _OUT_ //�����������
#endif

#ifndef _GLOBAL_
#define _GLOBAL_ //����ȫ�ֱ���
#endif


//����NULL
#ifndef NULL
#define NULL 0
#endif

//���庯������
#ifndef EXPORT_TL_COMMON
#define TL_COMMON_API_ _declspec (dllimport)
#else
#define TL_COMMON_API_ _declspec (dllexport)
#endif

//����·���ָ���
#ifdef _WINDOWS
#define PATH_SEPARATOR  _T("\\")
#else
#define PATH_SEPARATOR  "/"
#endif

#endif