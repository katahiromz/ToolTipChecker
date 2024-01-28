#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <stdio.h>

HINSTANCE g_hInst = NULL;
HWND g_hwndTT = NULL;

BOOL OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    g_hwndTT = CreateWindowEx(0, TOOLTIPS_CLASS, TEXT("This is a test"),
                              WS_POPUP | TTS_ALWAYSTIP,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              CW_USEDEFAULT, CW_USEDEFAULT,
                              hwnd, NULL, g_hInst, NULL);
    HWND hwndOK = GetDlgItem(hwnd, IDOK);
    TOOLINFO ti = {
        sizeof(ti), TTF_IDISHWND | TTF_SUBCLASS, hwnd, (UINT_PTR)hwndOK
    };
    GetWindowRect(hwndOK, &ti.rect);
    MapWindowPoints(NULL, hwnd, (POINT*)&ti.rect, 2);
    ti.lpszText = LPSTR_TEXTCALLBACK;
    ti.hinst = g_hInst;
    SendMessage(g_hwndTT, TTM_ADDTOOL, 0, (LPARAM)&ti);
    return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    switch (id)
    {
    case IDOK:
    case IDCANCEL:
        EndDialog(hwnd, id);
        break;
    }
}

LRESULT OnNotify(HWND hwnd, int idFrom, LPNMHDR pnmhdr)
{
    printf("code: %d\n", pnmhdr->code);
    switch (pnmhdr->code)
    {
        case TTN_NEEDTEXT:
        {
            TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pnmhdr;
            //pTTT->lpszText = TEXT("Success!");
            //pTTT->lpszText = NULL;
            pTTT->lpszText[0] = 0;
            break;
        }
    }
}

INT_PTR CALLBACK
DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        HANDLE_MSG(hwnd, WM_INITDIALOG, OnInitDialog);
        HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
        HANDLE_MSG(hwnd, WM_NOTIFY, OnNotify);
    }
    return 0;
}

int main(void)
{
    g_hInst = GetModuleHandle(NULL);
    InitCommonControls();

    DialogBox(g_hInst, MAKEINTRESOURCE(1), NULL, DialogProc);

    return 0;
}
