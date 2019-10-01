#include "STemWinWindowManager.h"
#include "strings.h"
#include "dialog.h"

uint32_t STemWinWindowManager::buttonHandlersCount;
ButtonHandlerItem STemWinWindowManager::buttonEventHandlers[MAX_BUTTONS_TOTAL];

STemWinWindowManager::STemWinWindowManager():
  m_width(0),
  m_height(0)
{
}

int32_t STemWinWindowManager::CreateWin(int32_t x, int32_t y, int32_t width, int32_t height)
{
  return WINDOW_CreateEx(x,y,width,height,0,0,0,GUI_ID_USER,&eventHandler);
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
  return TEXT_CreateEx(x,y,width,height,parent,WM_CF_SHOW,TEXT_CF_HCENTER|TEXT_CF_VCENTER,GUI_ID_TEXT0,text);
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
  BUTTON_Handle hButton = BUTTON_CreateEx(x, y, width, height, parent, WM_CF_SHOW, 0, GUI_ID_BUTTON0);
  BUTTON_SetText(hButton, text);
  buttonEventHandlers[buttonHandlersCount].buttonHandle = hButton;
  buttonEventHandlers[buttonHandlersCount].buttonEventHandler = buttonEventHandler;
  buttonHandlersCount++;
  return hButton;
}

int32_t STemWinWindowManager::CreateListView(int32_t parent, int32_t x,
    int32_t y, int32_t width, int32_t height)
{
  LISTVIEW_Handle hList = LISTVIEW_CreateEx(x, y, width, height,
      parent, WM_CF_SHOW, 0, GUI_ID_LISTVIEW0);
  HEADER_Handle hHeader = LISTVIEW_GetHeader(hList);

  HEADER_SetDragLimit(hHeader,1);
  HEADER_SetHeight(hHeader,LISTVIEW_ROW_HEIGHT);

  LISTVIEW_SetAutoScrollV(hList,1);

  return hList;
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
      break;
    default:
      WM_DefaultProc(pMsg);
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


