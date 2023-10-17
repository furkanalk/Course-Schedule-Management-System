#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H
#include <windows.h>
#include <string>
#include <map>

class WindowHandler {
    std::map<std::string, bool> windowCreationStatus;

    struct LoginType {
        // Window Handlers
        HWND welcome;
        HWND admin;
        HWND teacher;
        HWND student;

        // Set all windows to NULL by default
        LoginType() : welcome(NULL), admin(NULL), teacher(NULL), student(NULL) {}
    } loginType;

    struct AdminLogin {
        // Window Handlers
        HWND header;
        HWND username;
        HWND usernameInput;
        HWND password;
        HWND passwordInput;
        HWND previous;
        HWND proceed;

        // Set all windows to NULL by default
        AdminLogin() : header(NULL), username(NULL), usernameInput(NULL), password(NULL), passwordInput(NULL), previous(NULL), proceed(NULL) {}
    } adminLogin;

    struct TeacherLogin {
        // Window Handlers
        HWND header;
        HWND username;
        HWND usernameInput;
        HWND password;
        HWND passwordInput;
        HWND previous;
        HWND proceed;

        // Set all windows to NULL by default
        TeacherLogin() : header(NULL), username(NULL), usernameInput(NULL), password(NULL), passwordInput(NULL), previous(NULL), proceed(NULL) {}
    } teacherLogin;

    struct StudentLogin {
        // Window Handlers
        HWND header;
        HWND username;
        HWND usernameInput;
        HWND password;
        HWND passwordInput;
        HWND previous;
        HWND proceed;

        // Set all windows to NULL by default
        StudentLogin() : header(NULL), username(NULL), usernameInput(NULL), password(NULL), passwordInput(NULL), previous(NULL), proceed(NULL) {}
    } studentLogin;

public:
    WindowHandler();

    // Function to set the status when a window is created.
    void setWindowCreated(const std::string& windowName, bool isCreated);

    // Function to check if a window is created.
    bool isWindowCreated(const std::string& windowName);

    // Login Type
    void createLoginTypeWindows(HWND hWnd);
    void setLoginTypeVisibility(bool isVisible);

    // Admin Login
    void createAdminLoginWindows(HWND hWnd);
    void setAdminLoginVisibility(bool isVisible);

    // Teacher Login
    void createTeacherLoginWindows(HWND hWnd);
    void setTeacherLoginVisibility(bool isVisible);

    // Student Login
    void createStudentLoginWindows(HWND hWnd);
    void setStudentLoginVisibility(bool isVisible);
};

#endif