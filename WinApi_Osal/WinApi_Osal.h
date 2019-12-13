#ifndef WINAPI_OSAL_H
#define WINAPI_OSAL_H

#include "IOsal.h"

class WinApiOsal: public IOsal
{
	private:
		static ITask *m_pTask;
		static char taskName[6];
		static const uint32_t MAX_TASKS_COUNT = 10;
		static HANDLE threadHandles[MAX_TASKS_COUNT];
		static uint32_t threadsCount;
		static DWORD WINAPI threadFunction(LPVOID lpParameter);
		static uint8_t *m_value;

		static WinApiOsal *osal;
	public:
		virtual void Start() override;
		virtual int32_t RegisterTask(ITask *task) override;
};

#endif
