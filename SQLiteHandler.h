#ifndef SQLITE_HANDLER_H
#define SQLITE_HANDLER_H

#include "sqlite3.h"
#include "user.h"
#include "TeacherManagement.h"
#include "CourseManagement.h"
#include "RoomManagement.h"
#include "schedule.h"
#include <string>
#include <vector>

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
    bool operator+=(TeacherManagement& teacher);
    bool update(TeacherManagement& updatedTeacher);
    bool operator-=(TeacherManagement& teacher);
    TeacherManagement* getTeachers();
    TeacherManagement* getTeacherByName(std::string teacherName);
    std::string getTeacherByCourseName(std::string courseName);

    // Classrooms
    bool operator+=(RoomManagement& room);
    bool update(RoomManagement& updatedClassroom);
    bool operator-=(RoomManagement& room);
    RoomManagement* getClassrooms();
    int getRoomCount();
    int getRoomIdByName(std::string roomName);

    // Courses
    bool operator+=(CourseManagement& course);
    bool update(CourseManagement& updatedCourse);
    bool operator-=(CourseManagement& course);
    CourseManagement* getCourses();
};

#endif