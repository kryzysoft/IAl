#include "StmTouchScreen.h"
#include "stm32746g_discovery_ts.h"

void StmTouchScreen::Init()
{
  BSP_TS_Init(480,272);
}

TouchPosition StmTouchScreen::GetTouchPosition()
{
  TS_StateTypeDef ts;
  BSP_TS_GetState(&ts);
  TouchPosition tp;
  tp.x = ts.touchX[0];
  tp.y = ts.touchY[0];
	return tp;
}

bool StmTouchScreen::IsTouched()
{
	TS_StateTypeDef ts;

	BSP_TS_GetState(&ts);
	if(ts.touchDetected>0) return true;
	else return false;
}
