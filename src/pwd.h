#include "commandline.h"
class ShellBuiltinPwd : public Call {
  public:
    ShellBuiltinPwd(std::string command) : Call(command) {};
    virtual ~ShellBuiltinPwd() {};
    virtual void exec(int, int) override;
};
