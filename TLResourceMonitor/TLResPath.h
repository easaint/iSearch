#ifndef _TL_RES_PATH_H
#define _TL_RES_PATH_H
#include <string>
#include <list>

/**
 * �ļ�����
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
	std::wstring m_filePathName;//·�����ļ���
	std::wstring m_filePath;    //�ļ�·���������ļ���
	std::wstring m_createTime;
	std::wstring m_modifyTime;
	std::wstring m_fileSize;   //�ļ���С����λ�ֽ�
	std::wstring m_fileName;   //�ļ�����������׺
	std::wstring m_fileSuffix; //�ļ���׺
};

/**
 * ��Դ·����
 */
class CResPath {

public:
	/***
	 * ����˵����
	 *   ��ȡ·���µ������ܱ��������ĵ�
	 * ������
	 *   path��Ŀ¼������·��
	 * ����ֵ��
	 *   �����ĵ�����
	 */
	static std::list<std::wstring> docsUnderPath(std::wstring path);

	/***
	 * ����˵��
	 *   ��ȡĿ¼�µ���Ŀ¼�б�
	 * ����
	 *   path����Ŀ¼
	 * ����ֵ
	 *   ������Ŀ¼�б�
	 */
	static std::list<std::wstring> subPathUnderPath(std::wstring path);

	/**
	 * ����˵����
	 *   ��ȡ�����߼����̳�ȥϵͳ��
	 * ����ֵ
	 *   �����߼����б�
	 */
	static std::list<std::string> allLogicDiskExceptSystem();

	/**
	 * ����˵��
	 *   ��ȡ�ļ�����
	 * ����
	 *   pathName���ļ�·��
	 * ����ֵ
	 *   �����ļ�����
	 */
	static CFileAttribute fileAttribute(std::wstring pathName);

	/**
	 * ����˵��
	 *   ��ȡ��Ҫ��ɨ���·��
	 * ����ֵ
	 *   ������Ҫ��ɨ���·��
	 */
	static std::list<std::string> needScanPath();

	/**
	 * ����˵��
	 *   ͨ���ļ�·�����������ļ�md5�������ļ��ڴ����е�Ψһλ��
	 * ����
	 *   filePathName���ļ�·�����ƣ�����·��
	 * ����ֵ
	 *   ����md5��32λ
	 */
	static std::wstring fileMD5(std::wstring filePathName);

	/**
	 * ����˵��
	 *   ͨ���ļ�·�����������ļ�hash��ͨ���ļ��ľ���·��������ʱ�䣬�޸�ʱ�䣬��С�����ɹ̶���hash
	 * ����
	 *   filePathName���ļ�·�����ƣ�����·��
	 * ����ֵ
	 *   ����hash��64λ
	 */
	static std::wstring fileHash(std::wstring filePathName);

public:
	/**
	 * ����˵����
	 *   ����ĵ��Ƿ��ܹ�������
	 * ������
	 *   fileName���ļ���������׺
	 *  ����ֵ��
	 *   true�����Ա�������false�����ܽ���
	 */
	static bool checkDocParsed(std::wstring fileName);
};

#endif