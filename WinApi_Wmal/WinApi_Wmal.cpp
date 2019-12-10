#include "WinApi_Wmal.h"
#include "strings.h"
#include "DebugLog.h"
#include <commctrl.h>

uint32_t WinApi_Wmal::buttonHandlersCount;
ButtonHandlerItem WinApi_Wmal::buttonEventHandlers[MAX_BUTTONS_TOTAL];

uint32_t WinApi_Wmal::windowsCount = 0;
HWND WinApi_Wmal::windowHandles[MAX_WINDOWS_COUNT];

uint32_t WinApi_Wmal::textsCount = 0;
TextStruct WinApi_Wmal::textStructs[MAX_STATIC_TEXTS];


WinApi_Wmal::WinApi_Wmal(HINSTANCE appInstance):
  m_width(0),
  m_height(0)
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
  m_hFont = CreateFontIndirect(&logFont);
  DBG_ASSERT(m_hFont!=NULL);
  int32_t releaseResult = ReleaseDC(0, hdc);
  DBG_ASSERT(releaseResult == 1);

  WNDCLASSEX wc;
  wc.cbSize = sizeof( WNDCLASSEX );
  wc.style = 0;
  wc.lpfnWndProc = eventHandler;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = m_appInstance;
  wc.hIcon = LoadIcon( NULL, IDI_APPLICATION );
  DBG_ASSERT(wc.hIcon!=NULL);
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  DBG_ASSERT(wc.hCursor!=NULL);
  wc.hbrBackground =( HBRUSH )( COLOR_WINDOW + 1 );
  wc.lpszMenuName = NULL;
  wc.lpszClassName = "NewWinApi_WmalWindow";
  wc.hIconSm = LoadIcon( NULL, IDI_APPLICATION );
  wc.hbrBackground = CreateSolidBrush(DEFAULT_BK_COLOR);
  DBG_ASSERT(wc.hIconSm!=NULL);

  int32_t registerClassResult = RegisterClassEx(&wc);
  DBG_ASSERT((registerClassResult != 0));
}

void WinApi_Wmal::Init(int32_t width, int32_t height)
{
  m_width = width;
  m_height = height;

  m_mainWindowCreated = false;

}

void WinApi_Wmal::createMainWindow()
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
    createMainWindow();
  }
  RECT rect;
  rect.left = 0;
  rect.top = 0;
  rect.right = width;
  rect.bottom = height;

  const uint32_t WINDOW_STYLE = WS_POPUP|WS_SYSMENU | WS_CHILD;// WS_OVERLAPPEDWINDOW & (~(WS_MAXIMIZE | WS_MINIMIZE | WS_THICKFRAME | WS_BORDER | WS_CAPTION));

  bool adjustResult = AdjustWindowRectEx(&rect,WINDOW_STYLE,false,/*WS_EX_CLIENTEDGE|*/ WS_EX_TOOLWINDOW);
  DBG_ASSERT(adjustResult==true);

  HWND hwnd = CreateWindowEx( /*WS_EX_CLIENTEDGE|*/WS_EX_TOOLWINDOW, "NewWinApi_WmalWindow", "WinApi_Wmal", WINDOW_STYLE,
  x, y, rect.right - rect.left, rect.bottom-rect.top, NULL, NULL, m_appInstance, NULL );

  SetParent(hwnd,m_hMainWindow);
  DBG_ASSERT(hwnd != NULL);
  windowHandles[windowsCount] = hwnd;
  windowsCount++;
  DBG_ASSERT(windowsCount<MAX_WINDOWS_COUNT);
  BringWindowToTop(hwnd);
  return (int32_t)hwnd;
}

int32_t WinApi_Wmal::GetWidth()
{
  return m_width;
}

int32_t WinApi_Wmal::GetHeight()
{
  return m_height;
}

int32_t WinApi_Wmal::CreateText(int32_t parent, int32_t x, int32_t y, int32_t width, int32_t height, const char *text)
{
  HWND hStatic = CreateWindowEx( 0, WC_STATIC, NULL, WS_CHILD | WS_VISIBLE |
  SS_CENTER | WS_EX_CLIENTEDGE, x, y, width, height, (HWND)parent, NULL, m_appInstance, NULL );

  SetWindowText(hStatic, text);

  textStructs[textsCount].textHandle = hStatic;
  textStructs[textsCount].bkColor = DEFAULT_BK_COLOR;
  textStructs[textsCount].parent = (HWND)parent;
  textStructs[textsCount].hBrush = CreateSolidBrush(DEFAULT_BK_COLOR);
  DBG_ASSERT(textStructs[textsCount].hBrush != NULL);
  textsCount++;
  DBG_ASSERT(textsCount<MAX_STATIC_TEXTS);

  return (int32_t)hStatic;
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
      InvalidateRect(textStructs[i].textHandle, &rect, true);
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
  if(GetMessage(&msg, NULL, 0, 0 ))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    return true;
  }
  else
  {
    return false;
  }
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
  SendMessage(hwndButton, WM_SETFONT, (WPARAM)m_hFont, (LPARAM)MAKELONG(TRUE, 0));

  buttonHandlersCount++;
  DBG_ASSERT(buttonHandlersCount < MAX_BUTTONS_TOTAL);
  return (int32_t)hwndButton;
}

int32_t WinApi_Wmal::CreateListView(int32_t parent, int32_t x,
    int32_t y, int32_t width, int32_t height)
{

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

void WinApi_Wmal::buttonClicked(int32_t buttonHandle)
{
  for(uint32_t i=0; i<buttonHandlersCount; i++)
  {
    DBG_ASSERT(buttonEventHandlers[i].buttonEventHandler != NULL);
    if(buttonEventHandlers[i].buttonHandle == buttonHandle)
    {
      buttonEventHandlers[i].buttonEventHandler->ButtonEventHandler(buttonHandle);
      break;
    }
  }
}

LRESULT CALLBACK WinApi_Wmal::eventHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  bool destroyResult;
  LRESULT lr = 0;
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
      buttonClicked(lParam);
    break;

    default:
        return DefWindowProc( hwnd, msg, wParam, lParam );
    break;
  }

  return lr;
}

WinApi_Wmal::~WinApi_Wmal()
{
  for(uint32_t i=0; i<textsCount; i++)
  {
    int32_t deleteResult = DeleteObject(textStructs[i].hBrush);
    DBG_ASSERT(deleteResult != 0);
  }
  DeleteObject(m_hFont);
}
