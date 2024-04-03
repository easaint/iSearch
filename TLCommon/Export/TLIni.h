#ifndef _TL_INIT_H
#define _TL_INIT_H
#include "TLCommonBase.h"

#include <vector>
#include <algorithm>
#include <string>
#include <string.h>

using std::string;
using std::vector;

#define RET_OK 0
// û���ҵ�ƥ���]
#define ERR_UNMATCHED_BRACKETS 2
// ��Ϊ��
#define ERR_SECTION_EMPTY 3
// �����Ѿ�����
#define ERR_SECTION_ALREADY_EXISTS 4
// ����key valueʧ��
#define ERR_PARSE_KEY_VALUE_FAILED 5
// ���ļ�ʧ��
#define ERR_OPEN_FILE_FAILED 6
// �ڴ治��
#define ERR_NO_ENOUGH_MEMORY 7
// û���ҵ���Ӧ��key
#define ERR_NOT_FOUND_KEY 8
// û���ҵ���Ӧ��section
#define ERR_NOT_FOUND_SECTION 9

namespace tl {

	const char delim[] = "\n";
	struct IniItem {
		string key;
		string value;
		string comment;  // ÿ������ע�ͣ�����ָ�����Ϸ�������
		string rightComment;
	};

	struct IniSection {
		typedef vector<IniItem>::iterator IniItem_it;  // ����һ������������ָ���Ԫ�ص�ָ��
		IniItem_it begin() {
			return items.begin();  // �νṹ���beginԪ��ָ��items��ͷָ��
		}

		IniItem_it end() {
			return items.end();  // �νṹ���beginԪ��ָ��items��βָ��
		}

		string name;
		string comment;  // ÿ���ε�ע�ͣ�����ָ�����Ϸ�������
		string rightComment;
		vector<IniItem> items;  // ��ֵ�����飬һ���ο����ж����ֵ��������vector������
	};

	/**
	 * ini�����ļ��Ķ�ȡ
	 */
	class TL_COMMON_API_ CTLIni {

	public:
    public:
        CTLIni();
        ~CTLIni() {
            release();
        }

    public:
        /* �򿪲�����һ����Ϊfname��INI�ļ� */
        int Load(const string& fname);
        /* �����ݱ��浽��ǰ�ļ� */
        int Save();
        /* ��������浽һ����Ϊfname���ļ� */
        int SaveAs(const string& fname);

        /* ��ȡsection�ε�һ����Ϊkey��stringֵ���ɹ�����0�����򷵻ش����� */
        int GetStringValue(const string& section, const string& key, string* value);
        /* ��ȡsection�ε�һ����Ϊkey��intֵ���ɹ�����0�����򷵻ش����� */
        int GetIntValue(const string& section, const string& key, int* value);
        /* ��ȡsection�ε�һ����Ϊkey��doubleֵ���ɹ�����0�����򷵻ش����� */
        int GetDoubleValue(const string& section, const string& key, double* value);
        /* ��ȡsection�ε�һ����Ϊkey��boolֵ���ɹ�����0�����򷵻ش����� */
        int GetBoolValue(const string& section, const string& key, bool* value);
        /* ��ȡע�ͣ����key=""���ȡ��ע�� */
        int GetComment(const string& section, const string& key, string* comment);
        /* ��ȡ��βע�ͣ����key=""���ȡ�ε���βע�� */
        int GetRightComment(const string& section, const string& key, string* rightComment);

        /* ��ȡsection�ε�һ����Ϊkey��stringֵ���ɹ����ػ�ȡ��ֵ�����򷵻�Ĭ��ֵ */
        void GetStringValueOrDefault(const string& section, const string& key, string* value, const string& defaultValue);
        /* ��ȡsection�ε�һ����Ϊkey��intֵ���ɹ����ػ�ȡ��ֵ�����򷵻�Ĭ��ֵ */
        void GetIntValueOrDefault(const string& section, const string& key, int* value, int defaultValue);
        /* ��ȡsection�ε�һ����Ϊkey��doubleֵ���ɹ����ػ�ȡ��ֵ�����򷵻�Ĭ��ֵ */
        void GetDoubleValueOrDefault(const string& section, const string& key, double* value, double defaultValue);
        /* ��ȡsection�ε�һ����Ϊkey��boolֵ���ɹ����ػ�ȡ��ֵ�����򷵻�Ĭ��ֵ */
        void GetBoolValueOrDefault(const string& section, const string& key, bool* value, bool defaultValue);

        /* ��ȡsection�����м�Ϊkey��ֵ,����ֵ����values��vector�� */
        int GetValues(const string& section, const string& key, vector<string>* values);

        /* ��ȡsection�б�,������section���� */
        int GetSections(vector<string>* sections);
        /* ��ȡsection���������ٴ���һ����section */
        int GetSectionNum();
        /* �Ƿ����ĳ��section */
        bool HasSection(const string& section);
        /* ��ȡָ��section������ken=value��Ϣ */
        IniSection* getSection(const string& section = "");

        /* �Ƿ����ĳ��key */
        bool HasKey(const string& section, const string& key);

        /* �����ַ���ֵ */
        int SetStringValue(const string& section, const string& key, const string& value);
        /* ��������ֵ */
        int SetIntValue(const string& section, const string& key, int value);
        /* ���ø�����ֵ */
        int SetDoubleValue(const string& section, const string& key, double value);
        /* ���ò���ֵ */
        int SetBoolValue(const string& section, const string& key, bool value);
        /* ����ע�ͣ����key=""�����ö�ע�� */
        int SetComment(const string& section, const string& key, const string& comment);
        /* ������βע�ͣ����key=""�����öε���βע�� */
        int SetRightComment(const string& section, const string& key, const string& rightComment);

        /* ɾ���� */
        void DeleteSection(const string& section);
        /* ɾ���ض��ε��ض����� */
        void DeleteKey(const string& section, const string& key);
        /*����ע�ͷָ�����Ĭ��Ϊ"#" */
        void SetCommentDelimiter(const string& delimiter);

        const string& GetErrMsg();
    private:
        /* ��ȡsection�����м�Ϊkey��ֵ,����ֵ����values��vector��,,��ע�͸���comments��vector�� */
        int GetValues(const string& section, const string& key, vector<string>* value, vector<string>* comments);

        /* ͬʱ����ֵ��ע�� */
        int setValue(const string& section, const string& key, const string& value, const string& comment = "");

        /* ȥ��strǰ���c�ַ� */
        static void trimleft(string& str, char c = ' ');
        /* ȥ��str�����c�ַ� */
        static void trimright(string& str, char c = ' ');
        /* ȥ��strǰ��ͺ���Ŀո��,Tab���ȿհ׷� */
        static void trim(string& str);
        /* ���ַ���str���ָ��delim�ָ�ɶ���Ӵ� */
    private:
        int UpdateSection(const string& cleanLine, const string& comment,
            const string& rightComment, IniSection** section);
        int AddKeyValuePair(const string& cleanLine, const string& comment,
            const string& rightComment, IniSection* section);

        void release();
        bool split(const string& str, const string& sep, string* left, string* right);
        bool IsCommentLine(const string& str);
        bool parse(const string& content, string* key, string* value);
        // for debug
        void print();

    private:
        /* ��ȡsection�ε�һ����Ϊkey��ֵ,����ֵ����value�� */
        int getValue(const string& section, const string& key, string* value);
        /* ��ȡsection�ε�һ����Ϊkey��ֵ,����ֵ����value��,��ע�͸���comment�� */
        int getValue(const string& section, const string& key, string* value, string* comment);

        bool StringCmpIgnoreCase(const string& str1, const string& str2);
        bool StartWith(const string& str, const string& prefix);

    private:
        typedef vector<IniSection*>::iterator IniSection_it;
        vector<IniSection*> sections_vt;  // ���ڴ洢�μ��ϵ�vector����
        string iniFilePath;
        string commentDelimiter;
        string errMsg;  // �������ʱ�Ĵ�����Ϣ
	};

}

#endif