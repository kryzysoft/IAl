#ifndef I_HAL_TOUCH_SCREEN_H
#define I_HAL_TOUCH_SCREEN_H

#include "stdint.h"

typedef struct
{
	int32_t x;
	int32_t y;
} TouchPosition;

class IHalTouchScreen
{
  public:
    virtual TouchPosition GetTouchPosition() = 0;
    virtual bool IsTouched() = 0;
    virtual ~IHalTouchScreen(){};
};

#endif