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
  int32_t windowHandle;
  IPaintEventHandler *paintWindowHandler;
} PaintHandlerItem;

typedef struct
{
  int32_t windowHandle;
  IClickEventHandler *clickWindowHandler;
} ClickHandlerItem;

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
  WinApi_Wmal(){};
  HINSTANCE m_appInstance;

  static LRESULT CALLBACK EventHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );
  int32_t m_width;
  int32_t m_height;
  static bool paintInProgress;
  static HFONT m_hFont;

  static const uint32_t MAX_BUTTONS_TOTAL = 100;
  static const uint32_t LISTVIEW_ROW_HEIGHT = 20;
  static const uint32_t MAX_WINDOWS_COUNT = 20;
  static const uint32_t MAX_STATIC_TEXTS = 100;
  static const uint32_t DEFAULT_BK_COLOR = RGB(200,200,200);

  static uint32_t buttonHandlersCount;
  static ButtonHandlerItem buttonEventHandlers[MAX_BUTTONS_TOTAL];
  static uint32_t paintHandlersCount;
  static PaintHandlerItem paintEventHandlers[MAX_WINDOWS_COUNT];
  static uint32_t clickHandlersCount;
  static ClickHandlerItem clickEventHandlers[MAX_WINDOWS_COUNT];
  static uint32_t windowsCount;
  static HWND windowHandles[MAX_WINDOWS_COUNT];
  static HDC currentHdc;

  static uint32_t textsCount;
  static TextStruct textStructs[MAX_STATIC_TEXTS];

  HWND m_hMainWindow;
  bool m_mainWindowCreated;

  HBRUSH m_hBkBrush;

  static void ButtonClicked(int32_t buttonHandle);
  static bool PaintWindow(int32_t windowHandle);
  static bool ClickWindow(int32_t windowHandle, int32_t x, int32_t y);
  void CreateMainWindow();
public:
  explicit WinApi_Wmal(HINSTANCE appInstance);
  ~WinApi_Wmal();

  virtual void Init(int32_t width, int32_t height);
  virtual int32_t CreateWin(int32_t x, int32_t y, int32_t width, int32_t height) override;
  virtual void DeleteControl(int32_t handle) override;
  virtual int32_t CreateWinMaximized() override;
  virtual void AssignPaintCallback(int32_t windowHandle, IPaintEventHandler *paintEventHandler) override;
  virtual void AssignClickCallback(int32_t windowHandle, IClickEventHandler *clickEventHandler) override;
  virtual void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1) override;
  virtual void DrawTextHvCenter(int32_t x0, int32_t y0, const char *text) override;
  virtual int32_t CreateText(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text) override;
  virtual void SetTextText(int32_t handle, const char *text) override;
  virtual int32_t CreateButton(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text, IButtonEventHandler *buttonEventHandler) override;
  virtual void SetButtonText(int32_t handle, const char *text) override;
  virtual bool IsButtonPressed(int32_t buttonHandle) override;
  virtual int32_t CreateListView(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height) override;
  virtual void AddColumnToListView(int32_t listViewHandle, int32_t size, const char *name) override;
  virtual void AddRowToListView(int32_t listViewHandle, const char **row) override;
  virtual int32_t CreateEdit(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, int32_t textLength, const char *text) override;
  virtual void SetEditText(int32_t editHandle, const char *text) override;
  virtual void GetEditText(int32_t editHandle, char *text, int32_t maxLength) override;
  virtual void SetTextBkColor(int32_t textHandle, uint32_t color) override;
  virtual int32_t CreateComboBox(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, const char **items, const int32_t itemsCount) override;
  virtual int32_t GetComboBoxSelection(int32_t comboBoxHandle) override;
  virtual void SetComboBoxSelection(int32_t comboBoxHandle, int32_t selection) override;
  virtual void DeleteListViewRows(int32_t listViewHandle) override;
  virtual void GetListViewText(int32_t listViewHandle, int32_t row, int32_t column, char *text, int32_t bufferSize) override;
  virtual int32_t GetSelectedRow(int32_t listViewHandle) override;
  virtual int32_t GetWidth() override;
  virtual int32_t GetHeight() override;
  virtual void Touch(int32_t x, int32_t y) override;
  virtual void Untouch() override;
  virtual void Show(int32_t windowHandle) override;
  virtual void Hide(int32_t windowHandle) override;
  virtual void InvalidateWindow(int32_t windowHandle) override;
  virtual bool Execute() override;
};

#endif
