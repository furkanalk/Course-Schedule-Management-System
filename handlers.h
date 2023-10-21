#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H
#include <windows.h>
#include <string>
#include <map>

class WindowHandler {
    WindowHandler();

    std::map<std::string, bool> windowCreationStatus;

    /* Main Menu */
    struct LoginType {
        // Window Handlers
        HWND welcome;
        HWND admin;
        HWND teacher;
        HWND student;

        // Set all windows to NULL by default
        LoginType() : welcome(nullptr), admin(nullptr), teacher(nullptr), student(nullptr) {}
    } loginType;

    /* Admin related handlers */
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
        AdminLogin() : header(nullptr), username(nullptr), usernameInput(nullptr), password(nullptr), passwordInput(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminLogin;

    struct AdminInterface {
        // Window Handlers
        HWND header;
        HWND courseManagement;
        HWND teachers;
        HWND students;
        HWND previous;
        HWND proceed;

        // Set all windows to NULL by default
        AdminInterface() : header(nullptr), courseManagement(nullptr), teachers(nullptr), students(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminInterface;

    struct AdminCourseManagement {
        // Window Handlers
        HWND header;
        HWND syllabus;
        HWND showCourses;
        HWND addCourse;
        HWND previous;
        HWND proceed;

        // Set all windows to NULL by default
        AdminCourseManagement() : header(nullptr), syllabus(nullptr), showCourses(nullptr), addCourse(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminCourseManagement;

    struct AdminTeacherManagement {
        // Window Handlers
        HWND header;
        HWND showTeachers;
        HWND addTeacher;
        HWND previous;
        HWND proceed;

        // Set all windows to NULL by default
        AdminTeacherManagement() : header(nullptr), showTeachers(nullptr), addTeacher(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminTeacherManagement;

    /* Teacher related handlers */
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
        TeacherLogin() : header(nullptr), username(nullptr), usernameInput(nullptr), password(nullptr), passwordInput(nullptr), previous(nullptr), proceed(nullptr) {}
    } teacherLogin;

    /* Student related handlers */
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
        StudentLogin() : header(nullptr), username(nullptr), usernameInput(nullptr), password(nullptr), passwordInput(nullptr), previous(nullptr), proceed(nullptr) {}
    } studentLogin;

public:
    // Singleton
    WindowHandler(const WindowHandler&) = delete;
    WindowHandler& operator=(const WindowHandler&) = delete;

    // Create an instance of the object
    static WindowHandler& getInstance() {
        static WindowHandler instance;
        return instance;
    }

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

    // Admin Interface
    void createAdminInterfaceWindows(HWND hWnd);
    void setAdminInterfaceVisibility(bool isVisible);

    // Admin Course Management
    void createAdminCourseManagementWindows(HWND hWnd);
    void setAdminCourseManagementVisibility(bool isVisible);

    // Admin Course Management
    void createAdminTeacherManagementWindows(HWND hWnd);
    void setAdminTeacherManagementVisibility(bool isVisible);

    // Teacher Login
    void createTeacherLoginWindows(HWND hWnd);
    void setTeacherLoginVisibility(bool isVisible);

    // Student Login
    void createStudentLoginWindows(HWND hWnd);
    void setStudentLoginVisibility(bool isVisible);
};

#endif