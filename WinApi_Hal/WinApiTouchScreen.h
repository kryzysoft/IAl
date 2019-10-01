#ifndef WINAPI_TOUCH_SCREEN_H
#define WINAPI_TOUCH_SCREEN_H

#include "IHalTouchScreen.h"

class WinApiTouchScreen: public IHalTouchScreen
{
  public:
  	void Init();
    virtual TouchPosition GetTouchPosition();
    virtual bool IsTouched();
};

#endif
