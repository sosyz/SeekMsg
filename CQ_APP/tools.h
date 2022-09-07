#pragma once

#include <time.h>
#include "historyRecord.h"
#include "au.h"

#define CQ_MSG_TYPE_TEXT 1
#define CQ_MSG_TYPE_BAN 2
#define CQ_MSG_TYPE_UPLOADFILE 3
#define CQ_MSG_TYPE_MEMBERDEC 4
#define CQ_MSG_TYPE_MEMBERINC 5
#define CQ_MSG_TYPE_ADMIN 6

string TimeToString(time_t stime);
string reCQ(string msg, string name, string groupNum, int mode);
string tohtml(string html);
string rehtml(string html);
string getName(const int64_t qq, const int64_t groupNum);
string aes_encrypt(const string& in, const string& key, unsigned char* iv);
string aes_decrypt(const string& in, const string& key, unsigned char* iv);
string md5(const string& srcStr);
string getListory2(const int64_t qq, const int64_t group, int num);
char* base64Encode(const char* buffer, int length, bool newLine);
char* base64Decode(char* input, int length, bool newLine);
//int aes_decrypt(char* in, char* key, char* out, unsigned char* iv);
//const char* readCQFile(const char* name);