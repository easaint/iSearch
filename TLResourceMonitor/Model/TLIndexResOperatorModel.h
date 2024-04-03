#ifndef _TL_INDEX_RES_OPERATOR_MODEL_H
#define _TL_INDEX_RES_OPERATOR_MODEL_H

#include <string>

//索引操作符
enum IndexOperatorEnum {
	Insert,
	Update,
	Remove
};

/**
 * 索引操作符模型
 */
class CTLIndexResOperatorModel {

public:
	CTLIndexResOperatorModel() {}
	CTLIndexResOperatorModel(std::wstring filePathName, IndexOperatorEnum idxOpt) : m_filePathName(filePathName), m_idxOpt(idxOpt) {}
	~CTLIndexResOperatorModel() {}

	CTLIndexResOperatorModel& operator=(const CTLIndexResOperatorModel& irom) {
		if (this != &irom) {
			this->m_filePathName = irom.m_filePathName;
			this->m_idxOpt = irom.m_idxOpt;
		}
		return *this;
	}

public:
	std::wstring getFilePathName() { return this->m_filePathName; }
	IndexOperatorEnum getIdxOpt() { return this->m_idxOpt; }
private:
	std::wstring m_filePathName;
	IndexOperatorEnum m_idxOpt;
};

#endif