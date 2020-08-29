#include "emWin_Wmal.h"
#include "strings.h"
#include "dialog.h"
#include "DebugLog.h"

uint32_t STemWinWindowManager::buttonHandlersCount;
ButtonHandlerItem STemWinWindowManager::buttonEventHandlers[MAX_BUTTONS_TOTAL];

uint32_t STemWinWindowManager::paintHandlersCount = 0;
PaintHandlerItem STemWinWindowManager::paintEventHandlers[MAX_WINDOWS_COUNT];

uint32_t STemWinWindowManager::clickHandlersCount = 0;
ClickHandlerItem STemWinWindowManager::clickEventHandlers[MAX_WINDOWS_COUNT];

uint32_t STemWinWindowManager::textClickHandlersCount = 0;
TextClickHandlerItem STemWinWindowManager::textClickHandlers[MAX_TEXTS_COUNT];

uint32_t STemWinWindowManager::editFocusHandlersCount = 0;
EditFocusHandlerItem STemWinWindowManager::editFocusHandlers[MAX_EDITS_COUNT];

STemWinWindowManager::STemWinWindowManager():
  m_width(0),
  m_height(0)
{
}

int32_t STemWinWindowManager::CreateWin(int32_t x, int32_t y, int32_t width, int32_t height)
{
  WM_HWIN hWindow = WINDOW_CreateEx(x,y,width,height,0,0,0,GUI_ID_USER
      ,&eventHandler);
  DBG_ASSERT(hWindow!=0);
  return hWindow;
}

void STemWinWindowManager::DeleteControl(int32_t handle)
{
  WM_DeleteWindow(handle);
  for(uint32_t i=0; i<buttonHandlersCount;i++)
  {
    if(buttonEventHandlers[i].buttonHandle == handle)
    {
      if(i<buttonHandlersCount-1)
      {
        buttonEventHandlers[i] = buttonEventHandlers[buttonHandlersCount-1];
      }
      buttonEventHandlers[buttonHandlersCount-1].buttonEventHandler = NULL;
      buttonEventHandlers[buttonHandlersCount-1].buttonHandle = 0;
      buttonHandlersCount--;
      break;
    }
  }
  for(uint32_t i=0; i<paintHandlersCount;i++)
  {
    if(paintEventHandlers[i].windowHandle == handle)
    {
      if(i<paintHandlersCount-1)
      {
        paintEventHandlers[i] = paintEventHandlers[paintHandlersCount-1];
      }
      paintEventHandlers[paintHandlersCount-1].paintWindowHandler = NULL;
      paintEventHandlers[paintHandlersCount-1].windowHandle = 0;
      paintHandlersCount--;
      break;
    }
  }
  for(uint32_t i=0; i<clickHandlersCount;i++)
  {
    if(clickEventHandlers[i].windowHandle == handle)
    {
      if(i<clickHandlersCount-1)
      {
        clickEventHandlers[i] = clickEventHandlers[clickHandlersCount-1];
      }
      clickEventHandlers[clickHandlersCount-1].clickWindowHandler = NULL;
      clickEventHandlers[clickHandlersCount-1].windowHandle = 0;
      clickHandlersCount--;
      break;
    }
  }
}

int32_t STemWinWindowManager::CreateWinMaximized()
{
  return CreateWin(0, 0, m_width, m_height);
}

void STemWinWindowManager::Init(int32_t width, int32_t height)
{
  m_width = width;
  m_height = height;
}

int32_t STemWinWindowManager::GetWidth()
{
  return m_width;
}

int32_t STemWinWindowManager::GetHeight()
{
  return m_height;
}

int32_t STemWinWindowManager::CreateText(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text)
{
  TEXT_Handle hText = TEXT_CreateEx(x,y,width,height,parent,WM_CF_SHOW
      ,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT0,text);
  DBG_ASSERT(hText!=0);
  return hText;
}

void STemWinWindowManager::SetTextText(int32_t handle, const char *text)
{
  TEXT_SetText(handle, text);
}

void STemWinWindowManager::AssignTextClickCallback(int32_t textHandle, ITextClickEventHandler *textClickEventHandler)
{
  DBG_ASSERT(textClickHandlersCount<MAX_TEXTS_COUNT);
  WM_SetCallback(textHandle, TextCallback);
  textClickHandlers[textClickHandlersCount].clickHandler = textClickEventHandler;
  textClickHandlers[textClickHandlersCount].textHandle = textHandle;
  textClickHandlersCount++;
}


bool STemWinWindowManager::Execute()
{
  GUI_Exec();
  return true;
}

int32_t STemWinWindowManager::CreateButton(
    int32_t parent, int32_t x, int32_t y,
    int32_t width, int32_t height, const char *text,
    IButtonEventHandler *buttonEventHandler)
{
  BUTTON_Handle hButton = 0;
  if(buttonHandlersCount<MAX_BUTTONS_TOTAL-1)
  {
    hButton = BUTTON_CreateEx(x, y, width, height, parent, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
    DBG_ASSERT(hButton!=0);
    BUTTON_SetText(hButton, text);

    buttonEventHandlers[buttonHandlersCount].buttonHandle = hButton;
    buttonEventHandlers[buttonHandlersCount].buttonEventHandler = buttonEventHandler;
    buttonHandlersCount++;
  }
  else
  {
    DBG_ASSERT(buttonHandlersCount<MAX_BUTTONS_TOTAL-1);
  }
  return hButton;
}

void STemWinWindowManager::SetButtonText(int32_t handle, const char *text)
{
  BUTTON_SetText(handle, text);
}

bool STemWinWindowManager::IsButtonPressed(int32_t buttonHandle)
{
  return BUTTON_IsPressed(buttonHandle);
}

int32_t STemWinWindowManager::CreateListView(int32_t parent, int32_t x,
    int32_t y, int32_t width, int32_t height)
{
  LISTVIEW_Handle hList = LISTVIEW_CreateEx(x, y, width, height,
      parent, WM_CF_SHOW, 0, GUI_ID_LISTVIEW0);
  DBG_ASSERT(hList!=0);
  LISTVIEW_SetRowHeight(hList, LISTVIEW_ROW_HEIGHT+10);
  HEADER_Handle hHeader = LISTVIEW_GetHeader(hList);

  HEADER_SetDragLimit(hHeader,1);
  HEADER_SetHeight(hHeader,LISTVIEW_ROW_HEIGHT);

  LISTVIEW_SetAutoScrollV(hList,1);

  return hList;
}

void STemWinWindowManager::GetListViewText(int32_t listViewHandle, int32_t row,
    int32_t column, char *text, int32_t bufferSize)
{
  LISTVIEW_GetItemText(listViewHandle, column, row, text, bufferSize);
}

int32_t STemWinWindowManager::CreateEdit(int32_t hParent, int32_t x, int32_t y,
    int32_t width, int32_t height, int32_t textLength, const char *text)
{
  EDIT_Handle hEdit = EDIT_CreateEx(x,y,width,height,hParent,WM_CF_SHOW,0,GUI_ID_EDIT0, textLength);
  DBG_ASSERT(hEdit!=0);
  EDIT_SetText(hEdit, text);
  return hEdit;
}

void STemWinWindowManager::SetEditText(int32_t editHandle, const char *text)
{
  EDIT_SetText(editHandle, text);
}

void STemWinWindowManager::GetEditText(int32_t editHandle, char *text,
    int32_t maxLength)
{
  EDIT_GetText(editHandle, text, maxLength);
}

void STemWinWindowManager::SendChar(int32_t controlHandle, char character)
{
  EDIT_AddKey(controlHandle, character);
}

void STemWinWindowManager::SetFocus(int32_t controlHandle)
{
  WM_SetFocus(controlHandle);
}

void STemWinWindowManager::AssignEditFocusCallback(int32_t editHandle, IEditFocusEventHandler *editFocusEventHandler)
{
  DBG_ASSERT(editFocusHandlersCount < MAX_EDITS_COUNT);
  editFocusHandlers[editFocusHandlersCount].editFocusHandler = editFocusEventHandler;
  editFocusHandlers[editFocusHandlersCount].editHandle = editHandle;
  editFocusHandlersCount++;
}

void STemWinWindowManager::SetTextBkColor(int32_t textHandle, uint32_t color)
{
  TEXT_SetBkColor(textHandle, color);
}

int32_t STemWinWindowManager::CreateComboBox(int32_t hParent, int32_t x, int32_t y,
    int32_t width, int32_t height, const char **items,
    const int32_t itemsCount)
{
  DROPDOWN_Handle hComboBox = DROPDOWN_CreateEx(x, y, width, height, hParent,
      WM_CF_SHOW,DROPDOWN_CF_AUTOSCROLLBAR,GUI_ID_DROPDOWN0);

  DBG_ASSERT(hComboBox!=0);

  DROPDOWN_SetTextHeight(hComboBox,19);
  DROPDOWN_SetTextAlign(hComboBox,TEXT_CF_VCENTER | TEXT_CF_LEFT);
  DROPDOWN_SetItemSpacing(hComboBox,10);

  for (int32_t i=0; i<itemsCount; i++)
  {
    DROPDOWN_AddString(hComboBox, items[i]);
  }
  return hComboBox;
}

int32_t STemWinWindowManager::GetComboBoxSelection(int32_t comboBoxHandle)
{
  return DROPDOWN_GetSel(comboBoxHandle);
}

void STemWinWindowManager::SetComboBoxSelection(int32_t comboBoxHandle, int32_t selection)
{
  DROPDOWN_SetSelExp(comboBoxHandle, selection);
}

void STemWinWindowManager::DeleteListViewRows(int32_t listViewHandle)
{
  LISTVIEW_DeleteAllRows(listViewHandle);
}

int32_t STemWinWindowManager::GetSelectedRow(int32_t listViewHandle)
{
  return LISTVIEW_GetSel(listViewHandle);
}

void STemWinWindowManager::AddColumnToListView(int32_t listViewHandle,
    int32_t size, const char *name)
{
  LISTVIEW_AddColumn(listViewHandle,size,name,GUI_TA_HCENTER|GUI_TA_VCENTER);
}

void STemWinWindowManager::AddRowToListView(int32_t listViewHandle, const char **row)
{
  LISTVIEW_AddRow(listViewHandle, row);
}

void STemWinWindowManager::Touch(int32_t x, int32_t y)
{
  GUI_TOUCH_StoreState(x, y);
}

void STemWinWindowManager::Untouch()
{
  GUI_TOUCH_StoreState(-1, -1);
}

void STemWinWindowManager::eventHandler(WM_MESSAGE * pMsg)
{
  switch (pMsg->MsgId)
  {
    case WM_NOTIFY_PARENT:
      if(pMsg->Data.v == WM_NOTIFICATION_RELEASED)
      {
        buttonClicked(pMsg->hWinSrc);
      }
      if(pMsg->Data.v == WM_NOTIFICATION_CLICKED)
      {
        const WM_PID_STATE_CHANGED_INFO *pidState = (WM_PID_STATE_CHANGED_INFO*)pMsg->Data.p;
        int32_t x = pidState->x;
        int32_t y = pidState->y;
        ClickWindow(pMsg->MsgId,x,y);
        EditFocused(pMsg->hWinSrc);
      }
      break;
    case WM_PID_STATE_CHANGED:
      {
        const WM_PID_STATE_CHANGED_INFO *pidState = (WM_PID_STATE_CHANGED_INFO*)pMsg->Data.p;
        if((pidState->State > 0)&&(pidState->StatePrev == 0))
        {
          int32_t x = pidState->x;
          int32_t y = pidState->y;
          ClickWindow(pMsg->hWin,x,y);
        }
      }
      break;
    case WM_PAINT:
      PaintWindow(pMsg->hWin);
      break;
    default:
      WM_DefaultProc(pMsg);
      break;
  }
}

void STemWinWindowManager::Show(int32_t windowHandle)
{
  WM_ShowWindow(windowHandle);
  WM_BringToTop(windowHandle);
}

void STemWinWindowManager::Hide(int32_t windowHandle)
{
  WM_HideWindow(windowHandle);
}

void STemWinWindowManager::AssignPaintCallback(int32_t windowHandle, IPaintEventHandler *paintEventHandler)
{
  DBG_ASSERT(paintHandlersCount < MAX_WINDOWS_COUNT);
  paintEventHandlers[paintHandlersCount].paintWindowHandler = paintEventHandler;
  paintEventHandlers[paintHandlersCount].windowHandle = windowHandle;
  paintHandlersCount++;
}

void STemWinWindowManager::AssignWindowClickCallback(int32_t windowHandle, IClickEventHandler *clickEventHandler)
{
  DBG_ASSERT(clickHandlersCount < MAX_WINDOWS_COUNT);
  clickEventHandlers[clickHandlersCount].clickWindowHandler = clickEventHandler;
  clickEventHandlers[clickHandlersCount].windowHandle = windowHandle;
  clickHandlersCount++;
}

void STemWinWindowManager::DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
  GUI_DrawLine(x0, y0, x1, y1);
}

void STemWinWindowManager::DrawTextHvCenter(int32_t x0, int32_t y0, const char *text)
{
  GUI_SetTextAlign(GUI_TA_HCENTER | GUI_TA_VCENTER);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_DispStringAt(text, x0, y0);
}

void STemWinWindowManager::DrawTextVCenter(int32_t x0, int32_t y0, const char *text)
{
  GUI_SetTextAlign(GUI_TA_LEFT | GUI_TA_VCENTER);
  GUI_SetTextMode(GUI_TM_TRANS);
  GUI_DispStringAt(text, x0, y0);
}

void STemWinWindowManager::InvalidateWindow(int32_t windowHandle)
{
  WM_InvalidateWindow(windowHandle);
}

void STemWinWindowManager::buttonClicked(int32_t buttonHandle)
{
  for(uint32_t i=0; i<buttonHandlersCount; i++)
  {
    if(buttonEventHandlers[i].buttonHandle == buttonHandle)
    {
      buttonEventHandlers[i].buttonEventHandler->ButtonEventHandler(buttonHandle);
      break;
    }
  }
}

void STemWinWindowManager::TextClicked(int32_t textHandle)
{
  for(uint32_t i=0; i<textClickHandlersCount; i++)
  {
    if(textClickHandlers[i].textHandle == textHandle)
    {
      textClickHandlers[i].clickHandler->TextClickEventHandler(textHandle);
      break;
    }
  }
}

void STemWinWindowManager::ClickWindow(int32_t windowHandle, int32_t x, int32_t y)
{
  for(uint32_t i=0; i<clickHandlersCount; i++)
  {
    if(clickEventHandlers[i].windowHandle == windowHandle)
    {
      DBG_ASSERT(clickEventHandlers[i].clickWindowHandler != NULL);
      clickEventHandlers[i].clickWindowHandler->ClickEventHandler(windowHandle, x, y);
      break;
    }
  }
}

void STemWinWindowManager::PaintWindow(int32_t windowHandle)
{
  GUI_SetColor(GUI_BLACK);
  for(uint32_t i=0; i<paintHandlersCount; i++)
  {
    if(paintEventHandlers[i].windowHandle == windowHandle)
    {
      DBG_ASSERT(paintEventHandlers[i].paintWindowHandler != NULL);
      GUI_Clear();
      paintEventHandlers[i].paintWindowHandler->PaintEventHandler(windowHandle);
      break;
    }
  }
}

void STemWinWindowManager::EditFocused(int32_t editHandle)
{
  for(uint32_t i=0; i<editFocusHandlersCount; i++)
  {
    if(editFocusHandlers[i].editHandle == editHandle)
    {
      if(editFocusHandlers[i].editFocusHandler!=NULL)
      {
        editFocusHandlers[i].editFocusHandler->EditFocusEventHandler(editHandle);
      }
      break;
    }
  }
}

void STemWinWindowManager::TextCallback(WM_MESSAGE *msg)
{
  GUI_PID_STATE * pidState;
  switch (msg->MsgId)
  {
    case WM_TOUCH:
      TextClicked(msg->hWin);
      pidState = (GUI_PID_STATE*)msg->Data.p;
      if (pidState->Pressed) {
        TextClicked(msg->hWin);
      }
      break;
    default:
      TEXT_Callback(msg);
      break;
  }
}

