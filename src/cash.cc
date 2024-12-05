#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <list>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unordered_map>
#include <vector>

#include "builtins.h"
#include "cash.h"

std::unordered_map<int, std::list<int>> Cash::jobs =
    std::unordered_map<int, std::list<int>>{};

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
}

// TODO iterator invalidation
void Cash::wait_for_bg() {
    std::vector<int> finished_jobs;
    for (auto& [key, job] : jobs) {
        std::vector<std::vector<int>::iterator> to_delete_pids;
        for (int const pid: job) {
            int result = waitpid(pid, nullptr, WNOHANG);
            if (result == key) {
                finished_jobs.push_back(key);
            }
        }
    }
    for (int job: finished_jobs) {
        // TODO sind alle prozesse fertig, wenn der letzte fertig ist?
        jobs.erase(job);
    }
}

void Cash::add_job(std::list<int> job) {
    jobs.insert_or_assign(job.back(), job);
}
