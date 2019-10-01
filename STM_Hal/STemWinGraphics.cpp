#include "STemWinGraphics.h"

#include "GUI.h"
#include "WM.h"
#include "dialog.h"
#include "stm32f7xx_hal.h"

extern "C" void GRAPHICS_HW_Init(void);

#define PALETTE_1 0xD0D0D0
#define PALETTE_2 0xC07070
#define PALETTE_3 0xA0A0A0

#define BACKGROUND_COLOR PALETTE_1
#define MIN_COLOR 0x500000
#define MAX_COLOR 0xF00000
#define MID_COLOR ((MIN_COLOR + MAX_COLOR)/2)
#define FONT_COLOR GUI_BLACK

#define LOWER_1 (PALETTE_2)
#define LOWER_0 (LOWER_1 + 0x0c0c0c)
#define UPPER_1 (LOWER_0 + 0x070707)
#define UPPER_0 (UPPER_1 + 0x0c0c0c)

#define FRAME_0 0xD00000
#define FRAME_1 (LOWER_0 - 0x040404)

#define LISTVIEW_ROW_HEIGHT 20

extern LTDC_HandleTypeDef            hltdc;

void StemWinGraphics::StemWinGraphics::Init()
{
  GRAPHICS_HW_Init();
  GUI_Init();

  HAL_LTDC_ProgramLineEvent(&hltdc, 0);
  WM_MULTIBUF_Enable(1);
  setSkin();
  BUTTON_SetReactOnTouch();
}

uint32_t StemWinGraphics::GetColorRgb(uint8_t r, uint8_t g, uint8_t b)
{
  return ((uint32_t)r<<16)|(g<<8)|(b);
}

void StemWinGraphics::Clear()
{
  GUI_Clear();
}

void StemWinGraphics::SetColor(uint32_t color)
{
  GUI_SetColor(color);
}

void StemWinGraphics::DisplayStringAt(const char *text, int32_t x, int32_t y)
{
  GUI_SetFont(&GUI_Font32_1);
  GUI_DispStringAt(text, x, y);
}

void StemWinGraphics::setSkin()
{
  WM_SetDesktopColor(BACKGROUND_COLOR);
  WINDOW_SetDefaultBkColor(BACKGROUND_COLOR);

  setButtonSkin();
  setHeaderSkin();
  setListviewSkin();
  setScrollbarSkin();
  setTextSkin();
  setEditSkin();
  setDropdownSkin();
  setListboxSkin();

//  GUI_Clear();
}

void StemWinGraphics::setButtonSkin()
{
  BUTTON_SKINFLEX_PROPS buttonProps;

  BUTTON_SetDefaultFont(&GUI_Font8_ASCII);

  buttonProps.aColorFrame[0] = FRAME_0;
  buttonProps.aColorFrame[1] = FRAME_1;
  buttonProps.aColorFrame[2] = LOWER_1;
  buttonProps.aColorUpper[0] = UPPER_0;
  buttonProps.aColorUpper[1] = UPPER_1;
  buttonProps.aColorLower[0] = LOWER_0;
  buttonProps.aColorLower[1] = LOWER_1;
  buttonProps.Radius = 3;
  BUTTON_SetSkinFlexProps(&buttonProps,BUTTON_SKINFLEX_PI_ENABLED);
  BUTTON_SetSkinFlexProps(&buttonProps,BUTTON_SKINFLEX_PI_FOCUSSED);
  BUTTON_SetSkinFlexProps(&buttonProps,BUTTON_SKINFLEX_PI_DISABLED);

  buttonProps.aColorUpper[0] += 0x202020;
  buttonProps.aColorUpper[1] += 0x202020;
  buttonProps.aColorLower[0] += 0x202020;
  buttonProps.aColorLower[1] += 0x202020;
  buttonProps.aColorFrame[0] += 0x202020;
  buttonProps.aColorFrame[1] += 0x202020;
  buttonProps.aColorFrame[2] = buttonProps.aColorLower[1];
  BUTTON_SetSkinFlexProps(&buttonProps,BUTTON_SKINFLEX_PI_PRESSED);

  BUTTON_SetDefaultTextColor(FONT_COLOR,BUTTON_CI_PRESSED);
  BUTTON_SetDefaultTextColor(FONT_COLOR,BUTTON_CI_UNPRESSED);
  BUTTON_SetDefaultTextColor(MIN_COLOR,BUTTON_CI_DISABLED);
}

void StemWinGraphics::setListviewSkin()
{
  LISTVIEW_SetDefaultFont(&GUI_Font8_ASCII);

  LISTVIEW_SetDefaultBkColor(LISTVIEW_CI_UNSEL,PALETTE_3);
  LISTVIEW_SetDefaultBkColor(LISTVIEW_CI_SEL,PALETTE_3 - 0x202020);
  LISTVIEW_SetDefaultBkColor(LISTVIEW_CI_SELFOCUS,PALETTE_3 - 0x202020);
  LISTVIEW_SetDefaultTextColor(LISTVIEW_CI_UNSEL,FONT_COLOR);
  LISTVIEW_SetDefaultTextColor(LISTVIEW_CI_SEL,FONT_COLOR);
  LISTVIEW_SetDefaultTextColor(LISTVIEW_CI_SELFOCUS,FONT_COLOR);
}

void StemWinGraphics::setHeaderSkin()
{
  HEADER_SetDefaultFont(&GUI_Font8_ASCII);
  HEADER_SetDefaultTextColor(FONT_COLOR);

  HEADER_SKINFLEX_PROPS headerProps;

  headerProps.aColorFrame[0] = FRAME_0;
  headerProps.aColorFrame[1] = LOWER_1;

  headerProps.aColorUpper[0] = UPPER_0;
  headerProps.aColorUpper[1] = UPPER_1;
  headerProps.aColorLower[0] = LOWER_0;
  headerProps.aColorLower[1] = LOWER_1;

  HEADER_SetSkinFlexProps(&headerProps,0);

}

void StemWinGraphics::setScrollbarSkin()
{
  SCROLLBAR_SKINFLEX_PROPS scrollProps;

  SCROLLBAR_SetDefaultWidth(20);

  scrollProps.aColorFrame[0] = FRAME_0;
  scrollProps.aColorFrame[1] = FRAME_1;
  scrollProps.aColorFrame[2] = FRAME_1;

  scrollProps.aColorUpper[0] = UPPER_0;
  scrollProps.aColorUpper[1] = UPPER_1;
  scrollProps.aColorLower[0] = LOWER_0;
  scrollProps.aColorLower[1] = LOWER_1;

  scrollProps.aColorShaft[0] = LOWER_1;
  scrollProps.aColorShaft[1] = UPPER_0;

  scrollProps.ColorArrow = GUI_BLACK;
  scrollProps.ColorGrasp = GUI_BLACK;

  SCROLLBAR_SetSkinFlexProps(&scrollProps,SCROLLBAR_SKINFLEX_PI_PRESSED);
  SCROLLBAR_SetSkinFlexProps(&scrollProps,SCROLLBAR_SKINFLEX_PI_UNPRESSED);

}

void StemWinGraphics::setTextSkin()
{
  TEXT_SetDefaultFont(&GUI_Font8_ASCII);
  TEXT_SetDefaultTextColor(FONT_COLOR);
}

void StemWinGraphics::setEditSkin()
{
  EDIT_SetDefaultBkColor(EDIT_CI_ENABLED,MAX_COLOR);
  EDIT_SetDefaultTextColor(EDIT_CI_ENABLED,MIN_COLOR);
  EDIT_SetDefaultFont(&GUI_Font8_ASCII);
}

void StemWinGraphics::setDropdownSkin()
{
  DROPDOWN_SKINFLEX_PROPS dropProps;

  DROPDOWN_SetDefaultFont(&GUI_Font8_ASCII);

  dropProps.aColorFrame[0] = GUI_BLACK;
  dropProps.aColorFrame[1] = MAX_COLOR;
  dropProps.aColorFrame[2] = GUI_BLACK;

  dropProps.aColorLower[0] = MAX_COLOR;
  dropProps.aColorLower[1] = MAX_COLOR;
  dropProps.aColorUpper[0] = MAX_COLOR;
  dropProps.aColorUpper[1] = MAX_COLOR;

  dropProps.ColorArrow = MIN_COLOR;
  dropProps.ColorSep = MIN_COLOR;
  dropProps.ColorText = MIN_COLOR;
  dropProps.Radius = 1;

  DROPDOWN_SetSkinFlexProps(&dropProps,DROPDOWN_SKINFLEX_PI_DISABLED);
  DROPDOWN_SetSkinFlexProps(&dropProps,DROPDOWN_SKINFLEX_PI_EXPANDED);
  DROPDOWN_SetSkinFlexProps(&dropProps,DROPDOWN_SKINFLEX_PI_ENABLED);
//  DROPDOWN_SetSkinFlexProps(&dropProps,DROPDOWN_SKINFLEX_PI_FOCUSSED);

}

void StemWinGraphics::setListboxSkin()
{
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_UNSEL,PALETTE_3);
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_SEL,PALETTE_3-0x202020);
  LISTBOX_SetDefaultBkColor(LISTBOX_CI_SELFOCUS,PALETTE_3-0x202020);

  LISTBOX_SetDefaultFont(&GUI_Font8_ASCII);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_UNSEL,FONT_COLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_SEL,FONT_COLOR);
  LISTBOX_SetDefaultTextColor(LISTBOX_CI_SELFOCUS,FONT_COLOR);

  LISTBOX_SetDefaultTextAlign(GUI_TA_CENTER|GUI_TA_VCENTER);
}

