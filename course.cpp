#include "courseSchedule.h"
#include "handlers.h"

// Setters
void courseSystem::setName(std::string username) { this->username = username; }
void courseSystem::setGender(std::string gender) { this->gender = gender; }
void courseSystem::setId(std::string id) { this->id = id; }
void courseSystem::setAge(std::string age) { this->age = age; }

// Getters
std::string courseSystem::getUsername() { return username; }
std::string courseSystem::getGender() { return gender; }
std::string courseSystem::getId() { return id; }
std::string courseSystem::getAge() { return age; }

std::wstring courseSystem::convertToWideString(const std::string& str) {
	int bufferLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	std::wstring wstr(bufferLength, L'\0');

	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], bufferLength);
	return wstr;
}

/* Main wnd */
void courseSystem::login(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// Hide other windows
	wh.setAdminLoginVisibility(false);
	wh.setTeacherLoginVisibility(false);
	wh.setStudentLoginVisibility(false);
	wh.setAdminInterfaceVisibility(false);

	sqlite3* DB;
	std::string url = "courseScheduleDB.sqlite";

	int exit = 0;
	exit = sqlite3_open(url.c_str(), &DB);

	if (exit != SQLITE_OK) {
		std::string errorMsg = "Error opening database: ";
		errorMsg += sqlite3_errmsg(DB);
		std::wstring wideErrorMsg = convertToWideString(errorMsg);

		MessageBox(
			NULL,
			wideErrorMsg.c_str(),
			L"Database Error", 
			MB_ICONERROR | MB_OK 
		);

		sqlite3_close(DB);
	}
	else {
		// Database opened
	}

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

	// Hide Admin Login
	wh.setAdminLoginVisibility(false);

	// Show Admin Inferface
	if (!wh.isWindowCreated("adminInterface")) {
		wh.createAdminInterfaceWindows(hWnd);
	}
	wh.setAdminInterfaceVisibility(true);
}

/* Admin course management */
void Admin::courseManagement(HWND hWnd) {
	WindowHandler& wh = wh.getInstance();

	// Hide Admin Login
	wh.setAdminInterfaceVisibility(false);

	// Show Admin Inferface
	if (!wh.isWindowCreated("adminCourseManagement")) {
		wh.createAdminCourseManagementWindows(hWnd);
	}
	wh.setAdminCourseManagementVisibility(true);
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
