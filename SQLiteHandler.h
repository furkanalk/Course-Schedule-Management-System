#ifndef SQLITE_HANDLER_H
#define SQLITE_HANDLER_H

#include "sqlite3.h"
#include "course.h"
#include <string>
#include <stdexcept>
#include <vector>
#include <memory>

class SQLiteHandler {
private:
    static std::unique_ptr<SQLiteHandler> instance;
    sqlite3* db;
    std::string databaseName;
public:
    SQLiteHandler(const std::string& databaseName);

    SQLiteHandler(const SQLiteHandler&) = delete;
    SQLiteHandler& operator=(const SQLiteHandler&) = delete;

    ~SQLiteHandler();

    static SQLiteHandler* getInstance(const std::string& databaseName);

    bool insertTeacher(const std::string& fullname, const std::string& course,
        int monday, int tuesday, int wednesday, int thursday, int friday, int saturday);

    bool insertRoom(const std::string& name, const std::string& floor, const std::string& category);
    const RoomManagement getClassrooms();

    bool insertCourse(const std::string& courseName, const std::vector<std::string>& classNames);
};

#endif
