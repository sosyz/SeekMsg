#include "sqlite3.h"
#include "sqlite3Tool.h"
#include <string>
#include <CQSDK.h>

static sqlite3* pdb;
using namespace std;

bool sqlite3Tool::exec(string sqlStr)
{
	char* errMsg = NULL;
	int ret = sqlite3_exec(pdb, sqlStr.c_str(), 0, 0, 0);
	sqlite3_free(errMsg);
	return ret == 0;
}

sqlite3_stmt *sqlite3Tool::prepare(string sqlStr)
{
	int ret = 0;
	sqlite3_stmt* stmt;
	ret = sqlite3_prepare_v2(pdb, sqlStr.c_str(), -1, &stmt, NULL);
	if (ret != 0) {
		finalize(stmt);
		return 0;
	}
	return stmt;
}

bool sqlite3Tool::open(string dir, bool isCraet)
{
	int tag = SQLITE_OPEN_READWRITE;
	if (isCraet) tag = tag | SQLITE_OPEN_CREATE;
	int ret = sqlite3_open_v2(dir.c_str(), &pdb, tag, NULL);
	if (ret != 0) return false;
	return exec("PRAGMA synchronous = OFF");
}


bool sqlite3Tool::beginTransaction()
{
	return exec("BEGIN");
}

bool sqlite3Tool::endTransaction()
{
	return exec("COMMIT");
}

bool sqlite3Tool::rollBackTransaction()
{
	return exec("ROLLBACK");
}


bool sqlite3Tool::addDatabase(string dir, string dbName)
{
	
	return false;
}

bool sqlite3Tool::bindText(sqlite3_stmt* stmt, int index, const char* value)
{
	return sqlite3_bind_text(stmt, index, value, strlen(value), 0) != 0 ? false : true;
}

bool sqlite3Tool::bindInt64(sqlite3_stmt* stmt, int index, sqlite3_int64 value)
{
	return sqlite3_bind_int64(stmt, index, value) != 0 ? false : true;
}

bool sqlite3Tool::bindInt(sqlite3_stmt* stmt, int index, int value)
{
	return sqlite3_bind_int(stmt, index, value) != 0 ? false : true;
}

int sqlite3Tool::step(sqlite3_stmt* stmt)
{
	return sqlite3_step(stmt);
}

string sqlite3Tool::readString(sqlite3_stmt* stmt, int index)
{
	return (char*)sqlite3_column_text(stmt, index);
}

int sqlite3Tool::readInt(sqlite3_stmt* stmt, int index)
{
	return sqlite3_column_int(stmt, index);
}

sqlite3_int64 sqlite3Tool::readInt64(sqlite3_stmt* stmt, int index)
{
	return sqlite3_column_int64(stmt, index);
}

double sqlite3Tool::readDouble(sqlite3_stmt* stmt, int index)
{
	return sqlite3_column_double(stmt, index);
}

bool sqlite3Tool::finalize(sqlite3_stmt* stmt)
{
	if (stmt == NULL) return false;
	return sqlite3_finalize(stmt) == 0;
}

bool sqlite3Tool::close()
{
	if (pdb == NULL) return false;
	int ret = sqlite3_close(pdb);
	return ret == 0;
}

int sqlite3Tool::errCode()
{
	return sqlite3_errcode(pdb);
}

string sqlite3Tool::errMsg()
{
	return sqlite3_errmsg(pdb);
}

bool sqlite3Tool::isTableExist(string tableName)
{
	sqlite3_stmt* stmt;
	int num = NULL;
	stmt = prepare("SELECT COUNT(*) FROM sqlite_master WHERE type = 'table' AND tbl_name = '" + tableName + "'");
	num = readInt(stmt, 0);
	finalize(stmt);
	return num != 0;
}