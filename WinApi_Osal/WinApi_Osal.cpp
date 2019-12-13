#include <windows.h>
#include "stdio.h"
#include "WinApi_Osal.h"

uint32_t WinApiOsal::threadsCount = 0;
char WinApiOsal::taskName[6];
HANDLE WinApiOsal::threadHandles[MAX_TASKS_COUNT];
ITask* WinApiOsal::m_pTask = NULL;
WinApiOsal *WinApiOsal::osal = NULL;
uint8_t *WinApiOsal::m_value = NULL;

void WinApiOsal::Start()
{
  for(uint32_t i=0; i<threadsCount; i++)
  {
    ResumeThread(threadHandles[i]);
  }
}

int32_t WinApiOsal::RegisterTask(ITask *task)
{
  HANDLE threadHandle = CreateThread(0, 0, threadFunction, task, CREATE_SUSPENDED, NULL);
	threadHandles[threadsCount] = threadHandle;
	threadsCount++;
	osal = this;
	return (int32_t)threadHandle;
}

DWORD WINAPI WinApiOsal::threadFunction(LPVOID lpParameter)
{
  static_cast<ITask*>(lpParameter)->Run();
  return 0;
}
