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

SQLiteHandler* SQLiteHandler::getInstance(const std::string& databaseName) {
    if (instance == nullptr) {
        if (databaseName.empty()) {
            throw std::runtime_error("Database name is empty.");
        }
        instance = std::make_unique<SQLiteHandler>(databaseName);
    }
    return instance.get();
}

// Insert Teacher to DB
bool SQLiteHandler::insertTeacher(const std::string& fullname, const std::string& course,
    int monday, int tuesday, int wednesday, int thursday, int friday, int saturday) {
    const std::string sqlQuery = "INSERT INTO teacher (fullname, course, monday, tuesday, wednesday, thursday, friday, saturday) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt_raw = nullptr;
    if (sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
        std::string errMsg = "Failed to prepare the SQL statement: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt_raw);
        throw std::runtime_error(errMsg);
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

    if (sqlite3_bind_text(stmt.get(), 1, fullname.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
        throw std::runtime_error("Failed to bind parameter: fullname");
    
    if (sqlite3_bind_text(stmt.get(), 2, course.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
        throw std::runtime_error("Failed to bind parameter: course");
    
    if (sqlite3_bind_int(stmt.get(), 3, monday) != SQLITE_OK)
        throw std::runtime_error("Failed to bind parameter: monday");
   
    if (sqlite3_bind_int(stmt.get(), 4, tuesday) != SQLITE_OK)
        throw std::runtime_error("Failed to bind parameter: tuesday");
    
    if (sqlite3_bind_int(stmt.get(), 5, wednesday) != SQLITE_OK)
        throw std::runtime_error("Failed to bind parameter: wednesday");
    
    if (sqlite3_bind_int(stmt.get(), 6, thursday) != SQLITE_OK)
        throw std::runtime_error("Failed to bind parameter: thursday");
    
    if (sqlite3_bind_int(stmt.get(), 7, friday) != SQLITE_OK)
        throw std::runtime_error("Failed to bind parameter: friday");

    if (sqlite3_bind_int(stmt.get(), 8, saturday) != SQLITE_OK)
        throw std::runtime_error("Failed to bind parameter: saturday");
 
    if (sqlite3_step(stmt.get()) != SQLITE_DONE)
        throw std::runtime_error("Failed to execute the SQL statement: " + std::string(sqlite3_errmsg(db)));

    return true;
}

// Insert Course to DB
bool SQLiteHandler::insertCourse(const std::string& courseName, const std::vector<std::string>& roomIds) {
    const std::string sqlInsertCourse = "INSERT INTO course (name, firstRoom, secondRoom) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt_raw = nullptr;

    if (sqlite3_prepare_v2(db, sqlInsertCourse.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
        std::string errMsg = "Failed to prepare the SQL statement: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt_raw);
        throw std::runtime_error(errMsg);
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

    if (sqlite3_bind_text(stmt.get(), 1, courseName.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        throw std::runtime_error("Failed to bind parameter: courseName");
    }

    try {
        int firstRoomId = std::stoi(roomIds[0]);
        if (sqlite3_bind_int(stmt.get(), 2, firstRoomId) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind parameter: firstRoom");
        }
    }
    catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid room ID for firstRoom: " + roomIds[0]);
    }

    try {
        int secondRoomId = std::stoi(roomIds[1]);
        if (sqlite3_bind_int(stmt.get(), 3, secondRoomId) != SQLITE_OK) {
            throw std::runtime_error("Failed to bind parameter: secondRoom");
        }
    }
    catch (const std::invalid_argument& e) {
        throw std::runtime_error("Invalid room ID for secondRoom: " + roomIds[1]);
    }

    if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
        throw std::runtime_error("Failed to execute the SQL statement: " + std::string(sqlite3_errmsg(db)));
    }

    return true;
}

// Get Classsroom from DB
const RoomManagement SQLiteHandler::getClassrooms() {
    RoomManagement classroomData;
    const std::string sqlQuery = "SELECT id, name, floor, category FROM classroom;";
    sqlite3_stmt* stmt;

    int result = sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt, nullptr);
    if (result != SQLITE_OK) {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Failed to execute statement: " + std::string(sqlite3_errmsg(db)));
    }

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) {
        classroomData.addId(sqlite3_column_int(stmt, 0)); // Adding the ID
        classroomData.addName(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)))); // Adding the name
        classroomData.addFloor(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)))); // Adding the floor
        classroomData.addCategory(std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)))); // Adding the category
    }

    sqlite3_finalize(stmt);

    if (result != SQLITE_DONE)
        throw std::runtime_error("Failed to retrieve all rows: " + std::string(sqlite3_errmsg(db)));

    return classroomData;
}

// Insert Classroom to DB
bool SQLiteHandler::insertRoom(const std::string& name, const std::string& floor, const std::string& category) {
    const std::string sqlInsertRoom = "INSERT INTO classroom (name, floor, category) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt_raw = nullptr;

    if (sqlite3_prepare_v2(db, sqlInsertRoom.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
        std::string errMsg = "Failed to prepare the SQL statement: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt_raw);
        throw std::runtime_error(errMsg);
    }

    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

    if (sqlite3_bind_text(stmt.get(), 1, name.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt.get(), 2, floor.c_str(), -1, SQLITE_STATIC) != SQLITE_OK ||
        sqlite3_bind_text(stmt.get(), 3, category.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
        throw std::runtime_error("Failed to bind parameters.");
    }

    if (sqlite3_step(stmt.get()) != SQLITE_DONE)
        throw std::runtime_error("Failed to execute the SQL statement: " + std::string(sqlite3_errmsg(db)));

    return true;
}
