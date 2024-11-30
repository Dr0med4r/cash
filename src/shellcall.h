#include "commandline.h"
#include <unordered_map>
class ShellCallPwd : public Call {
  public:
    ShellCallPwd(std::string command) : Call(command) {};
    virtual ~ShellCallPwd() {};
    virtual void exec(int, int) override;
};
class ShellCallCd : public Call {
  public:
    ShellCallCd(std::string command) : Call(command) {};
    virtual ~ShellCallCd() {};
    virtual void exec(int, int) override;
};

class ShellCallAlias : public Call {
  public:
    ShellCallAlias(std::string command) : Call(command){};
    ShellCallAlias(ShellCallAlias &&) = default;
    ShellCallAlias(const ShellCallAlias &) = default;
    ShellCallAlias &operator=(ShellCallAlias &&) = default;
    ShellCallAlias &operator=(const ShellCallAlias &) = default;
    virtual ~ShellCallAlias() {};
    virtual void exec (int, int) override;
    static void resolve_alias(std::string& to_expand);
    static bool contains_alias(std::string& key);
    static std::vector<std::string> get_alias(std::string& key);
  private:
    static std::unordered_map<std::string, std::vector<std::string>> aliases;
};

