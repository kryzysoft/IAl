#ifndef WINAPI_RTC_H
#define WINAPI_RTC_H

#include "chrono"
#include "IHalRtc.h"

class WinApiRtc: public IHalRtc
{
private:
  std::chrono::system_clock::time_point m_latchedTime;
public:
  WinApiRtc();
  virtual void LatchDateAndTime();
  virtual void GetLatchedDate(uint16_t *year, uint8_t *month, uint8_t *day);
  virtual void GetLatchedTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds);
  virtual void SetAlarmHandler(IRtcAlarmHandler *rtcAlarmHandler);
  virtual void SetupAlarmInSeconds(uint32_t seconds);
};

#endif
