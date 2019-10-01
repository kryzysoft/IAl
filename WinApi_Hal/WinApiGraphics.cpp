#include "WinApiGraphics.h"

void WinApiGraphics::WinApiGraphics::Init()
{
}

uint32_t WinApiGraphics::GetColorRgb(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r<<16)|(g<<8)|(b);
}

void WinApiGraphics::Clear()
{
}

void WinApiGraphics::SetColor(uint32_t color)
{
}

void WinApiGraphics::DisplayStringAt(const char *text, int32_t x, int32_t y)
{
}
