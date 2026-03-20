#define UNICODE
#define _UNICODE
#define OEMRESOURCE

#include <windows.h>

HINSTANCE g_hInst = nullptr;
HACCEL g_hAccel = nullptr;

int g_x = 100;
int g_y = 100;
bool g_dragging = false;

#define ID_FILE_EXIT      101
#define ID_OPTION1        102
#define ID_OPTION2        103
#define ID_HELP_ABOUT     104
#define ID_ACCEL1         105
#define ID_ACCEL2         106
#define ID_HOTKEY1        107

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ===================== SMALL HELPERS =====================

bool IsKeyDownAsync(int vk)
{
    return (GetAsyncKeyState(vk) & 0x8000) != 0;
}

bool IsKeyDownSync(int vk)
{
    return (GetKeyState(vk) & 0x8000) != 0;
}

bool IsKeyToggled(int vk)
{
    return (GetKeyState(vk) & 0x0001) != 0;
}

void EnableMouseTracking(HWND hwnd)
{
    TRACKMOUSEEVENT tme{};
    tme.cbSize = sizeof(tme);
    tme.dwFlags = TME_HOVER | TME_LEAVE;
    tme.hwndTrack = hwnd;
    tme.dwHoverTime = HOVER_DEFAULT;
    TrackMouseEvent(&tme);
}

void RegisterMyHotkey(HWND hwnd)
{
    RegisterHotKey(hwnd, ID_HOTKEY1, MOD_CONTROL | MOD_SHIFT, 'S');
}

void UnregisterMyHotkey(HWND hwnd)
{
    UnregisterHotKey(hwnd, ID_HOTKEY1);
}

void CreateMyCaret(HWND hwnd)
{
    CreateCaret(hwnd, nullptr, 2, 20);
    SetCaretPos(10, 10);
    ShowCaret(hwnd);
}

void DestroyMyCaret(HWND hwnd)
{
    HideCaret(hwnd);
    DestroyCaret();
}

HMENU CreateMainMenu()
{
    HMENU hMenuBar = CreateMenu();
    HMENU hFile = CreatePopupMenu();

    AppendMenuW(hFile, MF_STRING, ID_OPTION1, L"Option 1");
    AppendMenuW(hFile, MF_STRING, ID_OPTION2, L"Option 2");
    AppendMenuW(hFile, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(hFile, MF_STRING, ID_FILE_EXIT, L"Exit");

    AppendMenuW(hMenuBar, MF_POPUP, (UINT_PTR)hFile, L"File");
    return hMenuBar;
}

void ShowMyContextMenu(HWND hwnd, int x, int y)
{
    HMENU hMenu = CreatePopupMenu();
    AppendMenuW(hMenu, MF_STRING, ID_OPTION1, L"Option 1");
    AppendMenuW(hMenu, MF_STRING, ID_OPTION2, L"Option 2");

    TrackPopupMenuEx(hMenu, TPM_RIGHTBUTTON, x, y, hwnd, nullptr);
    DestroyMenu(hMenu);
}

HACCEL CreateMyAccelerators()
{
    ACCEL acc[2]{};

    acc[0].fVirt = FCONTROL | FVIRTKEY;
    acc[0].key = 'O';
    acc[0].cmd = ID_ACCEL1;

    acc[1].fVirt = FALT | FSHIFT | FVIRTKEY;
    acc[1].key = VK_F1;
    acc[1].cmd = ID_ACCEL2;

    return CreateAcceleratorTableW(acc, 2);
}

// ===================== EXTRA TOPICS FROM PDF =====================

// keyboard layout functions
void ExampleKeyboardLayoutFunctions()
{
    HKL currentLayout = GetKeyboardLayout(0);

    HKL layouts[16]{};
    int count = GetKeyboardLayoutList(16, layouts);

    wchar_t layoutName[KL_NAMELENGTH]{};
    GetKeyboardLayoutNameW(layoutName);

    // examples only
    // HKL loaded = LoadKeyboardLayoutW(L"00000409", KLF_ACTIVATE); // English US
    // ActivateKeyboardLayout(loaded, 0);
    // UnloadKeyboardLayout(loaded);

    UNREFERENCED_PARAMETER(currentLayout);
    UNREFERENCED_PARAMETER(count);
}

// key conversion helpers
void ExampleKeyConversion(LPARAM lParam)
{
    wchar_t keyName[64]{};
    GetKeyNameTextW((LONG)lParam, keyName, 64);

    UINT scan = MapVirtualKeyW(VK_LEFT, MAPVK_VK_TO_VSC);
    UINT vk = MapVirtualKeyW(scan, MAPVK_VSC_TO_VK);

    BYTE keyboardState[256]{};
    GetKeyboardState(keyboardState);

    WORD translated[2]{};
    ToAscii(VK_A, MapVirtualKeyW(VK_A, MAPVK_VK_TO_VSC), keyboardState, translated, 0);

    wchar_t uni[4]{};
    ToUnicode(VK_A, MapVirtualKeyW(VK_A, MAPVK_VK_TO_VSC), keyboardState, uni, 4, 0);

    UNREFERENCED_PARAMETER(vk);
}

// mouse capability checks
void ExampleMouseSystemMetrics()
{
    int mousePresent = GetSystemMetrics(SM_MOUSEPRESENT);
    int mouseButtons = GetSystemMetrics(SM_CMOUSEBUTTONS);
    int mouseWheel = GetSystemMetrics(SM_MOUSEWHEELPRESENT);
    int swapButtons = GetSystemMetrics(SM_SWAPBUTTON);

    UNREFERENCED_PARAMETER(mousePresent);
    UNREFERENCED_PARAMETER(mouseButtons);
    UNREFERENCED_PARAMETER(mouseWheel);
    UNREFERENCED_PARAMETER(swapButtons);
}

// generic resource APIs
void ExampleGenericResourceAPI()
{
    HRSRC hResInfo = FindResourceW(g_hInst, MAKEINTRESOURCEW(1), RT_RCDATA);
    if (!hResInfo) return;

    DWORD size = SizeofResource(g_hInst, hResInfo);
    HGLOBAL hResData = LoadResource(g_hInst, hResInfo);
    void* pData = LockResource(hResData);

    UNREFERENCED_PARAMETER(size);
    UNREFERENCED_PARAMETER(pData);
}

// string resource
void ExampleLoadString()
{
    wchar_t buf[128]{};
    LoadStringW(g_hInst, 1, buf, 128);
}

// image loading
void ExampleLoadImage()
{
    HBITMAP bmp = (HBITMAP)LoadImageW(nullptr, L"file.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    HCURSOR cur = LoadCursor(nullptr, IDC_HAND);
    HICON ico = LoadIcon(nullptr, IDI_INFORMATION);

    if (bmp) DeleteObject(bmp);
    UNREFERENCED_PARAMETER(cur);
    UNREFERENCED_PARAMETER(ico);
}

// simulated input
void ExampleSendInput()
{
    INPUT in[2]{};

    in[0].type = INPUT_KEYBOARD;
    in[0].ki.wVk = VK_SPACE;

    in[1].type = INPUT_KEYBOARD;
    in[1].ki.wVk = VK_SPACE;
    in[1].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(2, in, sizeof(INPUT));
}

// raw input registration example
void ExampleRegisterRawInput(HWND hwnd)
{
    RAWINPUTDEVICE rid{};
    rid.usUsagePage = 0x01; // generic desktop controls
    rid.usUsage = 0x02;     // mouse
    rid.dwFlags = 0;
    rid.hwndTarget = hwnd;

    RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

// touch registration example
void ExampleRegisterTouch(HWND hwnd)
{
    RegisterTouchWindow(hwnd, 0);
    // UnregisterTouchWindow(hwnd);
}

// gesture config example
void ExampleGestureConfig(HWND hwnd)
{
    GESTURECONFIG gc{};
    gc.dwID = 0;
    gc.dwWant = GC_ALLGESTURES;
    gc.dwBlock = 0;
    SetGestureConfig(hwnd, 0, 1, &gc, sizeof(gc));
}

// cursor info example
void ExampleCursorInfo()
{
    CURSORINFO ci{};
    ci.cbSize = sizeof(ci);
    GetCursorInfo(&ci);
}

// ===================== ENTRY =====================

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
    g_hInst = hInstance;

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyClass";
    wc.hCursor = nullptr; // use nullptr if handling WM_SETCURSOR yourself
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;

    RegisterClassExW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        wc.lpszClassName,
        L"WinAPI Full Template",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        900, 650,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    SetMenu(hwnd, CreateMainMenu());

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    g_hAccel = CreateMyAccelerators();

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0))
    {
        if (!TranslateAcceleratorW(hwnd, g_hAccel, &msg))
        {
            TranslateMessage(&msg); // needed for WM_CHAR
            DispatchMessageW(&msg);
        }
    }

    if (g_hAccel)
        DestroyAcceleratorTable(g_hAccel);

    return 0;
}

// ===================== WINDOW PROCEDURE =====================

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    // ---------- create / destroy ----------
    case WM_CREATE:
        RegisterMyHotkey(hwnd);
        ExampleKeyboardLayoutFunctions();
        ExampleMouseSystemMetrics();
        ExampleRegisterRawInput(hwnd);
        ExampleRegisterTouch(hwnd);
        ExampleGestureConfig(hwnd);
        return 0;

    case WM_DESTROY:
        UnregisterMyHotkey(hwnd);
        PostQuitMessage(0);
        return 0;

    // ---------- keyboard ----------
    case WM_KEYDOWN:
    {
        switch (wParam)
        {
        case VK_LEFT:  g_x -= 10; break;
        case VK_RIGHT: g_x += 10; break;
        case VK_UP:    g_y -= 10; break;
        case VK_DOWN:  g_y += 10; break;
        case VK_ESCAPE: DestroyWindow(hwnd); break;
        case VK_F2: ExampleSendInput(); break;
        }
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_KEYUP:
        return 0;

    case WM_SYSKEYDOWN:
        return DefWindowProcW(hwnd, msg, wParam, lParam);

    case WM_SYSKEYUP:
        return DefWindowProcW(hwnd, msg, wParam, lParam);

    case WM_CHAR:
    {
        wchar_t ch = (wchar_t)wParam;
        UNREFERENCED_PARAMETER(ch);
        return 0;
    }

    case WM_SYSCHAR:
        return DefWindowProcW(hwnd, msg, wParam, lParam);

    case WM_HOTKEY:
        if (wParam == ID_HOTKEY1)
        {
            MessageBoxW(hwnd, L"Ctrl+Shift+S", L"Hotkey", MB_OK);
        }
        return 0;

    case WM_APPCOMMAND:
    {
        int cmd = GET_APPCOMMAND_LPARAM(lParam);
        if (cmd == APPCOMMAND_MEDIA_PLAY_PAUSE)
        {
            MessageBoxW(hwnd, L"Play/Pause", L"App Command", MB_OK);
            return TRUE;
        }
        if (cmd == APPCOMMAND_VOLUME_UP)
        {
            MessageBoxW(hwnd, L"Volume Up", L"App Command", MB_OK);
            return TRUE;
        }
        return FALSE;
    }

    // ---------- caret ----------
    case WM_SETFOCUS:
        CreateMyCaret(hwnd);
        return 0;

    case WM_KILLFOCUS:
        DestroyMyCaret(hwnd);
        return 0;

    // ---------- mouse client area ----------
    case WM_LBUTTONDOWN:
        SetCapture(hwnd);
        g_dragging = true;
        return 0;

    case WM_LBUTTONUP:
        ReleaseCapture();
        g_dragging = false;
        return 0;

    case WM_RBUTTONDOWN:
        return 0;

    case WM_MBUTTONDOWN:
        return 0;

    case WM_XBUTTONDOWN:
    {
        WORD xb = GET_XBUTTON_WPARAM(wParam);
        if (xb == XBUTTON1)
            MessageBoxW(hwnd, L"XBUTTON1", L"Mouse", MB_OK);
        if (xb == XBUTTON2)
            MessageBoxW(hwnd, L"XBUTTON2", L"Mouse", MB_OK);
        return TRUE;
    }

    case WM_MOUSEMOVE:
        EnableMouseTracking(hwnd);
        if (g_dragging)
        {
            g_x = GET_X_LPARAM(lParam);
            g_y = GET_Y_LPARAM(lParam);
            InvalidateRect(hwnd, nullptr, TRUE);
        }
        return 0;

    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        g_y += (delta > 0 ? -10 : 10);
        InvalidateRect(hwnd, nullptr, TRUE);
        return 0;
    }

    case WM_MOUSEHWHEEL:
        return 0;

    case WM_LBUTTONDBLCLK:
        MessageBoxW(hwnd, L"Double Click", L"Mouse", MB_OK);
        return 0;

    case WM_MOUSEHOVER:
        return 0;

    case WM_MOUSELEAVE:
        return 0;

    // ---------- non-client mouse ----------
    case WM_NCMOUSEMOVE:
        return 0;

    case WM_NCLBUTTONDOWN:
        return DefWindowProcW(hwnd, msg, wParam, lParam);

    case WM_NCRBUTTONDOWN:
        return DefWindowProcW(hwnd, msg, wParam, lParam);

    // ---------- hit testing / activation / capture ----------
    case WM_NCHITTEST:
    {
        POINT pt{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ScreenToClient(hwnd, &pt);

        if (pt.y >= 0 && pt.y < 40)
            return HTCAPTION;

        return HTCLIENT;
    }

    case WM_MOUSEACTIVATE:
        return MA_ACTIVATE;

    case WM_CAPTURECHANGED:
        g_dragging = false;
        return 0;

    // ---------- cursor ----------
    case WM_SETCURSOR:
        SetCursor(LoadCursor(nullptr, IDC_HAND));
        return TRUE;

    // ---------- menus ----------
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_FILE_EXIT:
            DestroyWindow(hwnd);
            return 0;

        case ID_OPTION1:
            MessageBoxW(hwnd, L"Option 1 selected", L"Menu", MB_OK);
            return 0;

        case ID_OPTION2:
            MessageBoxW(hwnd, L"Option 2 selected", L"Menu", MB_OK);
            return 0;

        case ID_ACCEL1:
            MessageBoxW(hwnd, L"Ctrl+O accelerator", L"Accelerator", MB_OK);
            return 0;

        case ID_ACCEL2:
            MessageBoxW(hwnd, L"Alt+Shift+F1 accelerator", L"Accelerator", MB_OK);
            return 0;
        }
        return 0;

    case WM_CONTEXTMENU:
    {
        int x = GET_X_LPARAM(lParam);
        int y = GET_Y_LPARAM(lParam);

        if (x == -1 && y == -1)
        {
            POINT pt{ 30, 30 };
            ClientToScreen(hwnd, &pt);
            x = pt.x;
            y = pt.y;
        }

        ShowMyContextMenu(hwnd, x, y);
        return 0;
    }

    case WM_INITMENU:
        return 0;

    case WM_INITMENUPOPUP:
        return 0;

    case WM_UNINITMENUPOPUP:
        return 0;

    case WM_MENUSELECT:
        return 0;

    case WM_ENTERMENULOOP:
        return 0;

    case WM_EXITMENULOOP:
        return 0;

    case WM_MENURBUTTONUP:
        return 0;

    case WM_ENTERIDLE:
        return 0;

    // ---------- raw input ----------
    case WM_INPUT:
    {
        UINT size = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER));
        if (size > 0)
        {
            BYTE* buffer = new BYTE[size];
            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer, &size, sizeof(RAWINPUTHEADER)) == size)
            {
                RAWINPUT* raw = (RAWINPUT*)buffer;
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    // raw mouse data
                }
                else if (raw->header.dwType == RIM_TYPEKEYBOARD)
                {
                    // raw keyboard data
                }
            }
            delete[] buffer;
        }
        return 0;
    }

    // ---------- touch / gesture ----------
    case WM_TOUCH:
        return 0;

    case WM_GESTURE:
        return DefWindowProcW(hwnd, msg, wParam, lParam);

    case WM_GESTURENOTIFY:
        return 0;

    // ---------- paint ----------
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        Rectangle(hdc, g_x, g_y, g_x + 60, g_y + 60);
        TextOutW(hdc, 10, 10, L"WinAPI full template", 20);

        EndPaint(hwnd, &ps);
        return 0;
    }
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

/*
RESOURCE.H TEMPLATE

#define ID_FILE_EXIT      101
#define ID_OPTION1        102
#define ID_OPTION2        103
#define ID_HELP_ABOUT     104
#define ID_ACCEL1         105
#define ID_ACCEL2         106

RC TEMPLATE

#include "resource.h"

IDR_MENU1 MENU
BEGIN
    POPUP "&File"
    BEGIN
        MENUITEM "Option 1", ID_OPTION1
        MENUITEM "Option 2", ID_OPTION2
        MENUITEM SEPARATOR
        MENUITEM "Exit", ID_FILE_EXIT
    END
END

IDR_ACCELERATOR1 ACCELERATORS
BEGIN
    "O", ID_ACCEL1, VIRTKEY, CONTROL, NOINVERT
    VK_F1, ID_ACCEL2, VIRTKEY, ALT, SHIFT, NOINVERT
END

STRINGTABLE
BEGIN
    1 "Some string"
END
*/