#ifndef I_WINDOW_MANAGER_H
#define I_WINDOW_MANAGER_H

#include "stdint.h"


class IButtonEventHandler
{
public:
  virtual void ButtonEventHandler(int32_t buttonHandle) = 0;
};

class IWindowManager
{
public:
  #pragma push_macro("CreateWindow")
  #undef CreateWindow
  virtual int32_t CreateWin(int32_t x, int32_t y, int32_t width, int32_t height) = 0;

  #pragma pop_macro("CreateWindow")

  virtual int32_t CreateText(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text) = 0;
  virtual int32_t CreateButton(
      int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height, const char *text,
      IButtonEventHandler *buttonEventHandler) = 0;
  virtual int32_t CreateListView(int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height) = 0;
  virtual void AddColumnToListView(int32_t listViewHandle, int32_t size,
      const char *name) = 0;
  virtual void AddRowToListView(int32_t listViewHandle, const char **row) = 0;
  virtual bool Execute() = 0;
  virtual int32_t GetWidth() = 0;
  virtual int32_t GetHeight() = 0;
  virtual void Touch(int32_t x, int32_t y) = 0;
  virtual void Untouch() = 0;
  virtual void Show(int32_t windowHandle) = 0;
  virtual void Hide(int32_t windowHandle) = 0;
  virtual ~IWindowManager(){};
};

#endif
