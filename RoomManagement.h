#ifndef ROOMMANAGEMENT_H
#define ROOMMANAGEMENT_H
#include "user.h"

class RoomManagement : public User {
private:
	std::vector<std::string> floors;
	std::vector<std::string> categories;
public:
	void setFloor(int index, std::string newFloor);
	void addFloor(std::string floor);
	std::vector<std::string> getFloors();

	void setCategory(int index, std::string newCategory);
	void addCategory(std::string category);
	std::vector<std::string> getCategories();

	void clear();

	void showInterface(HWND hWnd) override;
	void manageData(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;

	bool updateRoom(HWND hWnd, HWND hComboBox);
	bool removeRoom(HWND hWnd, HWND hComboBox);
};

#endif