#ifndef COURSE_H
#define COURSE_H

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "sqlite3.h"

class User {
protected:
	int currentId;
	std::vector<int> ids;
	std::vector<std::string> names;
public:
	int getCurrentId();
	void setCurrentId(int id);

	void addId(int id);
	std::vector<int> getIds();

	void setName(size_t index, std::string newName);
	void addName(std::string name);
	std::vector<std::string> getNames();

	void login(HWND hWnd);
	virtual void addData(HWND hWnd);
	virtual void insertToDB(HWND hWnd);
	virtual void showInterface(HWND hWnd);
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
	// Override functions
	void showInterface(HWND hWnd) override;
	/*void getData(HWND hWnd) override;
	void readFromDB(HWND hWnd) override;*/
};

class TeacherManagement : public User {
private:
	std::vector<std::vector<bool>> workdays;
public:
	void setWorkdays(size_t index, std::vector<bool> isWorkday);
	void addWorkdays(std::vector<bool> weeklyWorkdays);
	std::vector<std::vector<bool>> getWorkdays();

	void showInterface(HWND hWnd) override;
	void manageData(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;

	bool updateTeacher(HWND hWnd, HWND hComboBox);
	bool removeTeacher(HWND hWnd, HWND hComboBox);
};

class CourseManagement : public User {
private:
	std::vector<std::vector<std::string>> rooms;
public:
	void setRooms(size_t index, std::vector<std::string> rooms);
	void addRooms(std::vector<std::string> rooms);
	std::vector<std::vector<std::string>> getRooms();

	void showInterface(HWND hWnd) override;
	void manageData(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;

	bool updateCourse(HWND hWnd, HWND hComboBox);
	bool removeCourse(HWND hWnd, HWND hComboBox);
};

class RoomManagement : public User {
private:
	std::vector<std::string> floors;
	std::vector<std::string> categories;
public:
	void setFloor(size_t index, std::string newFloor);
	void addFloor(std::string floor);
	std::vector<std::string> getFloors();

	void setCategory(size_t index, std::string newCategory);
	void addCategory(std::string category);
	std::vector<std::string> getCategories();

	void showInterface(HWND hWnd) override;
	void manageData(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;

	bool updateRoom(HWND hWnd, HWND hComboBox);
	bool removeRoom(HWND hWnd, HWND hComboBox);
};

class GradeManagement : public User {
public:
	// Override functions
	/*void showInterface(HWND hWnd) override;
	void getData(HWND hWnd) override;
	void readFromDB(HWND hWnd) override;
	void addData(HWND hWnd) override;
	void insertToDB(HWND hWnd) override;*/
};

#endif