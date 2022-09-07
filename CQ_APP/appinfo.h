#ifdef _DEBUG
#pragma comment(lib,"CQ_SDK_DEBUG.lib")
#else
#pragma comment(lib,"CQ_SDK.lib")
#endif // _DEBUG
#include <CQSDK.h>

// 插件名称
#define APP_ID "cn.sonui.seekmsg"


//以下内容需要CQJSON小工具支持
//如果您不使用,则需要手动编写JSON
//如果您不使用,请直接转到 appmain.cpp 以继续

//如果使用CQJSON小工具
//需要在<项目属性>的<生成后事件>的<命令行>写入以下命令
//默认已经写入
/*
$(SolutionDir)CQ_Json.exe $(ProjectDir) $(TargetDir)
*/

// 应用名称
#define APP_name "撤回查询"

// 应用版本
#include "appVer.h"

// 应用作者
#define APP_author "Sonui"

//一句话描述插件用途
#define APP_description "查询消息" //注释不影响解析效果
//              ↑    ↑           ↑
//              ↑始终第一个引号开始,无论是否是真实引号
//                    ↑中间引号全部忽略,与VS解释有所不同
//                                 ↑直接匹配最后一个引号作为结束

#define APP_auth_30 "接收语音"
#define APP_auth_101 "发送群消息"
#define APP_auth_103 "发送讨论组消息"
#define APP_auth_106 "发送私聊消息"
#define APP_auth_130 "取群成员信息"
#define APP_auth_132 "取群信息"
#define APP_auth_131 "取陌生人信息"
#define APP_auth_180 "撤回消息"

//酷Q目录,直接生成到酷Q目录
#define CQ_DIR "C:\\Users\\81414\\Desktop\\Tools\\酷Q Air\\"