#include "StmRtc.h"
#include "DebugLog.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_rtc.h"
#include "rtc.h"

void StmRtc::LatchDateAndTime()
{
  HAL_StatusTypeDef result = HAL_RTC_GetTime(&hrtc, &m_latchedTime, FORMAT_BIN);
  DBG_ASSERT(result == HAL_OK);

  result = HAL_RTC_GetDate(&hrtc, &m_latchedDate, FORMAT_BIN);
  DBG_ASSERT(result == HAL_OK);
}

void StmRtc::GetLatchedDate(uint16_t *year, uint8_t *month, uint8_t *day)
{
  *year = m_latchedDate.Year;
  *month = m_latchedDate.Month;
  *day = m_latchedDate.Date;
}

void StmRtc::GetLatchedTime(uint8_t *hour, uint8_t *minutes, uint8_t *seconds)
{
  *hour = m_latchedTime.Hours;
  *minutes = m_latchedTime.Minutes;
  *seconds = m_latchedTime.Seconds;
}

void StmRtc::SetAlarmHandler(IRtcAlarmHandler *rtcAlarmHandler)
{
  DBG_ASSERT(false);
}

void StmRtc::SetupAlarmInSeconds(uint32_t seconds)
{
  DBG_ASSERT(false);
}

uint32_t StmRtc::GetOnTimeInMs()
{
  return HAL_GetTick();
}
