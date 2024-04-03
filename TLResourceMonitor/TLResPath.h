#ifndef _TL_RES_PATH_H
#define _TL_RES_PATH_H
#include <string>
#include <list>

/**
 * 文件属性
 */
class CFileAttribute {

public:
	CFileAttribute() {}
	CFileAttribute(std::wstring filePathName,std::wstring createTime,std::wstring modifyTime,std::wstring fileSize)
		: m_filePathName(filePathName), m_createTime(createTime), m_modifyTime(modifyTime), m_fileSize(fileSize) {}

	CFileAttribute& operator=(const CFileAttribute& fa) {
		if (this != &fa) {
			m_createTime = fa.m_createTime;
			m_filePathName = fa.m_filePathName;
			m_modifyTime = fa.m_modifyTime;
			m_fileSize = fa.m_fileSize;
		}
		return *this;
	}
	
public:
	std::wstring getFilePathName() { return m_filePathName; }
	void setFilePathName(std::wstring filePathName) { m_filePathName = filePathName; }
	std::wstring getFilePath() { return m_filePath; }
	void setFilePath(std::wstring filePath) { m_filePath = filePath; }
	std::wstring getCreateTime() { return m_createTime; }
	void setCreateTime(std::wstring createTime) { m_createTime = createTime; }
	std::wstring getModifyTime() { return m_modifyTime; }
	void setModifyTime(std::wstring modifyTime) { m_modifyTime = modifyTime; }
	std::wstring getFileSize() { return m_fileSize; }
	void setFileSize(std::wstring fileSize) { m_fileSize = fileSize; }
	std::wstring getFileName() { return m_fileName; }
	void setFileName(std::wstring fileName) { m_fileName = fileName; }
	std::wstring getFileSuffix() { return m_fileSuffix; }
	void setFileSuffix(std::wstring fileSuffix) { m_fileSuffix = fileSuffix; }
private:
	std::wstring m_filePathName;//路径和文件名
	std::wstring m_filePath;    //文件路径，不带文件名
	std::wstring m_createTime;
	std::wstring m_modifyTime;
	std::wstring m_fileSize;   //文件大小，单位字节
	std::wstring m_fileName;   //文件名，不带后缀
	std::wstring m_fileSuffix; //文件后缀
};

/**
 * 资源路径类
 */
class CResPath {

public:
	/***
	 * 函数说明：
	 *   获取路径下的所有能被解析的文档
	 * 参数：
	 *   path：目录，绝对路径
	 * 返回值：
	 *   返回文档名称
	 */
	static std::list<std::wstring> docsUnderPath(std::wstring path);

	/***
	 * 函数说明
	 *   获取目录下的子目录列表
	 * 参数
	 *   path：父目录
	 * 返回值
	 *   返回子目录列表
	 */
	static std::list<std::wstring> subPathUnderPath(std::wstring path);

	/**
	 * 函数说明：
	 *   获取所有逻辑磁盘除去系统盘
	 * 返回值
	 *   返回逻辑盘列表
	 */
	static std::list<std::string> allLogicDiskExceptSystem();

	/**
	 * 函数说明
	 *   获取文件属性
	 * 参数
	 *   pathName：文件路径
	 * 返回值
	 *   返回文件属性
	 */
	static CFileAttribute fileAttribute(std::wstring pathName);

	/**
	 * 函数说明
	 *   获取需要被扫描的路径
	 * 返回值
	 *   返回需要被扫描的路径
	 */
	static std::list<std::string> needScanPath();

	/**
	 * 函数说明
	 *   通过文件路径名称生成文件md5，代表文件在磁盘中的唯一位置
	 * 参数
	 *   filePathName：文件路径名称，绝对路径
	 * 返回值
	 *   返回md5，32位
	 */
	static std::wstring fileMD5(std::wstring filePathName);

	/**
	 * 函数说明
	 *   通过文件路径名称生成文件hash，通过文件的绝对路径，创建时间，修改时间，大小来生成固定的hash
	 * 参数
	 *   filePathName：文件路径名称，绝对路径
	 * 返回值
	 *   返回hash，64位
	 */
	static std::wstring fileHash(std::wstring filePathName);

public:
	/**
	 * 函数说明：
	 *   检查文档是否能够被解析
	 * 参数：
	 *   fileName：文件名，带后缀
	 *  返回值：
	 *   true：可以被解析，false：不能解析
	 */
	static bool checkDocParsed(std::wstring fileName);
};

#endif