#include "commandline.h"

class ShellBuiltinCd : public Call {
  public:
    ShellBuiltinCd(std::string command) : Call(command) {};
    virtual ~ShellBuiltinCd() {};
    virtual void exec(int, int) override;
};
