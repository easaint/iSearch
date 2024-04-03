#include "../Export/TLSqlite.h"
#include "../ThirdParty/sqlite/sqlite3.h"
#include "../Export/TLPath.h"

#include <iostream>

namespace tl {
	CTLSqlite::CTLSqlite(std::string dbPathName)
	{
		this->m_dbPathName = dbPathName;
		this->m_pDb = nullptr;
	}

	CTLSqlite::~CTLSqlite()
	{
		if (this->m_pDb)
		{
			sqlite3_close((sqlite3*)this->m_pDb);
			this->m_pDb = nullptr;
		}
	}


	bool CTLSqlite::open()
	{
		int rc = sqlite3_open(this->m_dbPathName.c_str(), (sqlite3**)&this->m_pDb);
		if (rc != SQLITE_OK)
			return false;
		return true;
	}

	bool CTLSqlite::tableExist(std::string tableName)
	{
		std::string sql = "SELECT * FROM sqlite_master WHERE type = 'table' AND name = '";
		sql += tableName;
		sql += "'";

		int nCols = -1;
		int nRows = -1;
		char** azResult = NULL;
		char* errMsg = NULL;
		int rc = sqlite3_get_table((sqlite3*)this->m_pDb, sql.c_str(), &azResult, &nRows, &nCols, &errMsg);
		if (rc != SQLITE_OK)
		{
			//需要做异常抛出
			std::string errstr = errMsg;
			sqlite3_free(errMsg);
			throw SqlException(errstr);
		}
		if (nRows > 0)
		{
			return true;
		}
		return false;
	}

	bool CTLSqlite::createTable(std::string ddl)
	{
		char* errMsg;
		int rc = sqlite3_exec((sqlite3*)this->m_pDb, ddl.c_str(), NULL, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			//需要做异常抛出
			std::string errstr = errMsg;
			sqlite3_free(errMsg);
			throw SqlException(errstr);
			return false;
		}
		return true;
	}

	bool CTLSqlite::deleteTable(std::string tableName)
	{
		std::string sql = "DROP TABLE " + tableName;
		char* errMsg;
		int rc = sqlite3_exec((sqlite3*)this->m_pDb, sql.c_str(), NULL, 0, &errMsg);
		if (rc != SQLITE_OK)
		{
			//需要做异常抛出
			std::string errstr = errMsg;
			sqlite3_free(errMsg);
			throw SqlException(errstr);
			return false;
		}
		return true;
	}

	int CTLSqlite::insertData(std::string sql)
	{
		char* errMsg;
		int rc = sqlite3_exec((sqlite3*)this->m_pDb, sql.c_str(), NULL, NULL, &errMsg);
		if (rc != SQLITE_OK)
		{
			//需要做异常抛出
			std::string errstr = errMsg;
			sqlite3_free(errMsg);
			throw SqlException(errstr);
		}
		return 0;
	}

	int CTLSqlite::deleteData(std::string sql)
	{
		int nCols = 0;
		int nRows = 0;
		char** azResult = NULL;
		char* errMsg = NULL;
		int res = sqlite3_get_table((sqlite3*)this->m_pDb, sql.c_str(), &azResult, &nRows, &nCols, &errMsg);
		if (res != SQLITE_OK) {
			//需要做异常抛出
			std::string errstr = errMsg;
			sqlite3_free(errMsg);
			throw SqlException(errstr);
		}
		if (azResult) {
			sqlite3_free_table(azResult);
		}
		return 0;
	}

	int CTLSqlite::updateData(std::string sql)
	{
		char* zErrMsg = NULL;
		int ret = sqlite3_exec((sqlite3*)this->m_pDb, sql.c_str(), NULL, NULL, &zErrMsg);
		if (ret != SQLITE_OK) {
			//需要做异常抛出
			std::string errstr = zErrMsg;
			sqlite3_free(zErrMsg);
			throw SqlException(errstr);
		}
		return 0;
	}

	Table CTLSqlite::queryData(std::string sql)
	{
		Table t;
		t.rowCount = 0;
		char** pResult;
		int nRow;
		int nCol;
		char* zErrMsg = NULL;
		int ret = sqlite3_get_table((sqlite3*)this->m_pDb, sql.c_str(), &pResult, &nRow, &nCol, &zErrMsg);
		if (ret != SQLITE_OK)
		{
			std::string errstr = zErrMsg;
			sqlite3_free(zErrMsg);
			//抛出异常
			throw SqlException(errstr);
		}
		
		int nIndex = nCol;
		int i = 0;
		for (; i < nRow; i++)
		{
			Row r;
			r.columnCount = 0;
			int j = 0;
			for (; j < nCol; j++)
			{
				Column c;
				c.name = pResult[j];
				c.value = pResult[nIndex];
				++nIndex;
				r.columns.push_back(c);
			}
			r.columnCount = j;
			t.rows.push_back(r);
		}
		t.rowCount = i;
		sqlite3_free_table(pResult);
		return t;
	}
}