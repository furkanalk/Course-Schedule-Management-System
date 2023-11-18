#include "handlers.h"
#include "course.h"
#include "SQLiteHandler.h"
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

WindowHandler WindowHandler::wh;

// Constructor to set the default values of window status' to FALSE and windows' to NULL
WindowHandler::WindowHandler() {
    windowCreationStatus["loginType"] = false;
    windowCreationStatus["adminLogin"] = false;
    windowCreationStatus["adminInterface"] = false;
    windowCreationStatus["adminCourseManagement"] = false;
    windowCreationStatus["adminManageCourse"] = false;
    windowCreationStatus["adminAddCourse"] = false;
    windowCreationStatus["adminTeacherManagement"] = false;
    windowCreationStatus["adminManageTeacher"] = false;
    windowCreationStatus["adminAddTeacher"] = false;
    windowCreationStatus["adminRoomManagement"] = false;
    windowCreationStatus["adminManageRoom"] = false;
    windowCreationStatus["adminAddRoom"] = false;
    windowCreationStatus["teacherLogin"] = false;
    windowCreationStatus["studentLogin"] = false;
}

// Set window to created
void WindowHandler::setWindowCreated(std::string windowName, bool isCreated) {
    windowCreationStatus[windowName] = isCreated;
}

// Check if the window is already created
bool WindowHandler::isWindowCreated(std::string windowName) {
    if (windowCreationStatus.find(windowName) != windowCreationStatus.end())
        return windowCreationStatus[windowName];
    else
        return false;
}

WindowHandler* WindowHandler::getInstance() {
    return &wh;
}

// Convert to wide string
std::wstring WindowHandler::convertToWideString(std::string str) {
    if (str.empty()) return L"";
    int bufferLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::vector<wchar_t> buffer(bufferLength);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], bufferLength);
    return std::wstring(buffer.begin(), buffer.end() - 1);
}

std::wstring WindowHandler::getComboBoxSelectedText(HWND comboBox) {
    int selectedIndex = SendMessage(comboBox, CB_GETCURSEL, 0, 0);
    if (selectedIndex < 0) return L"";
    int length = SendMessage(comboBox, CB_GETLBTEXTLEN, selectedIndex, 0);
    std::vector<wchar_t> buffer(length + 1);
    SendMessage(comboBox, CB_GETLBTEXT, selectedIndex, (LPARAM)&buffer[0]);
    return std::wstring(buffer.begin(), buffer.end() - 1);
}

// Get input
std::wstring WindowHandler::getWindowText(HWND hWnd) {
    int length = GetWindowTextLength(hWnd);
    if (length == 0) return L"";
    std::vector<wchar_t> buffer(length + 1);
    GetWindowText(hWnd, &buffer[0], length + 1);
    return std::wstring(buffer.begin(), buffer.end() - 1);
}

/* Login Type */
void WindowHandler::createLoginTypeWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    loginType.welcome = CreateWindow(TEXT("static"), TEXT("Welcome to our\nCourse Schedule\nManagement System!"), SS_CENTER | WS_CHILD, 100, 20, 600, 100, hWnd, NULL, NULL, NULL);
    loginType.admin = CreateWindow(TEXT("button"), TEXT("Admin"), WS_CHILD, 260, 160, 300, 100, hWnd, (HMENU)501, NULL, NULL);
    loginType.teacher = CreateWindow(TEXT("button"), TEXT("Teacher"), WS_CHILD, 260, 270, 300, 100, hWnd, (HMENU)502, NULL, NULL);
    loginType.student = CreateWindow(TEXT("button"), TEXT("Grade"), WS_CHILD, 260, 390, 300, 100, hWnd, (HMENU)503, NULL, NULL);

    // Apply fonts to controls
    SendMessage(loginType.welcome, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(loginType.admin, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(loginType.teacher, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(loginType.student, WM_SETFONT, (WPARAM)hFontH2, TRUE);
}

void WindowHandler::setLoginTypeVisibility(bool isVisible) {
    int cmdShow = isVisible ? SW_SHOW : SW_HIDE;
    ShowWindow(loginType.welcome, cmdShow);
    ShowWindow(loginType.admin, cmdShow);
    ShowWindow(loginType.teacher, cmdShow);
    ShowWindow(loginType.student, cmdShow);
}

// Admin Windows

/* Admin Login */
void WindowHandler::createAdminLoginWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(21, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminLogin.header = CreateWindow(TEXT("static"), TEXT("- Administrator Login -"), WS_CHILD | ES_CENTER, 100, 100, 600, 250, hWnd, NULL, NULL, NULL);
    adminLogin.username = CreateWindow(TEXT("static"), TEXT("Username ="), WS_CHILD | ES_CENTER, 240, 180, 120, 20, hWnd, NULL, NULL, NULL);
    adminLogin.usernameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 370, 180, 180, 25, hWnd, NULL, NULL, NULL);
    adminLogin.password = CreateWindow(TEXT("static"), TEXT("Password ="), WS_CHILD | ES_CENTER, 240, 240, 120, 20, hWnd, NULL, NULL, NULL);
    adminLogin.passwordInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD | ES_PASSWORD, 370, 240, 180, 25, hWnd, NULL, NULL, NULL);
    adminLogin.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 310, 300, 80, 30, hWnd, (HMENU)101, NULL, NULL);
    adminLogin.proceed = CreateWindow(TEXT("button"), TEXT("Login"), WS_BORDER | WS_CHILD, 410, 300, 80, 30, hWnd, (HMENU)102, NULL, NULL);

    // Apply fonts to controls
    SendMessage(adminLogin.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(adminLogin.username, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminLogin.usernameInput, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminLogin.password, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminLogin.passwordInput, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminLogin.previous, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminLogin.proceed, WM_SETFONT, (WPARAM)hFontH2, TRUE);
}

void WindowHandler::setAdminLoginVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminLogin.header, cmdShow);
    ShowWindow(adminLogin.username, cmdShow);
    ShowWindow(adminLogin.usernameInput, cmdShow);
    ShowWindow(adminLogin.password, cmdShow);
    ShowWindow(adminLogin.passwordInput, cmdShow);
    ShowWindow(adminLogin.previous, cmdShow);
    ShowWindow(adminLogin.proceed, cmdShow);
}

/* Admin Interface */
void WindowHandler::createAdminInterfaceWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminInterface.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminInterface.courseManagement = CreateWindow(TEXT("button"), TEXT("Course Management"), WS_CHILD | ES_CENTER, 240, 170, 300, 80, hWnd, (HMENU)111, NULL, NULL);
    adminInterface.teachers = CreateWindow(TEXT("button"), TEXT("Teachers"), WS_CHILD | ES_CENTER, 240, 270, 300, 80, hWnd, (HMENU)112, NULL, NULL);
    adminInterface.students = CreateWindow(TEXT("button"), TEXT("Classrooms"), WS_BORDER | WS_CHILD, 240, 370, 300, 80, hWnd, (HMENU)113, NULL, NULL);
    adminInterface.previous = CreateWindow(TEXT("button"), TEXT("Exit"), WS_BORDER | WS_CHILD, 350, 480, 80, 30, hWnd, (HMENU)114, NULL, NULL);

    // Apply fonts to controls
    SendMessage(adminInterface.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(adminInterface.courseManagement, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminInterface.teachers, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminInterface.students, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminInterface.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
}

void WindowHandler::setAdminInterfaceVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminInterface.header, cmdShow);
    ShowWindow(adminInterface.courseManagement, cmdShow);
    ShowWindow(adminInterface.teachers, cmdShow);
    ShowWindow(adminInterface.students, cmdShow);
    ShowWindow(adminInterface.previous, cmdShow);
}

/* Course Management */
void WindowHandler::createAdminCourseManagementWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminCourseManagement.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminCourseManagement.syllabus = CreateWindow(TEXT("button"), TEXT("Create Syllabus"), WS_CHILD | ES_CENTER, 240, 170, 300, 80, hWnd, (HMENU)121, NULL, NULL);
    adminCourseManagement.showCourses = CreateWindow(TEXT("button"), TEXT("Manage Courses"), WS_CHILD | ES_CENTER, 240, 270, 300, 80, hWnd, (HMENU)122, NULL, NULL);
    adminCourseManagement.addCourse = CreateWindow(TEXT("button"), TEXT("Add Course"), WS_BORDER | WS_CHILD, 240, 370, 300, 80, hWnd, (HMENU)123, NULL, NULL);
    adminCourseManagement.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)124, NULL, NULL);
    adminCourseManagement.proceed = CreateWindow(TEXT("button"), TEXT("Exit"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)125, NULL, NULL);

    // Apply fonts to controls
    SendMessage(adminCourseManagement.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(adminCourseManagement.syllabus, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminCourseManagement.showCourses, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminCourseManagement.addCourse, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminCourseManagement.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminCourseManagement.proceed, WM_SETFONT, (WPARAM)hFontH3, TRUE);
}

void WindowHandler::setAdminCourseManagementVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminCourseManagement.header, cmdShow);
    ShowWindow(adminCourseManagement.syllabus, cmdShow);
    ShowWindow(adminCourseManagement.showCourses, cmdShow);
    ShowWindow(adminCourseManagement.addCourse, cmdShow);
    ShowWindow(adminCourseManagement.previous, cmdShow);
    ShowWindow(adminCourseManagement.proceed, cmdShow);
}

/* Manage Courses */
void WindowHandler::createAdminManageCourseWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminManageCourse.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminManageCourse.courseList = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 200, 150, 400, 80, hWnd, (HMENU)210, NULL, NULL);
    adminManageCourse.fetch = CreateWindow(TEXT("button"), TEXT("Fetch"), WS_CHILD | ES_CENTER, 300, 250, 80, 30, hWnd, (HMENU)211, NULL, NULL);
    adminManageCourse.deleteCourse = CreateWindow(TEXT("button"), TEXT("Delete"), WS_CHILD | ES_CENTER, 400, 250, 80, 30, hWnd, (HMENU)212, NULL, NULL);
    adminManageCourse.courseName = CreateWindow(TEXT("EDIT"), TEXT(""), WS_CHILD | ES_CENTER, 240, 310, 300, 30, hWnd, (HMENU)213, NULL, NULL);
    adminManageCourse.courseRoomF = CreateWindow(TEXT("EDIT"), TEXT(""), WS_CHILD | ES_CENTER, 80, 360, 300, 30, hWnd, (HMENU)218, NULL, NULL);
    adminManageCourse.courseRoomFirst = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 400, 360, 300, 80, hWnd, (HMENU)214, NULL, NULL);
    adminManageCourse.courseRoomS = CreateWindow(TEXT("EDIT"), TEXT(""), WS_CHILD | ES_CENTER, 80, 400, 300, 30, hWnd, (HMENU)219, NULL, NULL);
    adminManageCourse.courseRoomSecond = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 400, 400, 300, 80, hWnd, (HMENU)217, NULL, NULL);
    adminManageCourse.update = CreateWindow(TEXT("button"), TEXT("Update"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)215, NULL, NULL);
    adminManageCourse.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)216, NULL, NULL);

    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    CourseManagement* allCourses = dbHandler->getCourses();

    auto ids = allCourses->getIds();
    auto names = allCourses->getNames();

    SendMessage(adminManageCourse.courseList, CB_RESETCONTENT, 0, 0);

    for (int i = 0; i < names.size(); i++) {
        std::wstring wideName = convertToWideString(names[i]);
        LRESULT index = SendMessage(adminManageCourse.courseList, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
        SendMessage(adminManageCourse.courseList, CB_SETITEMDATA, index, (LPARAM)ids[i]);
    }

    RoomManagement* allClassrooms = dbHandler->getClassrooms();

    auto classroomNames = allClassrooms->getNames();
    SendMessage(adminManageCourse.courseRoomFirst, CB_RESETCONTENT, 0, 0);
    SendMessage(adminManageCourse.courseRoomSecond, CB_RESETCONTENT, 0, 0);

    for (auto& name : classroomNames) {
        std::wstring wideName = convertToWideString(name);
        SendMessage(adminManageCourse.courseRoomFirst, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
        SendMessage(adminManageCourse.courseRoomSecond, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
    }

    SendMessage(adminManageCourse.courseRoomFirst, CB_SETCURSEL, -1, 0);
    SendMessage(adminManageCourse.courseRoomSecond, CB_SETCURSEL, -1, 0);

    // Apply fonts to controls
    SendMessage(adminManageCourse.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    //SendMessage(adminManageRoom.classList, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    /*SendMessage(adminManageCourse.fetch, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageCourse.courseName, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageCourse.courseFloor, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageCourse.deleteClass, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageCourse.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminManageCourse.update, WM_SETFONT, (WPARAM)hFontH3, TRUE);*/
}

void WindowHandler::setAdminManageCourseVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminManageCourse.header, cmdShow);
    ShowWindow(adminManageCourse.courseList, cmdShow);
    ShowWindow(adminManageCourse.deleteCourse, cmdShow);
    ShowWindow(adminManageCourse.fetch, cmdShow);
    ShowWindow(adminManageCourse.courseName, cmdShow);
    ShowWindow(adminManageCourse.courseRoomF, cmdShow);
    ShowWindow(adminManageCourse.courseRoomS, cmdShow);
    ShowWindow(adminManageCourse.courseRoomFirst, cmdShow);
    ShowWindow(adminManageCourse.courseRoomSecond, cmdShow);
    ShowWindow(adminManageCourse.previous, cmdShow);
    ShowWindow(adminManageCourse.update, cmdShow);
}

/* Add Course */
void WindowHandler::createAdminAddCourseWindows(HWND hWnd) {
    // Window Controls
    adminAddCourse.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminAddCourse.course = CreateWindow(TEXT("static"), TEXT("Course Name ="), WS_CHILD | ES_CENTER, 220, 150, 120, 20, hWnd, NULL, NULL, NULL);
    adminAddCourse.courseInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 150, 220, 25, hWnd, (HMENU)153, NULL, NULL);
    adminAddCourse.headerSecond = CreateWindow(TEXT("static"), TEXT("Select Classes"), WS_CHILD | ES_CENTER, 320, 250, 160, 40, hWnd, NULL, NULL, NULL);

    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    RoomManagement* allClassrooms = dbHandler->getClassrooms();

    adminAddCourse.classList = CreateWindowEx(0, WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_MULTIPLESEL, 200, 300, 400, 100, hWnd, (HMENU)150, GetModuleHandle(NULL), NULL);

    auto ids = allClassrooms->getIds();
    auto names = allClassrooms->getNames();
    auto floors = allClassrooms->getFloors();
    auto categories = allClassrooms->getCategories();

    for (int i = 0; i < names.size(); i++) {
        std::wstring itemText = convertToWideString(names[i]) + L" - " + convertToWideString(floors[i]) + L" - " + convertToWideString(categories[i]);

        LRESULT index = SendMessage(adminAddCourse.classList, LB_ADDSTRING, 0, (LPARAM)itemText.c_str());

        int roomId = ids[i];
        if (roomId <= 0) {
            MessageBox(hWnd, _T("Error retrieving id for list box item."), _T("Erro: ID is invalid"), MB_ICONERROR | MB_OK);
            continue;
        }
        else {
            SendMessage(adminAddCourse.classList, LB_SETITEMDATA, (WPARAM)index, (LPARAM)roomId);
        }
    }

    adminAddCourse.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)151, NULL, NULL);
    adminAddCourse.insert = CreateWindow(TEXT("button"), TEXT("Add"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)152, NULL, NULL);
}

void WindowHandler::setAdminAddCourseVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminAddCourse.header, cmdShow);
    ShowWindow(adminAddCourse.course, cmdShow);
    ShowWindow(adminAddCourse.courseInput, cmdShow);
    ShowWindow(adminAddCourse.headerSecond, cmdShow);
    ShowWindow(adminAddCourse.classList, cmdShow);
    ShowWindow(adminAddCourse.previous, cmdShow);
    ShowWindow(adminAddCourse.insert, cmdShow);
}

/* Teacher Management */
void WindowHandler::createAdminTeacherManagementWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminTeacherManagement.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminTeacherManagement.showTeachers = CreateWindow(TEXT("button"), TEXT("Manage Teachers"), WS_CHILD | ES_CENTER, 240, 170, 300, 80, hWnd, (HMENU)131, NULL, NULL);
    adminTeacherManagement.addTeacher = CreateWindow(TEXT("button"), TEXT("Add Teacher"), WS_CHILD | ES_CENTER, 240, 270, 300, 80, hWnd, (HMENU)132, NULL, NULL);
    adminTeacherManagement.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)133, NULL, NULL);
    adminTeacherManagement.proceed = CreateWindow(TEXT("button"), TEXT("Exit"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)134, NULL, NULL);

    // Apply fonts to controls
    SendMessage(adminTeacherManagement.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(adminTeacherManagement.showTeachers, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminTeacherManagement.addTeacher, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminTeacherManagement.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminTeacherManagement.proceed, WM_SETFONT, (WPARAM)hFontH3, TRUE);
}

void WindowHandler::setAdminTeacherManagementVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminTeacherManagement.header, cmdShow);
    ShowWindow(adminTeacherManagement.showTeachers, cmdShow);
    ShowWindow(adminTeacherManagement.addTeacher, cmdShow);
    ShowWindow(adminTeacherManagement.previous, cmdShow);
    ShowWindow(adminTeacherManagement.proceed, cmdShow);
}

/* Manage Teachers */
void WindowHandler::createAdminManageTeacherWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminManageTeacher.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminManageTeacher.teacherList = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 200, 150, 400, 80, hWnd, (HMENU)190, NULL, NULL);
    adminManageTeacher.fetch = CreateWindow(TEXT("button"), TEXT("Fetch"), WS_CHILD | ES_CENTER, 300, 250, 80, 30, hWnd, (HMENU)191, NULL, NULL);
    adminManageTeacher.deleteTeacher = CreateWindow(TEXT("button"), TEXT("Delete"), WS_CHILD | ES_CENTER, 400, 250, 80, 30, hWnd, (HMENU)192, NULL, NULL);
    adminManageTeacher.teacherName = CreateWindow(TEXT("EDIT"), TEXT(""), WS_CHILD | ES_CENTER, 240, 320, 300, 30, hWnd, (HMENU)193, NULL, NULL);
    adminManageTeacher.mondayStatic = CreateWindow(TEXT("static"), TEXT("Monday"), WS_CHILD | ES_CENTER, 65, 375, 100, 30, hWnd, NULL, NULL, NULL);
    adminManageTeacher.tuesdayStatic = CreateWindow(TEXT("static"), TEXT("Tuesday"), WS_CHILD | ES_CENTER, 175, 375, 100, 30, hWnd, NULL, NULL, NULL);
    adminManageTeacher.wednesdayStatic = CreateWindow(TEXT("static"), TEXT("Wednesday"), WS_CHILD | ES_CENTER, 285, 375, 100, 30, hWnd, NULL, NULL, NULL);
    adminManageTeacher.thursdayStatic = CreateWindow(TEXT("static"), TEXT("Thursday"), WS_CHILD | ES_CENTER, 395, 375, 100, 30, hWnd, NULL, NULL, NULL);
    adminManageTeacher.fridayStatic = CreateWindow(TEXT("static"), TEXT("Friday"), WS_CHILD | ES_CENTER, 505, 375, 100, 30, hWnd, NULL, NULL, NULL);
    adminManageTeacher.saturdayStatic = CreateWindow(TEXT("static"), TEXT("Saturday"), WS_CHILD | ES_CENTER, 615, 375, 100, 30, hWnd, NULL, NULL, NULL);
    adminManageTeacher.mondayBool = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD, 65, 410, 100, 80, hWnd, (HMENU)194, NULL, NULL);
    adminManageTeacher.tuesdayBool = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD, 175, 410, 100, 80, hWnd, (HMENU)195, NULL, NULL);
    adminManageTeacher.wednesdayBool = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD, 285, 410, 100, 80, hWnd, (HMENU)196, NULL, NULL);
    adminManageTeacher.thursdayBool = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD, 395, 410, 100, 80, hWnd, (HMENU)197, NULL, NULL);
    adminManageTeacher.fridayBool = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD, 505, 410, 100, 80, hWnd, (HMENU)198, NULL, NULL);
    adminManageTeacher.saturdayBool = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD, 615, 410, 100, 80, hWnd, (HMENU)199, NULL, NULL);
    adminManageTeacher.update = CreateWindow(TEXT("button"), TEXT("Update"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)200, NULL, NULL);
    adminManageTeacher.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)201, NULL, NULL);

    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    TeacherManagement* allTeachers = dbHandler->getTeachers();

    auto ids = allTeachers->getIds();
    auto names = allTeachers->getNames();

    SendMessage(adminManageTeacher.teacherList, CB_RESETCONTENT, 0, 0);
    for (int i = 0; i < names.size(); i++) {
        std::wstring wideName = convertToWideString(names[i]);
        LRESULT index = SendMessage(adminManageTeacher.teacherList, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
        SendMessage(adminManageTeacher.teacherList, CB_SETITEMDATA, index, (LPARAM)ids[i]);
    }

    std::vector<std::wstring> boolOptions = { L"True", L"False" };
    std::vector<HWND> boolComboBoxes = {
        adminManageTeacher.mondayBool,
        adminManageTeacher.tuesdayBool,
        adminManageTeacher.wednesdayBool,
        adminManageTeacher.thursdayBool,
        adminManageTeacher.fridayBool,
        adminManageTeacher.saturdayBool
    };

    for (HWND comboBox : boolComboBoxes) {
        SendMessage(comboBox, CB_RESETCONTENT, 0, 0);
        for (auto& option : boolOptions) {
            SendMessage(comboBox, CB_ADDSTRING, 0, (LPARAM)option.c_str());
        }
        SendMessage(comboBox, CB_SETCURSEL, -1, 0);
    }

    // Apply fonts to controls
    SendMessage(adminManageTeacher.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    //SendMessage(adminManageTeacher.classList, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    /*SendMessage(adminManageTeacher.fetch, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageTeacher.className, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageTeacher.classFloor, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageTeacher.classCategory, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageTeacher.deleteClass, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageTeacher.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminManageTeacher.update, WM_SETFONT, (WPARAM)hFontH3, TRUE);*/
}

void WindowHandler::setAdminManageTeacherVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminManageTeacher.header, cmdShow);
    ShowWindow(adminManageTeacher.teacherList, cmdShow);
    ShowWindow(adminManageTeacher.fetch, cmdShow);
    ShowWindow(adminManageTeacher.deleteTeacher, cmdShow);
    ShowWindow(adminManageTeacher.teacherName, cmdShow);
    ShowWindow(adminManageTeacher.mondayStatic, cmdShow);
    ShowWindow(adminManageTeacher.mondayBool, cmdShow);
    ShowWindow(adminManageTeacher.tuesdayStatic, cmdShow);
    ShowWindow(adminManageTeacher.tuesdayBool, cmdShow);
    ShowWindow(adminManageTeacher.wednesdayStatic, cmdShow);
    ShowWindow(adminManageTeacher.wednesdayBool, cmdShow);
    ShowWindow(adminManageTeacher.thursdayStatic, cmdShow);
    ShowWindow(adminManageTeacher.thursdayBool, cmdShow);
    ShowWindow(adminManageTeacher.fridayStatic, cmdShow);
    ShowWindow(adminManageTeacher.fridayBool, cmdShow);
    ShowWindow(adminManageTeacher.saturdayStatic, cmdShow);
    ShowWindow(adminManageTeacher.saturdayBool, cmdShow);
    ShowWindow(adminManageTeacher.previous, cmdShow);
    ShowWindow(adminManageTeacher.update, cmdShow);
}

/* Add Teacher */
void WindowHandler::createAdminAddTeacherWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH4 = CreateFont(30, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminAddTeacher.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminAddTeacher.fullname = CreateWindow(TEXT("static"), TEXT("Full Name ="), WS_CHILD | ES_CENTER, 220, 150, 120, 20, hWnd, NULL, NULL, NULL);
    adminAddTeacher.fullnameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 150, 220, 25, hWnd, HMENU(149), NULL, NULL);
    adminAddTeacher.course = CreateWindow(TEXT("static"), TEXT("Course ="), WS_CHILD | ES_CENTER, 230, 200, 120, 20, hWnd, NULL, NULL, NULL);
    adminAddTeacher.courseInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 200, 220, 25, hWnd, HMENU(140), NULL, NULL);
    adminAddTeacher.headerSecond = CreateWindow(TEXT("static"), TEXT("Work Days"), WS_CHILD | ES_CENTER, 320, 250, 160, 40, hWnd, NULL, NULL, NULL);
    adminAddTeacher.monday = CreateWindow(TEXT("button"), TEXT("Monday"), WS_CHILD | ES_CENTER | BS_CHECKBOX, 360, 300, 160, 20, hWnd, (HMENU)143, NULL, NULL);
    adminAddTeacher.tuesday = CreateWindow(TEXT("button"), TEXT("Tuesday"), WS_CHILD | ES_CENTER | BS_CHECKBOX, 360, 320, 160, 20, hWnd, (HMENU)144, NULL, NULL);
    adminAddTeacher.wednesday = CreateWindow(TEXT("button"), TEXT("Wednesday"), WS_CHILD | ES_CENTER | BS_CHECKBOX, 360, 340, 160, 20, hWnd, (HMENU)145, NULL, NULL);
    adminAddTeacher.thursday = CreateWindow(TEXT("button"), TEXT("Thursday"), WS_CHILD | ES_CENTER | BS_CHECKBOX, 360, 360, 160, 20, hWnd, (HMENU)146, NULL, NULL);
    adminAddTeacher.friday = CreateWindow(TEXT("button"), TEXT("Friday"), WS_CHILD | ES_CENTER | BS_CHECKBOX, 360, 380, 160, 20, hWnd, (HMENU)147, NULL, NULL);
    adminAddTeacher.saturday = CreateWindow(TEXT("button"), TEXT("Saturday"), WS_CHILD | ES_CENTER | BS_CHECKBOX, 360, 400, 160, 20, hWnd, (HMENU)148, NULL, NULL);
    adminAddTeacher.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)141, NULL, NULL);
    adminAddTeacher.insert = CreateWindow(TEXT("button"), TEXT("Add"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)142, NULL, NULL);

    // Apply fonts to controls
    SendMessage(adminAddTeacher.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(adminAddTeacher.fullname, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminAddTeacher.fullnameInput, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddTeacher.course, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminAddTeacher.courseInput, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddTeacher.headerSecond, WM_SETFONT, (WPARAM)hFontH4, TRUE);
    SendMessage(adminAddTeacher.monday, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddTeacher.tuesday, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddTeacher.wednesday, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddTeacher.thursday, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddTeacher.friday, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddTeacher.saturday, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddTeacher.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminAddTeacher.insert, WM_SETFONT, (WPARAM)hFontH3, TRUE);
}

void WindowHandler::setAdminAddTeacherVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminAddTeacher.header, cmdShow);
    ShowWindow(adminAddTeacher.fullname, cmdShow);
    ShowWindow(adminAddTeacher.fullnameInput, cmdShow);
    ShowWindow(adminAddTeacher.course, cmdShow);
    ShowWindow(adminAddTeacher.courseInput, cmdShow);
    ShowWindow(adminAddTeacher.headerSecond, cmdShow);
    ShowWindow(adminAddTeacher.monday, cmdShow);
    ShowWindow(adminAddTeacher.tuesday, cmdShow);
    ShowWindow(adminAddTeacher.wednesday, cmdShow);
    ShowWindow(adminAddTeacher.thursday, cmdShow);
    ShowWindow(adminAddTeacher.friday, cmdShow);
    ShowWindow(adminAddTeacher.saturday, cmdShow);
    ShowWindow(adminAddTeacher.previous, cmdShow);
    ShowWindow(adminAddTeacher.insert, cmdShow);
}

/* Room Management */
void WindowHandler::createAdminRoomManagementWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminRoomManagement.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminRoomManagement.showRooms = CreateWindow(TEXT("button"), TEXT("Manage Classrooms"), WS_CHILD | ES_CENTER, 240, 170, 300, 80, hWnd, (HMENU)161, NULL, NULL);
    adminRoomManagement.addRoom = CreateWindow(TEXT("button"), TEXT("Add Classroom"), WS_CHILD | ES_CENTER, 240, 270, 300, 80, hWnd, (HMENU)162, NULL, NULL);
    adminRoomManagement.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)163, NULL, NULL);
    adminRoomManagement.proceed = CreateWindow(TEXT("button"), TEXT("Exit"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)164, NULL, NULL);

    // Apply fonts to controls
    SendMessage(adminRoomManagement.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(adminRoomManagement.showRooms, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminRoomManagement.addRoom, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminRoomManagement.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminRoomManagement.proceed, WM_SETFONT, (WPARAM)hFontH3, TRUE);
}

void WindowHandler::setAdminRoomManagementVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminRoomManagement.header, cmdShow);
    ShowWindow(adminRoomManagement.showRooms, cmdShow);
    ShowWindow(adminRoomManagement.addRoom, cmdShow);
    ShowWindow(adminRoomManagement.previous, cmdShow);
    ShowWindow(adminRoomManagement.proceed, cmdShow);
}

/* Manage Rooms */
void WindowHandler::createAdminManageRoomWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminManageRoom.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminManageRoom.classList = CreateWindowEx(0, WC_COMBOBOX, NULL, CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL, 200, 150, 400, 80, hWnd, (HMENU)180, NULL, NULL);
    adminManageRoom.fetch = CreateWindow(TEXT("button"), TEXT("Fetch"), WS_CHILD | ES_CENTER, 300, 250, 80, 30, hWnd, (HMENU)181, NULL, NULL);
    adminManageRoom.deleteClass = CreateWindow(TEXT("button"), TEXT("Delete"), WS_CHILD | ES_CENTER, 400, 250, 80, 30, hWnd, (HMENU)182, NULL, NULL);
    adminManageRoom.className = CreateWindow(TEXT("EDIT"), TEXT(""), WS_CHILD | ES_CENTER, 240, 320, 300, 30, hWnd, (HMENU)185, NULL, NULL);
    adminManageRoom.classFloor = CreateWindow(TEXT("EDIT"), TEXT(""), WS_CHILD | ES_CENTER, 240, 360, 300, 30, hWnd, (HMENU)186, NULL, NULL);
    adminManageRoom.classCategory = CreateWindow(TEXT("EDIT"), TEXT(""), WS_CHILD | ES_CENTER, 240, 400, 300, 30, hWnd, (HMENU)187, NULL, NULL);
    adminManageRoom.update = CreateWindow(TEXT("button"), TEXT("Update"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)183, NULL, NULL);
    adminManageRoom.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)184, NULL, NULL);

    SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
    RoomManagement* allClassrooms = dbHandler->getClassrooms();

    auto ids = allClassrooms->getIds();
    auto names = allClassrooms->getNames();

    SendMessage(adminManageRoom.classList, CB_RESETCONTENT, 0, 0);

    for (int i = 0; i < names.size(); i++) {
        std::wstring wideName = convertToWideString(names[i]);
        LRESULT index = SendMessage(adminManageRoom.classList, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
        SendMessage(adminManageRoom.classList, CB_SETITEMDATA, index, (LPARAM)ids[i]);
    }

    // Apply fonts to controls
    SendMessage(adminManageRoom.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    //SendMessage(adminManageRoom.classList, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminManageRoom.fetch, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageRoom.className, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageRoom.classFloor, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageRoom.classCategory, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageRoom.deleteClass, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminManageRoom.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminManageRoom.update, WM_SETFONT, (WPARAM)hFontH3, TRUE);
}

void WindowHandler::setAdminManageRoomVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminManageRoom.header, cmdShow);
    ShowWindow(adminManageRoom.classList, cmdShow);
    ShowWindow(adminManageRoom.deleteClass, cmdShow);
    ShowWindow(adminManageRoom.fetch, cmdShow);
    ShowWindow(adminManageRoom.classFloor, cmdShow);
    ShowWindow(adminManageRoom.classCategory, cmdShow);
    ShowWindow(adminManageRoom.className, cmdShow);
    ShowWindow(adminManageRoom.previous, cmdShow);
    ShowWindow(adminManageRoom.update, cmdShow);
}

/* Add Room */
void WindowHandler::createAdminAddRoomWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminAddRoom.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminAddRoom.name = CreateWindow(TEXT("static"), TEXT("Classroom ="), WS_CHILD | ES_CENTER, 220, 150, 120, 20, hWnd, NULL, NULL, NULL);
    adminAddRoom.nameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 150, 220, 25, hWnd, HMENU(173), NULL, NULL);
    adminAddRoom.floor = CreateWindow(TEXT("static"), TEXT("Floor ="), WS_CHILD | ES_CENTER, 245, 200, 120, 20, hWnd, NULL, NULL, NULL);
    adminAddRoom.floorInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 200, 220, 25, hWnd, HMENU(174), NULL, NULL);
    adminAddRoom.headerSecond = CreateWindow(TEXT("static"), TEXT("Room Type"), WS_CHILD | ES_CENTER, 320, 260, 140, 30, hWnd, NULL, NULL, NULL);
    adminAddRoom.category = CreateWindow(TEXT("COMBOBOX"), TEXT(""), CBS_DROPDOWNLIST | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE, 320, 300, 160, 70, hWnd, (HMENU)170, NULL, NULL);
    adminAddRoom.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 300, 480, 80, 30, hWnd, (HMENU)171, NULL, NULL);
    adminAddRoom.insert = CreateWindow(TEXT("button"), TEXT("Add"), WS_BORDER | WS_CHILD, 400, 480, 80, 30, hWnd, (HMENU)172, NULL, NULL);

    // Fill the listbox
    SendMessage(adminAddRoom.category, CB_ADDSTRING, 0, (LPARAM)TEXT("Classroom"));
    SendMessage(adminAddRoom.category, CB_ADDSTRING, 0, (LPARAM)TEXT("Laboratory"));
    SendMessage(adminAddRoom.category, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

    // Apply fonts to controls
    SendMessage(adminAddRoom.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(adminAddRoom.name, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminAddRoom.nameInput, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddRoom.floor, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminAddRoom.floorInput, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddRoom.headerSecond, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminAddRoom.category, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(adminAddRoom.previous, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(adminAddRoom.insert, WM_SETFONT, (WPARAM)hFontH3, TRUE);
}

void WindowHandler::setAdminAddRoomVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(adminAddRoom.header, cmdShow);
    ShowWindow(adminAddRoom.name, cmdShow);
    ShowWindow(adminAddRoom.nameInput, cmdShow);
    ShowWindow(adminAddRoom.floor, cmdShow);
    ShowWindow(adminAddRoom.floorInput, cmdShow);
    ShowWindow(adminAddRoom.headerSecond, cmdShow);
    ShowWindow(adminAddRoom.category, cmdShow);
    ShowWindow(adminAddRoom.previous, cmdShow);
    ShowWindow(adminAddRoom.insert, cmdShow);
}

// Teacher Windows

/* Teacher Login */
void WindowHandler::createTeacherLoginWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    teacherLogin.header = CreateWindow(TEXT("static"), TEXT("- Teacher Login -"), WS_CHILD | ES_CENTER, 100, 100, 600, 250, hWnd, NULL, NULL, NULL);
    teacherLogin.username = CreateWindow(TEXT("static"), TEXT("Username ="), WS_CHILD | ES_CENTER, 240, 180, 120, 20, hWnd, NULL, NULL, NULL);
    teacherLogin.usernameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 370, 180, 180, 20, hWnd, NULL, NULL, NULL);
    teacherLogin.password = CreateWindow(TEXT("static"), TEXT("Password ="), WS_CHILD | ES_CENTER, 240, 240, 120, 20, hWnd, NULL, NULL, NULL);
    teacherLogin.passwordInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD | ES_PASSWORD, 370, 240, 180, 20, hWnd, NULL, NULL, NULL);
    teacherLogin.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 310, 300, 80, 30, hWnd, (HMENU)301, NULL, NULL);
    teacherLogin.proceed = CreateWindow(TEXT("button"), TEXT("Login"), WS_BORDER | WS_CHILD, 410, 300, 80, 30, hWnd, (HMENU)302, NULL, NULL);

    // Apply fonts to controls
    SendMessage(teacherLogin.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(teacherLogin.username, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(teacherLogin.usernameInput, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(teacherLogin.password, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(teacherLogin.passwordInput, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(teacherLogin.previous, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(teacherLogin.proceed, WM_SETFONT, (WPARAM)hFontH2, TRUE);
}

void WindowHandler::setTeacherLoginVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(teacherLogin.header, cmdShow);
    ShowWindow(teacherLogin.username, cmdShow);
    ShowWindow(teacherLogin.usernameInput, cmdShow);
    ShowWindow(teacherLogin.password, cmdShow);
    ShowWindow(teacherLogin.passwordInput, cmdShow);
    ShowWindow(teacherLogin.previous, cmdShow);
    ShowWindow(teacherLogin.proceed, cmdShow);
}

// Grade Windows

/* Grade Login */
void WindowHandler::createStudentLoginWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    studentLogin.header = CreateWindow(TEXT("static"), TEXT("- Student Login -"), WS_CHILD | ES_CENTER, 100, 100, 600, 250, hWnd, NULL, NULL, NULL);
    studentLogin.username = CreateWindow(TEXT("static"), TEXT("Username ="), WS_CHILD | ES_CENTER, 240, 180, 120, 20, hWnd, NULL, NULL, NULL);
    studentLogin.usernameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 370, 180, 180, 20, hWnd, NULL, NULL, NULL);
    studentLogin.password = CreateWindow(TEXT("static"), TEXT("Password ="), WS_CHILD | ES_CENTER, 240, 240, 120, 20, hWnd, NULL, NULL, NULL);
    studentLogin.passwordInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD | ES_PASSWORD, 370, 240, 180, 20, hWnd, NULL, NULL, NULL);
    studentLogin.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 310, 300, 80, 30, hWnd, (HMENU)401, NULL, NULL);
    studentLogin.proceed = CreateWindow(TEXT("button"), TEXT("Login"), WS_BORDER | WS_CHILD, 410, 300, 80, 30, hWnd, (HMENU)402, NULL, NULL);

    // Apply fonts to controls
    SendMessage(studentLogin.header, WM_SETFONT, (WPARAM)hFontH1, TRUE);
    SendMessage(studentLogin.username, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(studentLogin.usernameInput, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(studentLogin.password, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(studentLogin.passwordInput, WM_SETFONT, (WPARAM)hFontH3, TRUE);
    SendMessage(studentLogin.previous, WM_SETFONT, (WPARAM)hFontH2, TRUE);
    SendMessage(studentLogin.proceed, WM_SETFONT, (WPARAM)hFontH2, TRUE);
}

void WindowHandler::setStudentLoginVisibility(bool visible) {
    int cmdShow = visible ? SW_SHOW : SW_HIDE;
    ShowWindow(studentLogin.header, cmdShow);
    ShowWindow(studentLogin.username, cmdShow);
    ShowWindow(studentLogin.usernameInput, cmdShow);
    ShowWindow(studentLogin.password, cmdShow);
    ShowWindow(studentLogin.passwordInput, cmdShow);
    ShowWindow(studentLogin.previous, cmdShow);
    ShowWindow(studentLogin.proceed, cmdShow);
}