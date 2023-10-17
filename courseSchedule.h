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

	WindowHandler wh; // Window Handler
public:
	/* Setters */
	void setName(std::string username);
	void setGender(std::string gender);
	void setId(std::string id);
	void setAge(std::string age);

	/* Getters */
	std::string getUsername();
	std::string getGender();
	std::string getId();
	std::string getAge();

	/* Convert to Wide String */
	std::wstring convertToWideString(const std::string& str);

	/* Login */
	void loginTypes(HWND hWnd);
	
	/* Admin Login */
	void loginAdmin(HWND hWnd);
	
	/* Teacher Login */
	void loginTeacher(HWND hWnd);

	/* Student Login */
	void loginStudent(HWND hWnd);
};

#endif