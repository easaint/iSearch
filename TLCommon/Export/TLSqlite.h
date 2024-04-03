/**
 * ��ģ����Ҫ�Ƿ�װsqlite�Ĳ����࣬��Ҫ����sqlite3����Ӧ�����Ĳ�������Ҫ����UTF8����,sqlite3Ĭ������utf8�洢
 */
#ifndef _TL_SQLITE_H
#define _TL_SQLITE_H
#include <string>
#include <list>
#include <exception>

#include "TLCommonBase.h"

namespace tl {

	template class __declspec(dllexport) std::basic_string<char, std::char_traits<char>, std::allocator<char>>;

	/**
	 * �ж���
	 */
	struct Column {
		std::string name;//�ֶ�����
		std::string type;//�ֶ�����
		std::string value;//�ֶ�ֵ
		Column& operator=(const Column& c)
		{
			if (this != &c)
			{
				this->name = c.name;
				this->type = c.type;
				this->value = c.value;
			}
			return *this;
		}
	};

	/**
	 * �ж���
	 */
	struct Row {
		int columnCount;//������
		std::list<Column> columns;//��
		Row& operator=(const Row& r)
		{
			if (this != &r)
			{
				this->columnCount = r.columnCount;
				if (r.columns.size() > 0)
				{
					std::list<Column> tmpList = r.columns;
					std::list<Column>::iterator it = tmpList.begin();
					while (it != tmpList.end())
					{
						this->columns.push_back(*it++);
					}
				}
			}
			return *this;
		}
	};

	/**
	 * ����
	 */
	struct Table {
		int rowCount;//������
		std::list<Row> rows;//���б�
		Table& operator=(const Table& t)
		{
			if (this != &t)
			{
				this->rowCount = t.rowCount;
				if (t.rows.size() > 0)
				{
					std::list<Row> tmpList = t.rows;
					std::list<Row>::iterator it = tmpList.begin();
					while (it != tmpList.end())
					{
						this->rows.push_back(*it++);
					}
				}
			}
			return *this;
		}
	};	

	/**
	 * ����sql�쳣
	 */
	class SqlException : public std::exception {
	public:
		SqlException(const std::string& message) : message_(message) {}

		const char* what() const override {
			return message_.c_str();
		}

	private:
		std::string message_;
	};

	/**
	 * sqlite ������
	 */
	class TL_COMMON_API_ CTLSqlite {
	public:
		CTLSqlite(std::string dbPathName);
		~CTLSqlite();

	public:
		/**
		 * ����˵��
		 *   �����ݿ⣬������ݿⲻ�����򴴽�
		 * ����ֵ
		 *   true���ɹ���false��ʧ��
		 */
		bool open();

		/***
		 * ����˵��
		 *   �жϱ��Ƿ����
		 *   �׳��쳣��SqlException
		 * ������
		 *   tableName������
		 * ����ֵ
		 *   true�����ڣ�false��������
		 */
		bool tableExist(std::string tableName);

		/**
		 * ����˵��
		 *   ������
		 *   �׳��쳣��SqlException
		 * ����ֵ
		 *   true���ɹ���false��ʧ��
		 */
		bool createTable(std::string ddl);

		/**
		 * ����˵��
		 *   ɾ����
		 *   �׳��쳣��SqlException
		 * ����ֵ
		 *   true���ɹ���false��ʧ��
		 */
		bool deleteTable(std::string tableName);

		/**
		 * ����˵��
		 *   ���������
		 *   �׳��쳣��SqlException
		 * ����
		 *   sql������sql��insert into table ....
		 * ����ֵ
		 *   �ɹ�����0��ʧ�ܷ���-1
		 */
		int insertData(std::string sql);

		/**
		 * ����˵��
		 *   ɾ��������
		 *   �׳��쳣��SqlException
		 * ����
		 *   sql��ɾ��sql��delete from table where ...
		 * ����ֵ
		 *   �ɹ�����0��ʧ�ܷ���-1
		 */
		int deleteData(std::string sql);

		/**
		 * ����˵��
		 *   �޸ı�����
		 *   �׳��쳣��SqlException
		 * ����
		 *   sql���޸�sql��update table set ...
		 * ����ֵ
		 *   �ɹ�����0��ʧ�ܷ���-1
		 */
		int updateData(std::string sql);

		/**
		 * ����˵��
		 *   ��ѯ����
		 *   �׳��쳣��SqlException
		 * ����
		 *   sql����ѯsql��select xxx from xxx
		 * ����ֵ
		 *   ���ز�ѯ���ı�����
		 */
		Table queryData(std::string sql);

	private:
		void* m_pDb; /*DB����*/
		std::string m_dbPathName;
	};

}
#endif