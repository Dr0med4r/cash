#include <list>
#include <string>
#include <unordered_map>

// currently available shell - cash
class Cash {
  public:
    Cash();
    Cash(Cash &&) = default;
    Cash(const Cash &) = default;
    Cash &operator=(Cash &&) = delete;
    Cash &operator=(const Cash &) = delete;
    ~Cash();
    static void setup_signals();
    static void reset_signals();
    static void wait_for_bg();
    static void add_job(std::list<int> job);

  private:
    static std::unordered_map<int, std::list<int>> jobs;
};
