#include <cerrno>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <mutex>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <vector>

#include "builtins.h"
#include "cash.h"

std::vector<int> Cash::jobs = std::vector<int>{};

std::mutex Cash::finished = std::mutex{};

void fin_action(int, siginfo_t* info, void*) {
    {
        std::lock_guard<std::mutex> lock(Cash::finished);
        Cash::add_pid(info->si_pid);
    }
}

void Cash::setup_signals() {
    struct sigaction act = {};
    act.sa_handler = SIG_IGN;
    if (sigaction(SIGINT, &act, NULL) == -1) {
        std::cerr << "set sigaction failed\n";
        exit(1);
    }
    if (sigaction(SIGTERM, &act, NULL) == -1) {
        std::cerr << "set sigaction failed\n";
        exit(1);
    }
    struct sigaction sigchld {};
    sigchld.sa_sigaction = fin_action;
    // set the flags to use sa_sigaction instead of sa_handler
    sigchld.sa_flags = SA_SIGINFO;
    if (sigaction(SIGCHLD, &sigchld, NULL) == -1) {
        std::cerr << "set sigaction failed\n";
        exit(1);
    }
}
void Cash::reset_signals() {
    struct sigaction act = {};
    act.sa_handler = SIG_DFL;
    if (sigaction(SIGINT, &act, NULL) == -1) {
        std::cerr << "set sigaction failed\n";
        exit(1);
    }
    if (sigaction(SIGTERM, &act, NULL) == -1) {
        std::cerr << "set sigaction failed\n";
        exit(1);
    }
    if (sigaction(SIGCHLD, &act, NULL) == -1) {
        std::cerr << "set sigaction failed\n";
        exit(1);
    }
}

void Cash::wait_for_bg() {
    while (jobs.size() > 0) {
        int pid;
        {
            std::lock_guard<std::mutex> lock(finished);
            pid = jobs.back();
            jobs.pop_back();
        }
        waitpid(pid, nullptr, 0);
    }
}

void Cash::add_pid(int pid) { jobs.push_back(pid); }
