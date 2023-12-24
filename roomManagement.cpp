#include "RoomManagement.h"
#include "handlers.h"
#include "SQLiteHandler.h"

void RoomManagement::setFloor(int index, std::string newFloor) {
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

void RoomManagement::setCategory(int index, std::string newCategory) {
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

void RoomManagement::clear() {
	ids.clear();
	names.clear();
	floors.clear();
	categories.clear();
}

/* Admin room management Wnd */
void RoomManagement::showInterface(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminInterfaceVisible(false);
	wh->setAdminManageRoomVisible(false);
	wh->setAdminAddRoomVisible(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminRoomManagement")) {
		wh->createAdminRoomManagementWindows(hWnd);
	}
	wh->setAdminRoomManagementVisible();
}

/* Admin manage rooms Wnd */
void RoomManagement::manageData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide other Admin windows
	wh->setAdminRoomManagementVisible(false);

	// Show Admin Inferface
	if (!wh->isWindowCreated("adminManageRoom")) {
		wh->createAdminManageRoomWindows(hWnd);
	}
	wh->setAdminManageRoomVisible();
}

/* Admin add clasroom Wnd */
void RoomManagement::addData(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();

	// Hide Teacher Management
	wh->setAdminRoomManagementVisible(false);

	// Show Add Teacher
	if (!wh->isWindowCreated("adminAddRoom")) {
		wh->createAdminAddRoomWindows(hWnd);
	}
	wh->setAdminAddRoomVisible();
}

/* Admin insert classroom into DB */
void RoomManagement::insertToDB(HWND hWnd) {
	WindowHandler* wh = WindowHandler::getInstance();
	std::wstring roomName = wh->getWindowText(GetDlgItem(hWnd, 173));
	std::wstring roomFloor = wh->getWindowText(GetDlgItem(hWnd, 174));
	std::wstring roomCategory = wh->getComboBoxSelectedText(GetDlgItem(hWnd, 170));

	if (roomName.empty() || roomFloor.empty() || roomCategory.empty()) {
		MessageBox(NULL, TEXT("Room Name, floor, and category cannot be empty!"), TEXT("Error"), MB_OK | MB_ICONWARNING);
		return;
	}

	std::string roomName_utf8(roomName.begin(), roomName.end());
	std::string roomFloor_utf8(roomFloor.begin(), roomFloor.end());
	std::string roomCategory_utf8(roomCategory.begin(), roomCategory.end());

	RoomManagement roomData;
	roomData.addName(roomName_utf8);
	roomData.addFloor(roomFloor_utf8);
	roomData.addCategory(roomCategory_utf8);

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();

	if (dbHandler->insert(roomData)) {
		MessageBox(hWnd, TEXT("Room inserted successfully!"), TEXT("Success"), MB_OK | MB_ICONINFORMATION);
	}
	else {
		MessageBox(NULL, TEXT("Failed to insert data."), TEXT("Error"), MB_OK | MB_ICONERROR);
	}
}

/* Update room */
bool RoomManagement::updateRoom(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No classroom selected.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	int roomId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (roomId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the room ID.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	WindowHandler* wh = WindowHandler::getInstance();
	std::wstring roomName = wh->getWindowText(GetDlgItem(hWnd, 185));
	std::wstring roomFloor = wh->getWindowText(GetDlgItem(hWnd, 186));
	std::wstring roomCategory = wh->getWindowText(GetDlgItem(hWnd, 187));

	std::string roomName_utf8(roomName.begin(), roomName.end());
	std::string roomFloor_utf8(roomFloor.begin(), roomFloor.end());
	std::string roomCategory_utf8(roomCategory.begin(), roomCategory.end());

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
	RoomManagement* updatedClassrooms = dbHandler->getClassrooms();

	int roomIndex = -1;
	for (int i = 0; i < updatedClassrooms->ids.size(); i++) {
		if (updatedClassrooms->ids[i] == roomId) {
			roomIndex = i;
			break;
		}
	}

	if (roomIndex < 0 || roomIndex >= updatedClassrooms->getIds().size()) {
		MessageBox(hWnd, L"Invalid room ID.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	updatedClassrooms->setName(roomIndex, roomName_utf8);
	updatedClassrooms->setFloor(roomIndex, roomFloor_utf8);
	updatedClassrooms->setCategory(roomIndex, roomCategory_utf8);
	updatedClassrooms->setCurrentId(roomId);
	updatedClassrooms->setCurrentIndex(roomIndex);

	if (!dbHandler->update(*updatedClassrooms)) {
		MessageBox(hWnd, L"The update operation returned false.", L"Error", MB_OK);
		return false;
	}

	SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	for (int i = 0; i < updatedClassrooms->getNames().size(); i++) {
		std::wstring wideName = wh->convertToWideString(updatedClassrooms->getNames()[i]);
		LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)updatedClassrooms->getIds()[i]);
	}

	MessageBox(hWnd, L"The room has been updated successfully.", L"Success", MB_OK);
	SendMessage(hComboBox, CB_SETCURSEL, selectedIndex, 0);
	return true;
}

/* Remove room */
bool RoomManagement::removeRoom(HWND hWnd, HWND hComboBox) {
	LRESULT selectedIndex = SendMessage(hComboBox, CB_GETCURSEL, 0, 0);
	if (selectedIndex == CB_ERR) {
		MessageBox(hWnd, L"No classroom selected.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	SQLiteHandler* dbHandler = SQLiteHandler::getInstance();
	RoomManagement* updatedClassrooms = dbHandler->getClassrooms();

	int roomId = (int)SendMessage(hComboBox, CB_GETITEMDATA, selectedIndex, 0);
	if (roomId == CB_ERR) {
		MessageBox(hWnd, L"Failed to retrieve the room ID.", L"Error", MB_ICONWARNING | MB_OK);
		return false;
	}

	updatedClassrooms->setCurrentId(roomId);

	if (!dbHandler->deleteData(*updatedClassrooms)) {
		MessageBox(hWnd, L"Failed to delete the classroom.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	int roomIndex = -1;
	for (int i = 0; i < updatedClassrooms->ids.size(); ++i) {
		if (updatedClassrooms->ids[i] == roomId) {
			roomIndex = i;
			break;
		}
	}

	if (roomIndex < 0 || roomIndex >= updatedClassrooms->getIds().size()) {
		MessageBox(hWnd, L"Room ID not found.", L"Error", MB_ICONERROR | MB_OK);
		return false;
	}

	updatedClassrooms->ids.erase(updatedClassrooms->ids.begin() + roomIndex);
	updatedClassrooms->names.erase(updatedClassrooms->names.begin() + roomIndex);
	updatedClassrooms->floors.erase(updatedClassrooms->floors.begin() + roomIndex);
	updatedClassrooms->categories.erase(updatedClassrooms->categories.begin() + roomIndex);

	SendMessage(hComboBox, CB_RESETCONTENT, 0, 0);
	WindowHandler* wh = WindowHandler::getInstance();
	for (int i = 0; i < updatedClassrooms->getNames().size(); i++) {
		std::wstring wideName = wh->convertToWideString(updatedClassrooms->getNames()[i]);
		LRESULT index = SendMessage(hComboBox, CB_ADDSTRING, 0, (LPARAM)wideName.c_str());
		SendMessage(hComboBox, CB_SETITEMDATA, index, (LPARAM)updatedClassrooms->getIds()[i]);
	}

	MessageBox(hWnd, L"The classroom has been deleted successfully.", L"Success", MB_OK);
	SendMessage(hComboBox, CB_SETCURSEL, 0, 0);
	return true;
}