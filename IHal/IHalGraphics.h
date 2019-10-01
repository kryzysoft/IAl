#ifndef I_HAL_GRAPHICS_H
#define I_HAL_GRAPHICS_H

#include "stdint.h"

class IHalGraphics
{
  public:
    virtual uint32_t GetColorRgb(uint8_t r, uint8_t g, uint8_t b) = 0;
    virtual void Init() = 0;
    virtual void Clear() = 0;
    virtual void SetColor(uint32_t color) = 0;
    virtual void DisplayStringAt(const char *text, int32_t x, int32_t y) = 0;
    virtual ~IHalGraphics(){};
};

#endif
