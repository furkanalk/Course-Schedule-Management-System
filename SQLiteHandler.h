#ifndef SQLITE_HANDLER_H
#define SQLITE_HANDLER_H

#include "sqlite3.h"
#include <string>

class SQLiteHandler {
private:
    sqlite3* db;
    std::string databaseName;

public:
    // Create database if it doesn't exist
    SQLiteHandler(const std::string& databaseName);
    ~SQLiteHandler();

    /* Convert to Wide String */
    std::wstring convertToWideString(const std::string& str);

    /* Error Message */
    void showError(const std::string& message);

    bool openDatabase();
    bool closeDatabase();

    bool insertTeacher(const std::string& username, const std::string& course,
        int monday, int tuesday, int wednesday, int thursday, int friday, int saturday);
};

#endif
