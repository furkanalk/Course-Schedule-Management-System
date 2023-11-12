#include "course.h"
#include "handlers.h"
#include "SQLiteHandler.h"
#include <memory>

int User::getCurrentId() {
	return currentId;
}

void User::setCurrentId(int id) {
	currentId = id;
}

void User::addId(int id) {
	ids.push_back(id);
}

std::vector<int> User::getIds() {
	return ids;
}

void User::setName(size_t index, std::string newName) {
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
	wh->setAdminLoginVisibility(false);
	wh->setTeacherLoginVisibility(false);
	wh->setStudentLoginVisibility(false);
	// Admin
	wh->setAdminInterfaceVisibility(false);
	wh->setAdminCourseManagementVisibility(false);
	wh->setAdminTeacherManagementVisibility(false);

	// Show Login Type	
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
void User::login(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Login Type
	wh->setLoginTypeVisibility(false);

	// Show Admin Login
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

void User::addData(HWND hWnd) {
	// Will be filled with admin data
}

void User::insertToDB(HWND hWnd) {
	// Will be filled with admin data
}

void CourseManagement::setRooms(size_t index, std::vector<std::string> newRooms) {
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

bool CourseManagement::updateCourse(HWND hWnd, HWND hCourseComboBox) {
	LRESULT selectedIndex = SendMessage(hCourseComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No course selected.", L"Selection Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int courseId = (int)SendMessage(hCourseComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (courseId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the course ID.", L"ID Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	WindowHandler* wh = WindowHandler::getInstance();
	std::wstring courseName = wh->getWindowText(GetDlgItem(hWnd, 213));
	HWND hFirstRoomComboBox = GetDlgItem(hWnd, 214);
	HWND hSecondRoomComboBox = GetDlgItem(hWnd, 217);

	std::wstring firstRoomName = wh->getComboBoxSelectedText(hFirstRoomComboBox);
	std::wstring secondRoomName = wh->getComboBoxSelectedText(hSecondRoomComboBox);

	std::string courseName_utf8(courseName.begin(), courseName.end());
	std::string firstRoomName_utf8(firstRoomName.begin(), firstRoomName.end());
	std::string secondRoomName_utf8(secondRoomName.begin(), secondRoomName.end());

	try {
		SQLiteHandler* dbHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");
		CourseManagement updatedCourses = dbHandler->getCourses();

		size_t courseIndex = -1;
		for (size_t i = 0; i < updatedCourses.getIds().size(); ++i) {
			if (updatedCourses.getIds()[i] == courseId) {
				courseIndex = i;
				break;
			}
		}

		if (courseIndex < 0 || courseIndex >= updatedCourses.getIds().size()) {
			MessageBox(hWnd, L"Invalid course ID.", L"Error", MB_ICONERROR | MB_OK);
			return false;
		}
		updatedCourses.setCurrentId(courseId);

		updatedCourses.setName(courseIndex, courseName_utf8);
		std::vector<std::string> roomNames = { firstRoomName_utf8, secondRoomName_utf8 };
		updatedCourses.setRooms(courseIndex, roomNames);

		bool result = dbHandler->updateCourse(updatedCourses);
		if (!result) {
			MessageBox(hWnd, L"The update operation failed.", L"Update Failed", MB_ICONERROR | MB_OK);
			return false;
		}

		SendMessage(hCourseComboBox, CB_RESETCONTENT, 0, 0);
		for (size_t i = 0; i < updatedCourses.getNames().size(); ++i) {
			std::wstring wideName = wh->convertToWideString(updatedCourses.getNames()[i]);
			LRESULT index = SendMessage(hCourseComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
			SendMessage(hCourseComboBox, CB_SETITEMDATA, index, (LPARAM)updatedCourses.getIds()[i]);
		}

		MessageBox(hWnd, L"The course has been updated successfully.", L"Success", MB_OK);
		SendMessage(hCourseComboBox, CB_SETCURSEL, selectedIndex, 0);
		return true;
	}
	catch (const std::runtime_error& e) {
		std::wstring errorMessage = L"Update Error: " + std::wstring(e.what(), e.what() + strlen(e.what()));
		MessageBox(hWnd, errorMessage.c_str(), L"Database Error", MB_OK | MB_ICONERROR);
	}
	return false;
}

bool CourseManagement::removeCourse(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No course selected.", L"Selection Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int courseId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (courseId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the course ID.", L"ID Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");
	CourseManagement updatedCourses = dbHandler->getCourses();

	updatedCourses.setCurrentId(courseId);

	if (!dbHandler->deleteCourse(updatedCourses)) {
		MessageBox(hWnd, L"Failed to delete the course.", L"Deletion Error", MB_ICONERROR | MB_OK);
		return false;
	}

	int courseIndex = -1;
	for (size_t i = 0; i < updatedCourses.getIds().size(); ++i) {
		if (updatedCourses.getIds()[i] == courseId) {
			courseIndex = i;
			break;
		}
	}

	if (courseIndex < 0 || courseIndex >= updatedCourses.getIds().size()) {
		MessageBox(hWnd, L"Course ID not found.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	updatedCourses.ids.erase(updatedCourses.ids.begin() + courseIndex);
	updatedCourses.names.erase(updatedCourses.names.begin() + courseIndex);
	updatedCourses.rooms.erase(updatedCourses.rooms.begin() + courseIndex);

	SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	WindowHandler* wh = WindowHandler::getInstance();
	for (size_t i = 0; i < updatedCourses.getNames().size(); ++i) {
		std::wstring wideName = wh->convertToWideString(updatedCourses.getNames()[i]);
		LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)updatedCourses.getIds()[i]);
	}

	MessageBox(hWnd, L"The course has been deleted successfully.", L"Success", MB_OK);
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	return true;
}


/* Admin course management Wnd */
void CourseManagement::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminInterfaceVisibility(false);
	wh->setAdminAddCourseVisibility(false);
	wh->setAdminManageCourseVisibility(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminCourseManagement")) {
		wh->createAdminCourseManagementWindows(hWnd);
	}
	wh->setAdminCourseManagementVisibility(true);
}

/* Admin manage courses Wnd */
void CourseManagement::manageData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminCourseManagementVisibility(false);

	// Show Manage Course
	if (!wh->isWindowCreated("adminManageCourse")) {
		wh->createAdminManageCourseWindows(hWnd);
	}
	wh->setAdminManageCourseVisibility(true);
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

void TeacherManagement::setWorkdays(size_t index, std::vector<bool> isWorkday) {
	if (index < workdays.size()) {
		workdays[index] = isWorkday;
	}
}

void TeacherManagement::addWorkdays(std::vector<bool> weeklyWorkdays) {
	workdays.push_back(weeklyWorkdays);
}

std::vector<std::vector<bool>> TeacherManagement::getWorkdays() {
	return workdays;
}

/* Admin teacher management Wnd */
void TeacherManagement::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminInterfaceVisibility(false);
	wh->setAdminAddTeacherVisibility(false);
	wh->setAdminManageTeacherVisibility(false);

	// Show Teacher Management
	if (!wh->isWindowCreated("adminTeacherManagement")) {
		wh->createAdminTeacherManagementWindows(hWnd);
	}
	wh->setAdminTeacherManagementVisibility(true);
}

/* Admin manage teachers Wnd */
void TeacherManagement::manageData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminTeacherManagementVisibility(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminManageTeacher")) {
		wh->createAdminManageTeacherWindows(hWnd);
	}
	wh->setAdminManageTeacherVisibility(true);
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

/* Update teacher */
bool TeacherManagement::updateTeacher(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No teacher selected.", L"Selection Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int teacherId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (teacherId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the teacher ID.", L"ID Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	WindowHandler* wh = WindowHandler::getInstance();
	std::wstring teacherName = wh->getWindowText(GetDlgItem(hWnd, 193));
	std::string teacherName_utf8(teacherName.begin(), teacherName.end());

	HWND hMonday = GetDlgItem(hWnd, 194);
	HWND hTuesday = GetDlgItem(hWnd, 195);
	HWND hWednesday = GetDlgItem(hWnd, 196);
	HWND hThursday = GetDlgItem(hWnd, 197);
	HWND hFriday = GetDlgItem(hWnd, 198);
	HWND hSaturday = GetDlgItem(hWnd, 199);

	std::vector<bool> newWorkdays;
	std::vector<HWND> weekdayCombos = { hMonday, hTuesday, hWednesday, hThursday, hFriday, hSaturday };

	for (HWND hCombo : weekdayCombos) {
		LRESULT selected = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
		bool isWorking;

		if (selected == 0)
			isWorking = true;
		else
			isWorking = false;

		newWorkdays.push_back(isWorking);
	}

	try {
		SQLiteHandler* dbHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");
		TeacherManagement allTeachers = dbHandler->getTeachers();

		size_t teacherIndex = -1;
		for (size_t i = 0; i < allTeachers.ids.size(); ++i) {
			if (allTeachers.ids[i] == teacherId) {
				teacherIndex = i;
				break;
			}
		}

		if (teacherIndex == (size_t)-1 || teacherIndex >= allTeachers.getIds().size()) {
			MessageBox(hWnd, L"Invalid teacher ID.", L"Error", MB_ICONERROR | MB_OK);
			return false;
		}

		allTeachers.setName(teacherIndex, teacherName_utf8);
		allTeachers.setWorkdays(teacherIndex, newWorkdays);

		if (!dbHandler->updateTeacher(allTeachers)) {
			MessageBox(hWnd, L"The update operation failed.", L"Update Error", MB_ICONERROR | MB_OK);
			return false;
		}

		SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
		for (size_t i = 0; i < allTeachers.getNames().size(); ++i) {
			std::wstring wideName = wh->convertToWideString(allTeachers.getNames()[i]);
			LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
			SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)allTeachers.getIds()[i]);
		}

		MessageBox(hWnd, L"The teacher has been updated successfully.", L"Success", MB_OK);
		SendMessage(hComboBox, CB_SETCURSEL, selectedIndex, 0);

		return true;
	}
	catch (const std::runtime_error& e) {
		std::wstring errorMessage = L"Update Error: " + std::wstring(e.what(), e.what() + strlen(e.what()));
		MessageBox(hWnd, errorMessage.c_str(), L"Database Error", MB_OK | MB_ICONERROR);
	}
	return false;
}

// Remove teacher
bool TeacherManagement::removeTeacher(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No teacher selected.", L"Selection Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int teacherId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (teacherId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the teacher ID.", L"ID Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");
	TeacherManagement updatedTeachers = dbHandler->getTeachers();

	if (!dbHandler->deleteTeacher(updatedTeachers)) {
		MessageBox(hWnd, L"Failed to delete the teacher.", L"Deletion Error", MB_ICONERROR | MB_OK);
		return false;
	}

	int teacherIndex = -1;
	for (size_t i = 0; i < updatedTeachers.ids.size(); ++i) {
		if (updatedTeachers.ids[i] == teacherId) {
			teacherIndex = i;
			break;
		}
	}

	if (teacherIndex < 0 || teacherIndex >= updatedTeachers.getIds().size()) {
		MessageBox(hWnd, L"Room ID not found.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	updatedTeachers.ids.erase(updatedTeachers.ids.begin() + teacherIndex);
	updatedTeachers.names.erase(updatedTeachers.names.begin() + teacherIndex);

	SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	WindowHandler* wh = WindowHandler::getInstance();
	for (size_t i = 0; i < updatedTeachers.getNames().size(); ++i) {
		std::wstring wideName = wh->convertToWideString(updatedTeachers.getNames()[i]);
		LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)updatedTeachers.getIds()[i]);
	}

	MessageBox(hWnd, L"The teacher has been deleted successfully.", L"Success", MB_OK);
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	return true;
}

void RoomManagement::setFloor(size_t index, std::string newFloor) {
	if (index < floors.size()) {
		floors[index] = newFloor;
	}
}
void RoomManagement::addFloor(std::string floor) {
	floors.push_back(floor);
}
std::vector<std::string> RoomManagement::getFloors() {
	return floors;
}

void RoomManagement::setCategory(size_t index, std::string newCategory) {
	if (index < categories.size()) {
		categories[index] = newCategory;
	}
}
void RoomManagement::addCategory(std::string category) {
	categories.push_back(category);
}
std::vector<std::string> RoomManagement::getCategories() {
	return categories;
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

/* Update room */
bool RoomManagement::updateRoom(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No classroom selected.", L"Selection Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int roomId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (roomId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the room ID.", L"ID Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	WindowHandler* wh = WindowHandler::getInstance();
	std::wstring roomName = wh->getWindowText(GetDlgItem(hWnd, 185));
	std::wstring roomFloor = wh->getWindowText(GetDlgItem(hWnd, 186));
	std::wstring roomCategory = wh->getWindowText(GetDlgItem(hWnd, 187));

	RoomManagement updatedClassrooms;

	std::string roomName_utf8(roomName.begin(), roomName.end());
	std::string roomFloor_utf8(roomFloor.begin(), roomFloor.end());
	std::string roomCategory_utf8(roomCategory.begin(), roomCategory.end());

	try {
		SQLiteHandler* dbHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");
		updatedClassrooms = dbHandler->getClassrooms();

		size_t roomIndex = -1;
		for (size_t i = 0; i < updatedClassrooms.ids.size(); ++i) {
			if (updatedClassrooms.ids[i] == roomId) {
				roomIndex = i;
				break;
			}
		}

		if (roomIndex < 0 || roomIndex >= updatedClassrooms.getIds().size()) {
			MessageBox(hWnd, L"Invalid room ID.", L"Error", MB_ICONERROR | MB_OK);
			return false;
		}

		updatedClassrooms.setName(roomIndex, roomName_utf8);
		updatedClassrooms.setFloor(roomIndex, roomFloor_utf8);
		updatedClassrooms.setCategory(roomIndex, roomCategory_utf8);

		bool result = dbHandler->updateRoom(updatedClassrooms);
		if (!result) {
			MessageBox(hWnd, L"The update operation returned false.", L"Update Failed", MB_OK);
			return false;
		}

		SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
		for (size_t i = 0; i < updatedClassrooms.getNames().size(); ++i) {
			std::wstring wideName = wh->convertToWideString(updatedClassrooms.getNames()[i]);
			LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
			SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)updatedClassrooms.getIds()[i]);
		}

		MessageBox(hWnd, L"The room has been updated successfully.", L"Success", MB_OK);
		SendMessage(hComboBox, CB_SETCURSEL, selectedIndex, 0);
		return true;
	}
	catch (const std::runtime_error& e) {
		std::wstring errorMessage = L"Update Error: " + std::wstring(e.what(), e.what() + strlen(e.what()));
		MessageBox(hWnd, errorMessage.c_str(), L"Database Error", MB_OK | MB_ICONERROR);
	}
	return false;
}

/* Remove room */
bool RoomManagement::removeRoom(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No classroom selected.", L"Selection Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance("courseScheduleDB.sqlite");
	RoomManagement updatedClassrooms = dbHandler->getClassrooms();

	int roomId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (roomId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the room ID.", L"ID Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	updatedClassrooms.setCurrentId(roomId);

	if (!dbHandler->deleteRoom(updatedClassrooms)) {
		MessageBox(hWnd, L"Failed to delete the classroom.", L"Deletion Error", MB_ICONERROR | MB_OK);
		return false;
	}

	int roomIndex = -1;
	for (size_t i = 0; i < updatedClassrooms.ids.size(); ++i) {
		if (updatedClassrooms.ids[i] == roomId) {
			roomIndex = i;
			break;
		}
	}

	if (roomIndex < 0 || roomIndex >= updatedClassrooms.getIds().size()) {
		MessageBox(hWnd, L"Room ID not found.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	updatedClassrooms.ids.erase(updatedClassrooms.ids.begin() + roomIndex);
	updatedClassrooms.names.erase(updatedClassrooms.names.begin() + roomIndex);
	updatedClassrooms.floors.erase(updatedClassrooms.floors.begin() + roomIndex);
	updatedClassrooms.categories.erase(updatedClassrooms.categories.begin() + roomIndex);

	SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	WindowHandler* wh = WindowHandler::getInstance();
	for (size_t i = 0; i < updatedClassrooms.getNames().size(); ++i) {
		std::wstring wideName = wh->convertToWideString(updatedClassrooms.getNames()[i]);
		LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)updatedClassrooms.getIds()[i]);
	}

	MessageBox(hWnd, L"The classroom has been deleted successfully.", L"Success", MB_OK);
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	return true;
}

/* Teacher interface Wnd */
void Teacher::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Login Type
	wh->setLoginTypeVisibility(false);

	// Show Teacher Login
	if (!wh->isWindowCreated("teacherLogin")) {
		wh->createTeacherLoginWindows(hWnd);
	}
	wh->setTeacherLoginVisibility(true);
}

/* Student interface Wnd */
void Student::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Login Type
	wh->setLoginTypeVisibility(false);

	// Show Student Login
	if (!wh->isWindowCreated("studentLogin")) {
		wh->createStudentLoginWindows(hWnd);
	}
	wh->setStudentLoginVisibility(true);
}
