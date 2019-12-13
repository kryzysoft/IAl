#ifndef STM_TOUCH_SCREEN_H
#define STM_TOUCH_SCREEN_H

#include "IHalTouchScreen.h"

class StmTouchScreen: public IHalTouchScreen
{
  public:
  	void Init();
    virtual TouchPosition GetTouchPosition() override;
    virtual bool IsTouched() override;
};

#endif
