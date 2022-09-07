#pragma once

#include "sqlite3Tool.h"
#include <string>

class historyRecord
{
	public:
        int init();
        int saveGroupChat(int64_t group, int64_t qq, std::string msg, int msgType, int64_t time, string other = "");
        int savePrivateChat(int64_t qq, std::string msg, int msgType, int64_t tm, string other);
        int saveBotSend(int64_t group, std::string msg, int msgType, int64_t tm = 0, string other = "");
        int close();
};