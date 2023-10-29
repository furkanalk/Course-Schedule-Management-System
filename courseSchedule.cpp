#include "framework.h"
#include "course.h"
#include "resource.h"
#include "windows.h"
#include <memory>

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

// Window Handlers

/* Login Types */
void HandleLoginTypes(int wmID, HWND hWnd, Admin& userAdmin, User& userTeacher, User& userStudent) {
    switch (wmID) { 
        case 401:
            userAdmin.login(hWnd);
            break;
        case 402:
            userTeacher.showInterface(hWnd);
            break;
        case 403:
            userStudent.showInterface(hWnd);
            break;
    }
}

/* Admin Windows */
void HandleAdminLogin(int wmId, HWND hWnd, Admin& actualAdmin,
    User& userAdmin, User& userTeacher, User& userStudent, User& user) {
    
    static std::unique_ptr<User> gradeMngt = std::make_unique<GradeManagement>();
    static std::unique_ptr<User> roomMngt = std::make_unique<RoomManagement>();
    static std::unique_ptr<User> courseMngt = std::make_unique<CourseManagement>();
    static std::unique_ptr<User> teacherMngt = std::make_unique<TeacherManagement>();

    static std::unique_ptr<Admin> courseData = std::make_unique<CourseManagement>();
    static std::unique_ptr<Admin> teacherData = std::make_unique<TeacherManagement>();
    static std::unique_ptr<Admin> gradeData = std::make_unique<GradeManagement>();
    static std::unique_ptr<Admin> roomData = std::make_unique<RoomManagement>();

    switch (wmId) {
        /* Admin Login */
    case 101:
        // back to Login Types from Admin Login Panel (back)
        user.showInterface(hWnd);
        break;
    case 102:
        // go to Admin Interface from Admin Login Panel (login)
        userAdmin.showInterface(hWnd);
        break;

        /* Admin Interface */
    case 111:
        // go to Course Management from Admin Interface (proceed)
        courseMngt->showInterface(hWnd);
        break;
    case 112:
        // go to Teacher Management from Admin Interface (proceed)
        teacherMngt->showInterface(hWnd);
        break;
    case 113:
        // go to Room Management from Admin Interface (proceed)
        roomMngt->showInterface(hWnd);
        break;
    case 114:
        // back to Login Types from Admin Interface (exit)
        user.showInterface(hWnd);
        break;

        /* Course Management */
    case 123:
        // go to Add Course from Course Management (back)
        courseData->addData(hWnd);
        break;
    case 124:
        // back to Admin Interface from Course Management (back)
        userAdmin.showInterface(hWnd);
        break;
    case 125:
        // back to Login Types from Course Management (exit)
        user.showInterface(hWnd);
        break;

        /* Add Course */
    case 151:
        // back to Course Management from Add Course (back)
        courseMngt->showInterface(hWnd);
        break;
    case 152:
        // insert a new Course to Database (insert)
        courseData->insertToDB(hWnd);
        break;

        /* Teacher Management */
    case 132:
        // go to Admin Add Teacher from Teacher Management (proceed)
        teacherData->addData(hWnd);
        break;
    case 133:
        // back to Teacher Management from Teacher Add (back)
        userAdmin.showInterface(hWnd);
        break;
    case 134:
        // back to Login Types from Teacher Management (exit)
        user.showInterface(hWnd);
        break;

        /* Add Teacher */
    case 141:
        // back to Teacher Management from Add Teacher (back)
        teacherMngt->showInterface(hWnd);
        break;
    case 142:
        // insert a new Teacher to Database (insert)
        teacherData->insertToDB(hWnd);
        break;

        /* Classroom Management */
    case 161:
        // go to Add Course from Course Management (back)
        roomMngt->manageData(hWnd);
        break;
    case 162:
        // go to Add Room from Room Management (back)
        roomData->addData(hWnd);
        break;
    case 163:
        // back to Admin Interface from Room Management (back)
        userAdmin.showInterface(hWnd);
        break;
    case 164:
        // back to Login Types from Room Management (exit)
        user.showInterface(hWnd);
        break;

        /* Add Classroom */
    case 171:
        // back to Classroom Management from Room Management (back)
        roomMngt->showInterface(hWnd);
        break;
    case 172:
        // back to Classroom Management from Room Management (back)
        roomData->insertToDB(hWnd);
        break;
    //case 172:
    //    // back to Login Types from Room Management (exit)
    //    user.showInterface(hWnd);
    //    break;  
    }
}

void HandleTeacherLogin(int wmId, HWND hWnd, User& user) {
    switch (wmId) {
        // back to Login Types from Teacher Login (back)
        case 201:
            user.showInterface(hWnd);
            break;
    }
}

void HandleStudentLogin(int wmId, HWND hWnd, User& user) {
    switch (wmId) {
        // back to Login Types from Student Login (back)
        case 301:
            user.showInterface(hWnd);
            break;
        }
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

    SetWindowText(hWnd, L"Beykoz - Course Schedule Management");

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HBRUSH hbrBkgnd = CreateSolidBrush(RGB(200, 200, 255));

    // Parent
    static User user;
    // User's children
    static std::unique_ptr<User> userAdmin = std::make_unique<Admin>();
    static std::unique_ptr<User> userTeacher = std::make_unique<Teacher>();
    static std::unique_ptr<User> userStudent = std::make_unique<Student>();

    auto* actualAdmin = dynamic_cast<Admin*>(userAdmin.get());

    switch (message)
    {
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetBkColor(hdcStatic, RGB(200, 200, 255));
        return (INT_PTR)hbrBkgnd;
    }

    case WM_CREATE:
        user.showInterface(hWnd);
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

        if (wmId == 180 && HIWORD(wParam) == LBN_SELCHANGE) {
            HWND hListBox = GetDlgItem(hWnd, 180);

            int selectedCount = SendMessage(hListBox, LB_GETSELCOUNT, 0, 0);

            if (selectedCount > 2)
                MessageBox(hWnd, L"Only two selections are allowed.", L"Selection Limit", MB_ICONWARNING | MB_OK);
        }

        if (wmId == IDM_EXIT)
            DestroyWindow(hWnd);

        // Login Types
        if (wmId >= 400 && wmId < 500)
            HandleLoginTypes(wmId, hWnd, *actualAdmin, *userTeacher, *userStudent);

        // Admin Windows
        if (wmId >= 100 && wmId < 200)
            HandleAdminLogin(wmId, hWnd, *actualAdmin, *userAdmin, *userTeacher, *userStudent, user);

        // Teacher Windows
        else if (wmId >= 200 && wmId < 300)
            HandleTeacherLogin(wmId, hWnd, user);

        // Grade Windows
        else if (wmId >= 300 && wmId < 400)
            HandleStudentLogin(wmId, hWnd, user);

        else 
            return DefWindowProc(hWnd, message, wParam, lParam);
       
        break;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        break;
    }

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
