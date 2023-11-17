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
    RoomManagement classroomData;
    CourseManagement courseData;
    TeacherManagement teacherData;

    SQLiteHandler(const std::string& databaseName);
    SQLiteHandler(const SQLiteHandler&) = delete;
    SQLiteHandler& operator=(const SQLiteHandler&) = delete;

    ~SQLiteHandler();

    static SQLiteHandler* getInstance(const std::string databaseName);

    bool insert(TeacherManagement& teacherData);
    bool update(TeacherManagement& updatedTeachers);
    bool deleteData(TeacherManagement& teachers);
    TeacherManagement getTeachers();

    bool insert(RoomManagement& roomData);
    bool update(RoomManagement& updatedClassrooms);
    bool deleteData(RoomManagement& rooms);
    RoomManagement getClassrooms();

    bool insert(CourseManagement& courseData);
    bool update(CourseManagement& updatedCourses);
    bool deleteData(CourseManagement& courses);
    CourseManagement getCourses();
};

#endif