#include "userTable.h"
#include "sqlite3Tool.h"
#include "au.h"

#include <CQSDK.h>

static Logger logger("UsetTable");
using namespace std;
static sqlite3Tool db;

int userTable::init()
{
	if (db.open(":memory:", true)) {
		if (!db.isTableExist("data")) db.exec(
			"CREATE TABLE \"UserTable\" (\
			\"id\" INTEGER PRIMARY KEY AUTOINCREMENT,\
			\"qq\" BigInt,\
			\"name\" TEXT,\
			\"value\" TEXT,\
			\"type\" Integer);"
		);
	}
	else {
		logger.Error("Creat UserTable Error");
	}
	return 0;
}

int userTable::addUser(int type, int64_t qq, string name, string value)
{
	sqlite3_stmt* stmt = db.prepare("INSERT INTO UserTable(`qq`,`name`,`value`,`type`) VALUES(?,?,?,?)");
	db.bindInt64(stmt, 1, qq);
	db.bindText(stmt, 2, AnsiToUtf8(name.c_str()));
	db.bindText(stmt, 3, AnsiToUtf8(value.c_str()));
	db.bindInt(stmt, 4, type);
	if (db.step(stmt) != SQLITE_DONE) logger.Error("执行SQL语句失败，所处函数: " + string(__func__) + ", 错误代码: " + to_string(db.errCode()) + ", 错误信息: " + db.errMsg());
	db.finalize(stmt);
	return 0;
}

int userTable::deleteUser(int type, int64_t qq, string name, bool all)
{
	sqlite3_stmt* stmt = db.prepare("DELETE FROM UserTable WHERE `qq` = ? AND `type` = ? AND `name` = ?");
	db.bindInt64(stmt, 1, qq);
	db.bindInt(stmt, 2, type);
	db.bindText(stmt, 3, all ? "%%" : AnsiToUtf8(name.c_str()));
	if (db.step(stmt) != SQLITE_DONE) logger.Error("执行SQL语句失败，所处函数: " + string(__func__) + ", 错误代码: " + to_string(db.errCode()) + ", 错误信息: " + db.errMsg());
	db.finalize(stmt);
	return 0;
}

string userTable::getUser(int type, int64_t qq, std::string name)
{
	sqlite3_stmt* stmt = db.prepare("SELECT `value` FROM UserTable WHERE `qq` = ? AND `type` = ? AND `name` = ?");
	db.bindInt64(stmt, 1, qq);
	db.bindInt(stmt, 2, type);
	db.bindText(stmt, 3, AnsiToUtf8(name.c_str()));
	if (db.step(stmt) != SQLITE_DONE) logger.Error("执行SQL语句失败，所处函数: " + string(__func__) + ", 错误代码: " + to_string(db.errCode()) + ", 错误信息: " + db.errMsg());
	string ret = db.readString(stmt, 0);
	db.finalize(stmt);
	return ret;
}

int userTable::setUser(int type, int64_t qq, string name, string value)
{
	sqlite3_stmt* stmt = db.prepare("UPDATE UserTable SET `value` = ? WHERE `qq` = ? AND `name` = ? AND `type` = ?");
	db.bindText(stmt, 1, AnsiToUtf8(value.c_str()));
	db.bindInt64(stmt, 2, qq);
	db.bindText(stmt, 3, AnsiToUtf8(name.c_str()));
	db.bindInt(stmt, 4, type);
	if (db.step(stmt) != SQLITE_DONE) logger.Error("执行SQL语句失败，所处函数: " + string(__func__) + ", 错误代码: " + to_string(db.errCode()) + ", 错误信息: " + db.errMsg());
	db.finalize(stmt);
	return 0;
}

bool userTable::close()
{
	return db.close();
}
