#include <windows.h>
#include <string>
#include <map>

std::map<HWND, char> lastCharMap;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

    case WM_CHAR:
        lastCharMap[hwnd] = (char)wParam;
        InvalidateRect(hwnd, NULL, TRUE); 
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        char c = lastCharMap[hwnd];
        std::string text;

        if (c != '\0') {
            text += c;
        } 

        TextOutA(hdc, 20, 20, text.c_str(), text.length());

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    const char CLASS_NAME[] = "MyWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd1 = CreateWindowEx(0, CLASS_NAME, "Chat User 1", WS_OVERLAPPEDWINDOW,
        100, 100, 350, 200, nullptr, nullptr, hInstance, nullptr);

    HWND hwnd2 = CreateWindowEx(0, CLASS_NAME, "Chat User 2", WS_OVERLAPPEDWINDOW,
        500, 100, 350, 200, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd1, nCmdShow);
    ShowWindow(hwnd2, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
