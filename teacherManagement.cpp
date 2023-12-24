#include "TeacherManagement.h"
#include "handlers.h"
#include "SQLiteHandler.h"

void TeacherManagement::setWorkdays(int index, std::vector<int> isWorkday) {
	if (index < workdays.size()) {
		workdays[index] = isWorkday;
	}
}

void TeacherManagement::addWorkdays(std::vector<int> weeklyWorkdays) {
	workdays.push_back(weeklyWorkdays);
}

std::vector<std::vector<int>> TeacherManagement::getWorkdays() {
	return workdays;
}

void TeacherManagement::clear() {
	ids.clear();
	names.clear();
	workdays.clear();
}

/* Admin teacher management Wnd */
void TeacherManagement::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminInterfaceVisible(false);
	wh->setAdminAddTeacherVisible(false);
	wh->setAdminManageTeacherVisible(false);

	// Show Teacher Management
	if (!wh->isWindowCreated("adminTeacherManagement")) {
		wh->createAdminTeacherManagementWindows(hWnd);
	}
	wh->setAdminTeacherManagementVisible();
}

/* Admin manage teachers Wnd */
void TeacherManagement::manageData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminTeacherManagementVisible(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminManageTeacher")) {
		wh->createAdminManageTeacherWindows(hWnd);
	}
	wh->setAdminManageTeacherVisible();
}

/* Admin add teacher Wnd */
void TeacherManagement::addData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Teacher Management
	wh->setAdminTeacherManagementVisible(false);

	// Show Add Teacher
	if (!wh->isWindowCreated("adminAddTeacher")) {
		wh->createAdminAddTeacherWindows(hWnd);
	}
	wh->setAdminAddTeacherVisible();
}

/* Admin insert teacher into DB */
void TeacherManagement::insertToDB(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();
	std::wstring fullname = wh->getWindowText(GetDlgItem(hWnd, 149));
	std::wstring course = wh->getWindowText(GetDlgItem(hWnd, 140));

	if (fullname.empty() || course.empty()) {
		MessageBox(NULL, TEXT("Full name and course cannot be empty!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	std::vector<HWND> workdays = {
		GetDlgItem(hWnd, 143), GetDlgItem(hWnd, 144), GetDlgItem(hWnd, 145),
		GetDlgItem(hWnd, 146), GetDlgItem(hWnd, 147), GetDlgItem(hWnd, 148)
	};

	bool workdaySelected = false;
	for (HWND checkboxHwnd : workdays) {
		if (SendMessage(checkboxHwnd, BM_GETCHECK, 0, 0) == BST_CHECKED) {
			workdaySelected = true;
			break;
		}
	}

	if (!workdaySelected) {
		MessageBox(NULL, TEXT("At least one workday must be selected!"), TEXT("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	std::string fullname_utf8(fullname.begin(), fullname.end());
	std::string course_utf8(course.begin(), course.end());

	std::vector<int> workdayValues;
	for (HWND checkboxHwnd : workdays) {
		int value = SendMessage(checkboxHwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
		workdayValues.push_back(value);
	}

	TeacherManagement teacherData;
	teacherData.addName(fullname_utf8);
	teacherData.addName(course_utf8);
	teacherData.addWorkdays(workdayValues);

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();

	if (dbHandler->insert(teacherData)) {
		MessageBox(NULL, TEXT("Data inserted successfully!"), TEXT("Success"), MB_OK | MB_ICONINFORMATION);
	}
	else {
		MessageBox(NULL, TEXT("Failed to insert data."), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
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
		MessageBox(hWnd, L"Failed to retrieve the teacher ID.", L"Error", MB_ICONWARNING | MB_OK);
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

	std::vector<int> newWorkdays;
	std::vector<HWND> weekdayCombos = { hMonday, hTuesday, hWednesday, hThursday, hFriday, hSaturday };

	for (HWND hCombo : weekdayCombos) {
		LRESULT selected = SendMessage(hCombo, CB_GETCURSEL, 0, 0);
		int isWorking;

		if (selected == 0)
			isWorking = 1;
		else
			isWorking = 0;

		newWorkdays.push_back(isWorking);
	}

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
	TeacherManagement* allTeachers = dbHandler->getTeachers();

	int teacherIndex = -1;
	for (int i = 0; i < allTeachers->ids.size(); ++i) {
		if (allTeachers->ids[i] == teacherId) {
			teacherIndex = i;
			break;
		}
	}

	if (teacherIndex == -1 || teacherIndex >= allTeachers->getIds().size()) {
		MessageBox(hWnd, L"Invalid teacher ID.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	allTeachers->setName(teacherIndex, teacherName_utf8);
	allTeachers->setWorkdays(teacherIndex, newWorkdays);
	allTeachers->setCurrentId(teacherId);
	allTeachers->setCurrentIndex(teacherIndex);

	if (!dbHandler->update(*allTeachers)) {
		MessageBox(hWnd, L"The update failed.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	for (int i = 0; i < allTeachers->getNames().size(); i++) {
		std::wstring wideName = wh->convertToWideString(allTeachers->getNames()[i]);
		LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)allTeachers->getIds()[i]);
	}

	MessageBox(hWnd, L"The update is successful.", L"Success", MB_OK);
	SendMessage(hComboBox, CB_SETCURSEL, selectedIndex, 0);

	return true;
}

// Remove teacher
bool TeacherManagement::removeTeacher(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No teacher selected.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int teacherId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (teacherId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the teacher ID.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
	TeacherManagement* updatedTeachers = dbHandler->getTeachers();
	updatedTeachers->setCurrentId(teacherId);

	if (!dbHandler->deleteData(*updatedTeachers)) {
		MessageBox(hWnd, L"Failed to delete the teacher.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	int teacherIndex = -1;
	for (int i = 0; i < updatedTeachers->ids.size(); i++) {
		if (updatedTeachers->ids[i] == teacherId) {
			teacherIndex = i;
			break;
		}
	}

	if (teacherIndex < 0 || teacherIndex >= updatedTeachers->getIds().size()) {
		MessageBox(hWnd, L"Room ID not found.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	updatedTeachers->ids.erase(updatedTeachers->ids.begin() + teacherIndex);
	updatedTeachers->names.erase(updatedTeachers->names.begin() + teacherIndex);

	SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	WindowHandler* wh = WindowHandler::getInstance();
	for (int i = 0; i < updatedTeachers->getNames().size(); i++) {
		std::wstring wideName = wh->convertToWideString(updatedTeachers->getNames()[i]);
		LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)updatedTeachers->getIds()[i]);
	}

	MessageBox(hWnd, L"The teacher has been deleted successfully.", L"Success", MB_OK);
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	return true;
}