#ifndef _TL_FILE_ITEM_H
#define _TL_FILE_ITEM_H
#include <string>

/**
 * �ļ���
 */
struct STLFileItem {
	std::string fileFullPathName; //�ļ�ȫ·����
	std::string createTime;       //����ʱ��
	std::string modifyTime;       //�޸�ʱ��
	std::string accessTime;    //����ʱ��
	unsigned long long lAccessTime;              //����ʱ��
	std::string fileName;         //�ļ���
	std::string fileIconPath;     //�ļ�ͼ��·��
	std::string lastAccessTimeTip; //�ϴη���ʱ�䵽���ڵ�ʱ�����ʾ��1��ǰ/1����ǰ/3����ǰ/1Сʱǰ/1��ǰ/3��ǰ/�����ǰ/1����ǰ
};

#endif