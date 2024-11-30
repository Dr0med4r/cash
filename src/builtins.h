#include "commandline.h"
#include <unordered_map>
class ShellBuiltinPwd : public Call {
  public:
    ShellBuiltinPwd(std::string command) : Call(command) {};
    virtual ~ShellBuiltinPwd() {};
    virtual void exec(int, int) override;
};
class ShellBuiltinCd : public Call {
  public:
    ShellBuiltinCd(std::string command) : Call(command) {};
    virtual ~ShellBuiltinCd() {};
    virtual void exec(int, int) override;
};

class ShellBuiltinAlias : public Call {
  public:
    ShellBuiltinAlias(std::string command) : Call(command) {};
    ShellBuiltinAlias(ShellBuiltinAlias &&) = default;
    ShellBuiltinAlias(const ShellBuiltinAlias &) = default;
    ShellBuiltinAlias &operator=(ShellBuiltinAlias &&) = default;
    ShellBuiltinAlias &operator=(const ShellBuiltinAlias &) = default;
    virtual ~ShellBuiltinAlias() {};
    virtual void exec(int, int) override;
    static void resolve_alias(std::string &to_expand);
    static bool contains_alias(std::string &key);
    static std::vector<std::string> get_alias(std::string &key);

  private:
    static std::unordered_map<std::string, std::vector<std::string>> aliases;
};

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
