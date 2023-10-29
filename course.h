#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "handlers.h"
#include "sqlite3.h"

class User {
public:
	/* Convert to Wide String */
	/*std::wstring convertToWideString(const std::string& str);*/

	virtual void showInterface(HWND hWnd);
	// data of teacher and classes
	virtual void manageData(HWND hWnd);
};

// User Types
class Teacher : public User {
public:
	// Override functions
	void showInterface(HWND hWnd) override;
	/*void getData(HWND hWnd) override;
	void readFromDB(HWND hWnd) override;*/
};

class Student : public User {
public:
	// Override functions
	void showInterface(HWND hWnd) override;
	/*void getData(HWND hWnd) override;
	void readFromDB(HWND hWnd) override;*/
};

class Admin : public User {
public:
	void login(HWND hWnd);

	// Override functions
	void showInterface(HWND hWnd) override;
	/*void getData(HWND hWnd) override;
	void readFromDB(HWND hWnd) override;*/

	// Functions overridden in children
	//( manage admins )
	virtual void addData(HWND hWnd);
	virtual void insertToDB(HWND hWnd);
};

// Children of Admin
class TeacherManagement : public Admin {
public:
	// Override functions
	void showInterface(HWND hWnd) override;
	/*void getData(HWND hWnd) override;
	void readFromDB(HWND hWnd) override;*/
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;
};

class GradeManagement : public Admin {
public:
	// Override functions
	/*void showInterface(HWND hWnd) override;
	void getData(HWND hWnd) override;
	void readFromDB(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;*/
};

class CourseManagement : public Admin {
public:
	// Override functions
	void showInterface(HWND hWnd) override;
	/*void getData(HWND hWnd) override;
	void readFromDB(HWND hWnd) override;*/
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;
};

class RoomManagement : public Admin {
private:
	std::vector<int> ids;
	std::vector<std::string> names;
	std::vector<std::string> floors;
	std::vector<std::string> categories;
public:
	// Override functions
	void showInterface(HWND hWnd) override;
	void manageData(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;

	void addId(int id) {
		ids.push_back(id);
	}
	const std::vector<int>& getIds() const {
		return ids;
	}

	void addName(const std::string& name) {
		names.push_back(name);
	}
	const std::vector<std::string>& getNames() const {
		return names;
	}

	void addFloor(const std::string& floor) {
		floors.push_back(floor);
	}
	const std::vector<std::string>& getFloors() const {
		return floors;
	}

	void addCategory(const std::string& category) {
		categories.push_back(category);
	}
	const std::vector<std::string>& getCategories() const {
		return categories;
	}
};

#endif