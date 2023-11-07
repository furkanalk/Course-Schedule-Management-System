#include "course.h"
#include "handlers.h"

//std::wstring User::convertToWideString(const std::string& str) {
//	int bufferLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
//
//	if (bufferLength == 0)
//		throw std::runtime_error("Failed to convert string to wide character format.");
//
//	std::vector<wchar_t> buffer(bufferLength);
//	int conversionResult = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, buffer.data(), bufferLength);
//
//	if (conversionResult == 0)
//		throw std::runtime_error("Failed to convert string to wide character format.");
//
//	return std::wstring(buffer.begin(), buffer.end() - 1);
//}

/* Main wnd */
void User::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	/* Hide other windows */
	// Login Types
	wh->setAdminLoginVisibility(false);
	wh->setTeacherLoginVisibility(false);
	wh->setStudentLoginVisibility(false);
	// Admin
	wh->setAdminInterfaceVisibility(false);
	wh->setAdminCourseManagementVisibility(false);
	wh->setAdminTeacherManagementVisibility(false);

	// SHOW Login Type	
	if (!wh->isWindowCreated("loginType")) {
		wh->createLoginTypeWindows(hWnd);
	}
	wh->setLoginTypeVisibility(true);
}

void User::manageData(HWND hWnd) {
	// Will be filled with teachers' and users' data
}

// Admin related windows

/* Admin Login wnd Wnd */
void Admin::login(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// HIDE Login Type
	wh->setLoginTypeVisibility(false);

	// SHOW Admin Login
	if (!wh->isWindowCreated("adminLogin")) {
		wh->createAdminLoginWindows(hWnd);
	}
	wh->setAdminLoginVisibility(true);
}

/* Admin interface Wnd */
void Admin::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminLoginVisibility(false);
	wh->setAdminCourseManagementVisibility(false);
	wh->setAdminTeacherManagementVisibility(false);
	wh->setAdminRoomManagementVisibility(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminInterface")) {
		wh->createAdminInterfaceWindows(hWnd);
	}
	wh->setAdminInterfaceVisibility(true);
}

void Admin::addData(HWND hWnd) {
	// Will be filled with admin data
}

void Admin::insertToDB(HWND hWnd) {
	// Will be filled with admin data
}

/* Admin course management Wnd */
void CourseManagement::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminInterfaceVisibility(false);
	wh->setAdminAddCourseVisibility(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminCourseManagement")) {
		wh->createAdminCourseManagementWindows(hWnd);
	}
	wh->setAdminCourseManagementVisibility(true);
}

/* Admin add course Wnd */
void CourseManagement::addData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Course Management
	wh->setAdminCourseManagementVisibility(false);

	// Show Add Course
	if (!wh->isWindowCreated("adminAddCourse")) {
		wh->createAdminAddCourseWindows(hWnd);
	}
	wh->setAdminAddCourseVisibility(true);
}

/* Admin insert course to DB */
void CourseManagement::insertToDB(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Add course to DB
	wh->insertCourseIntoDatabase(hWnd);
}

/* Admin teacher management Wnd */
void TeacherManagement::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminInterfaceVisibility(false);
	wh->setAdminAddTeacherVisibility(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminTeacherManagement")) {
		wh->createAdminTeacherManagementWindows(hWnd);
	}
	wh->setAdminTeacherManagementVisibility(true);
}

/* Admin add teacher Wnd */
void TeacherManagement::addData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Teacher Management
	wh->setAdminTeacherManagementVisibility(false);

	// Show Add Teacher
	if (!wh->isWindowCreated("adminAddTeacher")) {
		wh->createAdminAddTeacherWindows(hWnd);
	}
	wh->setAdminAddTeacherVisibility(true);
}

/* Admin insert teacher into DB */
void TeacherManagement::insertToDB(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Add teacher into DB
	wh->insertTeacherIntoDatabase(hWnd);
}

/* Admin room management Wnd */
void RoomManagement::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminInterfaceVisibility(false);
	wh->setAdminManageRoomVisibility(false);
	wh->setAdminAddRoomVisibility(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminRoomManagement")) {
		wh->createAdminRoomManagementWindows(hWnd);
	}
	wh->setAdminRoomManagementVisibility(true);
}

/* Admin manage rooms Wnd */
void RoomManagement::manageData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminRoomManagementVisibility(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminManageRoom")) {
		wh->createAdminManageRoomWindows(hWnd);
	}
	wh->setAdminManageRoomVisibility(true);
}

/* Admin add clasroom Wnd */
void RoomManagement::addData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Teacher Management
	wh->setAdminRoomManagementVisibility(false);

	// Show Add Teacher
	if (!wh->isWindowCreated("adminAddRoom")) {
		wh->createAdminAddRoomWindows(hWnd);
	}
	wh->setAdminAddRoomVisibility(true);
}

/* Admin insert classroom into DB */
void RoomManagement::insertToDB(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Add room into DB
	wh->insertRoomIntoDatabase(hWnd);

}
/* Teacher interface Wnd */
void Teacher::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// HIDE Login Type
	wh->setLoginTypeVisibility(false);

	// SHOW Teacher Login
	if (!wh->isWindowCreated("teacherLogin")) {
		wh->createTeacherLoginWindows(hWnd);
	}
	wh->setTeacherLoginVisibility(true);
}

/* Student interface Wnd */
void Student::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// HIDE Login Type
	wh->setLoginTypeVisibility(false);

	// SHOW Student Login
	if (!wh->isWindowCreated("studentLogin")) {
		wh->createStudentLoginWindows(hWnd);
	}
	wh->setStudentLoginVisibility(true);
}
