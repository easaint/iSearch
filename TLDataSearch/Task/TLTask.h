#ifndef _TL_TASK_H
#define _TL_TASK_H
#include <string>

/**
 * ��������
 */
class CTLTask {
public:

	virtual ~CTLTask() {}

	/**
	 * ��ȡ��������
	 */
	virtual std::string taskName() = 0;

	/**
	 * ��������ִ�к���
	 */
	virtual void execute() = 0;
};

#endif