#pragma once

#include "sqlite3.h"
#include <string>

using namespace std;

class sqlite3Tool
{
public:
    //≥ı ºªØsql
    bool open(string dir = ":memory:", bool isCraet = false);
    bool isTableExist(string tableName);
    bool beginTransaction();
    bool endTransaction();
    bool rollBackTransaction();
    bool exec(string sqlStr);
    bool addDatabase(string dir, string dbName);
    sqlite3_stmt *prepare(string sqlStr);
    bool bindText(sqlite3_stmt* stmt, int index, const char* value);
    bool bindInt64(sqlite3_stmt* stmt, int index, sqlite3_int64 value);
    bool bindInt(sqlite3_stmt* stmt, int index, int value);
    int step(sqlite3_stmt* stmt);
    string readString(sqlite3_stmt* stmt, int index);
    int readInt(sqlite3_stmt* stmt, int index);
    sqlite3_int64 readInt64(sqlite3_stmt* stmt, int index);
    double readDouble(sqlite3_stmt* stmt, int index);
    bool finalize(sqlite3_stmt* stmt);
    bool close();
    int errCode();
    string errMsg();
};