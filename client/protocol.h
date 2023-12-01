#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <string>
#include "xpack-1.0.1/json.h"
struct UserDTO
{
    int id;
    std::string userName;
    std::string password;
    int role;  
    XPACK(O(id,userName, password,role));
};

struct LogDTO
{
    int id;
    std::string username;
    std::string connectTime;
    std::string ip;
    XPACK(O(id,username, connectTime,ip));
};


struct MsgDTO
{
    std::string msgType;
    std::string msgContent;
    bool res;
    XPACK(O(msgType, msgContent,res));
};
struct CourseDTO
{
    int id;
    std::string code;
    std::string title;
    std::string section;
    std::string time;
    std::string classroom;
    std::string instructor;
    XPACK(O(id, code,title, section ,time, classroom, instructor));
};




#endif // PROTOCOL_H
