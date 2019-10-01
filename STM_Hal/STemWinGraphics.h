#ifndef STEMWIN_GRAPHICS_H
#define STEMWIN_GRAPHICS_H

#include "IHalGraphics.h"

class StemWinGraphics: public IHalGraphics
{
private:
  void setSkin();
  void setButtonSkin();
  void setListviewSkin();
  void setHeaderSkin();
  void setScrollbarSkin();
  void setTextSkin();
  void setEditSkin();
  void setDropdownSkin();
  void setListboxSkin();
public:
  void Init();
  virtual uint32_t GetColorRgb(uint8_t r, uint8_t g, uint8_t b);
  virtual void Clear();
  virtual void SetColor(uint32_t color);
  virtual void DisplayStringAt(const char *text, int32_t x, int32_t y);
};



#endif
