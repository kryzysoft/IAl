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
  virtual int32_t CreateWin(int32_t x, int32_t y, int32_t width, int32_t height) override;
  virtual int32_t CreateText(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text) override;
  virtual int32_t CreateButton(
      int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height, const char *text,
      IButtonEventHandler *buttonEventHandler) override;
  virtual int32_t CreateListView(int32_t parent, int32_t x, int32_t y,
      int32_t width, int32_t height) override;
  virtual void AddColumnToListView(int32_t listViewHandle,
      int32_t size, const char *name) override;
  virtual void AddRowToListView(int32_t listViewHandle, const char **row) override;
  virtual int32_t GetWidth() override;
  virtual int32_t GetHeight() override;
  virtual void Touch(int32_t x, int32_t y) override;
  virtual void Untouch() override;
  virtual void Show(int32_t windowHandle) override;
  virtual void Hide(int32_t windowHandle) override;


  virtual void GetListViewText(int32_t listViewHandle, int32_t row, int32_t column, char *text, int32_t bufferSize) override { };
  virtual int32_t CreateEdit(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, int32_t textLength, const char *text)override { return 0; };
  virtual void SetEditText(int32_t editHandle, const char *text)override{ };
  virtual void GetEditText(int32_t editHandle, char *text, int32_t maxLength)override{ };
  virtual void SetTextBkColor(int32_t textHandle, uint32_t color)override{ };
  virtual int32_t CreateComboBox(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, const char **items, const int32_t itemsCount)override{ return 0; };
  virtual int32_t GetComboBoxSelection(int32_t comboBoxHandle)override{ return 0; };
  virtual void SetComboBoxSelection(int32_t comboBoxHandle, int32_t selection)override{ };
  virtual void DeleteListViewRows(int32_t listViewHandle)override{ };
  virtual int32_t GetSelectedRow(int32_t listViewHandle)override{ return 0; };




  virtual bool Execute()override;
};

#endif
