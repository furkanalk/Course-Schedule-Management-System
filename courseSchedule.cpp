/* Git Repo */
/* https://github.com/furkanalk/course-schedule-management.git */

#include "framework.h"
#include "course.h"
#include "SQLiteHandler.h"
#include "resource.h"
#include "handlers.h"
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
    if (!InitInstance(hInstance, nCmdShow))
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
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_COURSESCHEDULE));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(200, 200, 255));
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

// Window Handlers

/* Login Types */
void HandleLoginTypes(int wmID, HWND hWnd, User& userAdmin) {
    static std::unique_ptr<User> userTeacher = std::make_unique<Teacher>();
    static std::unique_ptr<User> userStudent = std::make_unique<Student>();

    switch (wmID) {
    case 501:
        userAdmin.login(hWnd);
        break;
    case 502:
        userTeacher->showInterface(hWnd);
        break;
    case 503:
        userStudent->showInterface(hWnd);
        break;
    }
}

/* Admin Windows */
void HandleAdminLogin(int wmId, HWND hWnd, User& userAdmin, User& user) {
    static std::unique_ptr<User> roomMngt = std::make_unique<RoomManagement>();
    static std::unique_ptr<User> courseMngt = std::make_unique<CourseManagement>();
    static std::unique_ptr<User> teacherMngt = std::make_unique<TeacherManagement>();

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
    case 122:
        // go to Manage Course from Course Management (proceed)
        courseMngt->manageData(hWnd);
        break;
    case 123:
        // go to Add Course from Course Management (back)
        courseMngt->addData(hWnd);
        break;
    case 124:
        // back to Admin Interface from Course Management (back)
        userAdmin.showInterface(hWnd);
        break;
    case 125:
        // back to Login Types from Course Management (exit)
        user.showInterface(hWnd);
        break;

        /* Courses Manage Course */
    case 216:
        // back to Course Management from Manage Course (back)
        courseMngt->showInterface(hWnd);
        break;
        /* Add Course */
    case 151:
        // back to Course Management from Add Course (back)
        courseMngt->showInterface(hWnd);
        break;
    case 152:
        // insert a new Course to Database (insert)
        courseMngt->insertToDB(hWnd);
        break;

        /* Teacher Management */
    case 131:
        // go to Manage Teacher from Teacher Management (proceed)
        teacherMngt->manageData(hWnd);
        break;
    case 132:
        // go to Admin Add Teacher from Teacher Management (proceed)
        teacherMngt->addData(hWnd);
        break;
    case 133:
        // back to Teacher Management from Teacher Add (back)
        userAdmin.showInterface(hWnd);
        break;
    case 134:
        // back to Login Types from Teacher Management (exit)
        user.showInterface(hWnd);
        break;

        /* Teacher Manage Teachers */
    case 201:
        // back to Teacher Management from Manage Teachers
        teacherMngt->showInterface(hWnd);
        break;

        /* Add Teacher */
    case 141:
        // back to Teacher Management from Add Teacher (back)
        teacherMngt->showInterface(hWnd);
        break;
    case 142:
        // insert a new Teacher to Database (insert)
        teacherMngt->insertToDB(hWnd);
        break;

        /* Classroom Management */
    case 161:
        // go to Manage Room from Course Management (back)
        roomMngt->manageData(hWnd);
        break;
    case 162:
        // go to Add Room from Room Management (back)
        roomMngt->addData(hWnd);
        break;
    case 163:
        // back to Admin Interface from Room Management (back)
        userAdmin.showInterface(hWnd);
        break;
    case 164:
        // back to Login Types from Room Management (exit)
        user.showInterface(hWnd);
        break;

        /* Classroom Manage Classes */
    case 184:
        // back to Classroom Management from Manage Classes
        roomMngt->showInterface(hWnd);
        break;

        /* Add Classroom */
    case 171:
        // back to Classroom Management from Room Management (back)
        roomMngt->showInterface(hWnd);
        break;
    case 172:
        // insert a new classroom to Database (insert)
        roomMngt->insertToDB(hWnd);
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
    case 301:
        user.showInterface(hWnd);
        break;
    }
}

void HandleStudentLogin(int wmId, HWND hWnd, User& user) {
    switch (wmId) {
        // back to Login Types from Student Login (back)
    case 401:
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

    static User user;
    static RoomManagement roomMngmt;
    static TeacherManagement teacherMngmt;
    static CourseManagement courseMngmt;
    static std::unique_ptr<User> userAdmin = std::make_unique<Admin>();
    
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
        int wmEvent = HIWORD(wParam);

        HWND hRoomComboBox = GetDlgItem(hWnd, 180);
        HWND hTeacherComboBox = GetDlgItem(hWnd, 190);
        HWND hCourseComboBox = GetDlgItem(hWnd, 210);

        if (wmId == 150 && HIWORD(wParam) == LBN_SELCHANGE) {
            HWND hListBox = GetDlgItem(hWnd, 150);

            int selectedCount = SendMessage(hListBox, LB_GETSELCOUNT, 0, 0);

            if (selectedCount > 2)
                MessageBox(hWnd, L"Only two selections are allowed.", L"Selection Limit", MB_ICONWARNING | MB_OK);
        }
  
        if (wmId == 211) { // Course fetch
            HWND hCourseComboBox = GetDlgItem(hWnd, 210);
            int idx = SendMessage(hCourseComboBox, CB_GETCURSEL, 0, 0);
            if (idx == CB_ERR) {
                MessageBox(hWnd, L"Please make a selection.", L"Selection Error", MB_ICONWARNING | MB_OK);
                return 0;
            }

            SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
            CourseManagement* allCourses = dbHandler->getCourses();

            auto courseNames = allCourses->getNames();
            auto courseRoomNames = allCourses->getRooms()[idx];

            HWND hNameInput = GetDlgItem(hWnd, 213);
            SetWindowText(hNameInput, std::wstring(courseNames[idx].begin(), courseNames[idx].end()).c_str());

            HWND hFirstRoomInput = GetDlgItem(hWnd, 218);
            HWND hSecondRoomInput = GetDlgItem(hWnd, 219);

            if (courseRoomNames.size() >= 1) {
                SetWindowText(hFirstRoomInput, std::wstring(courseRoomNames[0].begin(), courseRoomNames[0].end()).c_str());
            }
            else {
                SetWindowText(hFirstRoomInput, L"-Empty-");
            }

            if (courseRoomNames.size() >= 2) {
                SetWindowText(hSecondRoomInput, std::wstring(courseRoomNames[1].begin(), courseRoomNames[1].end()).c_str());
            }
            else {
                SetWindowText(hSecondRoomInput, L"-Empty-");
            }
        }

        if (wmId == 215) { // Update course
            if (!courseMngmt.updateCourse(hWnd, hCourseComboBox)) { return 0; }
        }
        if (wmId == 212) { // Delete course
            if (!courseMngmt.removeCourse(hWnd, hCourseComboBox)) { return 0; }
        }

        // Teacher fetch
        if (wmId == 191) {
            int itemCount = (int)SendMessage(hTeacherComboBox, CB_GETCOUNT, 0, 0);
            int idx = (int)SendMessage(hTeacherComboBox, CB_GETCURSEL, 0, 0);

            if (idx == CB_ERR) {
                MessageBox(hWnd, L"Please make a selection.", L"Selection Error", MB_ICONWARNING | MB_OK);
                break;
            }

            SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
            TeacherManagement* allTeachers = dbHandler->getTeachers();

            WindowHandler* wh = WindowHandler::getInstance();
            std::wstring teacherName = wh->convertToWideString(allTeachers->getNames()[idx]);
            SetWindowText(GetDlgItem(hWnd, 193), teacherName.c_str());

            auto workdays = allTeachers->getWorkdays()[idx];

            HWND hMonday = GetDlgItem(hWnd, 194);
            HWND hTuesday = GetDlgItem(hWnd, 195);
            HWND hWednesday = GetDlgItem(hWnd, 196);
            HWND hThursday = GetDlgItem(hWnd, 197);
            HWND hFriday = GetDlgItem(hWnd, 198);
            HWND hSaturday = GetDlgItem(hWnd, 199);

            SendMessage(hMonday, CB_SETCURSEL, workdays[0] ? 0 : 1, 0);
            SendMessage(hTuesday, CB_SETCURSEL, workdays[1] ? 0 : 1, 0);
            SendMessage(hWednesday, CB_SETCURSEL, workdays[2] ? 0 : 1, 0);
            SendMessage(hThursday, CB_SETCURSEL, workdays[3] ? 0 : 1, 0);
            SendMessage(hFriday, CB_SETCURSEL, workdays[4] ? 0 : 1, 0);
            SendMessage(hSaturday, CB_SETCURSEL, workdays[5] ? 0 : 1, 0);
        }

        if (wmId == 200) { // Update teacher
            if (!teacherMngmt.updateTeacher(hWnd, hTeacherComboBox)) { return 0; }
        }

        if (wmId == 192) { // Delete teacher
            if (!teacherMngmt.removeTeacher(hWnd, hTeacherComboBox)) { return 0; }
        }

        // Classroom fetch
        if (wmId == 181) {
            int idx = (int)SendMessage(hRoomComboBox, CB_GETCURSEL, 0, 0);
            if (idx == CB_ERR) {
                MessageBox(hWnd, L"Please make a selection.", L"Selection Error", MB_ICONWARNING | MB_OK);
                break;
            }

            SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
            RoomManagement* allClassrooms = dbHandler->getClassrooms();

            const auto& names = allClassrooms->getNames();
            const auto& floors = allClassrooms->getFloors();
            const auto& categories = allClassrooms->getCategories();

            WindowHandler* wh = WindowHandler::getInstance();
            const std::wstring name = wh->convertToWideString(names[idx]);
            const std::wstring floor = wh->convertToWideString(floors[idx]);
            const std::wstring category = wh->convertToWideString(categories[idx]);

            HWND hNameInput = GetDlgItem(hWnd, 185);
            HWND hFloorInput = GetDlgItem(hWnd, 186);
            HWND hCategoryInput = GetDlgItem(hWnd, 187);

            SetWindowText(hNameInput, name.c_str());
            SetWindowText(hFloorInput, floor.c_str());
            SetWindowText(hCategoryInput, category.c_str());
        }

        if (wmId == 183) { // Update classroom
            if (!roomMngmt.updateRoom(hWnd, hRoomComboBox)) { return 0; }
        }
        if (wmId == 182) { // Delete classroom
            if (!roomMngmt.removeRoom(hWnd, hRoomComboBox)) { return 0; }
        }

        if (wmId == IDM_EXIT)
            DestroyWindow(hWnd);

        // Login Types
        if (wmId >= 500 && wmId < 600)
            HandleLoginTypes(wmId, hWnd, *userAdmin);

        // Admin Windows
        if (wmId >= 100 && wmId < 300)
            HandleAdminLogin(wmId, hWnd, *userAdmin, user);

        // Teacher Windows
        if (wmId >= 300 && wmId < 400)
            HandleTeacherLogin(wmId, hWnd, user);

        // Grade Windows
        if (wmId >= 400 && wmId < 500)
            HandleStudentLogin(wmId, hWnd, user);

        return DefWindowProc(hWnd, message, wParam, lParam);
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