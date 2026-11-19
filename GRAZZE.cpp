#define UNICODE
#define _UNICODE
#include <windows.h>
#include <string>
#include <sstream>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

static const wchar_t CLASS_NAME[] = L"GRAZZEWindow";
static bool showInfo = true;
static HFONT titleFont, normalFont;

std::wstring GetRamText() {
    MEMORYSTATUSEX mem{};
    mem.dwLength = sizeof(mem);
    if (GlobalMemoryStatusEx(&mem)) {
        double used = (double)(mem.ullTotalPhys - mem.ullAvailPhys) / (1024.0 * 1024.0 * 1024.0);
        double total = (double)mem.ullTotalPhys / (1024.0 * 1024.0 * 1024.0);
        std::wostringstream s;
        s.setf(std::ios::fixed); s.precision(1);
        s << L"RAM: " << used << L" GB / " << total << L" GB";
        return s.str();
    }
    return L"RAM: unavailable";
}

void DrawTextLine(HDC dc, int x, int y, const std::wstring& text, COLORREF color, HFONT font) {
    SetTextColor(dc, color);
    SetBkMode(dc, TRANSPARENT);
    auto old = SelectObject(dc, font);
    TextOutW(dc, x, y, text.c_str(), (int)text.size());
    SelectObject(dc, old);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_CREATE:
        titleFont = CreateFontW(30,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH,L"Segoe UI");
        normalFont = CreateFontW(17,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH,L"Segoe UI");
        SetTimer(hwnd, 1, 1000, nullptr);
        RegisterHotKey(hwnd, 1, MOD_CONTROL | MOD_ALT, VK_F1);
        return 0;
    case WM_HOTKEY:
        showInfo = !showInfo;
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    case WM_TIMER:
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC dc = BeginPaint(hwnd, &ps);
        RECT r; GetClientRect(hwnd, &r);
        HBRUSH bg = CreateSolidBrush(RGB(15, 17, 23));
        FillRect(dc, &r, bg); DeleteObject(bg);

        HBRUSH accent = CreateSolidBrush(RGB(230, 45, 70));
        RECT bar{0,0,r.right,7}; FillRect(dc, &bar, accent); DeleteObject(accent);

        DrawTextLine(dc, 28, 28, L"GRAZZE", RGB(245,245,250), titleFont);
        DrawTextLine(dc, 30, 76, L"WINDOWS GAMING COMPANION", RGB(230,65,88), normalFont);

        if (showInfo) {
            DrawTextLine(dc, 30, 132, L"SYSTEM STATUS", RGB(245,245,250), normalFont);
            DrawTextLine(dc, 30, 168, GetRamText(), RGB(190,198,212), normalFont);

            MEMORYSTATUSEX mem{}; mem.dwLength = sizeof(mem);
            if (GlobalMemoryStatusEx(&mem)) {
                std::wstring pct = L"Memory load: " + std::to_wstring(mem.dwMemoryLoad) + L"%";
                DrawTextLine(dc, 30, 202, pct, RGB(190,198,212), normalFont);
            }
            DrawTextLine(dc, 30, 260, L"SHORTCUTS", RGB(245,245,250), normalFont);
            DrawTextLine(dc, 30, 294, L"Ctrl + Alt + F1   Show / hide system panel", RGB(190,198,212), normalFont);
            DrawTextLine(dc, 30, 328, L"Esc               Close GRAZZE", RGB(190,198,212), normalFont);
        } else {
            DrawTextLine(dc, 30, 150, L"Panel hidden. Press Ctrl + Alt + F1 to show it.", RGB(190,198,212), normalFont);
        }

        DrawTextLine(dc, 30, r.bottom - 42, L"Safe local monitoring only - no game modification.", RGB(115,125,145), normalFont);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_KEYDOWN:
        if (wp == VK_ESCAPE) DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        KillTimer(hwnd, 1);
        UnregisterHotKey(hwnd, 1);
        if (titleFont) DeleteObject(titleFont);
        if (normalFont) DeleteObject(normalFont);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    WNDCLASSW wc{};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = instance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(0, CLASS_NAME, L"GRAZZE - Gaming Companion",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 540, 440, nullptr, nullptr, instance, nullptr);
    if (!hwnd) return 0;
    ShowWindow(hwnd, show);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return (int)msg.wParam;
}
