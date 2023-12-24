#include "user.h"
#include "handlers.h"
#include "SQLiteHandler.h"
#include <algorithm>

int User::getCurrentId() {
	return currentId;
}

void User::setCurrentId(int id) {
	currentId = id;
}

int User::getCurrentIndex() {
	return currentIndex;
}

void User::setCurrentIndex(int index) {
	currentIndex = index;
}

void User::addId(int id) {
	ids.push_back(id);
}

std::vector<int> User::getIds() {
	return ids;
}

void User::setName(int index, std::string newName) {
	if (index < names.size()) {
		names[index] = newName;
	}
}

void User::addName(std::string name) {
	names.push_back(name);
}

std::vector<std::string> User::getNames() {
	return names;
}

/* Main wnd */
void User::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	/* Hide other windows */
	// Login Types
	wh->setAdminLoginVisible(false);
	wh->setTeacherLoginVisible(false);
	wh->setStudentLoginVisible(false);
	// Admin
	wh->setAdminInterfaceVisible(false);
	wh->setAdminCourseManagementVisible(false);
	wh->setAdminTeacherManagementVisible(false);

	// Show Login Type	
	if (!wh->isWindowCreated("loginType")) {
		wh->createLoginTypeWindows(hWnd);
	}
	wh->setLoginTypeVisible();
}

void User::manageData(HWND hWnd) {
	// Will be filled with teachers' and users' data
}

// Admin related windows

/* Admin Login wnd Wnd */
void User::login(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Login Type
	wh->setLoginTypeVisible(false);

	// Show Admin Login
	if (!wh->isWindowCreated("adminLogin")) {
		wh->createAdminLoginWindows(hWnd);
	}
	wh->setAdminLoginVisible();
}

/* Admin interface Wnd */
void Admin::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminLoginVisible(false);
	wh->setAdminCourseManagementVisible(false);
	wh->setAdminTeacherManagementVisible(false);
	wh->setAdminRoomManagementVisible(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminInterface")) {
		wh->createAdminInterfaceWindows(hWnd);
	}
	wh->setAdminInterfaceVisible();
}

void User::addData(HWND hWnd) {
	// Will be filled with user data
}

void User::insertToDB(HWND hWnd) {
	// Will be filled with user data
}

/* Teacher interface Wnd */
void Teacher::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Login Type
	wh->setLoginTypeVisible(false);

	// Show Teacher Login
	if (!wh->isWindowCreated("teacherLogin")) {
		wh->createTeacherLoginWindows(hWnd);
	}
	wh->setTeacherLoginVisible();
}

/* Student interface Wnd */
void Student::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Login Type
	wh->setLoginTypeVisible(false);

	// Show Student Login
	if (!wh->isWindowCreated("studentLogin")) {
		wh->createStudentLoginWindows(hWnd);
	}
	wh->setStudentLoginVisible();
}