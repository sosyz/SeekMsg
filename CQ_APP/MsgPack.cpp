#include "MsgPack.h"
/*
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
*/

#include "tools.h"

#include <time.h>

//using namespace rapidjson;
using namespace std;


//StringBuffer s;
//Writer<StringBuffer> json(s);
int index;
string ret;

void MsgPack::addMsg(int type, string qq, string name, string msg, int64_t stime, string group, int mode)
{
	switch (mode)
	{
	case 1:
		/*
		json.StartArray();

		json.Key("type");
		json.Int(type);
		json.Key("index");
		json.Int(index);
		json.Key("unix");
		json.Int64(time == 0 ? time(0) : stime);
		json.Key("qq");
		json.String(qq.c_str());
		json.Key("name");
		json.String(qq.c_str());
		json.Key("msg");
		json.String(reCQ(rehtml(msg), getName(atoll(group.c_str()), atoll(qq.c_str())), group, mode).c_str());

		json.EndArray();
		*/
		break;
	case 2:
		ret = "\n" + ret;
		ret = reCQ(rehtml(msg), name, group, mode) + ret;
		ret = "\n" + ret;
		ret = TimeToString(time == 0 ? time(0) : stime) + ret;
		ret = "  " + ret;
		ret = name + ret;
		break;
	default:
		break;
	}
	index++;
}

void MsgPack::addTip(string msg, int64_t stime, int mode, string classValue)
{
	switch (mode)
	{
	case 1:
		/*
		json.StartArray();

		json.Key("type");
		json.Int(3);
		json.Key("index");
		json.Int(index);
		json.Key("unix");
		json.Int64(time == 0 ? time(0) : stime);
		json.Key("class");
		json.String(classValue.c_str());
		json.Key("msg");
		json.String(msg.c_str());

		json.EndArray();
		*/
		break;
	default:
		break;
	}
	index++;
}

string MsgPack::getData(int mode)
{
	switch (mode)
	{
	case 1:
		return "";//s.GetString();
	case 2:
		return ret.substr(0, ret.length() - strlen("\n"));
	}
	return "";
}

string MsgPack::getData()
{
	return "";// s.GetString();
}

MsgPack::MsgPack()
{
	//json.StartObject();
	index = 0;
	ret = "";
}

MsgPack::~MsgPack()
{
	//json.EndObject();
}
