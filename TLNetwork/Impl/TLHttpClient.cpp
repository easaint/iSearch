#include "../Export/TLHttpClient.h"
#include "../Core/TLURLParser.h"
#include "../Util/TLUuid.h"
#include "../Util/TLString.h"
#include "../Util/TLEncoder.h"
#include "curl.h"

namespace tl {

	//下载数据接收的结构体
	typedef struct _DownloadDataReceipt {
		std::string url;//下载的url
		DownLoadCallback downloadCallback;//回调函数
		std::string httpResponseHead;//文件头
		std::string saveFilePath;//保存的文件路径
		std::string fileName; //解析到的文件名
		std::string filePathName;//保存的文件全路径名
		unsigned long long fileLength;//文件总长度
		unsigned long long hasReciptLength;//已下载的文件长度
		FILE* fp;//文件指针
	}DownloadDataReceiptor;

	/**
	 * ptr是指向存储数据的指针，
	 * size是每个块的大小，
	 * nmemb是指块的数目，
	 * stream是用户参数。
	 * 所以根据以上这些参数的信息可以知道，ptr中的数据的总长度是size*nmemb
	*/
	static size_t callWriteFunc(void* buffer, size_t size, size_t nmemb, void* lpVoid)
	{
		std::string* str = dynamic_cast<std::string*>((std::string*)lpVoid);
		if (NULL == str || NULL == buffer)
		{
			return -1;
		}

		char* pData = (char*)buffer;
		str->append(pData, size * nmemb);
		return nmemb;
	}

	// 返回http header回调函数    
	static size_t headerCallbackFunc(const char* buffer, size_t size, size_t nmemb, void* lpVoid)
	{
		std::string* str = dynamic_cast<std::string*>((std::string*)lpVoid);
		if (NULL == str || NULL == buffer)
		{
			return -1;
		}

		char* pData = (char*)buffer;
		str->append(pData, size * nmemb);
		return nmemb;
	}

	//解析下载的文件名称
	static std::string parseDownloadFileName(std::string responseHead)
	{
		std::string fileName = "";
		std::string headers = responseHead;
		//一行一行读取头
		std::string line = "";
		for (size_t i = 0; i < responseHead.size(); i++)
		{
			if (headers[i] == '\n')
			{
				//表示一行读取完毕
				if (line.find("Content-Disposition:") != std::string::npos)
				{
					line = line.replace(line.find("Content-Disposition:"), strlen("Content-Disposition:"), "");
					//去掉回车换行
					if (line.find_first_of('\r') != std::string::npos)
					{
						line = line.replace(line.find('\r'), 1, "");
					}
					if (line.find_first_of('\n') != std::string::npos)
					{
						line = line.replace(line.find('\n'), 1, "");
					}
					//查找文件名
					if (line.find("filename") != std::string::npos)
					{
						line = line.substr(line.find("filename"));
					}
					break;
				}
				//清空行
				line = "";
			}
			line += headers[i];
		}
		if (line.length() > 0)
		{
			int pos = line.find("filename=");
			if (pos != std::string::npos)
			{
				fileName = line.substr(pos + strlen("filename="));
			}
			//查找是否有分号(;)，如果有分号，表示后面还有参数
			pos = fileName.find(";");
			if (pos != std::string::npos)
			{
				fileName = fileName.substr(0, pos);
			}
			//替换引号("")
			pos = fileName.find("\"");
			while (pos != std::string::npos) {
				fileName = fileName.replace(pos, 1, "");
				pos = fileName.find("\"");
			}
		}
		//filename为utf8编码， 需要转成ascci码
		if (fileName.length() > 0)
		{
			int len = (fileName.length() + 1) * 3;
			char* ascFileName = new char[len];
			memset(ascFileName, 0, len);
			tl_utf8_to_ascii(fileName.c_str(), ascFileName);
			std::string name = ascFileName;
			delete[] ascFileName;
			return name;
		}
		return fileName;
	}

	//解析下载文件的大小
	static unsigned long long parseDownloadFileLength(std::string responseHead)
	{
		unsigned long long length = 0;
		std::string headers = responseHead;
		//一行一行读取头
		std::string line = "";
		for (size_t i = 0; i < responseHead.size(); i++)
		{
			if (headers[i] == '\n')
			{
				//表示一行读取完毕
				if (line.find("Content-Length:") != std::string::npos)
				{
					line = line.replace(line.find("Content-Length:"), strlen("Content-Length:"), "");
					//去掉回车换行
					if (line.find_first_of('\r') != std::string::npos)
					{
						line = line.replace(line.find('\r'), 1, "");
					}
					if (line.find_first_of('\n') != std::string::npos)
					{
						line = line.replace(line.find('\n'), 1, "");
					}
					//去掉空格
					if (line.find_first_of(' ') != std::string::npos)
					{
						line = line.replace(line.find(' '), 1, "");
					}
					break;
				}
				//清空行
				line = "";
			}
			line += headers[i];
		}
		if (line.length() > 0)
		{
			length = std::atoll(line.c_str());
		}
		return length;
	}

	//下载数据头回调
	static size_t dowloadWriteHead(void* buffer, size_t size, size_t nmemb, void* lpVoid)
	{
		DownloadDataReceiptor* dowloadDataReceiptor = dynamic_cast<DownloadDataReceiptor*>((DownloadDataReceiptor*)lpVoid);
		if (NULL == dowloadDataReceiptor || NULL == buffer)
		{
			return -1;
		}
		char* pData = (char*)buffer;
		dowloadDataReceiptor->httpResponseHead.append(pData, size * nmemb);
		return nmemb;
	}

	//下载数据体回调
	static size_t dowloadWriteDataFunc(void* buffer, size_t size, size_t nmemb, void* lpVoid)
	{
		DownloadDataReceiptor* dowloadDataReceiptor = dynamic_cast<DownloadDataReceiptor*>((DownloadDataReceiptor*)lpVoid);
		if (NULL == dowloadDataReceiptor || NULL == buffer)
		{
			return -1;
		}

		char* pData = (char*)buffer;
		std::string data;
		data.append(pData, size * nmemb);
		dowloadDataReceiptor->hasReciptLength += data.length();
		//将数据写入文件
		if (dowloadDataReceiptor->fileLength == 0)
		{
			dowloadDataReceiptor->fileLength = parseDownloadFileLength(dowloadDataReceiptor->httpResponseHead);
		}
		//是否首次进入
		if (dowloadDataReceiptor->filePathName.length() == 0)
		{
			if (dowloadDataReceiptor->fileName.length() == 0) //如果没有指定文件名，则从响应头中解析
			{
				dowloadDataReceiptor->fileName = parseDownloadFileName(dowloadDataReceiptor->httpResponseHead);
				//可能没有文件名，给一个uuid
				if (dowloadDataReceiptor->fileName.length() == 0)
				{
					dowloadDataReceiptor->fileName = CTLUUID::generateUUID();
				}
			}
			dowloadDataReceiptor->filePathName = dowloadDataReceiptor->saveFilePath;
			if (dowloadDataReceiptor->filePathName[dowloadDataReceiptor->filePathName.length() - 1] == '\\' || dowloadDataReceiptor->filePathName[dowloadDataReceiptor->filePathName.length() - 1] == '/')
			{
				dowloadDataReceiptor->filePathName += dowloadDataReceiptor->fileName;
			}
			else
			{
				dowloadDataReceiptor->filePathName += "\\";
				dowloadDataReceiptor->filePathName += dowloadDataReceiptor->fileName;
			}
			if (dowloadDataReceiptor->fp == NULL)
			{
				errno_t eResult = fopen_s(&dowloadDataReceiptor->fp, dowloadDataReceiptor->filePathName.c_str(), "wb");
				// 打开文件失败
				if (eResult != 0)
					return -1;
			}
		}
		fwrite(data.c_str(), sizeof(char), data.length(), dowloadDataReceiptor->fp);
		//将下载的进度放入消息队列
		if (dowloadDataReceiptor->downloadCallback != NULL)
		{
			dowloadDataReceiptor->downloadCallback(dowloadDataReceiptor->url, dowloadDataReceiptor->fileName, dowloadDataReceiptor->fileLength, dowloadDataReceiptor->hasReciptLength);
		}
		return nmemb;
	}

	//下载数据体回调,将数据写入到buffer中，不写文件
	static size_t dowloadWriteDataFuncWithBuffer(void* buffer, size_t size, size_t nmemb, void* lpVoid)
	{
		if (NULL == buffer)
		{
			return -1;
		}
		HttpResponse* pRes = (HttpResponse*)lpVoid;
		char* pData = (char*)buffer;
		int count = size * nmemb;
		pRes->body.append(pData, count);
		pRes->bodyLen += count;
		return nmemb;
	}

	HttpResponse CTLHttpClient::doGet(std::string url, std::map<std::string, std::string>* pHeader)
	{
		HttpResponse res;
		std::string szbuffer;
		std::string szheader_buffer;
		CTLURLParser urlParser(url);
		CURLcode curlCode;
		struct curl_slist* headers = NULL;
		CURL* curl = curl_easy_init();

		res.uri = urlParser.parse();

		if (pHeader)
		{
			for (auto it = pHeader->begin(); it != pHeader->end(); ++it) {
				std::string header = it->first + ": ";
				header += it->second;
				headers = curl_slist_append(headers, header.c_str());
			}
		}
		if (headers != NULL)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//抓取内容后，回调函数 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callWriteFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&szbuffer);
		//抓取头信息，回调函数  
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallbackFunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &szheader_buffer);
		/**
		 * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
		 * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
		 */
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8);//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//接收数据时超时设置，如果10秒内数据未接收完，直接退出
		curlCode = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
		if (CURLE_OK != curlCode)
		{
			//表示请求失败了，需要抛出异常
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("请求发生错误：" + errmsg);
		}
		//解析响应头
		parseResponseHeader(szheader_buffer, res);
		res.body = szbuffer;
		res.bodyLen = szbuffer.length();
		return res;
	}

	HttpResponse CTLHttpClient::doPost(std::string url, std::map<std::string, std::string>* pHeader, std::string param)
	{
		HttpResponse res;
		std::string szbuffer;
		std::string szheader_buffer;
		CTLURLParser urlParser(url);
		CURLcode curlCode;
		struct curl_slist* headers = NULL;
		CURL* curl = curl_easy_init();

		res.uri = urlParser.parse();

		if (pHeader)
		{
			for (auto it = pHeader->begin(); it != pHeader->end(); ++it) {
				std::string header = it->first + ": ";
				header += it->second;
				headers = curl_slist_append(headers, header.c_str());
			}
		}
		if (headers != NULL)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// 设置POST请求
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		// 设置请求参数
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, param.c_str());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//抓取内容后，回调函数 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callWriteFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&szbuffer);
		//抓取头信息，回调函数  
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallbackFunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &szheader_buffer);
		/**
		 * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
		 * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
		 */
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8);//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//接收数据时超时设置，如果10秒内数据未接收完，直接退出
		curlCode = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
		if (CURLE_OK != curlCode)
		{
			//表示请求失败了，需要抛出异常
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("请求发生错误：" + errmsg);
		}
		//解析响应头
		parseResponseHeader(szheader_buffer, res);
		res.body = szbuffer;
		res.bodyLen = szbuffer.length();
		return res;
	}

	std::string CTLHttpClient::downloadFile(std::string url, std::map<std::string, std::string>* pHeader, std::string downloadPath, std::string fileName, DownLoadCallback downloadCallback)
	{
		DownloadDataReceiptor downloadDataReceiptor;
		downloadDataReceiptor.url = url;
		downloadDataReceiptor.hasReciptLength = 0;
		downloadDataReceiptor.fp = NULL;
		downloadDataReceiptor.downloadCallback = downloadCallback;
		downloadDataReceiptor.saveFilePath = downloadPath;
		downloadDataReceiptor.fileName = fileName;
		downloadDataReceiptor.fileLength = 0;
		downloadDataReceiptor.filePathName = "";
		CURLcode curlCode;
		CURL* curl = curl_easy_init();
		if (NULL == curl)
		{
			throw tl::NetworkException("无法下载文件，系统内部错误");
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//抓取内容后，回调函数 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dowloadWriteDataFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&downloadDataReceiptor);
		//抓取头信息，回调函数  
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dowloadWriteHead);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&downloadDataReceiptor);
		/**
		* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
		* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
		*/
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1800L);//接收数据时超时设置，如果半个小时数据未接收完，直接退出
		curlCode = curl_easy_perform(curl);
		//关闭文件
		if (downloadDataReceiptor.fp != NULL)
		{
			fclose(downloadDataReceiptor.fp);
		}
		curl_easy_cleanup(curl);
		if (CURLE_OK != curlCode)
		{
			//表示请求失败了，需要抛出异常
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("请求发生错误：" + errmsg);
		}
		return downloadDataReceiptor.filePathName;
	}

	HttpResponse CTLHttpClient::downloadFile(std::string url, std::map<std::string, std::string>* pHeader)
	{
		HttpResponse hprs;
		hprs.bodyLen = 0;
		hprs.body = "";
		CTLURLParser urlParser(url);
		CURLcode curlCode;
		CURL* curl = curl_easy_init();
		if (NULL == curl)
		{
			throw tl::NetworkException("无法下载文件，系统内部错误");
		}
		hprs.uri = urlParser.parse();
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//抓取内容后，回调函数 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dowloadWriteDataFuncWithBuffer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&hprs);
		/**
		* 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
		* 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
		*/
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);//连接超时，这个数值如果设置太短可能导致数据请求不到就断开了
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1800L);//接收数据时超时设置，如果半个小时数据未接收完，直接退出
		curlCode = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (CURLE_OK != curlCode)
		{
			//表示请求失败了，需要抛出异常
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("请求发生错误：" + errmsg);
		}
		return hprs;
	}

	HttpResponse CTLHttpClient::uploadFile(std::string url, std::map<std::string, std::string>* pHeader, std::map<std::string, std::string>* pParams, std::string filePathName)
	{
		HttpResponse res;
		std::string szbuffer;
		std::string szheader_buffer;
		CTLURLParser urlParser(url);
		CURLcode curlCode;
		struct curl_slist* headers = NULL;
		// 构建表单数据
		struct curl_httppost* formpost = NULL;
		struct curl_httppost* lastptr = NULL;
		//utf8 编码的文件名
		char* pUtf8FileName = nullptr;
		int len = 0;

		CURL* curl = curl_easy_init();
		if (NULL == curl)
		{
			throw tl::NetworkException("无法上传文件，系统内部错误");
		}

		res.uri = urlParser.parse();

		if (pHeader)
		{
			for (auto it = pHeader->begin(); it != pHeader->end(); ++it) {
				std::string header = it->first + ": ";
				header += it->second;
				headers = curl_slist_append(headers, header.c_str());
			}
		}
		if (headers != NULL)
		{
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		// 设置POST请求
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		// 设置请求参数
		if (pParams != nullptr)
		{
			for (auto it = pParams->begin(); it != pParams->end(); ++it) {
				curl_formadd(&formpost, &lastptr, 
					         CURLFORM_PTRNAME, it->first.c_str(),
					         CURLFORM_PTRCONTENTS, it->second.c_str(),
					         CURLFORM_END);
			}
		}
		//获取文件名
		std::string fileName = filePathName.substr(filePathName.find_last_of("\\") + 1);
		//将文件名转成utf8编码
		len = (fileName.length() + 1) * 3;
		pUtf8FileName = new char[len];
		memset(pUtf8FileName, 0, len);
		tl_ascii_to_utf8(fileName.c_str(), pUtf8FileName);
		// 添加文件字段
		curl_formadd(&formpost, &lastptr,
			CURLFORM_PTRNAME, "file",
			CURLFORM_FILE, filePathName.c_str(),
			CURLFORM_FILENAME, pUtf8FileName,
			CURLFORM_END);
		// 设置表单数据
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//抓取内容后，回调函数 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callWriteFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&szbuffer);
		//抓取头信息，回调函数  
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallbackFunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &szheader_buffer);

		curlCode = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
		curl_formfree(formpost);
		if (pUtf8FileName != nullptr)
		{
			delete[] pUtf8FileName;
		}
		if (CURLE_OK != curlCode)
		{
			//表示请求失败了，需要抛出异常
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("请求发生错误：" + errmsg);
		}
		parseResponseHeader(szheader_buffer, res);
		res.body = szbuffer;
		res.bodyLen = szbuffer.length();
		return res;
	}

	void CTLHttpClient::parseResponseHeader(std::string headers, HttpResponse& hr)
	{
		// 按行分割响应头
		size_t start = 0;
		size_t end = headers.find("\r\n");
		while (end != std::string::npos) {
			std::string line = headers.substr(start, end - start);
			//查找状态码，http响应头部第一行就是状态码
			int pos = line.find_first_of(' ');
			line = line.substr(pos + 1);
			pos = line.find_first_of(' ');
			line = line.substr(0, pos);
			hr.statusCode = line;
			hr.iStatusCode = std::stoi(line);
			break;
			//// 找到冒号的位置，将行分割为名称和值
			//size_t colonPos = line.find(":");
			//if (colonPos != std::string::npos) {
			//	std::string name = line.substr(0, colonPos);S
			//	std::string value = line.substr(colonPos + 1);
			//	
			//}
			//start = end + 2;  // 跳过\r\n
			//end = headers.find("\r\n", start);
		}
	}
}