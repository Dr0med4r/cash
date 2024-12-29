#include <list>
#include <mutex>
#include <string>
#include <vector>

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
    static void add_pid(int pid);
    static std::mutex finished;

  private:
    static std::vector<int> jobs;
};
