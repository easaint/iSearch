#ifndef _TL_FILE_ITEM_H
#define _TL_FILE_ITEM_H
#include <string>

/**
 * 文件项
 */
struct STLFileItem {
	std::string fileFullPathName; //文件全路径名
	std::string createTime;       //创建时间
	std::string modifyTime;       //修改时间
	std::string accessTime;    //访问时间
	unsigned long long lAccessTime;              //访问时间
	std::string fileName;         //文件名
	std::string fileIconPath;     //文件图标路径
	std::string lastAccessTimeTip; //上次访问时间到现在的时间段提示，1秒前/1分钟前/3分钟前/1小时前/1天前/3天前/半个月前/1个月前
};

#endif