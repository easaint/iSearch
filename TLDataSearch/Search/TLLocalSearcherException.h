#ifndef _TL_LOCAL_SEARCHER_EXCEPTION_H
#define _TL_LOCAL_SEARCHER_EXCEPTION_H
#include <exception>
#include <string>


//¼ìË÷Æ÷³õÊ¼»¯Òì³£
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