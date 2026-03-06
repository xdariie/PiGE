// ---------- ОСНОВА ПРОГРАММЫ ----------

// int WINAPI wWinMain(...)
// Точка входа GUI-программы Windows.
// Аналог main(), но для оконного приложения.
 
// RegisterClassExW(...)
// Регистрирует класс окна.
// Говорим Windows: "у моего окна будет такой обработчик сообщений,
// такой курсор, такой фон, такое имя класса".

// CreateWindowExW(...)
// Создает окно.
// Возвращает HWND — "идентификатор" окна.
// Можно создавать и главное окно, и второе окно, и дочерние окна.

// ShowWindow(...)
// Показывает окно на экране.

// UpdateWindow(...)
// Просит окно перерисоваться сразу, если нужно.

// GetMessageW(...)
// Берет следующее сообщение из очереди сообщений.
// Пока программа работает, этот цикл крутится постоянно.

// TranslateMessage(...)
// Дополнительная обработка клавиатурных сообщений.

// DispatchMessageW(...)
// Отправляет сообщение в WindowProc.

// DefWindowProcW(...)
// Стандартная обработка сообщений Windows.
// Если мы сами не обработали сообщение — отдаём его сюда.


// ---------- ТИПЫ / ЧТО ЭТО ТАКОЕ ----------

// HWND
// Handle window — идентификатор окна.

// HINSTANCE
// Идентификатор экземпляра программы.

// WPARAM / LPARAM
// Дополнительные данные сообщения.

// LRESULT
// Что возвращает обработчик сообщений WindowProc.


// ---------- ОБРАБОТЧИК СООБЩЕНИЙ ----------

// LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
// Главная функция, куда Windows присылает события.
// Например: закрытие окна, движение окна, нажатие клавиш, перерисовка.

// WM_CLOSE
// Сообщение: пользователь хочет закрыть окно.
// Обычно тут вызывают DestroyWindow(hwnd).

// DestroyWindow(...)
// Реально уничтожает окно.

// WM_DESTROY
// Сообщение: окно уже уничтожается.
// Для главного окна обычно тут вызывают PostQuitMessage(0).

// PostQuitMessage(...)
// Завершает message loop и закрывает программу.

// WM_NCCREATE
// Сообщение на очень раннем этапе создания окна.
// Удобно, чтобы привязать указатель на объект класса к HWND.

// WM_CREATE
// Сообщение при создании окна.
// Можно использовать для инициализации чего-то после создания.


// ---------- СВЯЗЬ HWND С КЛАССОМ ----------

// SetWindowLongPtrW(hwnd, GWLP_USERDATA, ...)
// Сохраняет произвольные данные внутри окна.
// Обычно туда кладут указатель на объект класса app_2048.

// GetWindowLongPtrW(hwnd, GWLP_USERDATA)
// Достает ранее сохраненный указатель на объект класса.
// Нужен, чтобы static WindowProc мог передать управление в app->window_proc().


// ---------- РАЗМЕРЫ И СТИЛИ ОКНА ----------

// AdjustWindowRectEx(...)
// Считает, какого размера должно быть всё окно,
// чтобы client area внутри была нужного размера.
// Очень важно, когда по заданию размер доски фиксированный.

// WS_OVERLAPPED
// Базовый стиль обычного верхнего окна.

// WS_CAPTION
// Добавляет заголовок окна.

// WS_SYSMENU
// Добавляет системное меню и крестик закрытия.

// WS_MINIMIZEBOX
// Добавляет кнопку сворачивания.

// WS_BORDER
// Добавляет границу окна.

// WS_SIZEBOX
// Позволяет изменять размер окна.
// В этой лабе обычно НЕ нужен, если окно должно быть фиксированного размера.

// WS_CHILD
// Стиль дочернего окна/контрола.

// WS_VISIBLE
// Окно сразу видно.

// SS_CENTER
// Стиль STATIC-контрола: текст по центру.


// ---------- ЦВЕТА И ФОН ----------

// CreateSolidBrush(RGB(...))
// Создает кисть одного цвета.
// Используется для фона окна или плиток.

// RGB(r,g,b)
// Собирает цвет из красного, зеленого и синего.

// hbrBackground в WNDCLASSEXW
// Кисть для фона всего окна.

// WM_CTLCOLORSTATIC
// Сообщение от STATIC-контрола к родителю.
// Позволяет задать цвет/фон STATIC-контрола.
// В лабе используется для окраски плиток, если плитки сделаны через STATIC.


// ---------- CHILD WINDOWS / STATIC ----------

// CreateWindowExW(..., L"STATIC", ...)
// Создает STATIC-контрол.
// В этой лабе им можно изображать плитки поля без GDI-рисования.


// ---------- РАЗМЕР ЭКРАНА И ПОЗИЦИЯ ОКОН ----------

// GetSystemMetrics(SM_CXSCREEN)
// Ширина экрана.

// GetSystemMetrics(SM_CYSCREEN)
// Высота экрана.

// GetSystemMetrics(...)
// Вообще позволяет узнать системные размеры:
// экран, рамки, заголовки и т.д.

// WM_WINDOWPOSCHANGED
// Сообщение: окно изменило позицию или размер.
// В лабе это ключевое сообщение для симметричного движения второго окна.

// WINDOWPOS
// Структура с новой позицией и размером окна.
// Приходит через lParam в WM_WINDOWPOSCHANGED.

// SetWindowPos(...)
// Двигает окно, меняет размер, z-order и т.д.
// В лабе используется для перемещения второго окна симметрично.

// GetWindowRect(...)
// Получает прямоугольник окна в координатах экрана.
// Но включает и невидимую тень, поэтому для прозрачности лучше не всегда подходит.


// ---------- DWM / ТОЧНЫЕ ГРАНИЦЫ ОКНА ----------

// DwmGetWindowAttribute(...)
// Получает дополнительные свойства окна от Desktop Window Manager.
// В лабе полезно для точных границ окна без лишней тени.

// DWMWA_EXTENDED_FRAME_BOUNDS
// Атрибут для DwmGetWindowAttribute.
// Возвращает "реальные" видимые границы окна.

// IntersectRect(...)
// Находит пересечение двух прямоугольников.
// Если пересечение не пустое — окна перекрываются.

// IsRectEmpty(...)
// Проверяет, пустой ли прямоугольник.
// Удобно после IntersectRect.


// ---------- ПРОЗРАЧНОСТЬ ----------

// WS_EX_LAYERED
// Extended style для окна, которое может быть прозрачным.

// SetLayeredWindowAttributes(...)
// Задает прозрачность layered window.
// Например alpha = 255 -> полностью видно
// alpha = 127 -> полупрозрачное.


// ---------- ИКОНКА ----------

// LoadImageW(..., IMAGE_ICON, ...)
// Загружает иконку из ресурсов или файла.

// LoadIconW(...)
// Более простой способ загрузки иконки.

// MAKEINTRESOURCEW(...)
// Позволяет использовать ID ресурса как "имя" ресурса.

// hIcon в WNDCLASSEXW
// Иконка окна.


// ---------- ТАЙМЕРЫ ----------

// SetTimer(...)
// Запускает таймер.
// Через заданный интервал Windows будет отправлять WM_TIMER.

// WM_TIMER
// Сообщение от таймера.
// Можно использовать для часов, анимаций и т.д.

// KillTimer(...)
// Останавливает таймер.


// ---------- МЕНЮ ----------

// WM_COMMAND
// Сообщение от меню, кнопок и некоторых контролов.
// По LOWORD(wParam) обычно узнают ID команды.

// LOWORD(wParam)
// Дает младшие 16 бит.
// Обычно это ID пункта меню или команды.

// SetWindowTextW(...)
// Меняет заголовок окна.

// LoadAcceleratorsW(...)
// Загружает таблицу горячих клавиш из ресурсов.

// TranslateAcceleratorW(...)
// Обрабатывает горячие клавиши до DispatchMessage.


// ---------- КЛАВИАТУРА ----------

// WM_KEYDOWN
// Сообщение: пользователь нажал клавишу.
// В домашке пригодится для W/A/S/D.

// VK_...
// Константы виртуальных клавиш Windows.


// ---------- РИСОВАНИЕ (если можно GDI) ----------

// WM_PAINT
// Сообщение: окно нужно перерисовать.
// Всё рисование обычно делают тут.

// BeginPaint(...)
// Начинает рисование и дает HDC.

// EndPaint(...)
// Завершает рисование.

// HDC
// Device Context — "контекст рисования".

// FillRect(...)
// Закрашивает прямоугольник кистью.

// RoundRect(...)
// Рисует прямоугольник с закругленными углами.

// DrawText(...)
// Рисует текст.

// SelectObject(...)
// Подключает кисть/перо/шрифт к HDC.

// DeleteObject(...)
// Удаляет созданный GDI-объект.
// Очень важно не забывать, чтобы не было утечек.


// ---------- ПОЛЕЗНЫЕ ИДЕИ ДЛЯ ЛАБЫ ----------

// Главное окно:
// обычно m_main

// Второе окно:
// обычно m_popup

// В static WindowProc:
// 1) на WM_NCCREATE достаем this из CREATESTRUCT
// 2) сохраняем this через SetWindowLongPtrW
// 3) потом все сообщения отправляем в app->window_proc()

// Симметрия окон:
// двигаем одно окно -> через WM_WINDOWPOSCHANGED
// считаем центр экрана -> ставим второе окно симметрично

// Прозрачность:
// после каждого движения окон проверяем пересечение прямоугольников
// если пересекаются -> popup делаем полупрозрачным
// если нет -> полностью непрозрачным

// Если задание запрещает GDI:
// плитки делаем через STATIC child controls

// Если задание разрешает GDI:
// удобнее рисовать всё в WM_PAINT


// ---------- ЧТО НУЖНО ПОМНИТЬ НА ЛАБЕ ----------

// 1. RegisterClassExW -> зарегистрировать класс окна
// 2. CreateWindowExW -> создать окно
// 3. ShowWindow -> показать
// 4. GetMessage / DispatchMessage -> цикл сообщений
// 5. WindowProc -> обработка событий
// 6. WM_CLOSE / WM_DESTROY -> корректное закрытие
// 7. AdjustWindowRectEx -> правильный размер окна
// 8. WM_WINDOWPOSCHANGED + SetWindowPos -> движение второго окна
// 9. CreateSolidBrush + WM_CTLCOLORSTATIC -> цвета
// 10. DwmGetWindowAttribute + SetLayeredWindowAttributes -> прозрачность






// ---------- ОСНОВА ПРОГРАММЫ ----------


// Точка входа Windows GUI программы
int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    PWSTR lpCmdLine,
    int nCmdShow
);

// hInstance  -> идентификатор программы
// hPrevInstance -> не используется (старый параметр)
// lpCmdLine -> аргументы командной строки
// nCmdShow -> как показать окно (SW_SHOW и т.д.)



// ---------- РЕГИСТРАЦИЯ КЛАССА ОКНА ----------

ATOM RegisterClassExW(
    const WNDCLASSEXW *lpwcx
);

// lpwcx -> структура с описанием класса окна
// возвращает идентификатор класса



// ---------- СОЗДАНИЕ ОКНА ----------

HWND CreateWindowExW(
    DWORD dwExStyle,
    LPCWSTR lpClassName,
    LPCWSTR lpWindowName,
    DWORD dwStyle,
    int X,
    int Y,
    int nWidth,
    int nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    LPVOID lpParam
);

// dwExStyle -> расширенный стиль окна
// lpClassName -> имя класса окна
// lpWindowName -> заголовок окна
// dwStyle -> стиль окна
// X,Y -> позиция окна
// nWidth,nHeight -> размер окна
// hWndParent -> родительское окно
// hMenu -> меню
// hInstance -> экземпляр программы
// lpParam -> пользовательские данные (например this)



// ---------- ПОКАЗ ОКНА ----------

BOOL ShowWindow(
    HWND hWnd,
    int nCmdShow
);

// hWnd -> окно
// nCmdShow -> как показать (SW_SHOW, SW_HIDE, SW_SHOWNA)



BOOL UpdateWindow(
    HWND hWnd
);

// заставляет окно сразу перерисоваться



// ---------- MESSAGE LOOP ----------

BOOL GetMessageW(
    LPMSG lpMsg,
    HWND hWnd,
    UINT wMsgFilterMin,
    UINT wMsgFilterMax
);

// lpMsg -> структура сообщения
// hWnd -> окно
// фильтры сообщений


BOOL TranslateMessage(
    const MSG *lpMsg
);

// преобразует клавиатурные сообщения


LRESULT DispatchMessageW(
    const MSG *lpMsg
);

// отправляет сообщение в WindowProc



// ---------- ОБРАБОТЧИК СООБЩЕНИЙ ----------

LRESULT CALLBACK WindowProc(
    HWND hwnd,
    UINT msg,
    WPARAM wParam,
    LPARAM lParam
);

// hwnd -> окно
// msg -> тип сообщения
// wParam/lParam -> дополнительные данные



LRESULT DefWindowProcW(
    HWND hWnd,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam
);

// стандартная обработка сообщений Windows



// ---------- ЗАКРЫТИЕ ПРОГРАММЫ ----------

BOOL DestroyWindow(
    HWND hWnd
);

// уничтожает окно


VOID PostQuitMessage(
    int nExitCode
);

// завершает message loop



// ---------- СОХРАНЕНИЕ УКАЗАТЕЛЯ В ОКНЕ ----------

LONG_PTR SetWindowLongPtrW(
    HWND hWnd,
    int nIndex,
    LONG_PTR dwNewLong
);

// сохраняет пользовательские данные внутри HWND


LONG_PTR GetWindowLongPtrW(
    HWND hWnd,
    int nIndex
);

// получает сохраненные данные



// ---------- РАЗМЕРЫ ОКНА ----------

BOOL AdjustWindowRectEx(
    LPRECT lpRect,
    DWORD dwStyle,
    BOOL bMenu,
    DWORD dwExStyle
);

// lpRect -> желаемый размер client area
// dwStyle -> стиль окна
// bMenu -> есть ли меню
// dwExStyle -> расширенный стиль



// ---------- ЦВЕТА ----------

HBRUSH CreateSolidBrush(
    COLORREF color
);

// color -> RGB цвет



COLORREF RGB(
    BYTE r,
    BYTE g,
    BYTE b
);

// создает цвет из r,g,b



// ---------- СИСТЕМНАЯ ИНФОРМАЦИЯ ----------

int GetSystemMetrics(
    int nIndex
);

// nIndex -> тип информации
// например SM_CXSCREEN (ширина экрана)



// ---------- ДВИЖЕНИЕ ОКНА ----------

BOOL SetWindowPos(
    HWND hWnd,
    HWND hWndInsertAfter,
    int X,
    int Y,
    int cx,
    int cy,
    UINT uFlags
);

// hWnd -> окно
// hWndInsertAfter -> порядок окон
// X,Y -> позиция
// cx,cy -> размер
// uFlags -> флаги (SWP_NOSIZE и т.д.)



// ---------- ПОЛУЧЕНИЕ РАЗМЕРА ОКНА ----------

BOOL GetWindowRect(
    HWND hWnd,
    LPRECT lpRect
);

// возвращает прямоугольник окна



// ---------- ПРОЗРАЧНОСТЬ ----------

BOOL SetLayeredWindowAttributes(
    HWND hwnd,
    COLORREF crKey,
    BYTE bAlpha,
    DWORD dwFlags
);

// hwnd -> окно
// crKey -> цвет прозрачности
// bAlpha -> уровень прозрачности
// dwFlags -> LWA_ALPHA



// ---------- DWM API ----------

HRESULT DwmGetWindowAttribute(
    HWND hwnd,
    DWORD dwAttribute,
    PVOID pvAttribute,
    DWORD cbAttribute
);

// hwnd -> окно
// dwAttribute -> тип атрибута
// pvAttribute -> куда записать результат
// cbAttribute -> размер структуры



// ---------- ПЕРЕСЕЧЕНИЕ ПРЯМОУГОЛЬНИКОВ ----------

BOOL IntersectRect(
    LPRECT lprcDst,
    const RECT *lprcSrc1,
    const RECT *lprcSrc2
);

// вычисляет пересечение двух прямоугольников



BOOL IsRectEmpty(
    const RECT *lprc
);

// проверяет пустой ли прямоугольник



// ---------- ТАЙМЕР ----------

UINT_PTR SetTimer(
    HWND hWnd,
    UINT_PTR nIDEvent,
    UINT uElapse,
    TIMERPROC lpTimerFunc
);

// запускает таймер


BOOL KillTimer(
    HWND hWnd,
    UINT_PTR uIDEvent
);

// останавливает таймер



// ---------- РИСОВАНИЕ ----------

HDC BeginPaint(
    HWND hWnd,
    LPPAINTSTRUCT lpPaint
);

// начинает рисование


BOOL EndPaint(
    HWND hWnd,
    const PAINTSTRUCT *lpPaint
);

// завершает рисование



BOOL FillRect(
    HDC hDC,
    const RECT *lprc,
    HBRUSH hbr
);

// закрашивает прямоугольник



BOOL RoundRect(
    HDC hdc,
    int left,
    int top,
    int right,
    int bottom,
    int width,
    int height
);

// рисует прямоугольник с закругленными углами



int DrawTextW(
    HDC hdc,
    LPCWSTR lpchText,
    int cchText,
    LPRECT lprc,
    UINT format
);

// рисует текст



// ---------- GDI ОБЪЕКТЫ ----------

HGDIOBJ SelectObject(
    HDC hdc,
    HGDIOBJ h
);

// выбирает объект для рисования


BOOL DeleteObject(
    HGDIOBJ ho
);

// удаляет GDI объект