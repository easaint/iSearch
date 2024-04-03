#include "../Export/TLFile.h"

namespace tl {

	long CTLFile::fileSize(std::string filePathName)
	{
		FILE* file = NULL;
		if (fopen_s(&file, filePathName.c_str(), "rb") != 0)
		{
			return -1;
		}

		fseek(file, 0L, SEEK_END);  // ���ļ�ָ�붨λ���ļ�ĩβ
		long size = ftell(file);    // ��ȡ�ļ�ָ�뵱ǰλ�ã����ļ���С
		fclose(file);               // �ر��ļ�

		return size;
	}
}