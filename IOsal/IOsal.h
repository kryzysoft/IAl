#ifndef I_OSAL_H
#define I_OSAL_H

#include "stdint.h"

class ITask
{
public:
	virtual void Run() = 0;
	uint8_t *m_pValue;
};

class IOsal
{
	public:
		virtual void Start() = 0;
		virtual int32_t RegisterTask(ITask *task) = 0;
		virtual ~IOsal(){};
};

#endif
