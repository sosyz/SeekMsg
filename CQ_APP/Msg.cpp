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
//name:Ⱥ����Ϣ
//priority:30000
{
	if (!load)
		return;
	if (e.isAnonymous()) {//�ж��Ƿ�Ϊ������Ϣ
		auto info = e.getFromAnonymousInfo();//��ȡ������Ϣ
		//Anonymous https://pub.idqqimg.com/pc/group/anony/portrait/img/2290001.png
		save.saveGroupChat(e.fromGroup, info.AID, e.message.c_str(), CQ_MSG_TYPE_TEXT, time(0), "Anonymous");
	}
	else {
		save.saveGroupChat(e.fromGroup, e.fromAccount, e.message.c_str(), CQ_MSG_TYPE_TEXT, time(0), "");
	}
}

EVE_PrivateMsg_EX(PrivateMsg_EX)
//name:������Ϣ
//priority:30000
//regex-key:qq
//regex-key:num
//regex-key:group
//regex-expression:^(?<qq>\d+)��Ⱥ*(?<group>\d+)\s*���(?<num>\d+)����Ϣ$
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
	Msg(&e);//����ת��Msg�����鿴��η��ͻظ�
	*/
}
//ʹ�ÿ�Qԭ���Դ���������Ϣ����
EVE_GroupMsg_EX(GroupMsg_help)
//name:������Ϣ
//priority:30000
//regex-key:Account
//regex-key:action
//regex-expression:^���ز�ѯ$
{
	e.sendMsg() << "[CQ:at,qq=" << e.fromAccount << "]\nHelp:" << "\n" << "�ٽ����ѯ(�ݲ�����)" << "\n" << "��[QQ]���[��Ŀ]����Ϣ(QQ�ſ����ð��ش���)" << "\n" << "�ۻ���[��Ŀ]����Ϣ" << send;
}


EVE_GroupMsg_EX(GroupMsg_use)
//name:������Ϣ
//priority:30000
//regex-key:qq
//regex-key:num
//regex-expression:^(\[CQ:at.*?)*(?<qq>\d+)(\])*\s*���(?<num>\d+)����Ϣ$
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
	save.saveGroupChat(e.fromGroup, 1, "name=" + e.�ļ�_���� + ",size=" + to_string(e.�ļ�_��С), CQ_MSG_TYPE_UPLOADFILE, e.sendTime, "");
}
EVE_System_GroupMemberDecrease_EX(System_GroupMemberDecrease_EX) {

}
EVE_System_GroupMemberIncrease_EX(System_GroupMemberIncrease_EX) {

}

static Logger logger("Ⱥ�����¼�");
EVE_System_GroupBan_EX(System_GroupBan_EX)
//name:������Ϣ
//priority:30000
{
	/*
	auto debug = logger.Debug();


	auto gInfo = getGroupInfo(e.fromGroup);
	debug << "[" << gInfo.���� << "]��";

	if (e.beingOperateAccount == 0) {
		debug << "[ȫ��]";
	}
	else {
		auto qInfo = getGroupMemberInfo(e.fromGroup, e.beingOperateAccount);
		debug << "[" << qInfo.��Ƭ << "(" << e.beingOperateAccount << ")]";
	}

	auto qInfo = getGroupMemberInfo(e.fromGroup, e.fromAccount);
	debug << "��[" << gInfo.���� << "(" << e.fromAccount << ")]";

	switch (e.subType)
	{
	case 1:
		debug << "�������";
		break;
	case 2:
		debug << "����" << e.duration << "��";
		break;
	default:
		debug << "������δ֪����(" << e.subType << ")";
		break;
	}
	debug << send;
	*/
}

EVE_Menu_EX(menu)
//name:����
{
}