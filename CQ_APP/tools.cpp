#include "tools.h"
#include <regex>
#include <CQSDK.h>
#include <fstream>
#include "sqlite3Tool.h"
#include "MsgPack.h"
#include "openssl\aes.h"
#include "openssl\bn.h"
#include "openssl\md5.h"
#include "openssl\evp.h"
#include "openssl\bio.h"
#include "openssl\buffer.h"

static Logger logger("Tools");

char* readFile(string dir, int& size) {
	fstream f;
	f.open(dir, ios::in | ios::binary);
	f.seekg(0, std::ios_base::end);
	std::streampos sp = f.tellg();
	size = sp;
	char* buffer = (char*)malloc(sizeof(char) * size);
	f.read(buffer, size);
	f.close();
	return buffer;
}

string getValue(string str, string name) {
	smatch result;
	regex pattern(name + "=([^(,|\\])]*)", regex::icase);
	if (!regex_match(str, result, pattern)) return "";
	return result[1];
}

string replace_all_distinct(string& str, const  string& old_value, const   string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos) {
			str.replace(pos, old_value.length(), new_value);
		}
		else {
			break;
		}
	}
	return str;
}

string mqqapi(string type, string id) {
	string ret("mqqapi://card/show_pslcard?src_type=internal&card_type=");
	ret.append(type);
	ret.append("&source=sharecard&version=1&uin=");
	ret.append(id);
	return ret;
}

string TimeToString(time_t stime)
{
	char tmp[64];
	struct tm now_time;
	localtime_s(&now_time, &stime);
	strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", &now_time);
	return tmp;
}

string getName(const int64_t qq, const int64_t groupNum)
{
	auto player = getGroupMemberInfo(groupNum, qq, false);
	string name = AnsiToUtf8(player.名片.c_str());
	if (name == "") {
		name = AnsiToUtf8(player.昵称.c_str());
	}
	if (name == "") {
		name = AnsiToUtf8(getStrangerInfo(qq, false).nick.c_str());
	}
	return name;
}

string getQQName(string qq) {
	return "";
}
string reCQ(string msg, string name, string groupNum, int mode)
{
	smatch result;
	regex pattern("\\[CQ:([\\S\\s-]+?),([\\S\\s-]+?)\\]", regex::icase);
	string::const_iterator iter = msg.begin();
	string::const_iterator iterEnd = msg.end();
	string temp, ret = msg, rname;
	fstream f;
	int index = 0, i = 0, n = 0;
	while (regex_search(iter, iterEnd, result, pattern))
	{
		i++;
		n++;
		string type = result.str(1),
			values = result.str(2),
			fileName;
		if (type == "image") index = 1;
		if (type == "contact") index = 2;
		if (type == "sign") index = 3;
		if (type == "music") index = 4;
		if (type == "at") index = 5;
		if (type == "share") index = 6;
		if (type == "record") index = 7;
		if (type == "face") index = 8;
		switch (index)
		{
		case 1:
			switch (mode)
			{
			case 1:
				fileName = getValue(values, "file");
				fileName = getImage(fileName);
				int size;
				temp = readFile(fileName, size);
				temp = "<img src=\"data:image/png;base64,";
				temp += base64Encode(temp.data(), size, false);
				temp += "\">";
				break;
			case 2:
				temp = "[图片" + to_string(i) + "]";
			}
			break;
		case 2:
			switch (mode)
			{
			case 1:
				temp = "<a href=\"" + mqqapi(getValue(values, "type"), getValue(values, "type"));
				temp += "\" target=\"_blank\">";
				temp += getValue(values, "id");
				temp += "</a>";
				break;
			case 2:
				temp = getValue(values, "type") == "1" ? "群:" : "QQ:" + getValue(values, "type") + " ";
				break;
			}
			break;
		case 3:
			/*
			switch (mode)
			{
			case 1:
				temp = "[签到]" + getValue(values, "title");
				break;
			case 2:
				temp = "[签到]" + getValue(values, "title");
				break;
			}*/
			temp = "[签到]" + getValue(values, "title");
			break;
		case 4:
			/*
			switch (mode)
			{
			case 1:

				break;
			case 2:

				break;
			}*/
			temp = "[音乐]该版本暂不支持转换此码";
			break;
		case 5:
			rname = getName(atoll(getValue(values, "qq").c_str()), atoll(groupNum.c_str()));
			switch (mode)
			{
			case 1:
				temp = "<a herf=\"";
				temp += mqqapi(getValue(values, "type"), getValue(values, "type"));
				temp += ">@";
				temp += rname;
				temp += "&nbsp;</a>";
				break;
			case 2:
				temp = "@" + rname + " ";
				break;
			}
			break;
		case 6:
			switch (mode)
			{
			case 1:
				temp = "<a href=\"";
				temp += getValue(values, "url");
				temp += "\" target=\"_blank\">";
				temp += getValue(values, "title");
				temp += "</a>";
				break;
			case 2:
				temp = getValue(values, "url");
				break;
			}
			break;
		case 7:
			switch (mode)
			{
			case 1:
				fileName = getRecord(getValue(values, "file"), "mp3");
				int size;
				temp = readFile(fileName, size);
				temp = "<audio controls=\"controls\"><source src=\"data:audio/mp3,";
				temp += base64Encode(temp.data(), size, false);
				temp += ">";
				break;
			case 2:
				temp = "[语音" + to_string(n) + "]";
				break;
			}
			break;
		case 8:
			switch (mode)
			{
			case 1:
				fileName = getRecord(getValue(values, "file"), "mp3");
				int size;
				temp = readFile(fileName, size);
				temp = "<audio controls=\"controls\"><source src=\"data:audio/mp3,";
				temp += base64Encode(temp.data(), size, false);
				temp += ">";
				break;
			case 2:
				temp = "[CQ:face, id=" + getValue(values, "id") + "]";
				break;
			}
			break;
		default:
			temp = "[暂未支持的转码类型: " + type + "]";
			break;
		}
		ret = replace_all_distinct(ret, result.str(0), temp);
		iter = result[0].second; //更新搜索起始位置
	}
	ret = AnsiToUtf8(ret.c_str());//鬼知道为啥这里gbk
	return ret;
}

string tohtml(string html)
{
	string ret = html;
	ret = replace_all_distinct(ret, "&", "&amp;");
	ret = replace_all_distinct(ret, "<", "&lt;");
	ret = replace_all_distinct(ret, " ", "&nbsp;");
	ret = replace_all_distinct(ret, ">", "&gt;");
	ret = replace_all_distinct(ret, "\"", "&quot;");
	ret = replace_all_distinct(ret, "'", "&apos;");
	ret = replace_all_distinct(ret, "\\", "\\\\");
	return ret;
}

string rehtml(string html)
{
	string ret = html;
	ret = replace_all_distinct(ret, "&amp;", "&");
	ret = replace_all_distinct(ret, "&lt;", "<");
	ret = replace_all_distinct(ret, "&nbsp;", " ");
	ret = replace_all_distinct(ret, "&gt;", ">");
	ret = replace_all_distinct(ret, "&quot;", "\"");
	ret = replace_all_distinct(ret, "&apos;", "'");
	ret = replace_all_distinct(ret, "\\\\", "\\");
	return ret;
}


int getIV(unsigned char* iv) {
	int num;
	BIGNUM* rnd;
	for (int i = 0; i < AES_BLOCK_SIZE; i++) {
		rnd = BN_new();
		BN_rand(rnd, 8, -1, 0);
		num = atoi(BN_bn2dec(rnd));
		iv[i] = (48 + num % 10);
		BN_free(rnd);
	}
	return 0;
}

int getPassword(int len, char* pw) {
	BIGNUM* rnd;
	int start = 0, ans = 0, num = 0;
	for (int i = 0; i < len; i++) {
		rnd = BN_new();
		BN_rand(rnd, 16, -1, 0);
		num = atoi(BN_bn2dec(rnd));
		BN_free(rnd);
		//33-126
		pw[i] = (33 + num % 94);
	}
	return 1;
}

string aes_encrypt(const string& in, const string& key, unsigned char* iv)
{
	AES_KEY aes;
	if (AES_set_encrypt_key((unsigned char*)key.data(), 256, &aes) < 0) return 0;
	string out;
	unsigned len = in.size();
	unsigned len2 = (len / AES_BLOCK_SIZE + 1) * AES_BLOCK_SIZE;
	unsigned padding = len2 - len;
	out.resize(len2);
	string plainwithpadding = in;
	plainwithpadding.resize(len2);
	plainwithpadding[len2 - 1] = (unsigned char)padding;
	AES_cbc_encrypt((unsigned char*)plainwithpadding.data(), (unsigned char*)out.data(), plainwithpadding.size(), &aes, iv, AES_ENCRYPT);
	out = base64Encode(out.data(), out.size(), false);
	return out;
}

string aes_decrypt(const string& in, const string& key, unsigned char* iv)
{
	AES_KEY aes;
	if (AES_set_decrypt_key((unsigned char*)key.data(), 256, &aes) < 0) return 0;
	string res, data;
	data = base64Decode((char*)in.data(), in.length(), false);
	//char sourceStringTemp[128];// = data.data();
	//strcpy_s((char*)sourceStringTemp, data.length() + 1, data.c_str());
	//cout << "sourceStringTemp: " << sourceStringTemp << endl;
	res.resize(data.size());
	AES_cbc_encrypt((unsigned char*)data.data(), (unsigned char*)res.data(), data.size(), &aes, iv, AES_DECRYPT);
	unsigned padding = (unsigned char)res[res.size() - 1];
	//res.resize(res.size() - padding);
	return res;
}

string md5(const string& srcStr)
{
	// 调用md5哈希
	unsigned char mdStr[33] = { 0 };
	MD5((const unsigned char*)srcStr.c_str(), srcStr.length(), mdStr);

	// 哈希后的十六进制串 32字节    
	char buf[65] = { 0 };
	char tmp[3] = { 0 };
	for (int i = 0; i < 32; i++)
	{
		sprintf_s(tmp, "%02x", mdStr[i]);
		strcat_s(buf, tmp);
	}
	buf[32] = '\0'; // 后面都是0，从32字节截断
	return string(buf);
}

string getListory2(const int64_t qq, const int64_t group, int num)
{
	sqlite3Tool search;
	string send = "", msg = "", name = "", dir = getAppDirectory() + "chat.db";
	MsgPack pack;
	if (!search.open(AnsiToUtf8(dir.c_str()), false))
		return "记录库不存在,无法进行查询";
	sqlite3_stmt* stmt = search.prepare("SELECT data.type,\
		data.msgType,\
		data.fGroup,\
		data.fQQ,\
		data.msg,\
		data.other,\
		data.time\
		FROM\
		data\
		WHERE\
		data.fQQ = ?\
		AND\
		data.fGroup = ?\
		ORDER BY\
		data.time DESC\
		LIMIT  ?");
	search.bindInt64(stmt, 1, qq);
	search.bindInt64(stmt, 2, group);
	search.bindInt(stmt, 3, num);
	name = getName(qq, group);
	logger.Debug("qq:" + to_string(qq) + ", group: " + to_string(group));
	while (search.step(stmt) == SQLITE_ROW)
	{
		int type = search.readInt(stmt, 0);
		/*
		封存代码 这个模式用不到
		switch (type)
		{
		case CQ_MSG_TYPE_TEXT:
			pack.addMsg(1, to_string(qq), getName(qq, group), search.readString(stmt, 3), search.readInt64(stmt, 5), to_string(group), 2);
			break;
		case CQ_MSG_TYPE_BAN:
			send = "";
			msg = search.readString(stmt, 3);
			switch (atoi(getValue(msg, "type").c_str()))
			{
			case 1:
				send = getName(atoll(getValue(msg, "qq").c_str()), group) + "(" + getValue(msg, "qq") + ")被管理员" + getValue(msg, "msg");
				break;
			case 2:
				send = "管理员" + getValue(msg, "msg") + "了全体禁言";
				break;
			default:
				send = "未知的消息";
			}
			pack.addMsg(1, "10000", "系统消息", send, search.readInt64(stmt, 5), to_string(group), 2);
			break;
		case CQ_MSG_TYPE_MEMBERDEC:
			pack.addMsg(1, "10000", "系统消息", getName(atoll(getValue(msg, "qq").c_str()), group), search.readInt64(stmt, 5), to_string(group), 2);
			break;
		default:
			break;
		}
		*/
		
		pack.addMsg(1, to_string(qq), name, search.readString(stmt, 4), search.readInt64(stmt, 6), to_string(group), 2);
	}
	search.close();
	return pack.getData(2);
}

char* base64Encode(const char* buffer, int length, bool newLine)
{
	BIO* bmem = NULL;
	BIO* b64 = NULL;
	BUF_MEM* bptr;

	b64 = BIO_new(BIO_f_base64());
	if (!newLine) {
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new(BIO_s_mem());
	b64 = BIO_push(b64, bmem);
	BIO_write(b64, buffer, length);
	BIO_flush(b64);
	BIO_get_mem_ptr(b64, &bptr);
	BIO_set_close(b64, BIO_NOCLOSE);

	char* buff = (char*)malloc(bptr->length + 1);
	memcpy(buff, bptr->data, bptr->length);
	buff[bptr->length] = 0;
	BIO_free_all(b64);

	return buff;
}

// base64 解码
char* base64Decode(char* input, int length, bool newLine)
{
	BIO* b64 = NULL;
	BIO* bmem = NULL;
	char* buffer = (char*)malloc(length);
	memset(buffer, 0, length);
	b64 = BIO_new(BIO_f_base64());
	if (!newLine) {
		BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	}
	bmem = BIO_new_mem_buf(input, length);
	bmem = BIO_push(b64, bmem);
	BIO_read(bmem, buffer, length);
	BIO_free_all(bmem);

	return buffer;
}