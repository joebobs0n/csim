#pragma once
#ifndef EXCEPTIONS_H

#include <string>

class fatal_ex : public std::exception {
public:
    fatal_ex();
    fatal_ex(const std::string& message, const int& err_code);
    ~fatal_ex();

    std::string& what();
    int which();

private:
    std::string message_;
    int err_code_;
};

class peaceful_ex : public std::exception {
public:
    peaceful_ex();
    peaceful_ex(const std::string& message, const int& err_code);
    ~peaceful_ex();

    std::string& what();

private:
    std::string message_;
};

#endif
