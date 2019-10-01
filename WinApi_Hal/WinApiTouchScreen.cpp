#include "WinApiTouchScreen.h"

void WinApiTouchScreen::Init()
{
}

TouchPosition WinApiTouchScreen::GetTouchPosition()
{
  TouchPosition tp;
  tp.x = 0;
  tp.y = 0;
  return tp;
}

bool WinApiTouchScreen::IsTouched()
{
  return false;
}
