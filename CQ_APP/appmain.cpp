#include <CQSDK.h>
#include "appinfo.h"

#include <string>
#include <Windows.h>
#include <time.h>
#include <direct.h>
#include <easyhook.h>
#include "tools.h"
#include "readFile.h"
//#include "rapidjson/error/en.h"
//#include "rapidjson/document.h"
//#include "rapidjson/writer.h"
//#include "rapidjson/stringbuffer.h"

MUST_AppInfo_RETURN(APP_ID)

using namespace std;

historyRecord save;
bool load = false;

HMODULE hModule;
FARPROC CQ_sendGroupMsg_addr;
HOOK_TRACE_INFO hHook = { NULL };
ULONG HookNtCreateFile_ACLEntries[1] = { 0 };

//extern rapidjson::Document config;
static Logger logger("Init");

int __stdcall fuckSendGroupMsg(int AuthCode, long long groupNum, const char* msg)
{
	save.saveGroupChat(groupNum, getLoginQQ(), msg, CQ_MSG_TYPE_TEXT, time(0), "");
	return CQ_sendGroupMsg(getAuthCode(), groupNum, msg);
}

std::string WChar2Ansi(LPCWSTR pwszSrc)
{
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
	if (nLen <= 0) return std::string("");
	char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");
	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;
	std::string strTemp(pszDst);
	delete[] pszDst;
	return strTemp;
}

EVE_Enable_EX(Enable)
//name:应用启用
//priority:30000
{
	MessageBoxA(0, "注意，此版本为测试版本，须注意几点\n功能尚不完善\n保存聊天记录(不会上传到服务器)，仅测试收集bug之用发布", "提示", 0);
	string temp, data, dir = getAppDirectory();
	save.init();
	/*
	data = _readFile(dir + "config.json");
	if (data == "" && !config.Parse(data.c_str()).IsObject()) {
		logger.Error("载入配置文件错误，启用默认配置");
		data = "{\
			\"version\": 2,\
			\"mode\":1,\
			\"password\" : \"\",\
			\"sendMode\" : 1,\
			\"fuckSendGroupMsg\" : 0,\
			\"token\" : \"\",\
			\"UTC\" : 8,\
			\"doname\" : \"https://seekmsg.sonui.cn\",\
			\"api\" : {\
			\"gettoken\":\"/gettoken.php\",\
				\"getinfo\" : \"/getinfo.php\",\
				\"uphistory\" : \"/uphistory.php\",\
				\"readhistory\" : \"/history\"\
			},\
			\"allow\":1\
		}";
		config.Parse(data.c_str());
		
	}
	logger.Debug(to_string(config.IsObject()));
	*/
	//hook初始化
	
	char buffer[MAX_PATH];
	_getcwd(buffer, MAX_PATH);
	strcat_s(buffer, sizeof(buffer), "\\bin\\CQP.dll");
	hModule = GetModuleHandleA(buffer);
	if (hModule == NULL) {
		logger.Error("获取DLL地址失败，错误信息："+ GetLastError());
	}
	CQ_sendGroupMsg_addr = GetProcAddress(hModule, "CQ_sendGroupMsg");

	if (CQ_sendGroupMsg_addr == NULL) {
		logger.Error("获取函数地址失败，无法安装函数钩子，错误信息：" + GetLastError());
	}
	else {
		NTSTATUS result = LhInstallHook(
			CQ_sendGroupMsg_addr,
			fuckSendGroupMsg,
			NULL,
			&hHook);
		LhSetExclusiveACL(HookNtCreateFile_ACLEntries, 1, &hHook);
		if (FAILED(result))
		{
			logger.Error("安装函数钩子失败，错误信息：" + WChar2Ansi(RtlGetLastErrorString()) + "无法保存机器人所发送的消息");
		}
		load = true;
	}
	
	
}

void closeUse() {
	LhUninstallHook(&hHook);
	LhWaitForPendingRemovals();
	save.close();
}

EVE_Disable_EX(Disable)
//name:应用停用
//priority:30000
{
	closeUse();
}


EVE_Exit_EX(Exit)
//name:退出清理
//priority:30000
{
	closeUse();
}