#include <string>
#include <CQSDK.h>
#include <time.h>
#include <fstream>
#include "sqlite3Tool.h"
#include "tools.h"

extern historyRecord save;
extern bool load;

using namespace std;

EVE_GroupMsg_EX(GroupMsg_EX)
//name:群聊消息
//priority:30000
{
	if (!load)
		return;
	if (e.isAnonymous()) {//判断是否为匿名信息
		auto info = e.getFromAnonymousInfo();//获取匿名信息
		//Anonymous https://pub.idqqimg.com/pc/group/anony/portrait/img/2290001.png
		save.saveGroupChat(e.fromGroup, info.AID, e.message.c_str(), CQ_MSG_TYPE_TEXT, time(0), "Anonymous");
	}
	else {
		save.saveGroupChat(e.fromGroup, e.fromAccount, e.message.c_str(), CQ_MSG_TYPE_TEXT, time(0), "");
	}
}

EVE_PrivateMsg_EX(PrivateMsg_EX)
//name:正则消息
//priority:30000
//regex-key:qq
//regex-key:num
//regex-key:group
//regex-expression:^(?<qq>\d+)在群*(?<group>\d+)\s*最近(?<num>\d+)条消息$
{

	auto reg = e.regexMsg();
	e.sendMsg(Utf8ToAnsi(getListory2(atoll(reg["qq"].c_str()), atoll(reg["group"].c_str()), atoi(reg["num"].c_str())).c_str()));
	//sendPrivateMsg(814146039, readCQFile("C:\\Users\\81414\\Desktop\\cards_1585125580.txt"));
	
	//logger.Debug(readCQFileContext());
	/*
	if (e.message == "test") {
		auto debug = logger.Debug();
		
		debug << DEBUGINFO << s << endl;
		debug << DEBUGINFO << db.isTableExist("testddd") << endl;
		debug << DEBUGINFO << db.close() << endl;
		debug << send;
	}
	logger.Debug() << DEBUGINFO << e.message;
	Msg(&e);//请跳转到Msg函数查看如何发送回复
	*/
}
//使用酷Q原生自带的正则消息捕获
EVE_GroupMsg_EX(GroupMsg_help)
//name:正则消息
//priority:30000
//regex-key:Account
//regex-key:action
//regex-expression:^撤回查询$
{
	e.sendMsg() << "[CQ:at,qq=" << e.fromAccount << "]\nHelp:" << "\n" << "①进入查询(暂不可用)" << "\n" << "②[QQ]最近[数目]条消息(QQ号可以用艾特代替)" << "\n" << "③回退[数目]条消息" << send;
}


EVE_GroupMsg_EX(GroupMsg_use)
//name:正则消息
//priority:30000
//regex-key:qq
//regex-key:num
//regex-expression:^(\[CQ:at.*?)*(?<qq>\d+)(\])*\s*最近(?<num>\d+)条消息$
{
	auto reg = e.regexMsg();
	
	e.sendMsg(Utf8ToAnsi(getListory2(atoll(reg["qq"].c_str()), e.fromGroup, atoi(reg["num"].c_str())).c_str()));
}

using namespace std;
extern historyRecord save;

EVE_System_GroupAdmin_EX(System_GroupAdmin_EX) {
	if (!load)
		return;
	save.saveGroupChat(e.fromGroup, 1, "qq=" + to_string(e.beingOperateAccount) + ",type=" + to_string(e.subType), CQ_MSG_TYPE_ADMIN, e.sendTime, "");
}

EVE_GroupUpload_EX(GroupUpload_EX) {
	if (!load)
		return;
	save.saveGroupChat(e.fromGroup, 1, "name=" + e.文件_名称 + ",size=" + to_string(e.文件_大小), CQ_MSG_TYPE_UPLOADFILE, e.sendTime, "");
}
EVE_System_GroupMemberDecrease_EX(System_GroupMemberDecrease_EX) {

}
EVE_System_GroupMemberIncrease_EX(System_GroupMemberIncrease_EX) {

}

static Logger logger("群禁言事件");
EVE_System_GroupBan_EX(System_GroupBan_EX)
//name:禁言消息
//priority:30000
{
	/*
	auto debug = logger.Debug();


	auto gInfo = getGroupInfo(e.fromGroup);
	debug << "[" << gInfo.名称 << "]的";

	if (e.beingOperateAccount == 0) {
		debug << "[全体]";
	}
	else {
		auto qInfo = getGroupMemberInfo(e.fromGroup, e.beingOperateAccount);
		debug << "[" << qInfo.名片 << "(" << e.beingOperateAccount << ")]";
	}

	auto qInfo = getGroupMemberInfo(e.fromGroup, e.fromAccount);
	debug << "被[" << gInfo.名称 << "(" << e.fromAccount << ")]";

	switch (e.subType)
	{
	case 1:
		debug << "解除禁言";
		break;
	case 2:
		debug << "禁言" << e.duration << "秒";
		break;
	default:
		debug << "进行了未知操作(" << e.subType << ")";
		break;
	}
	debug << send;
	*/
}

EVE_Menu_EX(menu)
//name:设置
{
}