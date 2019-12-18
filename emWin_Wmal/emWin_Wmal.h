#ifndef STEMWIN_WINDOW_MANAGER_H
#define STEMWIN_WINDOW_MANAGER_H

#include "IWmal.h"
#include "WM.h"

typedef struct
{
  int32_t buttonHandle;
  IButtonEventHandler *buttonEventHandler;
} ButtonHandlerItem;

class STemWinWindowManager: public IWmal
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


  virtual void GetListViewText(int32_t listViewHandle, int32_t row, int32_t column, char *text, int32_t bufferSize){ };
  virtual int32_t CreateEdit(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, int32_t textLength, const char *text){ };
  virtual void SetEditText(int32_t editHandle, const char *text){ };
  virtual void GetEditText(int32_t editHandle, char *text, int32_t maxLength){ };
  virtual void SetTextBkColor(int32_t textHandle, uint32_t color){ };
  virtual int32_t CreateComboBox(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, const char **items, const int32_t itemsCount){ };
  virtual int32_t GetComboBoxSelection(int32_t comboBoxHandle){ };
  virtual void SetComboBoxSelection(int32_t comboBoxHandle, int32_t selection){ };
  virtual void DeleteListViewRows(int32_t listViewHandle){ };
  virtual int32_t GetSelectedRow(int32_t listViewHandle){ };




  virtual bool Execute();
};

#endif
