#ifndef _TL_TASK_H
#define _TL_TASK_H
#include <string>

/**
 * 定义任务
 */
class CTLTask {
public:

	virtual ~CTLTask() {}

	/**
	 * 获取任务名称
	 */
	virtual std::string taskName() = 0;

	/**
	 * 定义任务执行函数
	 */
	virtual void execute() = 0;
};

#endif