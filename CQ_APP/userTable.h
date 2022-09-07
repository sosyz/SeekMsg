#include <string>

class userTable
{
public:
    int init();
    int addUser(int type, int64_t qq, std::string name, std::string value);
    int deleteUser(int type, int64_t qq, std::string name, bool all);
    std::string getUser(int type, int64_t qq, std::string name);
    int setUser(int type, int64_t qq, std::string name, std::string value);
    bool close();
};
