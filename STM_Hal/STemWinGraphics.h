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
  virtual void Init() override;
  virtual uint32_t GetColorRgb(uint8_t r, uint8_t g, uint8_t b) override;
  virtual void Clear() override;
  virtual void SetColor(uint32_t color) override;
  virtual void DisplayStringAt(const char *text, int32_t x, int32_t y) override;
};



#endif
