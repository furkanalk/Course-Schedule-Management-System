#include "CourseManagement.h"
#include "handlers.h"
#include "SQLiteHandler.h"

void CourseManagement::setRooms(int index, std::vector<std::string> newRooms) {
	if (index < rooms.size()) {
		rooms[index] = newRooms;
	}
}

void CourseManagement::addRooms(std::vector<std::string> room) {
	rooms.push_back(room);
}

std::vector<std::vector<std::string>> CourseManagement::getRooms() {
	return rooms;
}

void CourseManagement::clear() {
	ids.clear();
	names.clear();
	rooms.clear();
}

// Update course
bool CourseManagement::updateCourse(HWND hWnd, HWND hCourseComboBox) {
	LRESULT selectedIndex = SendMessage(hCourseComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No course selected.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int courseId = (int)SendMessage(hCourseComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (courseId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the course ID.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	WindowHandler* wh = WindowHandler::getInstance();
	std::wstring courseName = wh->getWindowText(GetDlgItem(hWnd, 213));
	std::wstring firstRoomName = wh->getComboBoxSelectedText(GetDlgItem(hWnd, 214));
	std::wstring secondRoomName = wh->getComboBoxSelectedText(GetDlgItem(hWnd, 217));

	std::string courseName_utf8(courseName.begin(), courseName.end());
	std::string firstRoomName_utf8(firstRoomName.begin(), firstRoomName.end());
	std::string secondRoomName_utf8(secondRoomName.begin(), secondRoomName.end());

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
	CourseManagement* updatedCourses = dbHandler->getCourses();

	int courseIndex = -1;
	for (int i = 0; i < updatedCourses->getIds().size(); i++) {
		if (updatedCourses->getIds()[i] == courseId) {
			courseIndex = i;
			break;
		}
	}

	if (courseIndex < 0 || courseIndex >= updatedCourses->getIds().size()) {
		MessageBox(hWnd, L"Invalid course ID.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	updatedCourses->setCurrentId(courseId);
	updatedCourses->setName(courseIndex, courseName_utf8);
	std::vector<std::string> roomNames = { firstRoomName_utf8, secondRoomName_utf8 };
	updatedCourses->setRooms(courseIndex, roomNames);
	updatedCourses->setCurrentIndex(courseIndex);

	if (!dbHandler->update(*updatedCourses)) {
		MessageBox(hWnd, L"The update failed.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	SendMessage(hCourseComboBox, CB_RESETCONTENT, 0, 0);
	for (int i = 0; i < updatedCourses->getNames().size(); i++) {
		std::wstring wideName = wh->convertToWideString(updatedCourses->getNames()[i]);
		LRESULT index = SendMessage(hCourseComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hCourseComboBox, CB_SETITEMDATA, index, (LPARAM)updatedCourses->getIds()[i]);
	}

	MessageBox(hWnd, L"The course has been updated successfully.", L"Success", MB_OK);
	SendMessage(hCourseComboBox, CB_SETCURSEL, selectedIndex, 0);
	return true;
}

// Delete course
bool CourseManagement::removeCourse(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No course selected.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int courseId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (courseId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the course ID.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
	CourseManagement* updatedCourses = dbHandler->getCourses();

	updatedCourses->setCurrentId(courseId);

	if (!dbHandler->deleteData(*updatedCourses)) {
		MessageBox(hWnd, L"Failed to delete the course.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	int courseIndex = -1;
	for (int i = 0; i < updatedCourses->getIds().size(); i++) {
		if (updatedCourses->getIds()[i] == courseId) {
			courseIndex = i;
			break;
		}
	}

	if (courseIndex < 0 || courseIndex >= updatedCourses->getIds().size()) {
		MessageBox(hWnd, L"Course ID not found.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	updatedCourses->ids.erase(updatedCourses->ids.begin() + courseIndex);
	updatedCourses->names.erase(updatedCourses->names.begin() + courseIndex);
	updatedCourses->rooms.erase(updatedCourses->rooms.begin() + courseIndex);

	SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	WindowHandler* wh = WindowHandler::getInstance();
	for (int i = 0; i < updatedCourses->getNames().size(); i++) {
		std::wstring wideName = wh->convertToWideString(updatedCourses->getNames()[i]);
		LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)updatedCourses->getIds()[i]);
	}

	MessageBox(hWnd, L"The course has been deleted successfully.", L"Success", MB_OK);
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	return true;
}


/* Admin course management Wnd */
void CourseManagement::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminInterfaceVisible(false);
	wh->setAdminAddCourseVisible(false);
	wh->setAdminManageCourseVisible(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminCourseManagement")) {
		wh->createAdminCourseManagementWindows(hWnd);
	}
	wh->setAdminCourseManagementVisible();
}

/* Admin manage courses Wnd */
void CourseManagement::manageData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminCourseManagementVisible(false);

	// Show Manage Course
	if (!wh->isWindowCreated("adminManageCourse")) {
		wh->createAdminManageCourseWindows(hWnd);
	}
	wh->setAdminManageCourseVisible();
}

/* Admin add course Wnd */
void CourseManagement::addData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Course Management
	wh->setAdminCourseManagementVisible(false);

	// Show Add Course
	if (!wh->isWindowCreated("adminAddCourse")) {
		wh->createAdminAddCourseWindows(hWnd);
	}
	wh->setAdminAddCourseVisible();
}

/* Admin insert course to DB */
void CourseManagement::insertToDB(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();
	std::wstring courseName = wh->getWindowText(GetDlgItem(hWnd, 153));

	if (courseName.empty()) {
		MessageBox(NULL, TEXT("Course name cannot be empty!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	int itemCount = (int)SendMessage(GetDlgItem(hWnd, 150), LB_GETCOUNT, 0, 0);
	if (itemCount == LB_ERR) {
		MessageBox(NULL, TEXT("Error getting item count from list."), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	std::vector<std::wstring> selectedRoomNames;
	for (int i = 0; i < itemCount; i++) {
		if (SendMessage(GetDlgItem(hWnd, 150), LB_GETSEL, i, 0) > 0) {
			int len = (int)SendMessage(GetDlgItem(hWnd, 150), LB_GETTEXTLEN, i, 0);
			std::wstring itemText(len + 1, L'\0');
			SendMessage(GetDlgItem(hWnd, 150), LB_GETTEXT, i, (LPARAM)itemText.data());
			int pos = (int)itemText.find(L" - ");
			selectedRoomNames.push_back(pos != std::wstring::npos ? itemText.substr(0, pos) : L"");
		}
	}

	if (selectedRoomNames.empty()) {
		MessageBox(NULL, TEXT("Please select at least one class."), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}
	if (selectedRoomNames.size() > 2) {
		MessageBox(NULL, TEXT("Please select no more than two classes."), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	std::string courseName_utf8(courseName.begin(), courseName.end());
	std::vector<std::string> selectedRoomsUtf8;
	for (auto& roomName : selectedRoomNames) {
		selectedRoomsUtf8.push_back(std::string(roomName.begin(), roomName.end()));
	}

	CourseManagement courseData;
	courseData.addName(courseName_utf8);
	courseData.addRooms(selectedRoomsUtf8);

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();

	if (dbHandler->insert(courseData)) {
		MessageBox(hWnd, TEXT("Course inserted successfully!"), TEXT("Success"), MB_OK | MB_ICONINFORMATION);
	}
	else {
		MessageBox(NULL, TEXT("Failed to insert data."), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
}