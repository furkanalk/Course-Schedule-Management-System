#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include "sqlite3.h"

class User {
protected:
	int currentId;
	int currentIndex;
	std::vector<int> ids;
	std::vector<std::string> names;
public:
	int getCurrentId();
	void setCurrentId(int id);

	int getCurrentIndex();
	void setCurrentIndex(int id);

	void addId(int id);
	std::vector<int> getIds();

	void setName(int index, std::string newName);
	void addName(std::string name);
	std::vector<std::string> getNames();

	void login(HWND hWnd);
	virtual void addData(HWND hWnd);
	virtual void insertToDB(HWND hWnd);
	virtual void showInterface(HWND hWnd);
	virtual void manageData(HWND hWnd);
};

#endif