#ifdef _DEBUG
#pragma comment(lib,"CQ_SDK_DEBUG.lib")
#else
#pragma comment(lib,"CQ_SDK.lib")
#endif // _DEBUG
#include <CQSDK.h>

// �������
#define APP_ID "cn.sonui.seekmsg"


//����������ҪCQJSONС����֧��
//�������ʹ��,����Ҫ�ֶ���дJSON
//�������ʹ��,��ֱ��ת�� appmain.cpp �Լ���

//���ʹ��CQJSONС����
//��Ҫ��<��Ŀ����>��<���ɺ��¼�>��<������>д����������
//Ĭ���Ѿ�д��
/*
$(SolutionDir)CQ_Json.exe $(ProjectDir) $(TargetDir)
*/

// Ӧ������
#define APP_name "���ز�ѯ"

// Ӧ�ð汾
#include "appVer.h"

// Ӧ������
#define APP_author "Sonui"

//һ�仰���������;
#define APP_description "��ѯ��Ϣ" //ע�Ͳ�Ӱ�����Ч��
//              ��    ��           ��
//              ��ʼ�յ�һ�����ſ�ʼ,�����Ƿ�����ʵ����
//                    ���м�����ȫ������,��VS����������ͬ
//                                 ��ֱ��ƥ�����һ��������Ϊ����

#define APP_auth_30 "��������"
#define APP_auth_101 "����Ⱥ��Ϣ"
#define APP_auth_103 "������������Ϣ"
#define APP_auth_106 "����˽����Ϣ"
#define APP_auth_130 "ȡȺ��Ա��Ϣ"
#define APP_auth_132 "ȡȺ��Ϣ"
#define APP_auth_131 "ȡİ������Ϣ"
#define APP_auth_180 "������Ϣ"

//��QĿ¼,ֱ�����ɵ���QĿ¼
#define CQ_DIR "C:\\Users\\81414\\Desktop\\Tools\\��Q Air\\"