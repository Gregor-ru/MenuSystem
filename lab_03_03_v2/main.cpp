#include "menu.h" 

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Глобальные переменные
HINSTANCE hInst;
MainMenu mainMenu;
RECT rc;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Инициализация глобальных переменных
    hInst = hInstance;

    // Создание окна
    HWND hWnd;
    WNDCLASSEX wcex;
    

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = TEXT("MainMenuClass");
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, TEXT("Call to RegisterClassEx failed!"), TEXT("Win32 Guided Tour"), NULL);
        return 1;
    }

    hWnd = CreateWindow(TEXT("MainMenuClass"), TEXT("Win32 Guided Tour Application"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        MessageBox(NULL, TEXT("Call to CreateWindow failed!"), TEXT("Win32 Guided Tour"), NULL);
        return 1;
    }

    HDC hdc = GetDC(hWnd);
    MSG msg;

    // Чтение меню из файла
    mainMenu.ReadMenuFromFile("test.txt");

    // Отображение окна
    ShowWindow(hWnd, nCmdShow);

    while (1)
    {
        if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
        {

            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            mainMenu.Draw(hdc, rc);
        }
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;

    switch (message) {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_LBUTTONDOWN: {
        POINTS pt = MAKEPOINTS(lParam);
        POINT point = { pt.x, pt.y };
        mainMenu.OnMouseClick(point);
        break;
    }
    case WM_SIZE:
    {
        GetClientRect(hWnd, &rc);
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}
