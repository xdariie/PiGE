#include <windows.h>
#include <vector>
#include <random>
#include <ctime>

const wchar_t CLASS_NAME[] = L"moving_square_class";
const int CLIENT_WIDTH = 800;
const int CLIENT_HEIGHT = 600;
const int SQUARE_SIZE = 100;
const int STEP = 3;
const int TIMER_ID = 1;
const int TIMER_INTERVAL = 20;

// Main window background
HBRUSH g_background_brush = nullptr;

// Square
HWND g_square = nullptr;

// Square position
int g_x = 0;
int g_y = 0;

// Direction: start moving left
int g_dx = -STEP;
int g_dy = 0;

// Color handling
std::vector<HBRUSH> g_square_brushes;
int g_current_color = 0;

// Helper: return current square brush
HBRUSH current_square_brush()
{
    return g_square_brushes[g_current_color];
}

// Create 16 random brushes
void create_square_colors()
{
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(0, 255);

    for (int i = 0; i < 16; ++i)
    {
        COLORREF color = RGB(dist(rng), dist(rng), dist(rng));
        g_square_brushes.push_back(CreateSolidBrush(color));
    }
}

// Free all brushes
void cleanup_brushes()
{
    if (g_background_brush)
    {
        DeleteObject(g_background_brush);
        g_background_brush = nullptr;
    }

    for (HBRUSH brush : g_square_brushes)
    {
        if (brush)
            DeleteObject(brush);
    }
    g_square_brushes.clear();
}

// Move square and bounce from edges
void move_square(HWND hwnd)
{
    RECT rc;
    GetClientRect(hwnd, &rc);

    g_x += g_dx;
    g_y += g_dy;

    if (g_x <= 0)
    {
        g_x = 0;
        g_dx = STEP;
    }
    else if (g_x + SQUARE_SIZE >= rc.right)
    {
        g_x = rc.right - SQUARE_SIZE;
        g_dx = -STEP;
    }

    if (g_y <= 0)
    {
        g_y = 0;
        g_dy = STEP;
    }
    else if (g_y + SQUARE_SIZE >= rc.bottom)
    {
        g_y = rc.bottom - SQUARE_SIZE;
        g_dy = -STEP;
    }

    SetWindowPos(
        g_square,
        nullptr,
        g_x,
        g_y,
        0,
        0,
        SWP_NOSIZE | SWP_NOZORDER
    );
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        RECT rc;
        GetClientRect(hwnd, &rc);

        g_x = (rc.right - SQUARE_SIZE) / 2;
        g_y = (rc.bottom - SQUARE_SIZE) / 2;

        g_square = CreateWindowExW(
            0,
            L"STATIC",
            nullptr,
            WS_CHILD | WS_VISIBLE,
            g_x,
            g_y,
            SQUARE_SIZE,
            SQUARE_SIZE,
            hwnd,
            nullptr,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            nullptr
        );

        SetTimer(hwnd, TIMER_ID, TIMER_INTERVAL, nullptr);
        return 0;
    }

    case WM_CTLCOLORSTATIC:
    {
        HDC hdc = (HDC)wParam;
        SetBkMode(hdc, OPAQUE);
        return (LRESULT)current_square_brush();
    }

    case WM_TIMER:
    {
        if (wParam == TIMER_ID)
        {
            move_square(hwnd);
        }
        return 0;
    }

    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:
        case 'A':
            g_dx = -STEP;
            g_dy = 0;
            break;

        case VK_RIGHT:
        case 'D':
            g_dx = STEP;
            g_dy = 0;
            break;

        case VK_UP:
        case 'W':
            g_dx = 0;
            g_dy = -STEP;
            break;

        case VK_DOWN:
        case 'S':
            g_dx = 0;
            g_dy = STEP;
            break;

        case VK_SPACE:
            g_current_color = (g_current_color + 1) % 16;

            InvalidateRect(g_square, nullptr, TRUE);
            UpdateWindow(g_square);
            break;
        }
        return 0;
    }

    case WM_DESTROY:
        KillTimer(hwnd, TIMER_ID);
        cleanup_brushes();
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    g_background_brush = CreateSolidBrush(RGB(35, 35, 60));
    create_square_colors();

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = g_background_brush;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassExW(&wc);

    DWORD style =
        WS_OVERLAPPED |
        WS_CAPTION |
        WS_SYSMENU |
        WS_MINIMIZEBOX |
        WS_CLIPCHILDREN;

    RECT rect{ 0, 0, CLIENT_WIDTH, CLIENT_HEIGHT };
    AdjustWindowRectEx(&rect, style, FALSE, 0);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        L"poor man's snake",
        style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hwnd)
        return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}