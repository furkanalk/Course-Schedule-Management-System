#include "handlers.h"
#include <tchar.h>

// Constructor to set the default values of window status' to FALSE and windows to NULL
WindowHandler::WindowHandler() {
    windowCreationStatus["loginType"] = false;
    windowCreationStatus["adminLogin"] = false;
    windowCreationStatus["adminInterface"] = false;
    windowCreationStatus["adminCourseManagement"] = false;
    windowCreationStatus["teacherLogin"] = false;
    windowCreationStatus["studentLogin"] = false;
}

void WindowHandler::setWindowCreated(const std::string& windowName, bool isCreated) {
    windowCreationStatus[windowName] = isCreated;
}

bool WindowHandler::isWindowCreated(const std::string& windowName) {
    if (windowCreationStatus.find(windowName) != windowCreationStatus.end()) {
        return windowCreationStatus[windowName];
    }
    else {
        return false;
    }
}

/* Login Type */
void WindowHandler::createLoginTypeWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    loginType.welcome = CreateWindow(TEXT("static"), TEXT("Welcome to our\nCourse Schedule\nManagement System!"), SS_CENTER | WS_CHILD , 100, 20, 600, 100, hWnd, NULL, NULL, NULL);
    loginType.admin = CreateWindow(TEXT("button"), TEXT("Admin"), WS_CHILD, 260, 160, 300, 100, hWnd, (HMENU)401, NULL, NULL);
    loginType.teacher = CreateWindow(TEXT("button"), TEXT("Teacher"), WS_CHILD, 260, 270, 300, 100, hWnd, (HMENU)402, NULL, NULL);
    loginType.student = CreateWindow(TEXT("button"), TEXT("Student"), WS_CHILD, 260, 390, 300, 100, hWnd, (HMENU)403, NULL, NULL);

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

/* Admin Login */
void WindowHandler::createAdminLoginWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminLogin.header = CreateWindow(TEXT("static"), TEXT("- Administrator Login -"), WS_CHILD | ES_CENTER | WS_BORDER, 100, 100, 600, 250, hWnd, NULL, NULL, NULL);
    adminLogin.username = CreateWindow(TEXT("static"), TEXT("Username ="), WS_CHILD | ES_CENTER, 240, 180, 120, 20, hWnd, NULL, NULL, NULL);
    adminLogin.usernameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 370, 180, 180, 20, hWnd, NULL, NULL, NULL);
    adminLogin.password = CreateWindow(TEXT("static"), TEXT("Password ="), WS_CHILD | ES_CENTER, 240, 240, 120, 20, hWnd, NULL, NULL, NULL);
    adminLogin.passwordInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD | ES_PASSWORD, 370, 240, 180, 20, hWnd, NULL, NULL, NULL);
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
    adminInterface.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER | WS_BORDER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminInterface.courseManagement = CreateWindow(TEXT("button"), TEXT("Course Management"), WS_CHILD | ES_CENTER, 240, 170, 300, 80, hWnd, (HMENU)111, NULL, NULL);
    adminInterface.teachers = CreateWindow(TEXT("button"), TEXT("Teachers"), WS_CHILD | ES_CENTER, 240, 270, 300, 80, hWnd, (HMENU)112, NULL, NULL);
    adminInterface.students = CreateWindow(TEXT("button"), TEXT("Students"), WS_BORDER | WS_CHILD, 240, 370, 300, 80, hWnd, (HMENU)113, NULL, NULL);
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
    adminCourseManagement.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER | WS_BORDER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminCourseManagement.syllabus = CreateWindow(TEXT("button"), TEXT("Create Syllabus"), WS_CHILD | ES_CENTER, 240, 170, 300, 80, hWnd, (HMENU)121, NULL, NULL);
    adminCourseManagement.showCourses = CreateWindow(TEXT("button"), TEXT("See Courses"), WS_CHILD | ES_CENTER, 240, 270, 300, 80, hWnd, (HMENU)122, NULL, NULL);
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

/* Teacher Management */
void WindowHandler::createAdminTeacherManagementWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(32, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    adminTeacherManagement.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER | WS_BORDER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
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

/* Teacher Login */
void WindowHandler::createTeacherLoginWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    teacherLogin.header = CreateWindow(TEXT("static"), TEXT("- Teacher Login -"), WS_CHILD | ES_CENTER | WS_BORDER, 100, 100, 600, 250, hWnd, NULL, NULL, NULL);
    teacherLogin.username = CreateWindow(TEXT("static"), TEXT("Username ="), WS_CHILD | ES_CENTER, 240, 180, 120, 20, hWnd, NULL, NULL, NULL);
    teacherLogin.usernameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 370, 180, 180, 20, hWnd, NULL, NULL, NULL);
    teacherLogin.password = CreateWindow(TEXT("static"), TEXT("Password ="), WS_CHILD | ES_CENTER, 240, 240, 120, 20, hWnd, NULL, NULL, NULL);
    teacherLogin.passwordInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD | ES_PASSWORD, 370, 240, 180, 20, hWnd, NULL, NULL, NULL);
    teacherLogin.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 310, 300, 80, 30, hWnd, (HMENU)201, NULL, NULL);
    teacherLogin.proceed = CreateWindow(TEXT("button"), TEXT("Login"), WS_BORDER | WS_CHILD, 410, 300, 80, 30, hWnd, (HMENU)202, NULL, NULL);

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

/* Student Login */
void WindowHandler::createStudentLoginWindows(HWND hWnd) {
    // Font style
    HFONT hFontH1 = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH2 = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));
    HFONT hFontH3 = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

    // Window Controls
    studentLogin.header = CreateWindow(TEXT("static"), TEXT("- Student Login -"), WS_CHILD | ES_CENTER | WS_BORDER, 100, 100, 600, 250, hWnd, NULL, NULL, NULL);
    studentLogin.username = CreateWindow(TEXT("static"), TEXT("Username ="), WS_CHILD | ES_CENTER, 240, 180, 120, 20, hWnd, NULL, NULL, NULL);
    studentLogin.usernameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 370, 180, 180, 20, hWnd, NULL, NULL, NULL);
    studentLogin.password = CreateWindow(TEXT("static"), TEXT("Password ="), WS_CHILD | ES_CENTER, 240, 240, 120, 20, hWnd, NULL, NULL, NULL);
    studentLogin.passwordInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD | ES_PASSWORD, 370, 240, 180, 20, hWnd, NULL, NULL, NULL);
    studentLogin.previous = CreateWindow(TEXT("button"), TEXT("Back"), WS_BORDER | WS_CHILD, 310, 300, 80, 30, hWnd, (HMENU)301, NULL, NULL);
    studentLogin.proceed = CreateWindow(TEXT("button"), TEXT("Login"), WS_BORDER | WS_CHILD, 410, 300, 80, 30, hWnd, (HMENU)302, NULL, NULL);

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