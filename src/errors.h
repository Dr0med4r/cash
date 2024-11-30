#pragma once
#include <exception>
#include <ostream>

class ExecError : public std::runtime_error {
  public:
    ExecError(const std::string &msg) : std::runtime_error(msg) {};
    ExecError(ExecError &&) = default;
    ExecError(const ExecError &) = default;
    ExecError &operator=(ExecError &&) = default;
    ExecError &operator=(const ExecError &) = default;
    ~ExecError() = default;

  private:
};

class FileError : public std::runtime_error {
  public:
    FileError(const std::string &msg) : std::runtime_error(msg) {};
    FileError(const FileError &) = default;
    FileError(FileError &&) = default;
    FileError &operator=(const FileError &) = default;
    FileError &operator=(FileError &&) = default;
    ~FileError() = default;
};
