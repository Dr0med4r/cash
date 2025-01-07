#include "commandline.h"
#include <unordered_map>

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
    void alias();
    void unalias();
};
