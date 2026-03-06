#include <windows.h>

//эта функция получает события от виндоус
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

    switch(msg)
    {
        //сообщение когда нажали на крестик
        case WM_DESTROY:
        //заканчивай работу
            PostQuitMessage(0);
            return 0;
    }
    //это обрабатывает сообщения виндоус
    return DefWindowProc(hwnd,msg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) //последняя это как отобразить окно
{
    //структура где мы описываем окно
    WNDCLASS wc{};
    //все события отправляй в виндоупроц
    wc.lpfnWndProc = WindowProc;
    //указываем программу
    wc.hInstance = hInstance;
    //название типа окна
    wc.lpszClassName = L"MyClass";
    //регистрируем класс
    RegisterClass(&wc);

    //создание окна
    HWND hwnd = CreateWindowEx(
        0,
        L"MyClass", //тип окна
        L"Hello Window", //заголовок окна
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800, //размер
        600, //размер
        NULL,
        NULL,
        hInstance,
        NULL
    );
    //показваем окно
    ShowWindow(hwnd,nCmdShow);

    MSG msg;

    //ждет сообщение от виндоус
    while(GetMessage(&msg,NULL,0,0))
    {
        //переводит клавишы в символы
        TranslateMessage(&msg);
        //отпрявляет в виндоуспроц
        DispatchMessage(&msg);
    }

    return 0;
}