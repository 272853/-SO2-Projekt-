#include <windows.h>
#include <string>
#include <map>
#include <vector>

std::map<HWND, std::string> inputBuffers;
std::vector<std::string> sharedMessages;

// Trzymamy uchwyty do obu okien, by móc je odświeżać
HWND hwnd1 = nullptr;
HWND hwnd2 = nullptr;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

    case WM_CHAR:
        if (wParam == VK_RETURN) {
            std::string& buffer = inputBuffers[hwnd];
            if (!buffer.empty()) {
                sharedMessages.push_back(buffer);
                buffer.clear();

                // Odśwież oba okna
                InvalidateRect(hwnd1, NULL, TRUE);
                InvalidateRect(hwnd2, NULL, TRUE);
            }
        } else if (wParam == VK_BACK) {
            std::string& buffer = inputBuffers[hwnd];
            if (!buffer.empty()) {
                buffer.pop_back();
                InvalidateRect(hwnd, NULL, TRUE);
            }
        } else if (wParam >= 32 && wParam <= 126) { // tylko znaki ASCII
            inputBuffers[hwnd] += (char)wParam;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        std::string currentInput = inputBuffers[hwnd];

        TextOutA(hdc, 20, 20, "Type message and press Enter:", 30);
        TextOutA(hdc, 20, 40, currentInput.c_str(), currentInput.length());

        int y = 70;
        TextOutA(hdc, 20, y, "Chat messages:", 14);
        y += 20;
        for (const std::string& msg : sharedMessages) {
            TextOutA(hdc, 20, y, msg.c_str(), msg.length());
            y += 20;
        }

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

    hwnd1 = CreateWindowEx(0, CLASS_NAME, "Chat Window 1", WS_OVERLAPPEDWINDOW,
        100, 100, 400, 300, nullptr, nullptr, hInstance, nullptr);

    hwnd2 = CreateWindowEx(0, CLASS_NAME, "Chat Window 2", WS_OVERLAPPEDWINDOW,
        550, 100, 400, 300, nullptr, nullptr, hInstance, nullptr);

    ShowWindow(hwnd1, nCmdShow);
    ShowWindow(hwnd2, nCmdShow);

    UpdateWindow(hwnd1);
    UpdateWindow(hwnd2);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
