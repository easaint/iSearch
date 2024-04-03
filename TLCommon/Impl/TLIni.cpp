#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include "../Export/TLIni.h"

namespace tl{

    // ���캯�������ʼ��ע���ַ�����flags_����������Ŀǰֻʹ��#��;��Ϊע��ǰ׺
    CTLIni::CTLIni()
        :commentDelimiter("#")
    {
    }

    //����һ�����ݣ��õ���ֵ
    /* --------------------------------------------------------------------------*/
    /**
     * @brief   parse
     *
     * @param   content ����Դָ��
     * @param   key     ����
     * @param   value   ��ֵ
     * @param   c       �ָ���
     *
     * @return  bool
     */
     /* ----------------------------------------------------------------------------*/
    bool CTLIni::parse(const string& content, string* key, string* value)
    {
        return split(content, "=", key, value);
    }

    int CTLIni::UpdateSection(const string& cleanLine, const string& comment,
        const string& rightComment, IniSection** section)
    {
        IniSection* newSection;
        // ������������
        size_t index = cleanLine.find_first_of(']');
        if (index == string::npos) {
            errMsg = string("no matched ] found");
            return ERR_UNMATCHED_BRACKETS;
        }

        int len = index - 1;
        // ������Ϊ�գ�������һ��
        if (len <= 0) {
            errMsg = string("section name is empty");
            return ERR_SECTION_EMPTY;
        }

        // ȡ����
        string s(cleanLine, 1, len);

        trim(s);

        //�����Ƿ��Ѵ���
        if (getSection(s) != NULL) {
            errMsg = string("section ") + s + string("already exist");
            return ERR_SECTION_ALREADY_EXISTS;
        }

        // ����һ���¶Σ�����map�������Զ����򣬴���ԭ��˳����˸���vector�洢��sections_vt��
        newSection = new IniSection();
        // ������
        newSection->name = s;
        // ���ο�ͷ��ע��
        newSection->comment = comment;
        newSection->rightComment = rightComment;

        sections_vt.push_back(newSection);

        *section = newSection;

        return 0;
    }

    int CTLIni::AddKeyValuePair(const string& cleanLine, const string& comment,
        const string& rightComment, IniSection* section)
    {
        string key, value;

        if (!parse(cleanLine, &key, &value)) {
            errMsg = string("parse line failed:") + cleanLine;
            return ERR_PARSE_KEY_VALUE_FAILED;
        }

        IniItem item;
        item.key = key;
        item.value = value;
        item.comment = comment;
        item.rightComment = rightComment;

        section->items.push_back(item);

        return 0;
    }

    int CTLIni::Load(const string& filePath)
    {
        int err;
        string line;  // ��ע�͵���
        string cleanLine;  // ȥ��ע�ͺ����
        string comment;
        string rightComment;
        IniSection* currSection = NULL;  // ��ʼ��һ���ֶ�ָ��

        release();

        iniFilePath = filePath;
        std::ifstream ifs(iniFilePath);
        if (!ifs.is_open()) {
            errMsg = string("open") + iniFilePath + string(" file failed");
            return ERR_OPEN_FILE_FAILED;
        }

        //����Ĭ�϶Σ��� ������""
        currSection = new IniSection();
        currSection->name = "";
        sections_vt.push_back(currSection);

        // ÿ�ζ�ȡһ�����ݵ�line
        while (std::getline(ifs, line)) {
            trim(line);

            // step 0�����д����������Ϊ0��˵���ǿ��У���ӵ�comment��������ע�͵�һ����
            if (line.length() <= 0) {
                comment += delim;
                continue;
            }

            // step 1
            // ������ײ���ע�ͣ�������β�Ƿ����ע��
            // ���������ע�Ϳ�ͷ����ӵ�comment��������ǰѭ����continue
            if (IsCommentLine(line)) {
                comment += line + delim;
                continue;
            }

            // ������ײ���ע�ͣ�������β�Ƿ����ע�ͣ������ڣ��и���У���ע��������ӵ�rightComment
            split(line, commentDelimiter, &cleanLine, &rightComment);

            // step 2���ж�line�����Ƿ�Ϊ�λ��
            //�ο�ͷ���� [
            if (cleanLine[0] == '[') {
                err = UpdateSection(cleanLine, comment, rightComment, &currSection);
            }
            else {
                // ��������Ǽ�ֵ����ӵ�section�ε�items����
                err = AddKeyValuePair(cleanLine, comment, rightComment, currSection);
            }

            if (err != 0) {
                ifs.close();
                return err;
            }

            // comment����
            comment = "";
            rightComment = "";
        }

        ifs.close();

        return 0;
    }

    int CTLIni::Save()
    {
        return SaveAs(iniFilePath);
    }

    int CTLIni::SaveAs(const string& filePath)
    {
        string data = "";

        /* ����section���� */
        for (IniSection_it sect = sections_vt.begin(); sect != sections_vt.end(); ++sect) {
            if ((*sect)->comment != "") {
                data += (*sect)->comment;
            }

            if ((*sect)->name != "") {
                data += string("[") + (*sect)->name + string("]");
                data += delim;
            }

            if ((*sect)->rightComment != "") {
                data += " " + commentDelimiter + (*sect)->rightComment;
            }

            /* ����item���� */
            for (IniSection::IniItem_it item = (*sect)->items.begin(); item != (*sect)->items.end(); ++item) {
                if (item->comment != "") {
                    data += item->comment;
                    if (data[data.length() - 1] != '\n') {
                        data += delim;
                    }
                }

                data += item->key + "=" + item->value;

                if (item->rightComment != "") {
                    data += " " + commentDelimiter + item->rightComment;
                }

                if (data[data.length() - 1] != '\n') {
                    data += delim;
                }
            }
        }

        std::ofstream ofs(filePath);
        ofs << data;
        ofs.close();
        return 0;
    }

    IniSection* CTLIni::getSection(const string& section /*=""*/)
    {
        for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ++it) {
            if ((*it)->name == section) {
                return *it;
            }
        }

        return NULL;
    }

    int CTLIni::GetSections(vector<string>* sections)
    {
        for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ++it) {
            sections->push_back((*it)->name);
        }

        return sections->size();
    }

    int CTLIni::GetSectionNum()
    {
        return sections_vt.size();
    }

    int CTLIni::GetStringValue(const string& section, const string& key, string* value)
    {
        return getValue(section, key, value);
    }

    int CTLIni::GetIntValue(const string& section, const string& key, int* intValue)
    {
        int err;
        string strValue;

        err = getValue(section, key, &strValue);

        *intValue = atoi(strValue.c_str());

        return err;
    }

    int CTLIni::GetDoubleValue(const string& section, const string& key, double* value)
    {
        int err;
        string strValue;

        err = getValue(section, key, &strValue);

        *value = atof(strValue.c_str());

        return err;
    }

    int CTLIni::GetBoolValue(const string& section, const string& key, bool* value)
    {
        int err;
        string strValue;

        err = getValue(section, key, &strValue);

        if (StringCmpIgnoreCase(strValue, "true") || StringCmpIgnoreCase(strValue, "1")) {
            *value = true;
        }
        else if (StringCmpIgnoreCase(strValue, "false") || StringCmpIgnoreCase(strValue, "0")) {
            *value = false;
        }

        return err;
    }

    /* ��ȡsection�ε�һ����Ϊkey��stringֵ���ɹ����ػ�ȡ��ֵ�����򷵻�Ĭ��ֵ */
    void CTLIni::GetStringValueOrDefault(const string& section, const string& key,
        string* value, const string& defaultValue)
    {
        if (GetStringValue(section, key, value) != 0) {
            *value = defaultValue;
        }

        return;
    }

    /* ��ȡsection�ε�һ����Ϊkey��intֵ���ɹ����ػ�ȡ��ֵ�����򷵻�Ĭ��ֵ */
    void CTLIni::GetIntValueOrDefault(const string& section, const string& key, int* value, int defaultValue)
    {
        if (GetIntValue(section, key, value) != 0) {
            *value = defaultValue;
        }

        return;
    }

    /* ��ȡsection�ε�һ����Ϊkey��doubleֵ���ɹ����ػ�ȡ��ֵ�����򷵻�Ĭ��ֵ */
    void CTLIni::GetDoubleValueOrDefault(const string& section, const string& key, double* value, double defaultValue)
    {
        if (GetDoubleValue(section, key, value) != 0) {
            *value = defaultValue;
        }

        return;
    }

    /* ��ȡsection�ε�һ����Ϊkey��boolֵ���ɹ����ػ�ȡ��ֵ�����򷵻�Ĭ��ֵ */
    void CTLIni::GetBoolValueOrDefault(const string& section, const string& key, bool* value, bool defaultValue)
    {
        if (GetBoolValue(section, key, value) != 0) {
            *value = defaultValue;
        }

        return;
    }

    /* ��ȡע�ͣ����key=""���ȡ��ע�� */
    int CTLIni::GetComment(const string& section, const string& key, string* comment)
    {
        IniSection* sect = getSection(section);

        if (sect == NULL) {
            errMsg = string("not find the section ") + section;
            return ERR_NOT_FOUND_SECTION;
        }

        if (key == "") {
            *comment = sect->comment;
            return RET_OK;
        }

        for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
            if (it->key == key) {
                *comment = it->comment;
                return RET_OK;
            }
        }

        errMsg = string("not find the key ") + section;
        return ERR_NOT_FOUND_KEY;
    }

    /* ��ȡ��βע�ͣ����key=""���ȡ�ε���βע�� */
    int CTLIni::GetRightComment(const string& section, const string& key, string* rightComment)
    {
        IniSection* sect = getSection(section);

        if (sect == NULL) {
            errMsg = string("not find the section ") + section;
            return ERR_NOT_FOUND_SECTION;
        }

        if (key == "") {
            *rightComment = sect->rightComment;
            return RET_OK;
        }

        for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
            if (it->key == key) {
                *rightComment = it->rightComment;
                return RET_OK;
            }
        }

        errMsg = string("not find the key ") + key;
        return ERR_NOT_FOUND_KEY;
    }

    int CTLIni::getValue(const string& section, const string& key, string* value)
    {
        string comment;
        return getValue(section, key, value, &comment);
    }

    int CTLIni::getValue(const string& section, const string& key, string* value, string* comment)
    {
        IniSection* sect = getSection(section);

        if (sect == NULL) {
            errMsg = string("not find the section ") + section;
            return ERR_NOT_FOUND_SECTION;
        }

        for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
            if (it->key == key) {
                *value = it->value;
                *comment = it->comment;
                return RET_OK;
            }
        }

        errMsg = string("not find the key ") + key;
        return ERR_NOT_FOUND_KEY;
    }

    int CTLIni::GetValues(const string& section, const string& key, vector<string>* values)
    {
        vector<string> comments;
        return GetValues(section, key, values, &comments);
    }

    int CTLIni::GetValues(const string& section, const string& key, vector<string>* values, vector<string>* comments)
    {
        string value, comment;

        values->clear();
        comments->clear();

        IniSection* sect = getSection(section);

        if (sect == NULL) {
            errMsg = string("not find the section ") + section;
            return ERR_NOT_FOUND_SECTION;
        }

        for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
            if (it->key == key) {
                value = it->value;
                comment = it->comment;

                values->push_back(value);
                comments->push_back(comment);
            }
        }

        if (values->size() == 0) {
            errMsg = string("not find the key ") + key;
            return ERR_NOT_FOUND_KEY;
        }

        return RET_OK;
    }

    bool CTLIni::HasSection(const string& section)
    {
        return (getSection(section) != NULL);
    }

    bool CTLIni::HasKey(const string& section, const string& key)
    {
        IniSection* sect = getSection(section);

        if (sect != NULL) {
            for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
                if (it->key == key) {
                    return true;
                }
            }
        }

        return false;
    }

    int CTLIni::setValue(const string& section, const string& key, const string& value, const string& comment /*=""*/)
    {
        IniSection* sect = getSection(section);

        string comt = comment;

        if (comt != "") {
            comt = commentDelimiter + comt;
        }

        if (sect == NULL) {
            //����β����ڣ��½�һ��
            sect = new IniSection();

            if (sect == NULL) {
                errMsg = string("no enough memory!");
                return ERR_NO_ENOUGH_MEMORY;
            }

            sect->name = section;
            if (sect->name == "") {
                // ȷ����section�ڵ�һ��
                sections_vt.insert(sections_vt.begin(), sect);
            }
            else {
                sections_vt.push_back(sect);
            }
        }

        for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
            if (it->key == key) {
                it->value = value;
                it->comment = comt;
                return RET_OK;
            }
        }

        // not found key
        IniItem item;
        item.key = key;
        item.value = value;
        item.comment = comt;

        sect->items.push_back(item);

        return RET_OK;
    }

    int CTLIni::SetStringValue(const string& section, const string& key, const string& value)
    {
        return setValue(section, key, value);
    }

    int CTLIni::SetIntValue(const string& section, const string& key, int value)
    {
        char buf[64] = { 0 };
        sprintf_s(buf, sizeof(buf), "%d", value);
        return setValue(section, key, buf);
    }

    int CTLIni::SetDoubleValue(const string& section, const string& key, double value)
    {
        char buf[64] = { 0 };
        sprintf_s(buf, sizeof(buf), "%f", value);
        return setValue(section, key, buf);
    }

    int CTLIni::SetBoolValue(const string& section, const string& key, bool value)
    {
        if (value) {
            return setValue(section, key, "true");
        }
        else {
            return setValue(section, key, "false");
        }
    }

    int CTLIni::SetComment(const string& section, const string& key, const string& comment)
    {
        IniSection* sect = getSection(section);

        if (sect == NULL) {
            errMsg = string("Not find the section ") + section;
            return ERR_NOT_FOUND_SECTION;
        }

        if (key == "") {
            sect->comment = comment;
            return RET_OK;
        }

        for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
            if (it->key == key) {
                it->comment = comment;
                return RET_OK;
            }
        }

        errMsg = string("not find the key ") + key;
        return ERR_NOT_FOUND_KEY;
    }

    int CTLIni::SetRightComment(const string& section, const string& key, const string& rightComment)
    {
        IniSection* sect = getSection(section);

        if (sect == NULL) {
            errMsg = string("Not find the section ") + section;
            return ERR_NOT_FOUND_SECTION;
        }

        if (key == "") {
            sect->rightComment = rightComment;
            return RET_OK;
        }

        for (IniSection::IniItem_it it = sect->begin(); it != sect->end(); ++it) {
            if (it->key == key) {
                it->rightComment = rightComment;
                return RET_OK;
            }
        }

        errMsg = string("not find the key ") + key;
        return ERR_NOT_FOUND_KEY;
    }

    void CTLIni::SetCommentDelimiter(const string& delimiter)
    {
        commentDelimiter = delimiter;
    }

    void CTLIni::DeleteSection(const string& section)
    {
        for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ) {
            if ((*it)->name == section) {
                delete* it;
                it = sections_vt.erase(it);
                break;
            }
            else {
                it++;
            }
        }
    }

    void CTLIni::DeleteKey(const string& section, const string& key)
    {
        IniSection* sect = getSection(section);

        if (sect != NULL) {
            for (IniSection::IniItem_it it = sect->begin(); it != sect->end();) {
                if (it->key == key) {
                    it = sect->items.erase(it);
                    break;
                }
                else {
                    it++;
                }
            }
        }
    }

    /*-------------------------------------------------------------------------*/
    /**
      @brief    release: �ͷ�ȫ����Դ���������
      @param    none
      @return   none
     */
     /*--------------------------------------------------------------------------*/
    void CTLIni::release()
    {
        iniFilePath = "";

        for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ++it) {
            delete (*it);  // ���section
        }

        sections_vt.clear();
    }

    /*-------------------------------------------------------------------------*/
    /**
      @brief    �ж��Ƿ���ע��
      @param    str һ��string����
      @return   �����ע����Ϊ��
     */
     /*--------------------------------------------------------------------------*/
    bool CTLIni::IsCommentLine(const string& str)
    {
        return StartWith(str, commentDelimiter);
    }

    /*-------------------------------------------------------------------------*/
    /**
      @brief    print for debug
      @param    none
      @return   none
     */
     /*--------------------------------------------------------------------------*/
    void CTLIni::print()
    {
        printf("############ print start ############\n");
        printf("filePath:[%s]\n", iniFilePath.c_str());
        printf("commentDelimiter:[%s]\n", commentDelimiter.c_str());

        for (IniSection_it it = sections_vt.begin(); it != sections_vt.end(); ++it) {
            printf("comment :[\n%s]\n", (*it)->comment.c_str());
            printf("section :\n[%s]\n", (*it)->name.c_str());
            if ((*it)->rightComment != "") {
                printf("rightComment:\n%s", (*it)->rightComment.c_str());
            }

            for (IniSection::IniItem_it i = (*it)->items.begin(); i != (*it)->items.end(); ++i) {
                printf("    comment :[\n%s]\n", i->comment.c_str());
                printf("    parm    :%s=%s\n", i->key.c_str(), i->value.c_str());
                if (i->rightComment != "") {
                    printf("    rcomment:[\n%s]\n", i->rightComment.c_str());
                }
            }
        }

        printf("############ print end ############\n");
        return;
    }

    const string& CTLIni::GetErrMsg()
    {
        return errMsg;
    }

    bool CTLIni::StartWith(const string& str, const string& prefix)
    {
        if (strncmp(str.c_str(), prefix.c_str(), prefix.size()) == 0) {
            return true;
        }

        return false;
    }

    void CTLIni::trimleft(string& str, char c /*=' '*/)
    {
        int len = str.length();

        int i = 0;

        while (str[i] == c && str[i] != '\0') {
            i++;
        }

        if (i != 0) {
            str = string(str, i, len - i);
        }
    }

    void CTLIni::trimright(string& str, char c /*=' '*/)
    {
        int i = 0;
        int len = str.length();

        for (i = len - 1; i >= 0; --i) {
            if (str[i] != c) {
                break;
            }
        }

        str = string(str, 0, i + 1);
    }

    /*-------------------------------------------------------------------------*/
    /**
      @brief    trim������һ���ַ�����ȥ����β�ո�
      @param    str string����
     */
     /*--------------------------------------------------------------------------*/
    void CTLIni::trim(string& str)
    {
        int len = str.length();

        int i = 0;

        while ((i < len) && isspace(str[i]) && (str[i] != '\0')) {
            i++;
        }

        if (i != 0) {
            str = string(str, i, len - i);
        }

        len = str.length();

        for (i = len - 1; i >= 0; --i) {
            if (!isspace(str[i])) {
                break;
            }
        }

        str = string(str, 0, i + 1);
    }

    /*-------------------------------------------------------------------------*/
    /**
      @brief    split���÷ָ����и��ַ���
      @param    str �����ַ���
      @param    left_str �ָ���õ������ַ���
      @param    right_str �ָ���õ������ַ�����������seperator��
      @param    seperator �ָ���
      @return   pos
     */
     /*--------------------------------------------------------------------------*/
    bool CTLIni::split(const string& str, const string& sep, string* pleft, string* pright)
    {
        size_t pos = str.find(sep);
        string left, right;

        if (pos != string::npos) {
            left = string(str, 0, pos);
            right = string(str, pos + 1);

            trim(left);
            trim(right);

            *pleft = left;
            *pright = right;
            return true;
        }
        else {
            left = str;
            right = "";

            trim(left);

            *pleft = left;
            *pright = right;
            return false;
        }
    }

    bool CTLIni::StringCmpIgnoreCase(const string& str1, const string& str2)
    {
        string a = str1;
        string b = str2;
        transform(a.begin(), a.end(), a.begin(), towupper);
        transform(b.begin(), b.end(), b.begin(), towupper);

        return (a == b);
    }
}