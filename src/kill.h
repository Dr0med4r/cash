#include "commandline.h"

class ShellBuiltinKill : public Call {
  public:
    ShellBuiltinKill(std::string command) : Call(command) {};
    ShellBuiltinKill(ShellBuiltinKill &&) = default;
    ShellBuiltinKill(const ShellBuiltinKill &) = default;
    ShellBuiltinKill &operator=(ShellBuiltinKill &&) = default;
    ShellBuiltinKill &operator=(const ShellBuiltinKill &) = default;
    virtual ~ShellBuiltinKill() {};
    virtual void exec(int, int) override;
};
