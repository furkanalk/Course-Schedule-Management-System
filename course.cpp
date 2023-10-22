#include "course.h"
#include "handlers.h"

// Setters
void courseSystem::setName(std::string username) { this->username = username; }
void courseSystem::setGender(std::string gender) { this->gender = gender; }
void courseSystem::setId(int id) { this->id = id; }
void courseSystem::setAge(int age) { this->age = age; }

// Getters
std::string courseSystem::getUsername() { return username; }
std::string courseSystem::getGender() { return gender; }
int courseSystem::getId() { return id; }
int courseSystem::getAge() { return age; }

std::wstring courseSystem::convertToWideString(const std::string& str) {
	int bufferLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	std::wstring wstr(bufferLength, L'\0');

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], bufferLength);
	return wstr;
}

/* Main wnd */
void courseSystem::login(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	/* Hide other windows */
	// Login Types
	wh.setAdminLoginVisibility(false);
	wh.setTeacherLoginVisibility(false);
	wh.setStudentLoginVisibility(false);
	// Admin
	wh.setAdminInterfaceVisibility(false);
	wh.setAdminCourseManagementVisibility(false);
	wh.setAdminTeacherManagementVisibility(false);

	// SHOW Login Type	
	if (!wh.isWindowCreated("loginType")) {
		wh.createLoginTypeWindows(hWnd);
	}
	wh.setLoginTypeVisibility(true);
}

// Admin related windows

/* Admin Login wnd */
void Admin::login(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// HIDE Login Type
	wh.setLoginTypeVisibility(false);

	// SHOW Admin Login
	if (!wh.isWindowCreated("adminLogin")) {
		wh.createAdminLoginWindows(hWnd);
	}
	wh.setAdminLoginVisibility(true);
}

/* Admin main menu */
void Admin::showInterface(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// Hide other Admin windows
	wh.setAdminLoginVisibility(false);
	wh.setAdminCourseManagementVisibility(false);
	wh.setAdminTeacherManagementVisibility(false);

	// Show Admin Inferface
	if (!wh.isWindowCreated("adminInterface")) {
		wh.createAdminInterfaceWindows(hWnd);
	}
	wh.setAdminInterfaceVisibility(true);
}

/* Admin course management */
void Admin::courseManagement(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// Hide Admin Interface
	wh.setAdminInterfaceVisibility(false);

	// Show Admin Inferface
	if (!wh.isWindowCreated("adminCourseManagement")) {
		wh.createAdminCourseManagementWindows(hWnd);
	}
	wh.setAdminCourseManagementVisibility(true);
}

/* Admin teacher management */
void Admin::teacherManagement(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// Hide other Admin windows
	wh.setAdminInterfaceVisibility(false);
	wh.setAdminAddTeacherVisibility(false);

	// Show Admin Inferface
	if (!wh.isWindowCreated("adminTeacherManagement")) {
		wh.createAdminTeacherManagementWindows(hWnd);
	}
	wh.setAdminTeacherManagementVisibility(true);
}

/* Admin add teacher */
void Admin::teacherAdd(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// Hide Teacher Management
	wh.setAdminTeacherManagementVisibility(false);

	// Show Add Teacher
	if (!wh.isWindowCreated("adminAddTeacher")) {
		wh.createAdminAddTeacherWindows(hWnd);
	}
	wh.setAdminAddTeacherVisibility(true);
}

/* Admin add teacher */
void Admin::teacherInsertToDB(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// Add teacher to DB
	wh.insertTeacherIntoDatabase(hWnd);
}

/* Teacher Login wnd */
void Teacher::login(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// HIDE Login Type
	wh.setLoginTypeVisibility(false);

	// SHOW Teacher Login
	if (!wh.isWindowCreated("teacherLogin")) {
		wh.createTeacherLoginWindows(hWnd);
	}
	wh.setTeacherLoginVisibility(true);
}

/* Student Login wnd */
void Student::login(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// HIDE Login Type
	wh.setLoginTypeVisibility(false);

	// SHOW Student Login
	if (!wh.isWindowCreated("studentLogin")) {
		wh.createStudentLoginWindows(hWnd);
	}
	wh.setStudentLoginVisibility(true);
}
