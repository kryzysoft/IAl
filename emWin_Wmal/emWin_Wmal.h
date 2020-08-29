#ifndef STEMWIN_WINDOW_MANAGER_H
#define STEMWIN_WINDOW_MANAGER_H

#include "IWmal.h"
#include "WM.h"

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
  int32_t textHandle;
  ITextClickEventHandler *clickHandler;
} TextClickHandlerItem;

typedef struct
{
  int32_t editHandle;
  IEditFocusEventHandler *editFocusHandler;
} EditFocusHandlerItem;

class STemWinWindowManager: public IWmal {
private:
  static void eventHandler(WM_MESSAGE * pMsg);
  int32_t m_width;
  int32_t m_height;
  static const uint32_t MAX_BUTTONS_TOTAL = 200;
  static const uint32_t LISTVIEW_ROW_HEIGHT = 20;
  static const uint32_t MAX_WINDOWS_COUNT = 20;
  static const uint32_t MAX_TEXTS_COUNT = 100;
  static const uint32_t MAX_EDITS_COUNT = 100;
  static uint32_t buttonHandlersCount;
  static ButtonHandlerItem buttonEventHandlers[MAX_BUTTONS_TOTAL];
  static uint32_t paintHandlersCount;
  static PaintHandlerItem paintEventHandlers[MAX_WINDOWS_COUNT];
  static uint32_t clickHandlersCount;
  static ClickHandlerItem clickEventHandlers[MAX_WINDOWS_COUNT];
  static uint32_t textClickHandlersCount;
  static TextClickHandlerItem textClickHandlers[MAX_TEXTS_COUNT];
  static uint32_t editFocusHandlersCount;
  static EditFocusHandlerItem editFocusHandlers[MAX_EDITS_COUNT];
  static void buttonClicked(int32_t buttonHandle);
  static void ClickWindow(int32_t windowHandle, int32_t x, int32_t y);
  static void PaintWindow(int32_t windowHandle);
  static void TextCallback(WM_MESSAGE *msg);
  static void TextClicked(int32_t textHandle);
  static void EditFocused(int32_t editHandle);
public:
  STemWinWindowManager();
  virtual void Init(int32_t width, int32_t height);
  virtual int32_t CreateWin(int32_t x, int32_t y, int32_t width, int32_t height) override;
  virtual void DeleteControl(int32_t handle) override;
  virtual int32_t CreateWinMaximized() override;
  virtual void AssignPaintCallback(int32_t windowHandle, IPaintEventHandler *paintEventHandler) override;
  virtual void AssignWindowClickCallback(int32_t windowHandle, IClickEventHandler *clickEventHandler) override;
  virtual void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1) override;
  virtual void DrawTextHvCenter(int32_t x0, int32_t y0, const char *text) override;
  virtual void DrawTextVCenter(int32_t x0, int32_t y0, const char *text) override;
  virtual int32_t CreateText(int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height, const char *text) override;
  virtual void SetTextText(int32_t handle, const char *text) override;
  virtual void AssignTextClickCallback(int32_t textHandle, ITextClickEventHandler *textClickEventHandler) override;
  virtual int32_t CreateButton(int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height, const char *text,
      IButtonEventHandler *buttonEventHandler) override;
  virtual void SetButtonText(int32_t handle, const char *text) override;
  virtual bool IsButtonPressed(int32_t buttonHandle) override;
  virtual int32_t CreateListView(int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height) override;
  virtual void AddColumnToListView(int32_t listViewHandle, int32_t size,
      const char *name) override;
  virtual void AddRowToListView(int32_t listViewHandle, const char **row)
      override;
  virtual int32_t GetWidth() override;
  virtual int32_t GetHeight() override;
  virtual void Touch(int32_t x, int32_t y) override;
  virtual void Untouch() override;
  virtual void InvalidateWindow(int32_t windowHandle) override;
  virtual void Show(int32_t windowHandle) override;
  virtual void Hide(int32_t windowHandle) override;

  virtual void GetListViewText(int32_t listViewHandle, int32_t row,
      int32_t column, char *text, int32_t bufferSize) override;
  virtual int32_t CreateEdit(int32_t hParent, int32_t x, int32_t y,
      int32_t width, int32_t height, int32_t textLength, const char *text) override;
  virtual void SetEditText(int32_t editHandle, const char *text) override;
  virtual void GetEditText(int32_t editHandle, char *text, int32_t maxLength) override;
  virtual void SendChar(int32_t controlHandle, char character) override;
  virtual void SetFocus(int32_t controlHandle) override;
  virtual void AssignEditFocusCallback(int32_t editHandle, IEditFocusEventHandler *editFocusEventHandler) override;
  virtual void SetTextBkColor(int32_t textHandle, uint32_t color) override;
  virtual int32_t CreateComboBox(int32_t hParent, int32_t x, int32_t y,
      int32_t width, int32_t height, const char **items,
      const int32_t itemsCount) override;
  virtual int32_t GetComboBoxSelection(int32_t comboBoxHandle) override;
  virtual void SetComboBoxSelection(int32_t comboBoxHandle, int32_t selection)override;
  virtual void DeleteListViewRows(int32_t listViewHandle) override;
  virtual int32_t GetSelectedRow(int32_t listViewHandle) override;
  virtual bool Execute() override;
};

#endif
