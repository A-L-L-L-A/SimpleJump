
#include "SimpleJumper.h"
#include "Hind.h"
#include <Windows.h>
#include <math.h>
#include <stdlib.h>
#include "Color.h"
#include "Player.h"



#define TIMER_NEXT_FRAME  1
#define TIMER_ACCELERATION 2
#define TIME_ACCELERATION 10000
#define TIME_INTERVAL 40



HWND mainWindow;
RECT mainWindowSize,gameRegion;
HDC context;

Hind hind;
COLORREF backColorDay; 
COLORREF backColorNight; 
Color colorstruct;
Player player;
int score = 0;
int curSpeed = 0;
int acceleration = 2;
bool pause = true;
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM , LPARAM);
VOID    CALLBACK TimerProc(HWND, UINT, UINT, DWORD);
VOID CALLBACK TimerAcceleration(HWND, UINT, UINT, DWORD);
PAINTSTRUCT FillPaintStruct(RECT);
void GameLogic(HDC);
void DrawGameField(HDC);



int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex; MSG msg;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_DBLCLKS;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"SimpleJumperClass";
    wcex.hIconSm = wcex.hIcon;
    RegisterClassEx(&wcex);

    mainWindow = CreateWindow(L"SimpleJumperClass", L"SimpleJumper",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        640, 480, NULL, NULL, hInstance, NULL);
    //SetWindowLongPtr(mainWindow, GWL_STYLE, WS_VISIBLE | WS_POPUP);
    GetClientRect(mainWindow, &mainWindowSize);
    GetClientRect(mainWindow, &gameRegion);
    gameRegion.top += gameRegion.bottom/2 ;
    backColorDay = RGB(240, 240, 240); 
    backColorNight= RGB(100, 100, 100);

    ShowWindow(mainWindow, nCmdShow);
    UpdateWindow(mainWindow);

    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
               
        }

    }

    return (int)msg.wParam;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        SetTimer(hWnd, TIMER_NEXT_FRAME , TIME_INTERVAL, (TIMERPROC)TimerProc);
        

        
    }break;
    case WM_ACTIVATE:
    {
       // TODO: set function Pause()
    }
    break;
    case WM_COMMAND:
        {
          
        }
        break;
    case WM_KEYDOWN:
    {
        switch (wParam) {
        
        case 49:
            player.ChangeColor(Color::red);
            break;
        case 50:
            player.ChangeColor(Color::blue);
            break;
        case 51:
            player.ChangeColor(Color::green);
            break;
        case 52:
            player.ChangeColor(Color::black);
            break;
        case 53:
            player.ChangeColor(Color::white);
            break;
        case VK_SPACE:
            player.Jump();
            break;
        case VK_F1:
            if (!player.exists) {
                score = 0;
                curSpeed = TIME_INTERVAL;
                SetTimer(hWnd, TIMER_ACCELERATION, TIME_ACCELERATION, (TIMERPROC)TimerAcceleration);
                player.fillPlayer(gameRegion, colorstruct.getRandColor());
            }
            break;
        case VK_F2:
            if (pause) {
                KillTimer(hWnd, TIMER_ACCELERATION);
                KillTimer(hWnd, TIMER_NEXT_FRAME);
                pause = !pause;
            }
            else {
                SetTimer(hWnd, TIMER_ACCELERATION, TIME_ACCELERATION, (TIMERPROC)TimerAcceleration);
                SetTimer(hWnd, TIMER_NEXT_FRAME, curSpeed, (TIMERPROC)TimerProc);
                pause = !pause;
            }
            break;
        }
    }
    break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps; 
            ps = FillPaintStruct(mainWindowSize);
            HDC hdc = BeginPaint(hWnd, &ps);
           // HDC hdcComp = CreateCompatibleDC(hdc);
           // HBITMAP bitmap = CreateCompatibleBitmap(hdcComp, mainWindowSize.right-mainWindowSize.left, mainWindowSize.bottom-mainWindowSize.top);
           // SelectObject(hdcComp, bitmap);
            DrawGameField(hdc);
            GameLogic(hdc);
            //BitBlt(hdc, 0, 0, gameRegion.right, gameRegion.bottom, hdcComp, 0, 0, SRCCOPY);
            //DeleteObject(bitmap);
            //DeleteDC(hdcComp);
            DeleteDC(hdc);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_TIMER:
    {

    }
    break;
    case WM_SIZE:
    {
        GetClientRect(mainWindow, &mainWindowSize);
        GetClientRect(mainWindow, &gameRegion);
        gameRegion.top += gameRegion.bottom / 2;
        if (hind.exists)
        hind.Resize(gameRegion);
        if (player.exists)
            player.Resize(gameRegion);
        InvalidateRect(mainWindow, NULL, TRUE);
    }
    break;
    case WM_DESTROY:
        KillTimer(mainWindow,TIMER_NEXT_FRAME);
        KillTimer(mainWindow, TIMER_ACCELERATION);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


PAINTSTRUCT FillPaintStruct(RECT winSize) {
    PAINTSTRUCT ps;
    ps.fErase = true;
    ps.rcPaint = gameRegion;
    return ps;
}

VOID CALLBACK TimerProc(HWND hWnd, UINT uMessage, UINT uEventId, DWORD dwTime) {
    InvalidateRect(hWnd, NULL, TRUE);
   
}
VOID CALLBACK TimerAcceleration(HWND hWnd, UINT uMessage, UINT uEventId, DWORD dwTime) {
    KillTimer(hWnd, TIMER_NEXT_FRAME);
    curSpeed -= acceleration;
    SetTimer(hWnd, TIMER_NEXT_FRAME, curSpeed, (TIMERPROC)TimerProc);

}
void GameLogic(HDC hdc) {
    TCHAR score_str[50];

    // Plotting the current score
    wsprintf(score_str, TEXT("Score: %6d     "), score);
    TextOut(hdc, 20, 15, score_str, lstrlen(score_str));
    if (player.exists) {
        player.Decending();
        player.DrawPlayer(hdc);
        if (hind.exists) {
            hind.MoveHind();
            hind.DrawHind(hdc);
            if (hind.x < 0) {
                hind.emptyHind();
            }
            if (abs(player.x+player.width - hind.x) <= hind.step) {
                if (((player.y + player.height >= hind.y) && (hind.type == HindType::onGroundShort)) ||
                    ((player.y <= hind.y + hind.height) && (hind.type == HindType::FlyingShort))||
                    ((player.color!=hind.color)&&(HindType::onGroundLong==hind.type))) {
                    hind.exists = false;
                    player.Die();
                    KillTimer(mainWindow, TIMER_ACCELERATION);
                    KillTimer(mainWindow, TIMER_NEXT_FRAME);
                }
            }
            else {
                score++;
            }
        }
        else {
            HindType hindType = HindType::onGroundShort;
            COLORREF color = colorstruct.getRandColor();
            switch (rand() % 3) {
            case 0:
                hindType = HindType::onGroundShort;
                break;
            case 1:
                hindType = HindType::onGroundLong;
                break;
            case 2:
                hindType = HindType::FlyingShort;
                break;
            }

            hind.fillHind(hindType, gameRegion, color);
        }

    }
    else {
        TextOut(hdc,20, 50, L"Press F1 to start", 18);
        TextOut(hdc, 20, 70, L"Press F2 to pause", 18);
    }
}
void DrawGameField(HDC hdc) {
    HBRUSH brush = CreateSolidBrush(backColorDay);
    SelectObject(hdc, brush);
    Rectangle(hdc, gameRegion.left, gameRegion.top, gameRegion.right, gameRegion.bottom);
    DeleteObject(brush);
}
