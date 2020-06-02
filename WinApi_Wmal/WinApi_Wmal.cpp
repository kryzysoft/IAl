#include "WinApi_Wmal.h"
#include "strings.h"
#include "DebugLog.h"
#include "windowsx.h"
#include <commctrl.h>

uint32_t WinApi_Wmal::buttonHandlersCount = 0;
ButtonHandlerItem WinApi_Wmal::buttonEventHandlers[MAX_BUTTONS_TOTAL];

uint32_t WinApi_Wmal::paintHandlersCount = 0;
PaintHandlerItem WinApi_Wmal::paintEventHandlers[MAX_WINDOWS_COUNT];

uint32_t WinApi_Wmal::clickHandlersCount = 0;
ClickHandlerItem WinApi_Wmal::clickEventHandlers[MAX_WINDOWS_COUNT];

uint32_t WinApi_Wmal::windowsCount = 0;
HWND WinApi_Wmal::windowHandles[MAX_WINDOWS_COUNT];

uint32_t WinApi_Wmal::textsCount = 0;
TextStruct WinApi_Wmal::textStructs[MAX_STATIC_TEXTS];

uint32_t WinApi_Wmal::editFocusHandlersCount = 0;
EditFocusHandlerItem WinApi_Wmal::editFocusHandlers[MAX_EDITS_COUNT];

HFONT WinApi_Wmal::hFont;

bool WinApi_Wmal::paintInProgress = false;

HDC WinApi_Wmal::currentHdc;


WinApi_Wmal::WinApi_Wmal(HINSTANCE appInstance):
  m_width(0),
  m_height(0),
  m_hMainWindow(0),
  m_mainWindowCreated(false)
{
  m_appInstance = appInstance;
  const char* fontName = "Arial";
  const int32_t nFontSize = 7;
  HDC hdc = GetDC(0);
  DBG_ASSERT(hdc!=NULL);
  LOGFONT logFont = {0};
  logFont.lfHeight = -MulDiv(nFontSize, GetDeviceCaps(hdc, LOGPIXELSY), 72);
  logFont.lfWeight = FW_BOLD;
  strcpy(logFont.lfFaceName, fontName);
  hFont = CreateFontIndirect(&logFont);
  DBG_ASSERT(hFont!=NULL);
  int32_t releaseResult = ReleaseDC(0, hdc);
  DBG_ASSERT(releaseResult == 1);

  WNDCLASSEX wc;
  wc.cbSize = sizeof( WNDCLASSEX );
  wc.style = 0;
  wc.lpfnWndProc = EventHandler;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = m_appInstance;
  wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
  DBG_ASSERT(wc.hIcon!=NULL);
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  DBG_ASSERT(wc.hCursor!=NULL);

  wc.lpszMenuName = NULL;
  wc.lpszClassName = "NewWinApi_WmalWindow";
  wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
  DBG_ASSERT(wc.hIconSm!=NULL);
  m_hBkBrush = CreateSolidBrush(DEFAULT_BK_COLOR);
  DBG_ASSERT(m_hBkBrush!=NULL);
  wc.hbrBackground = m_hBkBrush;

  int32_t registerClassResult = RegisterClassEx(&wc);
  DBG_ASSERT((registerClassResult != 0));
}

void WinApi_Wmal::Init(int32_t width, int32_t height)
{
  m_width = width;
  m_height = height;

  m_mainWindowCreated = false;

}

void WinApi_Wmal::CreateMainWindow()
{
  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = m_width;
  rect.bottom = m_height;

  const uint32_t WINDOW_STYLE = ((WS_CLIPCHILDREN | WS_OVERLAPPEDWINDOW) & (~(WS_MAXIMIZE | WS_MINIMIZE | WS_THICKFRAME)));

  bool adjustResult = AdjustWindowRectEx(&rect,WINDOW_STYLE,false,WS_EX_CLIENTEDGE);
  DBG_ASSERT(adjustResult==true);

  HWND hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, "NewWinApi_WmalWindow", "WinApi_Wmal", WINDOW_STYLE | WS_VISIBLE,
  30, 30, rect.right - rect.left, rect.bottom-rect.top, NULL, NULL, m_appInstance, NULL );
  DBG_ASSERT(hwnd != NULL);
  windowHandles[windowsCount] = hwnd;
  windowsCount++;
  DBG_ASSERT(windowsCount<MAX_WINDOWS_COUNT);

  m_hMainWindow = hwnd;
  m_mainWindowCreated = true;
}

int32_t WinApi_Wmal::CreateWin(int32_t x, int32_t y, int32_t width, int32_t height)
{
  if(!m_mainWindowCreated)
  {
    CreateMainWindow();
  }
  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = width;
  rect.bottom = height;

  const uint32_t WINDOW_STYLE = WS_POPUP|WS_SYSMENU | WS_CHILD;// WS_OVERLAPPEDWINDOW & (~(WS_MAXIMIZE | WS_MINIMIZE | WS_THICKFRAME | WS_BORDER | WS_CAPTION));

  bool adjustResult = AdjustWindowRectEx(&rect,WINDOW_STYLE,false,/*WS_EX_CLIENTEDGE|*/ WS_EX_TOOLWINDOW);
  DBG_ASSERT(adjustResult==true);

  HWND hwnd = CreateWindowEx(WS_EX_COMPOSITED| /*WS_EX_CLIENTEDGE|*/WS_EX_TOOLWINDOW, "NewWinApi_WmalWindow", "WinApi_Wmal", WINDOW_STYLE,
  x, y, rect.right - rect.left, rect.bottom-rect.top, NULL, NULL, m_appInstance, NULL );

  SetParent(hwnd,m_hMainWindow);
  DBG_ASSERT(hwnd != NULL);
  windowHandles[windowsCount] = hwnd;
  windowsCount++;
  DBG_ASSERT(windowsCount<MAX_WINDOWS_COUNT);
  BringWindowToTop(hwnd);
  return (int32_t)hwnd;
}

void WinApi_Wmal::DeleteControl(int32_t handle)
{
  bool retVal = DestroyWindow((HWND)handle);
  DBG_ASSERT(retVal);

  for (uint32_t i=0; i<buttonHandlersCount; i++)
  {
    if(buttonEventHandlers[i].buttonHandle == handle)
    {
      if(i<buttonHandlersCount-1)
      {
        buttonEventHandlers[i] = buttonEventHandlers[buttonHandlersCount-1];
      }
      buttonHandlersCount--;
      break;
    }
  }

  for (uint32_t i=0; i<paintHandlersCount; i++)
  {
    if(paintEventHandlers[i].windowHandle == handle)
    {
      if(i<paintHandlersCount-1)
      {
        paintEventHandlers[i] = paintEventHandlers[paintHandlersCount-1];
      }
      paintHandlersCount--;
      break;
    }
  }

  for (uint32_t i=0; i<clickHandlersCount; i++)
  {
    if(clickEventHandlers[i].windowHandle == handle)
    {
      if(i<clickHandlersCount-1)
      {
        clickEventHandlers[i] = clickEventHandlers[clickHandlersCount-1];
      }
      clickHandlersCount--;
      break;
    }
  }

  for (uint32_t i=0; i<windowsCount; i++)
  {
    if(windowHandles[i] == (HWND)handle)
    {
      if(i<windowsCount-1)
      {
        windowHandles[i] = windowHandles[windowsCount-1];
      }
      windowsCount--;
      break;
    }
  }

  for (uint32_t i=0; i<textsCount; i++)
  {
    if(textStructs[i].textHandle == (HWND)handle)
    {
      if(i<textsCount-1)
      {
        textStructs[i] = textStructs[textsCount-1];
      }
      textsCount--;
      break;
    }
  }

  for (uint32_t i=0; i<editFocusHandlersCount; i++)
  {
    if(editFocusHandlers[i].editHandle == handle)
    {
      if(i<editFocusHandlersCount-1)
      {
        editFocusHandlers[i] = editFocusHandlers[editFocusHandlersCount-1];
      }
      editFocusHandlersCount--;
      break;
    }
  }
}


int32_t WinApi_Wmal::CreateWinMaximized()
{
  return CreateWin(0,0,m_width,m_height);
}

int32_t WinApi_Wmal::GetWidth()
{
  DBG_ASSERT(m_width>0);
  return m_width;
}

int32_t WinApi_Wmal::GetHeight()
{
  DBG_ASSERT(m_height>0);
  return m_height;
}

int32_t WinApi_Wmal::CreateText(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text)
{
  HWND hStatic = CreateWindowEx(0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE |
  SS_CENTER | WS_EX_CLIENTEDGE, x, y, width, height, (HWND)parent, NULL, m_appInstance, NULL );

  SetWindowText(hStatic, text);

  textStructs[textsCount].textHandle = hStatic;
  textStructs[textsCount].bkColor = DEFAULT_BK_COLOR;
  textStructs[textsCount].parent = (HWND)parent;
  textStructs[textsCount].hBrush = CreateSolidBrush(DEFAULT_BK_COLOR);
  textStructs[textsCount].textClickEventHandler = NULL;
  DBG_ASSERT(textStructs[textsCount].hBrush != NULL);
  textsCount++;
  DBG_ASSERT(textsCount<MAX_STATIC_TEXTS);

  return (int32_t)hStatic;
}

void WinApi_Wmal::SetTextText(int32_t handle, const char *text)
{
  SetWindowText((HWND)handle, text);
}

void WinApi_Wmal::AssignTextClickCallback(int32_t textHandle, ITextClickEventHandler *textClickEventHandler)
{
  for(uint32_t i=0; i<textsCount; i++)
  {
    if(textStructs[i].textHandle == (HWND)textHandle)
    {
      textStructs[i].textClickEventHandler = textClickEventHandler;
      SetWindowLong((HWND)textHandle, GWL_STYLE, WS_CHILD | WS_VISIBLE | SS_CENTER | WS_EX_CLIENTEDGE | SS_NOTIFY);
    }
  }
}

void WinApi_Wmal::SetTextBkColor(int32_t textHandle, uint32_t color)
{
  for(uint32_t i=0; i<textsCount; i++)
  {
    if(textHandle == (int32_t)textStructs[i].textHandle)
    {
      textStructs[i].bkColor = color;
      DeleteObject(textStructs[i].hBrush);
      textStructs[i].hBrush = CreateSolidBrush(color);
      RECT rect;
      GetClientRect((HWND)textHandle, &rect);
      InvalidateRect(textStructs[i].textHandle, &rect, false);
      UpdateWindow(textStructs[i].textHandle);
      break;
    }
  }
}

int32_t WinApi_Wmal::CreateComboBox(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, const char **items, const int32_t itemsCount)
{
  HWND hComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, x, y, width, height, (HWND)hParent, NULL, m_appInstance, NULL);
  for(int32_t i = 0; i<itemsCount; i++)
  {
    SendMessage(hComboBox, CB_ADDSTRING, 0,(LPARAM)const_cast<char*>(items[i]));
  }
  SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
  return (int32_t)hComboBox;
}

int32_t WinApi_Wmal::GetComboBoxSelection(int32_t comboBoxHandle)
{
  int32_t index=SendMessage((HWND)comboBoxHandle,CB_GETCURSEL,0,0);
  DBG_ASSERT(index != CB_ERR);
  return index;
}

void WinApi_Wmal::SetComboBoxSelection(int32_t comboBoxHandle, int32_t selection)
{
  int32_t result = SendMessage((HWND)comboBoxHandle,CB_SETCURSEL,selection,0);
  DBG_ASSERT(result != CB_ERR);
}

bool WinApi_Wmal::Execute()
{
  MSG msg;
  if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
  {
    bool done = false;
    if(msg.message == WM_QUIT)
    {
      done = true;
    }

    TranslateMessage(&msg);
    DispatchMessage(&msg);

    if(done)
    {
      return false;
    }
  }
  return true;
}

int32_t WinApi_Wmal::CreateButton(
    int32_t parent, int32_t x, int32_t y,
    int32_t width, int32_t height, const char *text,
    IButtonEventHandler *buttonEventHandler)
{
  HWND hwndButton = CreateWindow(
    "BUTTON",text,WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
    x, y, width, height, (HWND)parent, NULL, m_appInstance, NULL);

  DBG_ASSERT(hwndButton != NULL);

  buttonEventHandlers[buttonHandlersCount].buttonHandle = (int32_t)hwndButton;
  buttonEventHandlers[buttonHandlersCount].buttonEventHandler = buttonEventHandler;
  SendMessage(hwndButton, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));

  buttonHandlersCount++;
  DBG_ASSERT(buttonHandlersCount < MAX_BUTTONS_TOTAL);
  return (int32_t)hwndButton;
}

void WinApi_Wmal::SetButtonText(int32_t handle, const char *text)
{
  SetWindowText((HWND)handle, text);
}

bool WinApi_Wmal::IsButtonPressed(int32_t buttonHandle)
{
  return ((Button_GetState((HWND)buttonHandle) & BST_PUSHED)!=0);
}

void WinApi_Wmal::AssignPaintCallback(int32_t windowHandle, IPaintEventHandler *paintEventHandler)
{
  DBG_ASSERT(paintHandlersCount < MAX_WINDOWS_COUNT);
  paintEventHandlers[paintHandlersCount].paintWindowHandler = paintEventHandler;
  paintEventHandlers[paintHandlersCount].windowHandle = windowHandle;
  paintHandlersCount++;
}

void WinApi_Wmal::AssignWindowClickCallback(int32_t windowHandle, IClickEventHandler *clickEventHandler)
{
  DBG_ASSERT(clickHandlersCount < MAX_WINDOWS_COUNT);
  clickEventHandlers[clickHandlersCount].clickWindowHandler = clickEventHandler;
  clickEventHandlers[clickHandlersCount].windowHandle = windowHandle;
  clickHandlersCount++;
}

int32_t WinApi_Wmal::CreateListView(int32_t parent, int32_t x,
    int32_t y, int32_t width, int32_t height)
{
  // WS_EX_COMPOSITED is incopmatible with ListView
  // So we need to remove that style form the parent window
  // This disables double buffering for specific window, but hopefully we will never need that
  SetWindowLong((HWND)parent, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

  HWND hListView = CreateWindowEx( 0, WC_LISTVIEW, NULL, WS_CHILD | WS_VISIBLE | LVS_REPORT |
  LVS_EDITLABELS, x, y, width, height, (HWND)parent,0, m_appInstance, NULL );

  DBG_ASSERT(hListView != NULL);

  SendMessage(hListView, LVM_SETEXTENDEDLISTVIEWSTYLE,
    LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

  return (int32_t)hListView;
}

void WinApi_Wmal::AddColumnToListView(int32_t listViewHandle,
    int32_t size, const char *name)
{
  HWND hWndHdr = (HWND)SendMessage((HWND)(listViewHandle), LVM_GETHEADER, 0, 0);
  DBG_ASSERT(hWndHdr != NULL);

  int32_t columnCount = (int32_t)SendMessage(hWndHdr, HDM_GETITEMCOUNT, 0, 0L);

  LVCOLUMN lvc;
  lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

  lvc.iSubItem = 0;
  lvc.cx = size;
  lvc.pszText = const_cast<char*>(name);
  ListView_InsertColumn( (HWND)listViewHandle, columnCount, & lvc );
}

void WinApi_Wmal::AddRowToListView(int32_t listViewHandle, const char **row)
{

  HWND hWndHdr = (HWND)SendMessage((HWND)(listViewHandle), LVM_GETHEADER, 0, 0);
  DBG_ASSERT(hWndHdr != NULL);

  int32_t columnCount = (int32_t)SendMessage(hWndHdr, HDM_GETITEMCOUNT, 0, 0L);

  LVITEM lvi;
  memset(&lvi,0,sizeof(LVITEM));
  lvi.mask      = LVIF_TEXT | LVIF_PARAM | LVIF_STATE;
  lvi.state     = 0;
  lvi.stateMask = 0;
  lvi.iItem = 65535;

  int res = ListView_InsertItem((HWND)listViewHandle, &lvi);

  for(int32_t i=0; i<columnCount; i++)
  {
    lvi.iItem    = i;
    lvi.iSubItem = 0;
    ListView_SetItemText((HWND)listViewHandle,res,i,const_cast<char*>(row[i]));
  }
}

void WinApi_Wmal::GetListViewText(int32_t listViewHandle, int32_t row, int32_t column, char *text, int32_t bufferSize)
{
  ListView_GetItemText((HWND)listViewHandle,row,column,text,bufferSize);
}

void WinApi_Wmal::DeleteListViewRows(int32_t listViewHandle)
{
  ListView_DeleteAllItems((HWND)listViewHandle);
}

int32_t WinApi_Wmal::GetSelectedRow(int32_t listViewHandle)
{
  return ListView_GetSelectionMark((HWND)listViewHandle);
}

int32_t WinApi_Wmal::CreateEdit(int32_t hParent, int32_t x, int32_t y, int32_t width, int32_t height, int32_t textLength, const char *text)
{
  HWND hText = CreateWindowEx(0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, x, y, width, height,
    (HWND)hParent, NULL, m_appInstance, NULL );
  DBG_ASSERT(hText != NULL);
  SetWindowText(hText, text);
  SendMessage(hText, EM_SETLIMITTEXT, (WPARAM)textLength, (LPARAM)0);
  return (int32_t)hText;
}

void WinApi_Wmal::SetEditText(int32_t editHandle, const char *text)
{
  SetWindowText((HWND)editHandle, text);
}

void WinApi_Wmal::GetEditText(int32_t editHandle, char *text, int32_t maxLength)
{
  GetWindowText((HWND)editHandle, text, maxLength);
}

void WinApi_Wmal::SendChar(int32_t controlHandle, char character)
{
  SendMessage((HWND)controlHandle,WM_CHAR, character, 1);
}

void WinApi_Wmal::SetFocus(int32_t controlHandle)
{
  ::SetFocus((HWND)controlHandle);
}

void WinApi_Wmal::AssignEditFocusCallback(int32_t editHandle, IEditFocusEventHandler *editFocusEventHandler)
{
  DBG_ASSERT(editFocusHandlersCount < MAX_EDITS_COUNT);
  editFocusHandlers[editFocusHandlersCount].editFocusHandler = editFocusEventHandler;
  editFocusHandlers[editFocusHandlersCount].editHandle = editHandle;
  editFocusHandlersCount++;
}


void WinApi_Wmal::Touch(int32_t x, int32_t y)
{
}

void WinApi_Wmal::Untouch()
{

}

void WinApi_Wmal::Show(int32_t windowHandle)
{
  ShowWindow((HWND)windowHandle, SW_SHOW);
  bool updateWindowResult = UpdateWindow((HWND)windowHandle);
  DBG_ASSERT(updateWindowResult);
}

void WinApi_Wmal::Hide(int32_t windowHandle)
{
  ShowWindow((HWND)windowHandle, SW_HIDE);
}

void WinApi_Wmal::InvalidateWindow(int32_t windowHandle)
{
  RECT rect;
  bool result = false;
  result = GetClientRect((HWND)windowHandle, &rect);
  DBG_ASSERT(result);
  result = InvalidateRect((HWND)windowHandle, &rect, false);
  DBG_ASSERT(result);
  result = UpdateWindow((HWND)windowHandle);
  DBG_ASSERT(result);
}

void WinApi_Wmal::ButtonClicked(int32_t buttonHandle)
{
  for(uint32_t i=0; i<buttonHandlersCount; i++)
  {
    if(buttonEventHandlers[i].buttonHandle == buttonHandle)
    {
      if(buttonEventHandlers[i].buttonEventHandler!=NULL)
      {
        buttonEventHandlers[i].buttonEventHandler->ButtonEventHandler(buttonHandle);

      }
      break;
    }
  }
}

void WinApi_Wmal::TextClicked(int32_t textHandle)
{
  for(uint32_t i=0; i<textsCount; i++)
  {
    if(textStructs[i].textHandle == (HWND)textHandle)
    {
      if(textStructs[i].textClickEventHandler!=NULL)
      {
        textStructs[i].textClickEventHandler->TextClickEventHandler(textHandle);
      }
      break;
    }
  }
}

void WinApi_Wmal::EditFocused(int32_t editHandle)
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


void WinApi_Wmal::DrawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{

  DBG_ASSERT(paintInProgress == true);
  bool result = MoveToEx(currentHdc, x0, y0, NULL);
  DBG_ASSERT(result == true);
  result = LineTo(currentHdc, x1, y1);
  DBG_ASSERT(result == true);
}

void WinApi_Wmal::DrawTextHvCenter(int32_t x0, int32_t y0, const char *text)
{
  DBG_ASSERT(paintInProgress == true);
  int32_t result = SetBkMode(currentHdc, TRANSPARENT);
  DBG_ASSERT(result!=0);
  RECT rect;
  rect.left = x0;
  rect.right = x0;
  rect.top = y0;
  rect.bottom = y0;
  result = DrawText(currentHdc, text, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
  DBG_ASSERT(result > 0);
}

void WinApi_Wmal::DrawTextVCenter(int32_t x0, int32_t y0, const char *text)
{
  DBG_ASSERT(paintInProgress == true);
  int32_t result = SetBkMode(currentHdc, TRANSPARENT);
  DBG_ASSERT(result!=0);
  RECT rect;
  rect.left = x0;
  rect.right = x0;
  rect.top = y0;
  rect.bottom = y0;
  result = DrawText(currentHdc, text, -1, &rect, DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
  DBG_ASSERT(result > 0);
}

bool WinApi_Wmal::PaintWindow(int32_t windowHandle)
{
  bool retVal = false;
  for(uint32_t i=0; i<paintHandlersCount; i++)
  {
    if(paintEventHandlers[i].windowHandle == windowHandle)
    {
      DBG_ASSERT(paintEventHandlers[i].paintWindowHandler != NULL);
      PAINTSTRUCT ps;
      currentHdc = BeginPaint((HWND)windowHandle, &ps);
      DBG_ASSERT(currentHdc != NULL);
      HGDIOBJ hGdiObj = SelectObject( currentHdc, hFont );
      DBG_ASSERT(hGdiObj!=NULL);
      paintInProgress = true;
      RECT rect;
      GetWindowRect((HWND)windowHandle,&rect);
      rect.right = rect.right - rect.left;
      rect.bottom = rect.bottom - rect.top;
      rect.left = 0;
      rect.top = 0;
      HBRUSH hBrush = CreateSolidBrush(DEFAULT_BK_COLOR);
      FillRect(currentHdc,&rect,hBrush);
      DeleteObject(hBrush);
      paintEventHandlers[i].paintWindowHandler->PaintEventHandler(windowHandle);
      paintInProgress = false;
      EndPaint((HWND)windowHandle, &ps);
      retVal = true;
      break;
    }
  }
  return retVal;
}

bool WinApi_Wmal::ClickWindow(int32_t windowHandle, int32_t x, int32_t y)
{
  bool retVal = false;
  for(uint32_t i=0; i<clickHandlersCount; i++)
  {
    if(clickEventHandlers[i].windowHandle == windowHandle)
    {
      DBG_ASSERT(clickEventHandlers[i].clickWindowHandler != NULL);
      clickEventHandlers[i].clickWindowHandler->ClickEventHandler(windowHandle, x, y);
      retVal = true;
      break;
    }
  }
  return retVal;
}

LRESULT CALLBACK WinApi_Wmal::EventHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  bool destroyResult;
  LRESULT lr = 0;
  int32_t xPos = 0;
  int32_t yPos = 0;

  switch( msg )
  {
    case WM_CLOSE:
      destroyResult = DestroyWindow(hwnd);
      DBG_ASSERT(destroyResult);
    break;

    case WM_CTLCOLORSTATIC:
      for (uint32_t i=0; i<textsCount; i++)
      {
        if((HWND)lParam == textStructs[i].textHandle)
        {
          int32_t setResult = SetBkMode((HDC)wParam, TRANSPARENT);
          DBG_ASSERT(setResult != 0);
          return (LRESULT)textStructs[i].hBrush;
        }
      }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
    break;

    case WM_COMMAND:
      if(HIWORD(wParam) == BN_CLICKED)
      {
        TextClicked(lParam);
        ButtonClicked(lParam);
      }
      if(HIWORD(wParam) == EN_SETFOCUS)
      {
        EditFocused(lParam);
      }
    break;
    case WM_PAINT:
      if(!PaintWindow((int32_t)hwnd))
      {
        lr = DefWindowProc( hwnd, msg, wParam, lParam );
      }
      break;

    case WM_LBUTTONDOWN:
      xPos = GET_X_LPARAM(lParam);
      yPos = GET_Y_LPARAM(lParam);
      if(!ClickWindow((int32_t)hwnd, xPos, yPos))
      {
        lr = DefWindowProc( hwnd, msg, wParam, lParam );
      }
      break;
    default:
        lr = DefWindowProc( hwnd, msg, wParam, lParam );
    break;
  }

  return lr;
}

WinApi_Wmal::~WinApi_Wmal()
{
  int32_t deleteResult;
  for(uint32_t i=0; i<textsCount; i++)
  {
    deleteResult = DeleteObject(textStructs[i].hBrush);
    DBG_ASSERT(deleteResult != 0);
  }
  deleteResult = DeleteObject(hFont);
  DBG_ASSERT(deleteResult != 0);
  deleteResult = DeleteObject(m_hBkBrush);
  DBG_ASSERT(deleteResult != 0);
}
