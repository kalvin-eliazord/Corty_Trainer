#include "HWindow.h"

BOOL CALLBACK HWindow::enumWndCallBack(HWND pEnumHwnd, LPARAM pLp)
{
    DWORD procId{};

    GetWindowThreadProcessId(pEnumHwnd, &procId);

    if (procId != GetCurrentProcessId())
        return false;

    if (pEnumHwnd == GetConsoleWindow())
        return false;

    if (IsWindowVisible(pEnumHwnd))
        return false;
    
    return true;
}

HWND HWindow::GetProcessWindow()
{
    EnumWindows(enumWndCallBack, NULL);
    return hWindow;
}

bool HWindow::SetRectWindow()
{
    hWindow = GetProcessWindow();
    if (!hWindow) return false;

    RECT size{};
    GetWindowRect(hWindow, &size);

    rWinWidth = size.right - size.left;
    rWinHeight = size.bottom - size.top;

    constexpr long winWidthBorder{ 5 };
    constexpr long winHeightBorder{ 29 };

    rWinWidth -= winWidthBorder;
    rWinHeight -= winHeightBorder;

    return true;
}