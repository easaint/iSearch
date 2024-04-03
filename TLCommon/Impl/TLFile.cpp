#include "../Export/TLFile.h"

namespace tl {

	long CTLFile::fileSize(std::string filePathName)
	{
		FILE* file = NULL;
		if (fopen_s(&file, filePathName.c_str(), "rb") != 0)
		{
			return -1;
		}

		fseek(file, 0L, SEEK_END);  // 将文件指针定位到文件末尾
		long size = ftell(file);    // 获取文件指针当前位置，即文件大小
		fclose(file);               // 关闭文件

		return size;
	}
}