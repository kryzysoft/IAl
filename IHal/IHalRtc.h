#ifndef I_HAL_RTC_H
#define I_HAL_RTC_H

#include "stdint.h"

class IRtcAlarmHandler
{
  public:
    virtual void RtcAlarmHandler() = 0;
};

class IHalRtc
{
  public:
    virtual void LatchDateAndTime() = 0;
    virtual void GetLatchedDate(uint16_t *year, uint8_t *month, uint8_t *day) = 0;
    virtual void GetLatchedTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds) = 0;
    virtual void SetAlarmHandler(IRtcAlarmHandler *rtcAlarmHandler) = 0;
    virtual void SetupAlarmInSeconds(uint32_t seconds) = 0;
    virtual ~IHalRtc(){};
};

#endif
