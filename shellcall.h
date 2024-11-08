#include "commandline.h"
class ShellCallPwd : public Call {
public:
    ShellCallPwd(std::string command): Call(command){};
    virtual ~ShellCallPwd(){};
    virtual void exec(int,int) override;
};
class ShellCallCd : public Call {
public:
    ShellCallCd(std::string command): Call(command){};
    virtual ~ShellCallCd(){};
    virtual void exec(int,int) override;
};
