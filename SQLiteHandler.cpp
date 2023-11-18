#include "SQLiteHandler.h"
#include "course.h"

SQLiteHandler SQLiteHandler::dbHandler;

SQLiteHandler::SQLiteHandler() {
    int result = sqlite3_open_v2("courseScheduleDB.sqlite", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
}

SQLiteHandler::~SQLiteHandler() {
    sqlite3_close(db);
}

SQLiteHandler* SQLiteHandler::getInstance() {
    return &dbHandler;
}

// Insert Teacher into DB
bool SQLiteHandler::insert(TeacherManagement& teacher) {
    std::string query = "INSERT INTO teacher (fullname, course, monday, tuesday, wednesday, thursday, friday, saturday) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    std::string fullname = teacher.getNames()[0];
    std::string course = teacher.getNames()[1];

    if (teacher.getNames().empty() || teacher.getWorkdays().empty()) {
        return false;
    }

    if (sqlite3_bind_text(stmt, 1, fullname.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_bind_text(stmt, 2, course.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    auto workdays = teacher.getWorkdays()[0];

    for (int i = 0; i < workdays.size(); i++) {
        if (sqlite3_bind_int(stmt, i + 3, workdays[i]) != SQLITE_OK) {
            sqlite3_finalize(stmt);
            return false;
        }
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Update teacher
bool SQLiteHandler::update(TeacherManagement& updatedTeacher) {
    std::string query = "UPDATE teacher SET fullname = ?, monday = ?, tuesday = ?, wednesday = ?, thursday = ?, friday = ?, saturday = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    int index = updatedTeacher.getCurrentIndex();
    int id  = updatedTeacher.getCurrentId();

    if (sqlite3_bind_text(stmt, 1, updatedTeacher.getNames()[index].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (updatedTeacher.getWorkdays()[index].empty()) {
        sqlite3_finalize(stmt);
        return false;
    }

    auto workdays = updatedTeacher.getWorkdays()[index];
    for (int i = 0; i < workdays.size(); i++) {
        if (sqlite3_bind_int(stmt, i + 2, workdays[i]) != SQLITE_OK) {
            sqlite3_finalize(stmt);
            return false;
        }
    }

    if (sqlite3_bind_int(stmt, 8, id) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Delete teacher from DB
bool SQLiteHandler::deleteData(TeacherManagement& teacher) {
    std::string query = "DELETE FROM teacher WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, teacher.getCurrentId()) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Get teachers from DB
TeacherManagement* SQLiteHandler::getTeachers() {
    std::string query = "SELECT id, fullname, monday, tuesday, wednesday, thursday, friday, saturday FROM teacher;";
    sqlite3_stmt* stmt;
    int check;

    if (check = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
    }

    teacherData.clear();
    while ((check = sqlite3_step(stmt)) == SQLITE_ROW) {
        teacherData.addId(sqlite3_column_int(stmt, 0));
        teacherData.addName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
    
        std::vector<int> weeklyWorkdays;
        for (int i = 2; i <= 7; i++) {
            weeklyWorkdays.push_back(sqlite3_column_int(stmt, i) > 0);
        }
        teacherData.addWorkdays(weeklyWorkdays);
    }
    
    sqlite3_finalize(stmt);
    return &teacherData;
}

// Insert Course into DB
bool SQLiteHandler::insert(CourseManagement& courseData) {
    std::string query = "INSERT INTO course (name, firstRoom, secondRoom) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    std::string name = courseData.getNames()[0];
    std::vector<std::string> rooms = courseData.getRooms()[0];
    std::string roomFirst = rooms[0];
    std::string roomSecond = rooms.size() > 1 ? rooms[1] : roomFirst;

    if (sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (!roomFirst.empty() && sqlite3_bind_text(stmt, 2, roomFirst.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (!roomSecond.empty() && sqlite3_bind_text(stmt, 3, roomSecond.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Update course
bool SQLiteHandler::update(CourseManagement& updatedCourse) {
    std::string query = "UPDATE course SET name = ?, firstRoom = ?, secondRoom = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    int index = updatedCourse.getCurrentIndex();
    int id = updatedCourse.getCurrentId();

    if (sqlite3_bind_text(stmt, 1, updatedCourse.getNames()[index].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, updatedCourse.getRooms()[index][0].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, updatedCourse.getRooms()[index][1].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 4, id) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}


// Delete course
bool SQLiteHandler::deleteData(CourseManagement& courses) {
    std::string query = "DELETE FROM course WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, courses.getCurrentId()) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

// Get Course from DB
CourseManagement* SQLiteHandler::getCourses() {
    std::string query = "SELECT id, name, firstRoom, secondRoom FROM course;";
    sqlite3_stmt* stmt;
    int check;

    if (check = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
    }

    courseData.clear();
    while ((check = sqlite3_step(stmt)) == SQLITE_ROW) {
        courseData.addId(sqlite3_column_int(stmt, 0));
        courseData.addName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));

        std::vector<std::string> roomNames;
        std::string firstRoomName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        std::string secondRoomName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        roomNames.push_back(firstRoomName);
        roomNames.push_back(secondRoomName);
        courseData.addRooms(roomNames);
    }

    sqlite3_finalize(stmt);
    return &courseData;
}

// Get Classsroom from DB
RoomManagement* SQLiteHandler::getClassrooms() {
    std::string query = "SELECT id, name, floor, category FROM classroom;";
    sqlite3_stmt* stmt;
    int check;

    if (check = sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
    }

    classroomData.clear();
    while ((check = sqlite3_step(stmt)) == SQLITE_ROW) {
        classroomData.addId(sqlite3_column_int(stmt, 0));
        classroomData.addName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        classroomData.addFloor(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
        classroomData.addCategory(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
    }

    sqlite3_finalize(stmt);
    return &classroomData;
}

// Insert Classroom into DB
bool SQLiteHandler::insert(RoomManagement& room) {
    std::string sqlInsertRoom = "INSERT INTO classroom (name, floor, category) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sqlInsertRoom.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    std::string name = room.getNames()[0];
    std::string floor = room.getFloors()[0];
    std::string category = room.getCategories()[0];

    if (sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, floor.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, category.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
    {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }
        
    sqlite3_finalize(stmt);
    return true;
}

// Update Classroom
bool SQLiteHandler::update(RoomManagement& updatedClassroom) {
    std::string query = "UPDATE classroom SET name = ?, floor = ?, category = ? WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    int index = updatedClassroom.getCurrentIndex();
    int id = updatedClassroom.getCurrentId();

    if (sqlite3_bind_text(stmt, 1, updatedClassroom.getNames()[index].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 2, updatedClassroom.getFloors()[index].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_text(stmt, 3, updatedClassroom.getCategories()[index].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
        sqlite3_bind_int(stmt, 4, id) != SQLITE_OK) 
    {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}


// Delete Classroom
bool SQLiteHandler::deleteData(RoomManagement& room) {
    std::string query = "DELETE FROM classroom WHERE id = ?;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, room.getCurrentId()) != SQLITE_OK) {
        sqlite3_finalize(stmt);
        return false;
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}