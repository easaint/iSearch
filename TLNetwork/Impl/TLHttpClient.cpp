#include "../Export/TLHttpClient.h"
#include "../Core/TLURLParser.h"
#include "../Util/TLUuid.h"
#include "../Util/TLString.h"
#include "../Util/TLEncoder.h"
#include "curl.h"

namespace tl {

	//�������ݽ��յĽṹ��
	typedef struct _DownloadDataReceipt {
		std::string url;//���ص�url
		DownLoadCallback downloadCallback;//�ص�����
		std::string httpResponseHead;//�ļ�ͷ
		std::string saveFilePath;//������ļ�·��
		std::string fileName; //���������ļ���
		std::string filePathName;//������ļ�ȫ·����
		unsigned long long fileLength;//�ļ��ܳ���
		unsigned long long hasReciptLength;//�����ص��ļ�����
		FILE* fp;//�ļ�ָ��
	}DownloadDataReceiptor;

	/**
	 * ptr��ָ��洢���ݵ�ָ�룬
	 * size��ÿ����Ĵ�С��
	 * nmemb��ָ�����Ŀ��
	 * stream���û�������
	 * ���Ը���������Щ��������Ϣ����֪����ptr�е����ݵ��ܳ�����size*nmemb
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

	// ����http header�ص�����    
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

	//�������ص��ļ�����
	static std::string parseDownloadFileName(std::string responseHead)
	{
		std::string fileName = "";
		std::string headers = responseHead;
		//һ��һ�ж�ȡͷ
		std::string line = "";
		for (size_t i = 0; i < responseHead.size(); i++)
		{
			if (headers[i] == '\n')
			{
				//��ʾһ�ж�ȡ���
				if (line.find("Content-Disposition:") != std::string::npos)
				{
					line = line.replace(line.find("Content-Disposition:"), strlen("Content-Disposition:"), "");
					//ȥ���س�����
					if (line.find_first_of('\r') != std::string::npos)
					{
						line = line.replace(line.find('\r'), 1, "");
					}
					if (line.find_first_of('\n') != std::string::npos)
					{
						line = line.replace(line.find('\n'), 1, "");
					}
					//�����ļ���
					if (line.find("filename") != std::string::npos)
					{
						line = line.substr(line.find("filename"));
					}
					break;
				}
				//�����
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
			//�����Ƿ��зֺ�(;)������зֺţ���ʾ���滹�в���
			pos = fileName.find(";");
			if (pos != std::string::npos)
			{
				fileName = fileName.substr(0, pos);
			}
			//�滻����("")
			pos = fileName.find("\"");
			while (pos != std::string::npos) {
				fileName = fileName.replace(pos, 1, "");
				pos = fileName.find("\"");
			}
		}
		//filenameΪutf8���룬 ��Ҫת��ascci��
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

	//���������ļ��Ĵ�С
	static unsigned long long parseDownloadFileLength(std::string responseHead)
	{
		unsigned long long length = 0;
		std::string headers = responseHead;
		//һ��һ�ж�ȡͷ
		std::string line = "";
		for (size_t i = 0; i < responseHead.size(); i++)
		{
			if (headers[i] == '\n')
			{
				//��ʾһ�ж�ȡ���
				if (line.find("Content-Length:") != std::string::npos)
				{
					line = line.replace(line.find("Content-Length:"), strlen("Content-Length:"), "");
					//ȥ���س�����
					if (line.find_first_of('\r') != std::string::npos)
					{
						line = line.replace(line.find('\r'), 1, "");
					}
					if (line.find_first_of('\n') != std::string::npos)
					{
						line = line.replace(line.find('\n'), 1, "");
					}
					//ȥ���ո�
					if (line.find_first_of(' ') != std::string::npos)
					{
						line = line.replace(line.find(' '), 1, "");
					}
					break;
				}
				//�����
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

	//��������ͷ�ص�
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

	//����������ص�
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
		//������д���ļ�
		if (dowloadDataReceiptor->fileLength == 0)
		{
			dowloadDataReceiptor->fileLength = parseDownloadFileLength(dowloadDataReceiptor->httpResponseHead);
		}
		//�Ƿ��״ν���
		if (dowloadDataReceiptor->filePathName.length() == 0)
		{
			if (dowloadDataReceiptor->fileName.length() == 0) //���û��ָ���ļ����������Ӧͷ�н���
			{
				dowloadDataReceiptor->fileName = parseDownloadFileName(dowloadDataReceiptor->httpResponseHead);
				//����û���ļ�������һ��uuid
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
				// ���ļ�ʧ��
				if (eResult != 0)
					return -1;
			}
		}
		fwrite(data.c_str(), sizeof(char), data.length(), dowloadDataReceiptor->fp);
		//�����صĽ��ȷ�����Ϣ����
		if (dowloadDataReceiptor->downloadCallback != NULL)
		{
			dowloadDataReceiptor->downloadCallback(dowloadDataReceiptor->url, dowloadDataReceiptor->fileName, dowloadDataReceiptor->fileLength, dowloadDataReceiptor->hasReciptLength);
		}
		return nmemb;
	}

	//����������ص�,������д�뵽buffer�У���д�ļ�
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
		//ץȡ���ݺ󣬻ص����� 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callWriteFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&szbuffer);
		//ץȡͷ��Ϣ���ص�����  
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallbackFunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &szheader_buffer);
		/**
		 * ������̶߳�ʹ�ó�ʱ�����ʱ��ͬʱ���߳�����sleep����wait�Ȳ�����
		 * ������������ѡ�libcurl���ᷢ�źŴ�����wait�Ӷ����³����˳���
		 */
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8);//���ӳ�ʱ�������ֵ�������̫�̿��ܵ����������󲻵��ͶϿ���
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//��������ʱ��ʱ���ã����10��������δ�����ֱ꣬���˳�
		curlCode = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
		if (CURLE_OK != curlCode)
		{
			//��ʾ����ʧ���ˣ���Ҫ�׳��쳣
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("����������" + errmsg);
		}
		//������Ӧͷ
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
		// ����POST����
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		// �����������
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, param.c_str());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//ץȡ���ݺ󣬻ص����� 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callWriteFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&szbuffer);
		//ץȡͷ��Ϣ���ص�����  
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, headerCallbackFunc);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &szheader_buffer);
		/**
		 * ������̶߳�ʹ�ó�ʱ�����ʱ��ͬʱ���߳�����sleep����wait�Ȳ�����
		 * ������������ѡ�libcurl���ᷢ�źŴ�����wait�Ӷ����³����˳���
		 */
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 8);//���ӳ�ʱ�������ֵ�������̫�̿��ܵ����������󲻵��ͶϿ���
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);//��������ʱ��ʱ���ã����10��������δ�����ֱ꣬���˳�
		curlCode = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
		if (CURLE_OK != curlCode)
		{
			//��ʾ����ʧ���ˣ���Ҫ�׳��쳣
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("����������" + errmsg);
		}
		//������Ӧͷ
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
			throw tl::NetworkException("�޷������ļ���ϵͳ�ڲ�����");
		}
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//ץȡ���ݺ󣬻ص����� 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dowloadWriteDataFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&downloadDataReceiptor);
		//ץȡͷ��Ϣ���ص�����  
		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, dowloadWriteHead);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&downloadDataReceiptor);
		/**
		* ������̶߳�ʹ�ó�ʱ�����ʱ��ͬʱ���߳�����sleep����wait�Ȳ�����
		* ������������ѡ�libcurl���ᷢ�źŴ�����wait�Ӷ����³����˳���
		*/
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);//���ӳ�ʱ�������ֵ�������̫�̿��ܵ����������󲻵��ͶϿ���
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1800L);//��������ʱ��ʱ���ã�������Сʱ����δ�����ֱ꣬���˳�
		curlCode = curl_easy_perform(curl);
		//�ر��ļ�
		if (downloadDataReceiptor.fp != NULL)
		{
			fclose(downloadDataReceiptor.fp);
		}
		curl_easy_cleanup(curl);
		if (CURLE_OK != curlCode)
		{
			//��ʾ����ʧ���ˣ���Ҫ�׳��쳣
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("����������" + errmsg);
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
			throw tl::NetworkException("�޷������ļ���ϵͳ�ڲ�����");
		}
		hprs.uri = urlParser.parse();
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//ץȡ���ݺ󣬻ص����� 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, dowloadWriteDataFuncWithBuffer);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&hprs);
		/**
		* ������̶߳�ʹ�ó�ʱ�����ʱ��ͬʱ���߳�����sleep����wait�Ȳ�����
		* ������������ѡ�libcurl���ᷢ�źŴ�����wait�Ӷ����³����˳���
		*/
		curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
		curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);//���ӳ�ʱ�������ֵ�������̫�̿��ܵ����������󲻵��ͶϿ���
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1800L);//��������ʱ��ʱ���ã�������Сʱ����δ�����ֱ꣬���˳�
		curlCode = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		if (CURLE_OK != curlCode)
		{
			//��ʾ����ʧ���ˣ���Ҫ�׳��쳣
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("����������" + errmsg);
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
		// ����������
		struct curl_httppost* formpost = NULL;
		struct curl_httppost* lastptr = NULL;
		//utf8 ������ļ���
		char* pUtf8FileName = nullptr;
		int len = 0;

		CURL* curl = curl_easy_init();
		if (NULL == curl)
		{
			throw tl::NetworkException("�޷��ϴ��ļ���ϵͳ�ڲ�����");
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
		// ����POST����
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		// �����������
		if (pParams != nullptr)
		{
			for (auto it = pParams->begin(); it != pParams->end(); ++it) {
				curl_formadd(&formpost, &lastptr, 
					         CURLFORM_PTRNAME, it->first.c_str(),
					         CURLFORM_PTRCONTENTS, it->second.c_str(),
					         CURLFORM_END);
			}
		}
		//��ȡ�ļ���
		std::string fileName = filePathName.substr(filePathName.find_last_of("\\") + 1);
		//���ļ���ת��utf8����
		len = (fileName.length() + 1) * 3;
		pUtf8FileName = new char[len];
		memset(pUtf8FileName, 0, len);
		tl_ascii_to_utf8(fileName.c_str(), pUtf8FileName);
		// ����ļ��ֶ�
		curl_formadd(&formpost, &lastptr,
			CURLFORM_PTRNAME, "file",
			CURLFORM_FILE, filePathName.c_str(),
			CURLFORM_FILENAME, pUtf8FileName,
			CURLFORM_END);
		// ���ñ�����
		curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		//ץȡ���ݺ󣬻ص����� 
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callWriteFunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&szbuffer);
		//ץȡͷ��Ϣ���ص�����  
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
			//��ʾ����ʧ���ˣ���Ҫ�׳��쳣
			std::string errmsg = curl_easy_strerror(curlCode);
			throw tl::NetworkException("����������" + errmsg);
		}
		parseResponseHeader(szheader_buffer, res);
		res.body = szbuffer;
		res.bodyLen = szbuffer.length();
		return res;
	}

	void CTLHttpClient::parseResponseHeader(std::string headers, HttpResponse& hr)
	{
		// ���зָ���Ӧͷ
		size_t start = 0;
		size_t end = headers.find("\r\n");
		while (end != std::string::npos) {
			std::string line = headers.substr(start, end - start);
			//����״̬�룬http��Ӧͷ����һ�о���״̬��
			int pos = line.find_first_of(' ');
			line = line.substr(pos + 1);
			pos = line.find_first_of(' ');
			line = line.substr(0, pos);
			hr.statusCode = line;
			hr.iStatusCode = std::stoi(line);
			break;
			//// �ҵ�ð�ŵ�λ�ã����зָ�Ϊ���ƺ�ֵ
			//size_t colonPos = line.find(":");
			//if (colonPos != std::string::npos) {
			//	std::string name = line.substr(0, colonPos);S
			//	std::string value = line.substr(colonPos + 1);
			//	
			//}
			//start = end + 2;  // ����\r\n
			//end = headers.find("\r\n", start);
		}
	}
}