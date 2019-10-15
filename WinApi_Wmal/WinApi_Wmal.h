#ifndef WINAPI_WINDOW_MANAGER_H
#define WINAPI_WINDOW_MANAGER_H

#include "IWmal.h"
#include "Windows.h"

typedef struct
{
  int32_t buttonHandle;
  IButtonEventHandler *buttonEventHandler;
} ButtonHandlerItem;

typedef struct
{
  HWND textHandle;
  HWND parent;
  HBRUSH hBrush;
  uint32_t bkColor;
} TextStruct;

class WinApi_Wmal: public IWmal
{
private:
  HINSTANCE m_appInstance;

  static LRESULT CALLBACK eventHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
  int32_t m_width;
  int32_t m_height;
  HFONT m_hFont;

  static const uint32_t MAX_BUTTONS_TOTAL = 100;
  static const uint32_t LISTVIEW_ROW_HEIGHT = 20;
  static const uint32_t MAX_WINDOWS_COUNT = 20;
  static const uint32_t MAX_STATIC_TEXTS = 100;
  static const uint32_t DEFAULT_BK_COLOR = RGB(200,200,200);

  static uint32_t buttonHandlersCount;
  static ButtonHandlerItem buttonEventHandlers[MAX_BUTTONS_TOTAL];
  static uint32_t windowsCount;
  static HWND windowHandles[MAX_WINDOWS_COUNT];

  static uint32_t textsCount;
  static TextStruct textStructs[MAX_STATIC_TEXTS];

  HWND m_hMainWindow;
  bool m_mainWindowCreated;

  static void buttonClicked(int32_t buttonHandle);
  void createMainWindow();
public:
  WinApi_Wmal(HINSTANCE appInstance);
  ~WinApi_Wmal();

  virtual void Init(int32_t width, int32_t height);
  virtual int32_t CreateWin(int32_t x, int32_t y, int32_t width, int32_t height);
  virtual int32_t CreateText(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text);
  virtual int32_t CreateButton(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text, IButtonEventHandler *buttonEventHandler);
  virtual int32_t CreateListView(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height);
  virtual void AddColumnToListView(int32_t listViewHandle, int32_t size, const char *name);
  virtual void AddRowToListView(int32_t listViewHandle, const char **row);
  virtual int32_t CreateEdit(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, int32_t textLength, const char *text);
  virtual void SetEditText(int32_t editHandle, const char *text);
  virtual void GetEditText(int32_t editHandle, char *text, int32_t maxLength);
  virtual void SetTextBkColor(int32_t textHandle, uint32_t color);
  virtual int32_t CreateComboBox(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, const char **items, const int32_t itemsCount);
  virtual int32_t GetWidth();
  virtual int32_t GetHeight();
  virtual void Touch(int32_t x, int32_t y);
  virtual void Untouch();
  virtual void Show(int32_t windowHandle);
  virtual void Hide(int32_t windowHandle);
  virtual bool Execute();
};

#endif
