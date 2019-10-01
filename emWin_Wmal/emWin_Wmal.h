#ifndef STEMWIN_WINDOW_MANAGER_H
#define STEMWIN_WINDOW_MANAGER_H

#include "IWindowManager.h"
#include "WM.h"

typedef struct
{
  int32_t buttonHandle;
  IButtonEventHandler *buttonEventHandler;
} ButtonHandlerItem;

class STemWinWindowManager: public IWindowManager
{
private:
  static void eventHandler(WM_MESSAGE * pMsg);
  int32_t m_width;
  int32_t m_height;
  static const uint32_t MAX_BUTTONS_TOTAL = 100;
  static const uint32_t LISTVIEW_ROW_HEIGHT = 20;
  static uint32_t buttonHandlersCount;
  static ButtonHandlerItem buttonEventHandlers[MAX_BUTTONS_TOTAL];
  static void buttonClicked(int32_t buttonHandle);
public:
  STemWinWindowManager();
  virtual void Init(int32_t width, int32_t height);
  virtual int32_t CreateWin(int32_t x, int32_t y, int32_t width, int32_t height);
  virtual int32_t CreateText(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text);
  virtual int32_t CreateButton(
      int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height, const char *text,
      IButtonEventHandler *buttonEventHandler);
  virtual int32_t CreateListView(int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height);
  virtual void AddColumnToListView(int32_t listViewHandle,
      int32_t size, const char *name);
  virtual void AddRowToListView(int32_t listViewHandle, const char **row);
  virtual int32_t GetWidth();
  virtual int32_t GetHeight();
  virtual void Touch(int32_t x, int32_t y);
  virtual void Untouch();
  virtual void Show(int32_t windowHandle);
  virtual void Hide(int32_t windowHandle);
  virtual bool Execute();
};

#endif
