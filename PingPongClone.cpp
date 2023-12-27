// PingPongClone.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Resource.h"

#define MAX_LOADSTRING 100


// ---------------------------//
// Variables and Intro items. //
// ---------------------------//

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Ball dimensions
const int BALL_SIZE = 20;
const int ballXOrigin = 380;
const int ballYOrigin = 280;

// Ball position.  Used for respawning.
int ballX = ballXOrigin;
int ballY = ballYOrigin;

// Ball speed.  This is initial speed.  Speed will increase on paddle contact.
int counter = 0;
int ballSpeedX = 3;
int ballSpeedY = 3;

// Paddle dimensions.  This seems thick.  Tweaking this may require collision mods.
const int PADDLE_WIDTH = 20;
const int PADDLE_HEIGHT = 80;

// Paddle position
int paddleL = 200;
int paddleR = 200;

// Scorecards.  This just shows the initial score for each side.
int scoreLeft = 0;
int scoreRight = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HWND, HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

// wWinMain - Entry point. 
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PINGPONGCLONE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // You may notice I pulled hWnd out of InitInstance, and am passing the variable in.
    // I was experimenting here and didn't put it back.  Feel free to modify.
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, 0, 800, 600, nullptr, nullptr, hInstance, nullptr);

    // Perform application initialization:
    if (!InitInstance(hWnd, hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PINGPONGCLONE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PINGPONGCLONE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PINGPONGCLONE);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HWND, HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HWND hWnd, HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // Store instance handle in our global variable

    if (!hWnd)
    {
        return FALSE;
    }
    // Set up a timer to update the ball position
    SetTimer(hWnd, 1, 16, NULL); // 16 milliseconds (approximately 60 frames per second)

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//  WM_TIMER    - 
//  WM_KEYDOWN  -  W and S for left side.  Up and Down keys for right side. 
//                 Note: Additional work could be performed to improve the keystroke/game performance.
//                 Using a key while other player is moving their paddle will disrupt their movement.
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    RECT rect;
    GetClientRect(hWnd, &rect);

    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        // These menu options came with the VS2022 template.  It's just 'about' and 'exit'.
        // However, feel free to take advantage of them for future ideas.
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        // Create a font with a specific size
        HFONT hFont = CreateFont(36, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Arial");

        HDC hdc = BeginPaint(hWnd, &ps);

        // Select the font into the device context
        SelectObject(hdc, hFont);

        // Calculate the midpoint of the window
        int midPoint = rect.right / 2;

        // Create brushes for blue and turquoise.  I picked the colors, and liked the contrast.
        HBRUSH hBrushBlue = CreateSolidBrush(RGB(0, 165, 255));
        HBRUSH hBrushTurquoise = CreateSolidBrush(RGB(0, 185, 255));

        // Draw the left half in blue
        rect.right = midPoint;
        FillRect(hdc, &rect, hBrushBlue);

        // Draw the right half in turquoise
        rect.left = midPoint;
        rect.right = rect.right * 2;
        FillRect(hdc, &rect, hBrushTurquoise);

        // Draw the paddles.  
        RECT paddleRect = { 10, paddleL, 10 + PADDLE_WIDTH, paddleL + PADDLE_HEIGHT };
        FillRect(hdc, &paddleRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        RECT rightPaddleRect = { rect.right - 10 - PADDLE_WIDTH, paddleR, rect.right - 10, paddleR + PADDLE_HEIGHT };
        FillRect(hdc, &rightPaddleRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Draw the ball
        RECT ballRect = { ballX, ballY, ballX + BALL_SIZE, ballY + BALL_SIZE };
        FillRect(hdc, &ballRect, (HBRUSH)GetStockObject(BLACK_BRUSH));

        // Note: I'm not a fan of using buffers, and there are multiple ways to go about printing this.
        RECT textRect_scoreLeft = { rect.right / 4, 25, 150, 150 };
        RECT textRect_scoreRight = { rect.right / 1.25, 25, 650, 650 };

        wchar_t bufferLeft[20];
        wchar_t bufferRight[20];
        swprintf(bufferLeft, sizeof(bufferLeft) / sizeof(bufferLeft[0]), L"%d", scoreLeft);
        swprintf(bufferRight, sizeof(bufferRight) / sizeof(bufferRight[0]), L"%d", scoreRight);

        // Set color and print text for both scores.  They should match the background, respectively.
        SetBkColor(hdc, RGB(0, 165, 255));
        TextOut(hdc, textRect_scoreLeft.left, textRect_scoreLeft.top, bufferLeft, static_cast<int>(wcslen(bufferLeft))); // print.

        SetBkColor(hdc, RGB(0, 185, 255));
        TextOut(hdc, textRect_scoreRight.left, textRect_scoreRight.top, bufferRight, static_cast<int>(wcslen(bufferRight))); // print.

        // Clean up
        DeleteObject(hBrushBlue);
        DeleteObject(hBrushTurquoise);

        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_TIMER:
        // Update ball position
        ballX += ballSpeedX;
        ballY += ballSpeedY;

        // Check for collisions with window boundaries.
        // If boundary is touched, then increment score on other side.
        if (ballX < 0 || ballX + BALL_SIZE > rect.right) {
            if (ballX < 0)
                scoreRight++;

            if (ballX + BALL_SIZE > rect.right)
                scoreLeft++;

            if (ballSpeedX > 0)
                ballSpeedX = 3;
            else
                ballSpeedX = -3;

            ballSpeedX = -ballSpeedX;
            ballX = ballXOrigin;
            ballY = ballYOrigin;
        }

        if (ballY < 0 || ballY + BALL_SIZE > rect.bottom) {
            ballSpeedY = -ballSpeedY; // Reverse Y direction
        }

        // Check for collisions with paddles
        if (ballX < 25 + PADDLE_WIDTH && ballY + BALL_SIZE > paddleL && ballY < paddleL + PADDLE_HEIGHT) {
            if (ballSpeedX < 0) {
                ballSpeedX = -ballSpeedX + 1;
                ballX = 20 + PADDLE_WIDTH; // Reverse X direction
            }
        }

        if (ballX + BALL_SIZE > rect.right - 20 - PADDLE_WIDTH && ballY + BALL_SIZE > paddleR && ballY < paddleR + PADDLE_HEIGHT) {
            if (ballSpeedX > 0) {
                ballSpeedX = -ballSpeedX - 1; // Reverse X direction
                ballX = rect.right - 20 - PADDLE_WIDTH - BALL_SIZE;
            }
        }
        // Note: Gave the paddles some wiggle room to account for touching the side of paddle first.
        // This was causing some initial wonkiness like sliding against/dribbling on/ignoring the paddle.


        // Invalidate the client area to trigger a repaint
        InvalidateRect(hWnd, NULL, TRUE);

        break;
    case WM_KEYDOWN:
        // Check for simultaneous key presses
        if (GetAsyncKeyState('W') & 0x8000)
        {
            // 'W' key is pressed
            paddleL -= 15;
        }

        if (GetAsyncKeyState('S') & 0x8000)
        {
            // 'S' key is pressed
            paddleL += 15;
        }

        if (GetAsyncKeyState(VK_UP) & 0x8000)
        {
            // 'Up' arrow key is pressed
            paddleR -= 15;
        }

        if (GetAsyncKeyState(VK_DOWN) & 0x8000)
        {
            // 'Down' arrow key is pressed
            paddleR += 15;
        }

        // Make sure the paddles stay within the window bounds
        if (paddleL < 0)
            paddleL = 0;
        else if (paddleL + PADDLE_HEIGHT > 540)
            paddleL = 540 - PADDLE_HEIGHT;

        if (paddleR < 0)
            paddleR = 0;
        else if (paddleR + PADDLE_HEIGHT > 540)
            paddleR = 540 - PADDLE_HEIGHT;

        InvalidateRect(hWnd, NULL, TRUE);

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
