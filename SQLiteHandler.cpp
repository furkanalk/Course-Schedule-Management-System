#include "SQLiteHandler.h"
#include "course.h"
#include <stdexcept>
#include <sstream>

std::unique_ptr<SQLiteHandler> SQLiteHandler::instance = nullptr;

SQLiteHandler::SQLiteHandler(const std::string& dbName) : databaseName(dbName), db(nullptr) {
    int result = sqlite3_open_v2(databaseName.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
    if (result != SQLITE_OK)
        throw std::runtime_error("Failed to open database.");
}

SQLiteHandler::~SQLiteHandler() {
    sqlite3_close(db);
}

SQLiteHandler* SQLiteHandler::getInstance(const std::string databaseName) {
    if (instance == nullptr) {
        if (databaseName.empty()) {
            throw std::runtime_error("Database name is empty.");
        }
        instance = std::make_unique<SQLiteHandler>(databaseName);
    }
    return instance.get();
}

// Insert Teacher into DB
bool SQLiteHandler::insert(TeacherManagement& teacherData) {
    const std::string sqlQuery = "INSERT INTO teacher (fullname, course, monday, tuesday, wednesday, thursday, friday, saturday) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt_raw = nullptr;

    if (sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
        std::string errMsg = "Failed to prepare the SQL statement: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt_raw);
        throw std::runtime_error(errMsg);
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

    std::string fullname = teacherData.getNames()[0];
    std::string course = teacherData.getNames()[1];

    if (!teacherData.getNames().empty()) {
        if (sqlite3_bind_text(stmt.get(), 1, fullname.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
            throw std::runtime_error("Failed to bind parameter: fullname");
        if (teacherData.getNames().size() > 1 && sqlite3_bind_text(stmt.get(), 2, course.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
            throw std::runtime_error("Failed to bind parameter: course");
    }

    if (teacherData.getWorkdays().empty()) {
        throw std::runtime_error("No workdays selected");
        return false;
    }

    auto workdays = teacherData.getWorkdays()[0];

    for (size_t i = 0; i < workdays.size(); ++i) {
        if (sqlite3_bind_int(stmt.get(), i + 3, workdays[i]) != SQLITE_OK) {
            std::string error = "Failed to bind workday parameter for index " + std::to_string(i + 3);
            throw std::runtime_error(error);
            return false;
        }
    }

    if (sqlite3_step(stmt.get()) != SQLITE_DONE)
        throw std::runtime_error("Failed to execute the SQL statement: " + std::string(sqlite3_errmsg(db)));

    return true;
}

bool SQLiteHandler::update(TeacherManagement& updatedTeachers) {
    const std::string sqlUpdateTeacher = "UPDATE teacher SET fullname = ?, monday = ?, tuesday = ?, wednesday = ?, thursday = ?, friday = ?, saturday = ? WHERE id = ?;";
    sqlite3_stmt* stmt_raw = nullptr;

    for (size_t i = 0; i < updatedTeachers.getIds().size(); ++i) {
        if (sqlite3_prepare_v2(db, sqlUpdateTeacher.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
            std::string errMsg = "Failed to prepare the update statement: " + std::string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt_raw);
            throw std::runtime_error(errMsg);
        }

        std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

        if (sqlite3_bind_text(stmt.get(), 1, updatedTeachers.getNames()[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind name parameter for update.");
        }

        auto workdays = updatedTeachers.getWorkdays()[i];
        for (size_t j = 0; j < workdays.size(); ++j) {
            if (sqlite3_bind_int(stmt.get(), j + 2, workdays[j]) != SQLITE_OK) {
                throw std::runtime_error("Failed to bind workday parameter for update.");
            }
        }

        if (sqlite3_bind_int(stmt.get(), 8, updatedTeachers.getIds()[i]) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind ID parameter for update.");
        }

        if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
            throw std::runtime_error("Failed to execute the update statement: " + std::string(sqlite3_errmsg(db)));
        }

        sqlite3_reset(stmt.get());
    }

    return true;
}

bool SQLiteHandler::deleteData(TeacherManagement& teachers) {
    const std::string sqlDeleteTeacher = "DELETE FROM teacher WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    int teacherId = teachers.getCurrentId();

    if (sqlite3_prepare_v2(db, sqlDeleteTeacher.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, teacherId) != SQLITE_OK) {
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

TeacherManagement SQLiteHandler::getTeachers() {
    const std::string sqlQuery = "SELECT id, fullname, monday, tuesday, wednesday, thursday, friday, saturday FROM teacher;";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
    }

    teacherData.clear();
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        teacherData.addId(sqlite3_column_int(stmt, 0));
        teacherData.addName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));

        std::vector<int> weeklyWorkdays;
        for (int dayIndex = 2; dayIndex <= 7; ++dayIndex) {
            weeklyWorkdays.push_back(sqlite3_column_int(stmt, dayIndex) > 0);
        }
        teacherData.addWorkdays(weeklyWorkdays);
    }

    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE)
        throw std::runtime_error("Failed to retrieve all rows: " + std::string(sqlite3_errmsg(db)));

    return teacherData;
}

// Insert Course into DB
bool SQLiteHandler::insert(CourseManagement& courseData) {
    const std::string sqlInsertCourse = "INSERT INTO course (name, firstRoom, secondRoom) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt_raw = nullptr;

    if (sqlite3_prepare_v2(db, sqlInsertCourse.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
        std::string errMsg = "Failed to prepare the SQL statement: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt_raw);
        throw std::runtime_error(errMsg);
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

    std::string name = courseData.getNames()[0];
    std::vector<std::string> rooms = courseData.getRooms()[0];
    std::string roomFirst = rooms[0];
    std::string roomSecond = rooms.size() > 1 ? rooms[1] : roomFirst;

    if (sqlite3_bind_text(stmt.get(), 1, name.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        throw std::runtime_error("Failed to bind parameter: courseName");
    }

    if (!roomFirst.empty() && sqlite3_bind_text(stmt.get(), 2, roomFirst.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        throw std::runtime_error("Failed to bind parameter: firstRoom");
    }

    if (!roomSecond.empty() && sqlite3_bind_text(stmt.get(), 3, roomSecond.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        throw std::runtime_error("Failed to bind parameter: secondRoom");
    }

    if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
        throw std::runtime_error("Failed to execute the SQL statement: " + std::string(sqlite3_errmsg(db)));
    }

    return true;
}

// Update course
bool SQLiteHandler::update(CourseManagement& updatedCourses) {
    const std::string sqlUpdateCourse = "UPDATE course SET name = ?, firstRoom = ?, secondRoom = ? WHERE id = ?;";
    sqlite3_stmt* stmt_raw = nullptr;

    for (size_t i = 0; i < updatedCourses.getIds().size(); ++i) {
        if (sqlite3_prepare_v2(db, sqlUpdateCourse.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
            std::string errMsg = "Failed to prepare the update statement: " + std::string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt_raw);
            throw std::runtime_error(errMsg);
        }

        std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

        if (sqlite3_bind_text(stmt.get(), 1, updatedCourses.getNames()[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt.get(), 2, updatedCourses.getRooms()[i][0].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt.get(), 3, updatedCourses.getRooms()[i][1].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_int(stmt.get(), 4, updatedCourses.getIds()[i]) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind parameters for update.");
        }

        if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
            throw std::runtime_error("Failed to execute the update statement: " + std::string(sqlite3_errmsg(db)));
        }

        sqlite3_reset(stmt.get());
    }

    return true;
}

// Delete course
bool SQLiteHandler::deleteData(CourseManagement& courses) {
    const std::string sqlDeleteCourse = "DELETE FROM course WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    int courseId = courses.getCurrentId();

    if (sqlite3_prepare_v2(db, sqlDeleteCourse.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::string errMsg = "Failed to prepare delete statement: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw std::runtime_error(errMsg);
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt_unique_ptr(stmt, sqlite3_finalize);

    if (sqlite3_bind_int(stmt_unique_ptr.get(), 1, courseId) != SQLITE_OK) {
        std::string errMsg = "Failed to bind course ID for deletion: " + std::string(sqlite3_errmsg(db));
        throw std::runtime_error(errMsg);
    }

    if (sqlite3_step(stmt_unique_ptr.get()) != SQLITE_DONE) {
        std::string errMsg = "Failed to execute delete statement: " + std::string(sqlite3_errmsg(db));
        throw std::runtime_error(errMsg);
    }

    return true;
}

// Get Course from DB
CourseManagement SQLiteHandler::getCourses() {
    const std::string sqlQuery = "SELECT id, name, firstRoom, secondRoom FROM course;";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
    }

    courseData.clear();
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
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

    if (result != SQLITE_DONE) {
        throw std::runtime_error("Failed to retrieve all rows: " + std::string(sqlite3_errmsg(db)));
    }

    return courseData;
}

// Get Classsroom from DB
RoomManagement SQLiteHandler::getClassrooms() {
    const std::string sqlQuery = "SELECT id, name, floor, category FROM classroom;";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
    }

    classroomData.clear();
    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        classroomData.addId(sqlite3_column_int(stmt, 0));
        classroomData.addName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        classroomData.addFloor(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
        classroomData.addCategory(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
    }

    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE)
        throw std::runtime_error("Failed to retrieve all rows: " + std::string(sqlite3_errmsg(db)));

    return classroomData;
}

// Insert Classroom into DB
bool SQLiteHandler::insert(RoomManagement& roomData) {
    const std::string sqlInsertRoom = "INSERT INTO classroom (name, floor, category) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt_raw = nullptr;

    if (sqlite3_prepare_v2(db, sqlInsertRoom.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
        std::string errMsg = "Failed to prepare the SQL statement: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt_raw);
        throw std::runtime_error(errMsg);
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

    std::string name = roomData.getNames()[0];
    std::string floor = roomData.getFloors()[0];
    std::string category = roomData.getCategories()[0];

    if (sqlite3_bind_text(stmt.get(), 1, name.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt.get(), 2, floor.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt.get(), 3, category.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        throw std::runtime_error("Failed to bind parameters.");
    }

    if (sqlite3_step(stmt.get()) != SQLITE_DONE)
        throw std::runtime_error("Failed to execute the SQL statement: " + std::string(sqlite3_errmsg(db)));

    return true;
}

// Update Classroom
bool SQLiteHandler::update(RoomManagement& updatedClassrooms) {
    const std::string sqlUpdateRoom = "UPDATE classroom SET name = ?, floor = ?, category = ? WHERE id = ?;";
    sqlite3_stmt* stmt_raw = nullptr;

    for (size_t i = 0; i < updatedClassrooms.getIds().size(); ++i) {
        if (sqlite3_prepare_v2(db, sqlUpdateRoom.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
            std::string errMsg = "Failed to prepare the update statement: " + std::string(sqlite3_errmsg(db));
            sqlite3_finalize(stmt_raw);
            throw std::runtime_error(errMsg);
        }

        std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

        if (sqlite3_bind_text(stmt.get(), 1, updatedClassrooms.getNames()[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt.get(), 2, updatedClassrooms.getFloors()[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(stmt.get(), 3, updatedClassrooms.getCategories()[i].c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_int(stmt.get(), 4, updatedClassrooms.getIds()[i]) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind parameters for update.");
        }

        if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
            throw std::runtime_error("Failed to execute the update statement: " + std::string(sqlite3_errmsg(db)));
        }

        sqlite3_reset(stmt.get());
    }

    return true;
}

// Delete Classroom
bool SQLiteHandler::deleteData(RoomManagement& rooms) {
    const std::string sqlDeleteRoom = "DELETE FROM classroom WHERE id = ?;";
    sqlite3_stmt* stmt = nullptr;

    int roomId = rooms.getCurrentId();

    if (sqlite3_prepare_v2(db, sqlDeleteRoom.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return false;
    }

    if (sqlite3_bind_int(stmt, 1, roomId) != SQLITE_OK) {
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