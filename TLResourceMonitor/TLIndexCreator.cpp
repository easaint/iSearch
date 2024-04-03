#include "TLIndexCreator.h"
#include "TLSearchEngine.h"
#include "../TLCommon/Export/TLEncoder.h"
#include "../TLDocReader/Export/TLAnyDocReader.h"
#include "../TLCommon/Export/TLString.h"
#include "../TLCommon/Export/TLDatetime.h"
#include "../TLCommon/Export/TLPath.h"
#include "Util/zip.h"
#include "Util/unzip.h"
#include "TLResPath.h"
#include "TLResMonitorBase.h"

CTLIndexCreator::CTLIndexCreator(std::string indexPath, tl::CLogger* pLogger, tl::CTLSqlite* pSqlite)
{
	this->m_indexPath = indexPath;
	this->m_pLogger = pLogger;
	this->m_bThreadLoop = true;
	this->m_pSqlite = pSqlite;
}

CTLIndexCreator::~CTLIndexCreator()
{

}

bool CTLIndexCreator::init()
{
	int ret = TLSE_Init((char*)this->m_indexPath.c_str(), false, false);
	if (ret != 0)
	{
		this->m_pLogger->error(FILE_STACK + "不能初始化检索引擎...");
		return false;
	}

	//首先检测索引是否有效完整，必须放在创建索引之前，因为如果索引无效，TLSE_IndexExist函数则返回不存在-1，导致会重新创建索引结构
	//如果索引文件太大，此操作会导致服务启动失败
	//this->checkIndexValid();

	if (TLSE_IndexExist(INDEX_NAME) == -1) //不存在则创建索引
	{
		//创建索引
		TLIndexField idField;
		idField.index = 3;
		strcpy_s(idField.name, "id");
		idField.store = 1;

		TLIndexField fileHashField;
		fileHashField.index = 3;
		strcpy_s(fileHashField.name, "file_hash");
		fileHashField.store = 1;

		TLIndexField fileNameField;
		fileNameField.index = 2;
		strcpy_s(fileNameField.name, "file_name");
		fileNameField.store = 1;

		TLIndexField filePathField;
		filePathField.index = 3;
		strcpy_s(filePathField.name, "file_path");
		filePathField.store = 1;

		TLIndexField fileContentField;
		fileContentField.index = 2;
		strcpy_s(fileContentField.name, "file_content");
		fileContentField.store = 1;

		TLIndexField fileSuffixField;
		fileSuffixField.index = 3;
		strcpy_s(fileSuffixField.name, "file_suffix");
		fileSuffixField.store = 1;

		TLIndexField* fields[6] = { 0 };
		fields[0] = &idField;
		fields[1] = &fileHashField;
		fields[2] = &fileNameField;
		fields[3] = &filePathField;
		fields[4] = &fileContentField;
		fields[5] = &fileSuffixField;

		ret = TLSE_IndexCreate(INDEX_NAME, fields, 6);
		if (ret != 0 && ret != -2)
		{
			this->m_pLogger->error(FILE_STACK + "创建索引失败");
			return false;
		}

		try 
		{
			//重新创建记录表
			if (this->m_pSqlite->tableExist(T_INDEX_META_DATA))
				this->m_pSqlite->deleteTable(T_INDEX_META_DATA);

			//创建表
			if (!this->m_pSqlite->createTable(T_INDEX_META_DATA_CREATE_SQL))
				return false;
		}
		catch (const tl::SqlException& e)
		{
			std::string errmsg = FILE_STACK + "重新创建索引表失败：";
			errmsg += e.what();
			this->m_pLogger->error(errmsg);
		}
	}

	//判断当天的索引是否备份，如果没有备份则需要进行备份
	//如果索引文件太大，此操作会导致服务启动失败
	//this->backupIndex();

	//创建索引线程
	this->m_indexThread = std::thread(&CTLIndexCreator::threadLoop, this);
	return true;
}

void CTLIndexCreator::unint()
{
	//等待线程
	this->m_bThreadLoop = false;
	if (this->m_indexThread.joinable())
	{
		this->m_indexThread.join();
	}
	TLSE_Destory();
}

void CTLIndexCreator::addFileOperator(CTLIndexResOperatorModel irom)
{
	//查询该文件是否已经被索引，如果被索引那么不需要再次进行索引，只在插入或者更新时有效,初次过滤
	if (irom.getIdxOpt() == IndexOperatorEnum::Insert || irom.getIdxOpt() == IndexOperatorEnum::Update)
	{
		//先判断该文件是否已被删除，如果删除，那么不需要创建索引
		if (irom.getFilePathName().find(L"\\$RECYCLE.BIN\\") != std::string::npos)
			return;
		//生成id
		std::wstring fileId = CResPath::fileMD5(irom.getFilePathName());
		std::string fid = tl::CTLString::ws2s(fileId);
		//生成文件hash
		std::wstring fileHash = CResPath::fileHash(irom.getFilePathName());
		std::string fhash = tl::CTLString::ws2s(fileHash);
		std::string sql = "select id,file_hash from t_index_meta_data where id = '";
		sql += fid;
		sql += "'";
		try
		{
			tl::Table t = this->m_pSqlite->queryData(sql);
			if (t.rowCount > 0)
			{
				//如果有数据信息，那么比较文件hash
				std::string oldFileHash = "";
				std::list<tl::Row>::iterator itRow = t.rows.begin();
				while (itRow != t.rows.end())
				{
					tl::Row r = *itRow++;
					std::list<tl::Column>::iterator itCol = r.columns.begin();
					while (itCol != r.columns.end())
					{
						tl::Column c = *itCol++;
						if (strcmp("file_hash", c.name.c_str()) == 0)
						{
							oldFileHash = c.value;
							break;
						}
					}
					if (oldFileHash.length() > 0)
					{
						break;
					}
				}
				if (fhash.compare(oldFileHash) == 0) //hash 相等，则文件未修改，那么不需要重建索引
				{
					return;
				}
			}
		}
		catch (const tl::SqlException& e)
		{
			std::string errmsg = FILE_STACK + "不能查询文件元数据信息：";
			errmsg += e.what();
			this->m_pLogger->error(errmsg);
			return;
		}
	}
	this->m_idxResQueue.enqueue(irom);
}

void CTLIndexCreator::threadLoop()
{
	while (m_bThreadLoop)
	{
		CTLIndexResOperatorModel irom;
		this->m_idxResQueue.try_dequeue(irom);
		if (irom.getFilePathName().length() == 0)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(50));
			continue;
		}
		if (irom.getIdxOpt() == IndexOperatorEnum::Insert || irom.getIdxOpt() == IndexOperatorEnum::Update)
		{
			int len = (irom.getFilePathName().length() + 1) * sizeof(wchar_t);
			char* tmpFilePath = new char[len];
			memset(tmpFilePath, 0, len);
			tl_unicode_to_ascii(irom.getFilePathName().c_str(), tmpFilePath);
			CTLAnyDocReader docReader(tmpFilePath);
			std::wstring text = docReader.readText();
			if (text.length() > 0)
			{
				//生成id
				std::wstring fileId = CResPath::fileMD5(irom.getFilePathName());
				//生成文件hash
				CFileAttribute fileAttribute = CResPath::fileAttribute(irom.getFilePathName());
				std::wstring fileHash = CResPath::fileHash(irom.getFilePathName());
				//创建索引
				addOrUpdateIndexData((wchar_t*)fileId.c_str(), (wchar_t*)fileHash.c_str(), (wchar_t*)fileAttribute.getFileName().c_str(), (wchar_t*)fileAttribute.getFilePath().c_str(), (wchar_t*)text.c_str(), text.length(),(wchar_t*)fileAttribute.getFileSuffix().c_str());
			}
			delete[] tmpFilePath;
		}
		else if (irom.getIdxOpt() == IndexOperatorEnum::Remove) 
		{
			//删除索引
			std::wstring fileId = CResPath::fileMD5(irom.getFilePathName());
			removeIndexData((wchar_t*)fileId.c_str());
			delayIndex(100);
		}
	}
	this->m_pLogger->info("索引线程退出");
}

void CTLIndexCreator::addOrUpdateIndexData(wchar_t* id, wchar_t* fileHash, wchar_t* fileName, wchar_t* filePath, wchar_t* fileContent, long contentLength, wchar_t* fileSuffix)
{
	int ret = 0;

	TLIndexField idField;
	idField.index = 3;
	strcpy_s(idField.name, "id");
	idField.store = 1;
	idField.value = id;
	//再次查询该文件是否已被索引
	std::string fid = tl::CTLString::ws2s(id);
	std::string fhash = tl::CTLString::ws2s(fileHash);
	std::string sql = "select id,file_hash from t_index_meta_data where id = '";
	sql += fid;
	sql += "'";
	try
	{
		tl::Table t = this->m_pSqlite->queryData(sql);
		if (t.rowCount > 0)
		{
			//如果有数据信息，那么比较文件hash
			std::string oldFileHash = "";
			std::list<tl::Row>::iterator itRow = t.rows.begin();
			while (itRow != t.rows.end())
			{
				tl::Row r = *itRow++;
				std::list<tl::Column>::iterator itCol = r.columns.begin();
				while (itCol != r.columns.end())
				{
					tl::Column c = *itCol++;
					if (strcmp("file_hash", c.name.c_str()) == 0)
					{
						oldFileHash = c.value;
						break;
					}
				}
				if (oldFileHash.length() > 0)
				{
					break;
				}
			}
			if (fhash.compare(oldFileHash) == 0) //hash 相等，则文件未修改，那么不需要重建索引
			{
				return;
			}
			else
			{
				//删除索引
				removeIndexData(id);
			}
		}
	}
	catch (const tl::SqlException& e)
	{
		std::string errmsg = FILE_STACK + "不能查询文件元数据信息：";
		errmsg += e.what();
		this->m_pLogger->error(errmsg);
		return;
	}

	TLIndexField fileHashField;
	fileHashField.index = 3;
	strcpy_s(fileHashField.name, "file_hash");
	fileHashField.store = 1;
	fileHashField.value = fileHash;

	TLIndexField fileNameField;
	fileNameField.index = 2;
	strcpy_s(fileNameField.name, "file_name");
	fileNameField.store = 1;
	fileNameField.value = fileName;

	TLIndexField filePathField;
	filePathField.index = 3;
	strcpy_s(filePathField.name, "file_path");
	filePathField.store = 1;
	filePathField.value = filePath;

	TLIndexField fileContentField;
	fileContentField.index = 2;
	strcpy_s(fileContentField.name, "file_content");
	fileContentField.store = 1;
	fileContentField.value = fileContent;

	TLIndexField fileSuffixField;
	fileSuffixField.index = 3;
	strcpy_s(fileSuffixField.name, "file_suffix");
	fileSuffixField.store = 1;
	fileSuffixField.value = fileSuffix;

	TLIndexField* fields[6] = { 0 };
	fields[0] = &idField;
	fields[1] = &fileHashField;
	fields[2] = &fileNameField;
	fields[3] = &filePathField;
	fields[4] = &fileContentField;
	fields[5] = &fileSuffixField;
	//添加索引数据
	TLSE_IndexAddData(INDEX_NAME, fields, 6);
	std::string currentDateTime = tl::CTLDateTime::currenDateTime();
	//添加元数据信息
	sql = "insert into t_index_meta_data(id,file_hash,file_name,file_path,file_suffix,create_time) values('";
	sql += fid;
	sql += "','";
	sql += fhash;
	sql += "','";
	sql += tl::CTLString::ws2s(fileName);
	sql += "','";
	sql += tl::CTLString::ws2s(filePath);
	sql += "','";
	sql += tl::CTLString::ws2s(fileSuffix);
	sql += "','";
	sql += currentDateTime;
	sql += "')";
	//转utf8编码
	int len = (sql.length() + 1) * 3;
	char* utf8Sql = new char[len];
	memset(utf8Sql, 0, len);
	tl_ascii_to_utf8(sql.c_str(), utf8Sql);
	try 
	{
		this->m_pSqlite->insertData(utf8Sql);
	}
	catch (const tl::SqlException& e)
	{
		std::string errmsg = FILE_STACK + "插入元数据到本地数据库失败：";
		errmsg += e.what();
		this->m_pLogger->error(errmsg);
	}
	delete[] utf8Sql;
	//控制索引速度的快慢，如果太快，CPU会占用很高
	delayIndex(contentLength);
}

void CTLIndexCreator::removeIndexData(wchar_t* id)
{
	TLIndexField idField;
	idField.index = 3;
	strcpy_s(idField.name, "id");
	idField.store = 1;
	idField.value = id;
	//移除索引数据
	if (TLSE_DeleteIndexData(INDEX_NAME, &idField) != 0)
	{
		this->m_pLogger->error(FILE_STACK + "移除索引失败");
	}
	//删除索引元数据
	std::string sql = "delete from t_index_meta_data where id = '";
	sql += tl::CTLString::ws2s(id);
	sql += "'";
	this->m_pSqlite->deleteData(sql);
}

void CTLIndexCreator::checkIndexValid()
{
	this->m_pLogger->info("检查索引是否有效");
	if (TLSE_IndexValid(INDEX_NAME) == -1)
	{
		//检测是否存在备份索引数据，如果存在则进行还原
		char indexPath[1024] = { 0 };
		TLSE_GetIndexPath(INDEX_NAME, indexPath);
		if (strlen(indexPath) > 0)
		{
			//提取引擎创建的索引名称
			std::string indexName = indexPath;
			int pos = indexName.find_last_of('\\');
			if (pos == std::string::npos)
				return;
			indexName = indexName.substr(pos + 1);
			WIN32_FIND_DATA findFileData;
			HANDLE hFind;

			std::wstring sesrchParentPath = tl::CTLString::s2ws(this->m_indexPath);
			std::wstring searchPath = sesrchParentPath + L"\\*.zip";
			hFind = FindFirstFile(searchPath.c_str(), &findFileData);

			if (hFind != INVALID_HANDLE_VALUE)
			{
				do
				{
					std::wstring fileName = findFileData.cFileName;
					if (tl::CTLString::strStartWith(fileName,tl::CTLString::s2ws(indexName)))
					{
						this->recoverIndex(sesrchParentPath + L"\\" + fileName, indexPath);
						break;
					}
				} while (FindNextFile(hFind, &findFileData) != 0);

				FindClose(hFind);
			}
		}
	}
}

void CTLIndexCreator::recoverIndex(std::wstring indexBackupFullPath, std::string indexPath)
{
	//删除索引目录
	while (!CTLPath::deleteDir(tl::CTLString::s2ws(indexPath))) std::this_thread::sleep_for(std::chrono::microseconds(50));;
	
	//删除数据库，当天创建的索引记录
	std::string currentDate = tl::CTLDateTime::currentDate();
	std::string deleteSql = "delete from ";
	deleteSql += T_INDEX_META_DATA;
	deleteSql += " where create_time like '";
	deleteSql += currentDate;
	deleteSql += "%'";
	try
	{
		this->m_pSqlite->deleteData(deleteSql);
	}
	catch (const tl::SqlException& e)
	{
		std::string errmsg = FILE_STACK + "恢复索引数据时，不能删除当日sqlite数据：";
		errmsg += e.what();
		this->m_pLogger->error(errmsg);
	}
	//解压索引
	HZIP hZip = OpenZip(indexBackupFullPath.c_str(),NULL);
	if (hZip == NULL)
		return;
	SetUnzipBaseDir(hZip, tl::CTLString::s2ws(this->m_indexPath).c_str());
	ZIPENTRY ze;
	GetZipItem(hZip, -1, &ze);
	int numitems = ze.index;
	for (size_t i = 0; i < numitems; i++)
	{
		GetZipItem(hZip, i, &ze);
		UnzipItem(hZip, i, ze.name);
	}
	CloseZip(hZip);
}

void CTLIndexCreator::backupIndex()
{
	if (TLSE_IndexValid(INDEX_NAME) == -1)
		return;
	//获取索引路径
	char indexPath[1024] = { 0 };
	TLSE_GetIndexPath(INDEX_NAME, indexPath);
	if (strlen(indexPath) == 0)
		return;
	//提取引擎创建的索引名称
	std::string indexName = indexPath;
	int pos = indexName.find_last_of('\\');
	if (pos == std::string::npos)
		return;
	indexName = indexName.substr(pos + 1);

	std::wstring indexBackupZipFullPathName;
	std::wstring sesrchParentPath = tl::CTLString::s2ws(this->m_indexPath);
	std::wstring searchPath = sesrchParentPath + L"\\*.zip";
	WIN32_FIND_DATA findFileData;
	HANDLE hFind;
	hFind = FindFirstFile(searchPath.c_str(), &findFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			std::wstring fileName = findFileData.cFileName;
			if (tl::CTLString::strStartWith(fileName, tl::CTLString::s2ws(indexName)))
			{
				indexBackupZipFullPathName = sesrchParentPath + L"\\" + fileName;
				break;
			}
		} while (FindNextFile(hFind, &findFileData) != 0);

		FindClose(hFind);
	}

	//
	std::wstring currentDate = tl::CTLString::s2ws(tl::CTLDateTime::currentDate());
	if (indexBackupZipFullPathName.length() > 0 && indexBackupZipFullPathName.find(currentDate) != std::wstring::npos)
		return; //表示今日已经备份

	//进行备份
	std::list<std::string> filePathNameList = CTLPath::allFileList(indexPath);
	if (filePathNameList.size() == 0)
		return;
	std::wstring indexPathW = tl::CTLString::s2ws(indexPath);
	std::wstring zipFilePathName = indexPathW + L"_";
	zipFilePathName += currentDate;
	zipFilePathName += L".backup.zip";

	HZIP hZip = CreateZip(zipFilePathName.c_str(), 0);
	if (hZip == NULL)
		return;
	std::list<std::string>::iterator it = filePathNameList.begin();
	for (; it != filePathNameList.end(); it++)
	{
		std::wstring idxfileNameW = tl::CTLString::s2ws(*it);
		std::wstring fullIdxFileName = indexPathW + L"\\";
		fullIdxFileName += idxfileNameW;
		std::wstring relatePathName = tl::CTLString::s2ws(indexName);
		relatePathName += L"\\";
		relatePathName += idxfileNameW;
		ZipAdd(hZip, relatePathName.c_str(), fullIdxFileName.c_str());
	}
	CloseZip(hZip);

	//删除源备份
	if (!indexBackupZipFullPathName.empty())
	{
		CTLPath::deleteFile(tl::CTLString::ws2s(indexBackupZipFullPathName));
	}
}

void CTLIndexCreator::delayIndex(long contentLength)
{
	if (contentLength < 1000)
	{
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
	else if (1000 <= contentLength && contentLength < 10000)
	{
		std::this_thread::sleep_for(std::chrono::seconds(6));
	}
	else if (10000 <= contentLength && contentLength < 100000)
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));
	}
	else if (100000 <= contentLength && contentLength < 1000000)
	{
		std::this_thread::sleep_for(std::chrono::seconds(20));
	}
	else if (1000000 <= contentLength)
	{
		std::this_thread::sleep_for(std::chrono::seconds(60));
	}
}