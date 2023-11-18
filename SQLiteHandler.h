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
    sqlite3* db;
    static SQLiteHandler dbHandler;
    RoomManagement classroomData;
    CourseManagement courseData;
    TeacherManagement teacherData;
public:
    SQLiteHandler();
    ~SQLiteHandler();

    static SQLiteHandler* getInstance();

    // Teachers
    bool insert(TeacherManagement& teacher);
    bool update(TeacherManagement& updatedTeacher);
    bool deleteData(TeacherManagement& teacher);
    TeacherManagement* getTeachers();

    // Classrooms
    bool insert(RoomManagement& room);
    bool update(RoomManagement& updatedClassroom);
    bool deleteData(RoomManagement& room);
    RoomManagement* getClassrooms();

    // Courses
    bool insert(CourseManagement& course);
    bool update(CourseManagement& updatedCourse);
    bool deleteData(CourseManagement& course);
    CourseManagement* getCourses();
};

#endif