#pragma once
#include <string>

using namespace std;
class MsgPack
{
	public:
		void addMsg(int type, string qq, string name, string msg, int64_t stime, string group, int mode);
		void addTip(string msg, int64_t stime, int mode, string classValue = "tips");
		string getData(int mode);
		string getData();
		MsgPack();
		~MsgPack();
};