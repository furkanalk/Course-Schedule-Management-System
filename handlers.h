#ifndef WINDOWHANDLER_H
#define WINDOWHANDLER_H

#include <windows.h>
#include <commctrl.h>
#include <string>
#include <vector>
#include <map>

class WindowHandler {
    static WindowHandler wh;
    std::map<std::string, bool> windowCreationStatus;

    /* Main Menu */
    struct LoginType {
        HWND welcome;
        HWND admin;
        HWND teacher;
        HWND student;

        LoginType() : welcome(nullptr), admin(nullptr), teacher(nullptr), student(nullptr) {}
    } loginType;

    /* Admin related handlers */
    struct AdminLogin {
        HWND header;
        HWND username;
        HWND usernameInput;
        HWND password;
        HWND passwordInput;
        HWND previous;
        HWND proceed;

        AdminLogin() : header(nullptr), username(nullptr), usernameInput(nullptr), password(nullptr), passwordInput(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminLogin;

    struct AdminInterface {
        HWND header;
        HWND courseManagement;
        HWND teachers;
        HWND students;
        HWND previous;
        HWND proceed;

        AdminInterface() : header(nullptr), courseManagement(nullptr), teachers(nullptr), students(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminInterface;

    struct AdminCourseScheduling {
        HWND header;
        HWND mondayHeader;
        HWND monday;
        HWND tuesdayHeader;
        HWND tuesday;
        HWND wednesdayHeader;
        HWND wednesday;
        HWND thursdayHeader;
        HWND thursday;
        HWND fridayHeader;
        HWND friday;
        HWND saturdayHeader;
        HWND saturday;
        HWND previous;

        AdminCourseScheduling() : header(nullptr), monday(nullptr), tuesday(nullptr), wednesday(nullptr), thursday(nullptr), friday(nullptr), saturday(nullptr),
            mondayHeader(nullptr), tuesdayHeader(nullptr), wednesdayHeader(nullptr), thursdayHeader(nullptr), fridayHeader(nullptr), saturdayHeader(nullptr), previous(nullptr) {}
    } adminCourseScheduling;

    struct AdminCourseManagement {
        HWND header;
        HWND syllabus;
        HWND showCourses;
        HWND addCourse;
        HWND previous;
        HWND proceed;

        AdminCourseManagement() : header(nullptr), syllabus(nullptr), showCourses(nullptr), addCourse(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminCourseManagement;

    struct AdminManageCourse {
        HWND header;
        HWND courseList;
        HWND fetch;
        HWND deleteCourse;
        HWND courseName;
        HWND courseRoomFirst;
        HWND courseRoomSecond;
        HWND courseRoomF;
        HWND courseRoomS;
        HWND previous;
        HWND update;

        AdminManageCourse() : header(nullptr), courseList(nullptr), fetch(nullptr), deleteCourse(nullptr), courseName(nullptr), courseRoomFirst(nullptr), courseRoomSecond(nullptr),
            courseRoomF(nullptr), courseRoomS(nullptr), previous(nullptr), update(nullptr) {}
    } adminManageCourse;

    struct AdminAddCourse {
        HWND header;
        HWND course;
        HWND courseInput;
        HWND headerSecond;
        HWND classList;
        HWND previous;
        HWND insert;

        AdminAddCourse() : header(nullptr), course(nullptr), courseInput(nullptr), headerSecond(nullptr), classList(nullptr), previous(nullptr), insert(nullptr) {}
    } adminAddCourse;

    struct AdminTeacherManagement {
        HWND header;
        HWND showTeachers;
        HWND addTeacher;
        HWND previous;
        HWND proceed;

        AdminTeacherManagement() : header(nullptr), showTeachers(nullptr), addTeacher(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminTeacherManagement;

    struct AdminManageTeacher {
        HWND header;
        HWND teacherList;
        HWND fetch;
        HWND deleteTeacher;
        HWND teacherName;
        HWND mondayStatic;
        HWND mondayBool;
        HWND tuesdayStatic;
        HWND tuesdayBool;
        HWND wednesdayStatic;
        HWND wednesdayBool;
        HWND thursdayStatic;
        HWND thursdayBool;
        HWND fridayStatic;
        HWND fridayBool;
        HWND saturdayStatic;
        HWND saturdayBool;
        HWND previous;
        HWND update;

        AdminManageTeacher() : header(nullptr), teacherList(nullptr), fetch(nullptr), deleteTeacher(nullptr), teacherName(nullptr), mondayStatic(nullptr), mondayBool(nullptr),
            tuesdayStatic(nullptr), tuesdayBool(nullptr), wednesdayStatic(nullptr), wednesdayBool(nullptr), thursdayStatic(nullptr), thursdayBool(nullptr), fridayStatic(nullptr), fridayBool(nullptr),
            saturdayStatic(nullptr), saturdayBool(nullptr), previous(nullptr), update(nullptr) {}
    } adminManageTeacher;

    struct AdminAddTeacher {
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

        AdminAddTeacher() : header(nullptr), fullname(nullptr), fullnameInput(nullptr), course(nullptr), courseInput(nullptr), headerSecond(nullptr), monday(nullptr), tuesday(nullptr),
            wednesday(nullptr), thursday(nullptr), friday(nullptr), saturday(nullptr), previous(nullptr), insert(nullptr) {}
    } adminAddTeacher;

    struct AdminRoomManagement {
        HWND header;
        HWND showRooms;
        HWND addRoom;
        HWND previous;
        HWND proceed;

        AdminRoomManagement() : header(nullptr), showRooms(nullptr), addRoom(nullptr), previous(nullptr), proceed(nullptr) {}
    } adminRoomManagement;

    struct AdminManageRoom {
        HWND header;
        HWND classList;
        HWND fetch;
        HWND deleteClass;
        HWND className;
        HWND classFloor;
        HWND classCategory;
        HWND previous;
        HWND update;

        AdminManageRoom() : header(nullptr), classList(nullptr), fetch(nullptr), deleteClass(nullptr), className(nullptr), classFloor(nullptr), classCategory(nullptr), previous(nullptr), update(nullptr) {}
    } adminManageRoom;

    struct AdminAddRoom {
        HWND header;
        HWND name;
        HWND nameInput;
        HWND floor;
        HWND floorInput;
        HWND headerSecond;
        HWND category;
        HWND previous;
        HWND insert;

        AdminAddRoom() : header(nullptr), name(nullptr), nameInput(nullptr), floor(nullptr), floorInput(nullptr), headerSecond(nullptr), category(nullptr), previous(nullptr), insert(nullptr) {}
    } adminAddRoom;

    /* Teacher related handlers */
    struct TeacherLogin {
        HWND header;
        HWND username;
        HWND usernameInput;
        HWND password;
        HWND passwordInput;
        HWND previous;
        HWND proceed;

        TeacherLogin() : header(nullptr), username(nullptr), usernameInput(nullptr), password(nullptr), passwordInput(nullptr), previous(nullptr), proceed(nullptr) {}
    } teacherLogin;

    /* Student related handlers */
    struct StudentLogin {
        HWND header;
        HWND username;
        HWND usernameInput;
        HWND password;
        HWND passwordInput;
        HWND previous;
        HWND proceed;

        StudentLogin() : header(nullptr), username(nullptr), usernameInput(nullptr), password(nullptr), passwordInput(nullptr), previous(nullptr), proceed(nullptr) {}
    } studentLogin;

public:
    WindowHandler();

    static WindowHandler* getInstance();
    std::wstring convertToWideString(std::string str);
    bool isWindowCreated(std::string windowName);

    std::wstring getComboBoxSelectedText(HWND comboBox);
    std::wstring getWindowText(HWND hWnd);

    // Login Type
    void createLoginTypeWindows(HWND hWnd);
    void setLoginTypeVisible(bool isVisible = true);

    // Admin Login
    void createAdminLoginWindows(HWND hWnd);
    void setAdminLoginVisible(bool isVisible = true);

    // Admin Interface
    void createAdminInterfaceWindows(HWND hWnd);
    void setAdminInterfaceVisible(bool isVisible = true);

    // Admin Create Course Scheduling
    void createAdminCourseSchedulingWindows(HWND hWnd);
    void setAdminCourseSchedulingVisible(bool isVisible = true);

    // Admin Course Management
    void createAdminCourseManagementWindows(HWND hWnd);
    void setAdminCourseManagementVisible(bool isVisible = true);

    // Admin Manage Course
    void createAdminManageCourseWindows(HWND hWnd);
    void setAdminManageCourseVisible(bool isVisible = true);

    // Admin Add Course
    void createAdminAddCourseWindows(HWND hWnd);
    void setAdminAddCourseVisible(bool isVisible = true);

    // Admin Teacher Management
    void createAdminTeacherManagementWindows(HWND hWnd);
    void setAdminTeacherManagementVisible(bool isVisible = true);

    // Admin Manage Teacher
    void createAdminManageTeacherWindows(HWND hWnd);
    void setAdminManageTeacherVisible(bool isVisible = true);

    // Admin Add Teacher
    void createAdminAddTeacherWindows(HWND hWnd);
    void setAdminAddTeacherVisible(bool isVisible = true);

    // Admin Room Management
    void createAdminRoomManagementWindows(HWND hWnd);
    void setAdminRoomManagementVisible(bool isVisible = true);

    // Admin Manage Room
    void createAdminManageRoomWindows(HWND hWnd);
    void setAdminManageRoomVisible(bool isVisible = true);

    // Admin Add Room
    void createAdminAddRoomWindows(HWND hWnd);
    void setAdminAddRoomVisible(bool isVisible = true);

    // Teacher Login
    void createTeacherLoginWindows(HWND hWnd);
    void setTeacherLoginVisible(bool isVisible = true);

    // Student Login
    void createStudentLoginWindows(HWND hWnd);
    void setStudentLoginVisible(bool isVisible = true);
};

#endif