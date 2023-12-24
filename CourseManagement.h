#ifndef COURSEMANAGEMENT_H
#define COURSEMANAGEMENT_H
#include "user.h"

class CourseManagement : public User {
private:
	std::vector<std::vector<std::string>> rooms;
public:
	void setRooms(int index, std::vector<std::string> rooms);
	void addRooms(std::vector<std::string> rooms);
	std::vector<std::vector<std::string>> getRooms();

	void clear();

	void showInterface(HWND hWnd) override;
	void manageData(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;

	bool updateCourse(HWND hWnd, HWND hComboBox);
	bool removeCourse(HWND hWnd, HWND hComboBox);
};

#endif