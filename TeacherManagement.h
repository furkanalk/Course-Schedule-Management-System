#ifndef TEACHERMANAGEMENT_H
#define TEACHERMANAGEMENT_H
#include "user.h"

class TeacherManagement : public User {
private:
	std::vector<std::vector<int>> workdays;
public:
	void setWorkdays(int index, std::vector<int> isWorkday);
	void addWorkdays(std::vector<int> weeklyWorkdays);
	std::vector<std::vector<int>> getWorkdays();

	void clear();

	void showInterface(HWND hWnd) override;
	void manageData(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;

	bool updateTeacher(HWND hWnd, HWND hComboBox);
	bool removeTeacher(HWND hWnd, HWND hComboBox);
};

#endif