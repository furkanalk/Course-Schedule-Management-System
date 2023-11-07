#include "handlers.h"
#include "course.h"
#include "SQLiteHandler.h"
#include <tchar.h>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <iterator>

std::unique_ptr<WindowHandler> WindowHandler::instance = nullptr;

// Constructor to set the default values of window status' to FALSE and windows' to NULL
WindowHandler::WindowHandler() {
    windowCreationStatus["loginType"] = false;
    windowCreationStatus["adminLogin"] = false;
    windowCreationStatus["adminInterface"] = false;
    windowCreationStatus["adminCourseManagement"] = false;
    windowCreationStatus["adminAddCourse"] = false;
    windowCreationStatus["adminTeacherManagement"] = false;
    windowCreationStatus["adminAddTeacher"] = false;
    windowCreationStatus["adminRoomManagement"] = false;
    windowCreationStatus["adminManageRoom"] = false;
    windowCreationStatus["adminAddRoom"] = false;
    windowCreationStatus["teacherLogin"] = false;
    windowCreationStatus["studentLogin"] = false;
}

// Set window to created
void WindowHandler::setWindowCreated(const std::string& windowName, bool isCreated) {
    windowCreationStatus[windowName] = isCreated;
}

// Check if the window is already created
bool WindowHandler::isWindowCreated(const std::string& windowName) {
    if (windowCreationStatus.find(windowName) != windowCreationStatus.end())
        return windowCreationStatus[windowName];
    else
        return false;
}

WindowHandler* WindowHandler::getInstance() {
    if (instance == nullptr)
        instance = std::make_unique<WindowHandler>();
    return instance.get();
}

// Convert to wide string
std::wstring WindowHandler::convertToWideString(const std::string& str) {
    int bufferLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);

    if (bufferLength == 0)
        throw std::runtime_error("Failed to convert string to wide character format.");

    std::vector<wchar_t> buffer(bufferLength);
    int conversionResult = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.data(), bufferLength);

    if (conversionResult == 0)
        throw std::runtime_error("Failed to convert string to wide character format.");

    return std::wstring(buffer.begin(), buffer.end() - 1);
}

// Error Display
void WindowHandler::displayError(const std::wstring& message, HWND hWnd) {
    MessageBox(hWnd, message.c_str(), TEXT("Error"), MB_OK | MB_ICONERROR);
}

// Get input
std::wstring WindowHandler::getWindowText(HWND hWnd) {
    int length = GetWindowTextLength(hWnd);
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
    loginType.welcome = CreateWindow(TEXT("static"), TEXT("Welcome to our\nCourse Schedule\nManagement System!"), SS_CENTER | WS_CHILD , 100, 20, 600, 100, hWnd, NULL, NULL, NULL);
    loginType.admin = CreateWindow(TEXT("button"), TEXT("Admin"), WS_CHILD, 260, 160, 300, 100, hWnd, (HMENU)401, NULL, NULL);
    loginType.teacher = CreateWindow(TEXT("button"), TEXT("Teacher"), WS_CHILD, 260, 270, 300, 100, hWnd, (HMENU)402, NULL, NULL);
    loginType.student = CreateWindow(TEXT("button"), TEXT("Grade"), WS_CHILD, 260, 390, 300, 100, hWnd, (HMENU)403, NULL, NULL);

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

/* Add Course */
void WindowHandler::createAdminAddCourseWindows(HWND hWnd) {
    // Window Controls
    adminAddCourse.header = CreateWindow(TEXT("static"), TEXT("- Administrator Panel -"), WS_CHILD | ES_CENTER, 100, 80, 600, 450, hWnd, NULL, NULL, NULL);
    adminAddCourse.course = CreateWindow(TEXT("static"), TEXT("Course Name ="), WS_CHILD | ES_CENTER, 220, 150, 120, 20, hWnd, NULL, NULL, NULL);
    adminAddCourse.courseInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 150, 220, 25, hWnd, NULL, NULL, NULL);
    adminAddCourse.headerSecond = CreateWindow(TEXT("static"), TEXT("Select Classes"), WS_CHILD | ES_CENTER, 320, 250, 160, 40, hWnd, NULL, NULL, NULL);

    auto sqliteHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");

    RoomManagement allClassrooms;
    try {
        allClassrooms = sqliteHandler->getClassrooms();
    }
    catch (const std::runtime_error& e) {
        std::wstring errorMessage = L"Database error: " + std::wstring(e.what(), e.what() + strlen(e.what()));
        OutputDebugString(errorMessage.c_str());
        return;
    }

    adminAddCourse.classList = CreateWindowEx(0, WC_LISTBOX, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_MULTIPLESEL, 200, 300, 400, 100, hWnd, (HMENU)150, GetModuleHandle(NULL), NULL);

    const auto& ids = allClassrooms.getIds();
    const auto& names = allClassrooms.getNames();
    const auto& floors = allClassrooms.getFloors();
    const auto& categories = allClassrooms.getCategories();

    for (int i = 0; i < names.size(); ++i) {
        std::wstring itemText = convertToWideString(names[i]) + L" - " +
            convertToWideString(floors[i]) + L" - " +
            convertToWideString(categories[i]);

        // Add the item to the ListBox
        LRESULT index = SendMessage(adminAddCourse.classList, LB_ADDSTRING, 0, (LPARAM)itemText.c_str());

        int roomId = ids[i];
        if (roomId <= 0) {
            MessageBox(hWnd, _T("Error retrieving id for list box item."), _T("Erro: ID is invalid"),MB_ICONERROR | MB_OK); 
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

// Insert Course into Database
void WindowHandler::insertCourseIntoDatabase(HWND hWnd) {
    std::wstring courseName = getWindowText(adminAddCourse.courseInput);

    if (courseName.empty()) {
        displayError(TEXT("Course name cannot be empty!"), hWnd);
        return;
    }

    int itemCount = SendMessage(adminAddCourse.classList, LB_GETCOUNT, 0, 0);
    if (itemCount == LB_ERR) {
        displayError(TEXT("Error getting item count from listbox."), hWnd);
        return;
    }

    std::vector<int> selectedIndices;
    for (int i = 0; i < itemCount; ++i) {
        if (SendMessage(adminAddCourse.classList, LB_GETSEL, i, 0) > 0)
            selectedIndices.push_back(i);
    }

    if (selectedIndices.empty()) {
        displayError(TEXT("Please select at least one class."), hWnd);
        return;
    }
    if (selectedIndices.size() > 2) {
        displayError(TEXT("Please select no more than two classes."), hWnd);
        return;
    }

    std::vector<int> selectedRoomIds;
    for (int index : selectedIndices) {
        LRESULT itemData = SendMessage(adminAddCourse.classList, LB_GETITEMDATA, (WPARAM)index, 0);
        if (itemData == LB_ERR) {
            MessageBox(hWnd,_T("Error retrieving data for list box item."),_T("Error"),MB_ICONERROR | MB_OK);
            continue;
        }
        int roomId = static_cast<int>(itemData);
        selectedRoomIds.push_back(roomId);
    }

    if (selectedRoomIds.size() == 1)
        selectedRoomIds.push_back(selectedRoomIds[0]);

    std::string courseName_utf8(courseName.begin(), courseName.end());

    try {
        auto sqliteHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");

        std::vector<std::string> roomIdsForInsertion;
        for (int id : selectedRoomIds) 
            roomIdsForInsertion.push_back(std::to_string(id));

        if (sqliteHandler->insertCourse(courseName_utf8, roomIdsForInsertion)) 
            MessageBox(hWnd, TEXT("Course inserted successfully!"), TEXT("Success"), MB_OK | MB_ICONINFORMATION);
        else 
            displayError(TEXT("Failed to insert course."), hWnd);
    }
    catch (const std::runtime_error& e) {
        std::wstring errorMessage = L"Error: " + std::wstring(e.what(), e.what() + strlen(e.what()));
        displayError(errorMessage.c_str(), hWnd);
    }
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
    adminAddTeacher.fullnameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 150, 220, 25, hWnd, NULL, NULL, NULL);
    adminAddTeacher.course = CreateWindow(TEXT("static"), TEXT("Course ="), WS_CHILD | ES_CENTER, 230, 200, 120, 20, hWnd, NULL, NULL, NULL);
    adminAddTeacher.courseInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 200, 220, 25, hWnd, NULL, NULL, NULL);
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

// Insert Teacher into DB
void WindowHandler::insertTeacherIntoDatabase(HWND hWnd) {
    std::wstring fullname = getWindowText(adminAddTeacher.fullnameInput);
    std::wstring course = getWindowText(adminAddTeacher.courseInput);

    if (fullname.empty() || course.empty()) {
        MessageBox(NULL, TEXT("Full name and course cannot be empty!"), TEXT("Input Required"), MB_OK | MB_ICONERROR);
        return;
    }

    std::vector<HWND> workdays = {
        adminAddTeacher.monday, adminAddTeacher.tuesday, adminAddTeacher.wednesday,
        adminAddTeacher.thursday, adminAddTeacher.friday, adminAddTeacher.saturday
    };

    bool workdaySelected = std::any_of(workdays.begin(), workdays.end(), [hWnd](HWND checkboxHwnd) {
        return SendMessage(checkboxHwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
        });

    if (!workdaySelected) {
        MessageBox(NULL, TEXT("At least one workday must be selected!"), TEXT("Selection Required"), MB_OK | MB_ICONERROR);
        return;
    }

    std::string fullname_utf8(fullname.begin(), fullname.end());
    std::string course_utf8(course.begin(), course.end());

    std::vector<int> workdayValues;
    for (HWND checkboxHwnd : workdays) {
        int value = SendMessage(checkboxHwnd, BM_GETCHECK, 0, 0) == BST_CHECKED ? 1 : 0;
        workdayValues.push_back(value);
    }

    try {
        auto sqliteHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");

        if (sqliteHandler->insertTeacher(fullname_utf8, course_utf8,
            workdayValues[0], workdayValues[1], workdayValues[2],
            workdayValues[3], workdayValues[4], workdayValues[5])) {
            MessageBox(NULL, TEXT("Data inserted successfully!"), TEXT("Success"), MB_OK | MB_ICONINFORMATION);
        }
        else {
            MessageBox(NULL, TEXT("Failed to insert data."), TEXT("Error"), MB_OK | MB_ICONERROR);
        }
    }
    catch (const std::runtime_error& e) {
        std::wstring errorMessage = L"Error: " + std::wstring(e.what(), e.what() + strlen(e.what()));
        MessageBox(hWnd, errorMessage.c_str(), TEXT("Database Error"), MB_OK | MB_ICONERROR);
    }
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

    auto sqliteHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");

    RoomManagement allClassrooms;
    try {
        allClassrooms = sqliteHandler->getClassrooms();
    }
    catch (const std::runtime_error& e) {
        std::wstring errorMessage = L"Error: " + std::wstring(e.what(), e.what() + strlen(e.what()));
        MessageBox(hWnd, errorMessage.c_str(), TEXT("Database Error"), MB_OK | MB_ICONERROR);
    }

    const auto& names = allClassrooms.getNames();

    for (const auto& name : names) {
        std::wstring wideName = convertToWideString(name);
        SendMessage(adminManageRoom.classList, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
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
    adminAddRoom.nameInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 150, 220, 25, hWnd, NULL, NULL, NULL);
    adminAddRoom.floor = CreateWindow(TEXT("static"), TEXT("Floor ="), WS_CHILD | ES_CENTER, 245, 200, 120, 20, hWnd, NULL, NULL, NULL);
    adminAddRoom.floorInput = CreateWindow(TEXT("EDIT"), TEXT(""), WS_BORDER | WS_CHILD, 350, 200, 220, 25, hWnd, NULL, NULL, NULL);
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

// Insert Room into DB
void WindowHandler::insertRoomIntoDatabase(HWND hWnd) {
    std::wstring roomName = getWindowText(adminAddRoom.nameInput);
    std::wstring roomFloor = getWindowText(adminAddRoom.floorInput);

    if (roomName.empty() || roomFloor.empty()) {
        MessageBox(NULL, TEXT("Room Name and floor cannot be empty!"), TEXT("Input Required"), MB_OK | MB_ICONWARNING);
        return;
    }

    int idx = SendMessage(adminAddRoom.category, CB_GETCURSEL, 0, 0);
    if (idx == CB_ERR) {
        MessageBox(NULL, TEXT("Please select a category."), TEXT("Selection Required"), MB_OK | MB_ICONWARNING);
        return;
    }

    std::wstring roomCategory = getWindowText(adminAddRoom.category);

    std::string roomName_utf8(roomName.begin(), roomName.end());
    std::string roomFloor_utf8(roomFloor.begin(), roomFloor.end());
    std::string roomCategory_utf8(roomCategory.begin(), roomCategory.end());

    try {
        auto sqliteHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");

        if (sqliteHandler->insertRoom(roomName_utf8, roomFloor_utf8, roomCategory_utf8)) {
            MessageBox(hWnd, TEXT("Room inserted successfully!"), TEXT("Success"), MB_OK | MB_ICONINFORMATION);
        }
        else {
            MessageBox(NULL, TEXT("Failed to insert data."), TEXT("Error"), MB_OK | MB_ICONERROR);
        }
    }
    catch (const std::runtime_error& e) {
        std::wstring errorMessage = L"Error: " + std::wstring(e.what(), e.what() + strlen(e.what()));
        MessageBox(hWnd, errorMessage.c_str(), TEXT("Database Error"), MB_OK | MB_ICONERROR);
    }
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