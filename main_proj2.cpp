#include <windows.h>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>

std::map<HWND, std::string> inputBuffers;
std::vector<std::string> sharedMessages;

std::map<HWND, std::queue<std::string>> pendingMessages;

std::mutex sharedMutex;
std::atomic<HWND> activeWindow = nullptr;

std::map<HWND, int> scrollPos;

HWND hwnd1 = nullptr;
HWND hwnd2 = nullptr;

void FlushPendingMessages(HWND hwnd) {
    if (hwnd == activeWindow) {
        std::lock_guard<std::mutex> lock(sharedMutex);
        auto& queue = pendingMessages[hwnd];
        while (!queue.empty()) {
            sharedMessages.push_back(queue.front());
            queue.pop();
        }
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {

    case WM_CHAR: {
        std::string& buffer = inputBuffers[hwnd];

        if (wParam == VK_RETURN) {
            if (!buffer.empty()) {
                if (hwnd == activeWindow) {
                    {
                        std::lock_guard<std::mutex> lock(sharedMutex);
                        sharedMessages.push_back(buffer);
                    }
                } else {
                    pendingMessages[hwnd].push(buffer);
                }
                buffer.clear();

                InvalidateRect(hwnd1, NULL, TRUE);
                InvalidateRect(hwnd2, NULL, TRUE);
            }
        } else if (wParam == VK_BACK) {
            if (!buffer.empty()) {
                buffer.pop_back();
                InvalidateRect(hwnd, NULL, TRUE);
            }
        } else if (wParam >= 32 && wParam <= 126) {
            buffer += (char)wParam;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        return 0;
    }

    case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    std::string currentInput = inputBuffers[hwnd];
    TextOutA(hdc, 20, 20, "Type message and press Enter:", 30);
    TextOutA(hdc, 20, 40, currentInput.c_str(), currentInput.length());

    FlushPendingMessages(hwnd);

    int lineHeight = 20;
    int yStart = 70;

    std::vector<std::string> localMessages;
    {
        std::lock_guard<std::mutex> lock(sharedMutex);
        localMessages = sharedMessages;
    }

    int totalLines = static_cast<int>(localMessages.size());
    int visibleLines = (ps.rcPaint.bottom - yStart) / lineHeight;
    int startIndex = scrollPos[hwnd];

    if (startIndex > totalLines - visibleLines)
        startIndex = std::max(0, totalLines - visibleLines);

    int y = yStart;
    for (int i = startIndex; i < totalLines && y < ps.rcPaint.bottom; ++i) {
        TextOutA(hdc, 20, y, localMessages[i].c_str(), localMessages[i].length());
        y += lineHeight;
    }

    EndPaint(hwnd, &ps);
    return 0;
}
    case WM_SIZE: {
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        int lineHeight = 20;
        int visibleLines = (clientRect.bottom - 70) / lineHeight;

        int totalLines = 0;
        {
            std::lock_guard<std::mutex> lock(sharedMutex);
            totalLines = static_cast<int>(sharedMessages.size());
        }

        SCROLLINFO si = {};
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_PAGE | SIF_RANGE | SIF_POS;
        si.nMin = 0;
        si.nMax = totalLines - 1;
        si.nPage = visibleLines;
        si.nPos = scrollPos[hwnd];
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        return 0;
    }
    
    case WM_VSCROLL: {
        SCROLLINFO si = {};
        si.cbSize = sizeof(SCROLLINFO);
        si.fMask = SIF_ALL;
        GetScrollInfo(hwnd, SB_VERT, &si);

        int pos = si.nPos;
        switch (LOWORD(wParam)) {
            case SB_LINEUP: pos--; break;
            case SB_LINEDOWN: pos++; break;
            case SB_PAGEUP: pos -= si.nPage; break;
            case SB_PAGEDOWN: pos += si.nPage; break;
            case SB_THUMBPOSITION:
            case SB_THUMBTRACK: pos = HIWORD(wParam); break;
        }

        int maxScroll = std::max(0, si.nMax - static_cast<int>(si.nPage) + 1);
        pos = std::clamp(pos, 0, maxScroll);

        si.nPos = pos;
        scrollPos[hwnd] = pos;
        SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
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

    std::thread toggleThread([]() {
        while (true) {
            if (hwnd1 && hwnd2) {
                activeWindow = hwnd1;
                InvalidateRect(hwnd1, NULL, TRUE);
                InvalidateRect(hwnd2, NULL, TRUE);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));

                activeWindow = hwnd2;
                InvalidateRect(hwnd1, NULL, TRUE);
                InvalidateRect(hwnd2, NULL, TRUE);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        }
    });
    toggleThread.detach();

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
