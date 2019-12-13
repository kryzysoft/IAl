#ifndef FREE_RTOS_H
#define FREE_RTOS_H

#include "IOsal.h"
#include "cmsis_os.h"

class FreeRtosOsal: public IOsal
{
	private:
		static ITask *m_pTask;
		static char taskName[6];
		static const uint32_t MAX_TASKS_COUNT = 10;
		static osThreadDef_t threads[MAX_TASKS_COUNT];
		static uint32_t threadsCount;
		static void threadFunction(void const *argument);
		static uint8_t *m_value;

		static FreeRtosOsal *osal;
	public:
		virtual void Start() override;
		virtual int32_t RegisterTask(ITask *task) override;
};

#endif
