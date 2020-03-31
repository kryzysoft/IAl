#ifndef I_WINDOW_MANAGER_H
#define I_WINDOW_MANAGER_H

#include "stdint.h"

class IClickEventHandler
{
public:
  virtual void ClickEventHandler(int32_t windowHandle, int32_t x, int32_t y) = 0;
};

class IButtonEventHandler
{
public:
  virtual void ButtonEventHandler(int32_t buttonHandle) = 0;
};

class IPaintEventHandler
{
public:
  virtual void PaintEventHandler(int32_t windowHandle) = 0;
};

class IWmal
{
public:
  virtual int32_t CreateWin(int32_t x, int32_t y, int32_t width, int32_t height) = 0;
  virtual void AssignPaintCallback(int32_t windowHandle, IPaintEventHandler *paintEventHandler) = 0;
  virtual void AssignClickCallback(int32_t windowHandle, IClickEventHandler *clickEventHandler) = 0;
  virtual void DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1) = 0;
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
  virtual void GetListViewText(int32_t listViewHandle, int32_t row, int32_t column, char *text, int32_t bufferSize) = 0;
  virtual int32_t CreateEdit(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, int32_t textLength, const char *text) = 0;
  virtual void SetEditText(int32_t editHandle, const char *text) = 0;
  virtual void GetEditText(int32_t editHandle, char *text, int32_t maxLength) = 0;
  virtual void SetTextBkColor(int32_t textHandle, uint32_t color) = 0;
  virtual int32_t CreateComboBox(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, const char **items, const int32_t itemsCount) = 0;
  virtual int32_t GetComboBoxSelection(int32_t comboBoxHandle) = 0;
  virtual void SetComboBoxSelection(int32_t comboBoxHandle, int32_t selection) = 0;
  virtual void DeleteListViewRows(int32_t listViewHandle) = 0;
  virtual int32_t GetSelectedRow(int32_t listViewHandle) = 0;
  virtual bool Execute() = 0;
  virtual int32_t GetWidth() = 0;
  virtual int32_t GetHeight() = 0;
  virtual void Touch(int32_t x, int32_t y) = 0;
  virtual void Untouch() = 0;
  virtual void InvalidateWindow(int32_t windowHandle) = 0;
  virtual void Show(int32_t windowHandle) = 0;
  virtual void Hide(int32_t windowHandle) = 0;
  virtual ~IWmal(){};
};

#endif
