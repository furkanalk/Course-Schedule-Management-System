#ifndef COURSESCHEDULE_H
#define COURSESCHEDULE_H

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "handlers.h"
#include "sqlite3.h"

class courseSystem {
	std::string username; // Login name holder
	std::string gender; // Login gender holder
	std::string id;  // Login id holder
	std::string age; // Login age holder

protected:
	/* Setters */
	void setName(std::string username);
	void setGender(std::string gender);
	void setId(std::string id);
	void setAge(std::string age);

public:
	/* Getters */
	std::string getUsername();
	std::string getGender();
	std::string getId();
	std::string getAge();

	/* Convert to Wide String */
	std::wstring convertToWideString(const std::string& str);

	/* Login Types */
	virtual void login(HWND hWnd);	
};

class Admin : public courseSystem {
public:
	/* Admin login */
	void login(HWND hWnd) override;
	void showInterface(HWND hWnd);
	void courseManagement(HWND hWnd);
	void teacherManagement(HWND hWnd);
};

class Teacher : public courseSystem {
public:
	/* Teacher login */
	void login(HWND hWnd) override;
};

class Student : public courseSystem {
public:
	/* Student login */
	void login(HWND hWnd) override;
};


#endif