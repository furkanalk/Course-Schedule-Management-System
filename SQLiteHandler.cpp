#include "SQLiteHandler.h"
#include <sstream>
#include <windows.h>

SQLiteHandler::SQLiteHandler(const std::string& dbName)
    : databaseName(dbName), db(nullptr)
{
    openDatabase();
}

SQLiteHandler::~SQLiteHandler() {
    closeDatabase();
}

std::wstring SQLiteHandler::convertToWideString(const std::string& str) {
    int bufferLength = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
    std::wstring wstr(bufferLength, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &wstr[0], bufferLength);
    return wstr;
}

bool SQLiteHandler::openDatabase() {
    if (db) return true;  // already open
    return sqlite3_open_v2(databaseName.c_str(), &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL) == SQLITE_OK;
}

bool SQLiteHandler::closeDatabase() {
    if (!db) return true;  // already closed
    return sqlite3_close(db) == SQLITE_OK;
}

bool SQLiteHandler::insertTeacher(const std::string& fullname, const std::string& course, int monday, int tuesday, int wednesday, int thursday, int friday, int saturday) {
    const std::string sqlQuery = "INSERT INTO teacher (fullname, course, monday, tuesday, wednesday, thursday, friday, saturday) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt* stmt_raw;
    if (sqlite3_prepare_v2(db, sqlQuery.c_str(), -1, &stmt_raw, nullptr) != SQLITE_OK) {
        showError("Failed to prepare the SQL statement: " + std::string(sqlite3_errmsg(db)));
        return false;
    }

    // RAII for sqlite3_stmt management
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> stmt(stmt_raw, sqlite3_finalize);

    sqlite3_bind_text(stmt.get(), 1, fullname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt.get(), 2, course.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt.get(), 3, monday);
    sqlite3_bind_int(stmt.get(), 4, tuesday);
    sqlite3_bind_int(stmt.get(), 5, wednesday);
    sqlite3_bind_int(stmt.get(), 6, thursday);
    sqlite3_bind_int(stmt.get(), 7, friday);
    sqlite3_bind_int(stmt.get(), 8, saturday);

    if (sqlite3_step(stmt.get()) != SQLITE_DONE) {
        showError("Failed to execute the SQL statement: " + std::string(sqlite3_errmsg(db)));
        return false;
    }
    return true;
}

void SQLiteHandler::showError(const std::string& message) {
    std::wstring wideErrorMessage = convertToWideString(message);
    MessageBox(NULL, wideErrorMessage.c_str(), L"SQLite Error", MB_OK | MB_ICONERROR);
}
