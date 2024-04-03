/**
 * 该模块主要是封装sqlite的操作类，主要是用sqlite3，对应操作的参数都需要传入UTF8编码,sqlite3默认是用utf8存储
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
	 * 列定义
	 */
	struct Column {
		std::string name;//字段名称
		std::string type;//字段类型
		std::string value;//字段值
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
	 * 行定义
	 */
	struct Row {
		int columnCount;//列数量
		std::list<Column> columns;//列
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
	 * 表定义
	 */
	struct Table {
		int rowCount;//行数量
		std::list<Row> rows;//行列表
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
	 * 定义sql异常
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
	 * sqlite 操作类
	 */
	class TL_COMMON_API_ CTLSqlite {
	public:
		CTLSqlite(std::string dbPathName);
		~CTLSqlite();

	public:
		/**
		 * 函数说明
		 *   打开数据库，如果数据库不存在则创建
		 * 返回值
		 *   true：成功，false：失败
		 */
		bool open();

		/***
		 * 函数说明
		 *   判断表是否存在
		 *   抛出异常：SqlException
		 * 参数：
		 *   tableName：表名
		 * 返回值
		 *   true：存在，false：不存在
		 */
		bool tableExist(std::string tableName);

		/**
		 * 函数说明
		 *   创建表
		 *   抛出异常：SqlException
		 * 返回值
		 *   true：成功，false：失败
		 */
		bool createTable(std::string ddl);

		/**
		 * 函数说明
		 *   删除表
		 *   抛出异常：SqlException
		 * 返回值
		 *   true：成功，false：失败
		 */
		bool deleteTable(std::string tableName);

		/**
		 * 函数说明
		 *   插入表数据
		 *   抛出异常：SqlException
		 * 参数
		 *   sql：插入sql，insert into table ....
		 * 返回值
		 *   成功返回0，失败返回-1
		 */
		int insertData(std::string sql);

		/**
		 * 函数说明
		 *   删除表数据
		 *   抛出异常：SqlException
		 * 参数
		 *   sql：删除sql，delete from table where ...
		 * 返回值
		 *   成功返回0，失败返回-1
		 */
		int deleteData(std::string sql);

		/**
		 * 函数说明
		 *   修改表数据
		 *   抛出异常：SqlException
		 * 参数
		 *   sql：修改sql，update table set ...
		 * 返回值
		 *   成功返回0，失败返回-1
		 */
		int updateData(std::string sql);

		/**
		 * 函数说明
		 *   查询数据
		 *   抛出异常：SqlException
		 * 参数
		 *   sql：查询sql，select xxx from xxx
		 * 返回值
		 *   返回查询到的表数据
		 */
		Table queryData(std::string sql);

	private:
		void* m_pDb; /*DB对象*/
		std::string m_dbPathName;
	};

}
#endif