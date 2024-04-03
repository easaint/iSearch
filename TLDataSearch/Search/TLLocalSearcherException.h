#ifndef _TL_LOCAL_SEARCHER_EXCEPTION_H
#define _TL_LOCAL_SEARCHER_EXCEPTION_H
#include <exception>
#include <string>


//��������ʼ���쳣
class LocalSearcherInitException : public std::exception {
public:
	LocalSearcherInitException(const std::string& message) : message_(message) {}

	const char* what() const override {
		return message_.c_str();
	}

private:
	std::string message_;
};

#endif