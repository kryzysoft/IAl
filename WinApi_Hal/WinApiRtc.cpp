#include "WinApiRtc.h"
#include "DebugLog.h"
#include "chrono"

WinApiRtc::WinApiRtc():m_latchedTime()
{
  m_startTime = std::chrono::steady_clock::now();
}

void WinApiRtc::LatchDateAndTime()
{
  m_latchedTime = std::chrono::system_clock::now();
}

void WinApiRtc::GetLatchedDate(uint16_t *year, uint8_t *month, uint8_t *day)
{
  auto currentTime = std::chrono::system_clock::to_time_t(m_latchedTime);
  auto localTime = localtime(&currentTime);
  *year = localTime->tm_year + 1900;
  *month = localTime->tm_mon + 1;
  *day = localTime->tm_mday;
}

void WinApiRtc::GetLatchedTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds)
{
  auto currentTime = std::chrono::system_clock::to_time_t(m_latchedTime);
  auto localTime = localtime(&currentTime);
  *hour = localTime->tm_hour;
  *minutes = localTime->tm_min;
  *seconds = localTime->tm_sec;
}

void WinApiRtc::SetAlarmHandler(IRtcAlarmHandler *rtcAlarmHandler)
{
  DBG_ASSERT(false);
}

void WinApiRtc::SetupAlarmInSeconds(uint32_t seconds)
{
  DBG_ASSERT(false);
}

uint32_t WinApiRtc::GetOnTimeInMs()
{
  auto now = std::chrono::steady_clock::now();
  auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_startTime);
  return nowMs.count();

 // return now.Count();
}



