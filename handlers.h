#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
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

    struct AdminAddCourse {
        // Window Handlers
        HWND header;
        HWND course;
        HWND courseInput;
        HWND headerSecond;
        HWND classList;
        HWND previous;
        HWND insert;

        // Set all windows to NULL by default
        AdminAddCourse() : header(nullptr), course(nullptr), courseInput(nullptr), headerSecond(nullptr), classList(nullptr), previous(nullptr), insert(nullptr) {}
    } adminAddCourse;

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

    struct AdminAddTeacher {
        // Window Handlers
        HWND header;
        HWND fullname;
        HWND fullnameInput;
        HWND course;
        HWND courseInput;
        HWND headerSecond;
        HWND monday;
        HWND tuesday;
        HWND wednesday;
        HWND thursday;
        HWND friday;
        HWND saturday;
        HWND previous;
        HWND insert;

        // Set all windows to NULL by default
        AdminAddTeacher() : header(nullptr), fullname(nullptr), fullnameInput(nullptr), course(nullptr) , courseInput(nullptr), headerSecond(nullptr), monday(nullptr), tuesday(nullptr),
            wednesday(nullptr), thursday(nullptr), friday(nullptr), saturday(nullptr), previous(nullptr), insert(nullptr) {}
    } adminAddTeacher;

    struct AdminRoomManagement {
        // Window Handlers
        HWND header;
        HWND showRooms;
        HWND addRoom;
        HWND previous;
        HWND proceed;

        // Set all windows to NULL by default
        AdminRoomManagement() : header(nullptr), showRooms(nullptr), addRoom(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminRoomManagement;

    struct AdminManageRoom {
        // Window Handlers
        HWND header;
        HWND classList;
        HWND fetch;
        HWND deleteClass;
        HWND className;
        HWND classFloor;
        HWND classCategory;
        HWND previous;
        HWND update;

        // Set all windows to NULL by default
        AdminManageRoom() : header(nullptr), classList(nullptr), fetch(nullptr), deleteClass(nullptr), className(nullptr), classFloor(nullptr), classCategory(nullptr), previous(nullptr), update(nullptr) {}
    } adminManageRoom;

    struct AdminAddRoom {
        // Window Handlers
        HWND header;
        HWND name;
        HWND nameInput;
        HWND floor;
        HWND floorInput;
        HWND headerSecond;
        HWND category;
        HWND previous;
        HWND insert;

        // Set all windows to NULL by default
        AdminAddRoom() : header(nullptr), name(nullptr), nameInput(nullptr), floor(nullptr), floorInput(nullptr), headerSecond(nullptr), category(nullptr), previous(nullptr), insert(nullptr) {}
    } adminAddRoom;

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

    /* Convert to Wide String */
    std::wstring convertToWideString(const std::string& str);

    // Set the status of a window when it is created.
    void setWindowCreated(const std::string& windowName, bool isCreated);

    // Check if a window is created.
    bool isWindowCreated(const std::string& windowName);

    // Show error
    void displayError(const std::wstring& message, HWND hWnd);
    
    // Get input
    std::wstring getWindowText(HWND hWnd);

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

    // Admin Add Course
    void createAdminAddCourseWindows(HWND hWnd);
    void setAdminAddCourseVisibility(bool isVisible);
    void insertCourseIntoDatabase(HWND hWnd);

    // Admin Teacher Management
    void createAdminTeacherManagementWindows(HWND hWnd);
    void setAdminTeacherManagementVisibility(bool isVisible);

    // Admin Add Teacher
    void createAdminAddTeacherWindows(HWND hWnd);
    void setAdminAddTeacherVisibility(bool isVisible);
    void insertTeacherIntoDatabase(HWND hWnd);

    // Admin Room Management
    void createAdminRoomManagementWindows(HWND hWnd);
    void setAdminRoomManagementVisibility(bool isVisible);

    // Admin Manage Room
    void createAdminManageRoomWindows(HWND hWnd);
    void setAdminManageRoomVisibility(bool isVisible);

    // Admin Add Room
    void createAdminAddRoomWindows(HWND hWnd);
    void setAdminAddRoomVisibility(bool isVisible);
    void insertRoomIntoDatabase(HWND hWnd);

    // Teacher Login
    void createTeacherLoginWindows(HWND hWnd);
    void setTeacherLoginVisibility(bool isVisible);

    // Student Login
    void createStudentLoginWindows(HWND hWnd);
    void setStudentLoginVisibility(bool isVisible);
};

#endif