#ifndef STM_RTC_H
#define STM_RTC_H

#include "IHalRtc.h"
#include "Stm32f7xx_hal.h"

class StmRtc: public IHalRtc
{
private:
  RTC_TimeTypeDef m_latchedTime;
  RTC_DateTypeDef m_latchedDate;
public:
  virtual void LatchDateAndTime();
  virtual void GetLatchedDate(uint16_t *year, uint8_t *month, uint8_t *day);
  virtual void GetLatchedTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds);
  virtual void SetAlarmHandler(IRtcAlarmHandler *rtcAlarmHandler) ;
  virtual void SetupAlarmInSeconds(uint32_t seconds);
  virtual uint32_t GetOnTimeInMs();
};

#endif
