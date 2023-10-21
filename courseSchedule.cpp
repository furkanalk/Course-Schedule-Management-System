#include "framework.h"
#include "courseSchedule.h"
#include "resource.h"
#include "windows.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HBRUSH hbrBackground;                           // Background color brush


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_COURSESCHEDULE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_COURSESCHEDULE));

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

    return (int) msg.wParam;
}

//  PURPOSE: Registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSESCHEDULE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(200, 200, 255));
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance;

    int clientWidth = 800;
    int clientHeight = 600;

    RECT rect = { 0, 0, clientWidth, clientHeight };

    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, TRUE);

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(200, 200, 255));

    static courseSystem cs; // creating an Admin, but treating it as courseSystem
    static courseSystem* userAdmin = new Admin(); // creating an Admin, but treating it as courseSystem
    static courseSystem* userTeacher = new Teacher(); // creating an Admin, but treating it as courseSystem
    static courseSystem* userStudent = new Student(); // creating an Admin, but treating it as courseSystem
    Admin* actualAdmin = static_cast<Admin*>(userAdmin); // creating an Admin and can access its own functions

    switch (message)
    {   
        case WM_CTLCOLORSTATIC:
        {
            HDC hdcStatic = (HDC)wParam;
            SetBkColor(hdcStatic, RGB(200, 200, 255));
            return (INT_PTR)hbrBkgnd;
        }

        case WM_CREATE:
            cs.login(hWnd);
            break;

        case WM_COMMAND:
            {
                int wmId = LOWORD(wParam);

                switch (wmId)
                {
                /* Login Types */
                case 401:
                    // Admin Login Panel (proceed)
                    userAdmin->login(hWnd);
                    break;
                case 402:
                    // go to Teacher Login Panel (proceed)
                    userTeacher->login(hWnd);
                    break;
                case 403:
                    // go to Student Login Panel (proceed)
                    userStudent->login(hWnd);
                    break;

                /* Admin Login */
                case 101:
                    // back to Login Types from Admin Login Panel (back)
                    cs.login(hWnd);
                    break;
                case 102:
                    // go to Admin Interface from Admin Login Panel (login)
                    actualAdmin->showInterface(hWnd);
                    break;

                /* Admin Interface */
                case 111:
                    // go to Course Management from Admin Interface (proceed)
                    actualAdmin->courseManagement(hWnd);
                    break;
                case 112:
                    // go to Teacher Management from Admin Interface (proceed)
                    actualAdmin->teacherManagement(hWnd);
                    break;
                case 114:
                    // back to Login Types from Admin Interface (exit)
                    cs.login(hWnd);
                    break;

                /* Course Management */
                case 124:
                    // back to Admin Interface from Course Management (back)
                    actualAdmin->showInterface(hWnd);
                    break;
                case 125:
                    // back to Login Types from Course Management (exit)
                    cs.login(hWnd);
                    break;

                /* Teacher Management */
                case 132:
                    // go to Admin Add Teacher from Teacher Management (proceed)
                    actualAdmin->addTeacher(hWnd);
                    break;
                case 133:
                    // back to Admin Interface from Teacher Management (back)
                    actualAdmin->showInterface(hWnd);
                    break;
                case 134:
                    // back to Login Types from Teacher Management (exit)
                    cs.login(hWnd);
                    break;

                /* Add Teacher */
                case 141:
                    // back to Teacher Management from Add Teacher (back)
                    actualAdmin->teacherManagement(hWnd);
                    break;
                case 201:
                    // Back to Login Types Menu
                    cs.login(hWnd);
                    break;
                case 301:
                    // Back to Login Types Menu
                    cs.login(hWnd);
                    break;
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

        case WM_DESTROY:
            delete userAdmin;
            userAdmin = nullptr;
            delete userTeacher;
            userTeacher = nullptr;
            delete userStudent;
            userStudent = nullptr;

            PostQuitMessage(0);
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
