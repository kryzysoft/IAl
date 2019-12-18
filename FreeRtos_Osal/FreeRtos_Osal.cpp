#include "FreeRtos_Osal.h"
#include "cmsis_os.h"
#include "stdio.h"

uint32_t FreeRtosOsal::threadsCount = 0;
char FreeRtosOsal::taskName[6];
osThreadDef_t FreeRtosOsal::threads[MAX_TASKS_COUNT];
ITask* FreeRtosOsal::m_pTask = NULL;
FreeRtosOsal *FreeRtosOsal::osal = NULL;
uint8_t *FreeRtosOsal::m_value = NULL;

void FreeRtosOsal::Start()
{
	osKernelStart();
}

int32_t FreeRtosOsal::RegisterTask(ITask *task)
{
	sprintf(taskName,"%s","Task");
	threads[threadsCount].name = taskName;
	threads[threadsCount].instances = 1;
	threads[threadsCount].pthread = threadFunction;
	threads[threadsCount].stacksize = 6096;
	threads[threadsCount].tpriority = osPriorityNormal;
	osThreadId threadId = osThreadCreate(&threads[threadsCount], task);
	threadsCount++;
	osal = this;
	return (int32_t)threadId;
}

extern "C" void StartDefaultTask(void const * argument);

void FreeRtosOsal::threadFunction(void const *argument)
{
  ((ITask*)argument)->Run();
}
