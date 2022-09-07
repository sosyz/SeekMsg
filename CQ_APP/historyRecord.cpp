#define CQDEBUG
#include <CQSDK.h>
#include <time.h>

#include "sqlite3Tool.h"
#include "historyRecord.h"
#include "au.h"

constexpr auto CQ_MSGTYPE_GROUP = 1;
constexpr auto CQ_MSGTYPE_Private = 2;

static Logger logger("SaveChat");

static sqlite3Tool db;

int historyRecord::init()
{
	string dir = getAppDirectory() + "chat.db";
	if (db.open(AnsiToUtf8(dir.c_str()), true)) {
		if (!db.isTableExist("data"))db.exec(
			"CREATE TABLE \"data\" (\
			\"id\" integer NOT NULL PRIMARY KEY AUTOINCREMENT,\
			\"type\" integer,\
			\"msgType\" integer,\
			\"fGroup\" integer,\
			\"fQQ\" integer,\
			\"time\" integer,\
			\"msg\" TEXT,\
			\"other\" TEXT);"
		);
	}
	else {
		logger.Error("Open chat database error");
	}
	return 0;
}

int historyRecord::saveGroupChat(int64_t group, int64_t qq, std::string msg, int msgType, int64_t tm, string other)
{
	if (tm == 0) tm = time(0);
	sqlite3_stmt* stmt = db.prepare("INSERT INTO data (`type`, `fGroup`, `fQQ`, `time`, `msg`, `other`, `msgType`) VALUES (?, ?, ?, ?, ?, ?, ?)");
	db.bindInt(stmt, 1, CQ_MSGTYPE_GROUP);
	db.bindInt64(stmt, 2, group);
	db.bindInt64(stmt, 3, qq);
	db.bindInt64(stmt, 4, tm);
	db.bindText(stmt, 5, msg.c_str());
	db.bindText(stmt, 6, other.c_str());
	db.bindInt(stmt, 7, msgType);
	if (db.step(stmt) != SQLITE_DONE) logger.Error("执行SQL语句失败，所处函数: " + string(__func__) + ", 错误代码: " + to_string(db.errCode()) + ", 错误信息: " + db.errMsg());
	db.finalize(stmt);
	return 0;
}

int historyRecord::savePrivateChat(int64_t qq, std::string msg, int msgType, int64_t tm, string other)
{
	if (tm == 0) tm = time(0);
	sqlite3_stmt* stmt = db.prepare("INSERT INTO data (`type`, `fGroup`, `fQQ`, `time`, `msg`, `other`, `msgType`) VALUES (?, ?, ?, ?, ?, ?, ?)");
	db.bindInt(stmt, 1, CQ_MSGTYPE_Private);
	db.bindInt64(stmt, 2, 0);
	db.bindInt64(stmt, 3, qq);
	db.bindInt64(stmt, 4, tm);
	db.bindText(stmt, 5, AnsiToUtf8(msg.c_str()));
	db.bindText(stmt, 6, AnsiToUtf8(other.c_str()));
	db.bindInt(stmt, 7, msgType);
	if (db.step(stmt) != SQLITE_DONE) logger.Error("执行SQL语句失败，所处函数: " + string(__func__) + ", 错误代码: " + to_string(db.errCode()) + ", 错误信息: " + db.errMsg());
	db.finalize(stmt);
	return 0;
}

int historyRecord::saveBotSend(int64_t group, std::string msg, int msgType, int64_t tm, string other)
{
	if (tm == 0) tm = time(0);
	sqlite3_stmt* stmt = db.prepare("INSERT INTO data (`type`, `fGroup`, `fQQ`, `time`, `msg`, `other`, `msgType`) VALUES (?, ?, ?, ?, ?, ?, ?)");
	db.bindInt(stmt, 1, CQ_MSGTYPE_Private);
	db.bindInt64(stmt, 2, 0);
	db.bindInt64(stmt, 3, getLoginQQ());
	db.bindInt64(stmt, 4, tm);
	db.bindText(stmt, 5, AnsiToUtf8(msg.c_str()));
	db.bindText(stmt, 6, AnsiToUtf8(other.c_str()));
	db.bindInt(stmt, 7, msgType);
	if (db.step(stmt) != SQLITE_DONE) logger.Error("执行SQL语句失败，所处函数: " + string(__func__) + ", 错误代码: " + to_string(db.errCode()) + ", 错误信息: " + db.errMsg());
	db.finalize(stmt);
	return 0;
	return 0;
}

int historyRecord::close()
{
	db.close();
	return 0;
}
