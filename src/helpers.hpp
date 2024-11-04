#pragma once
#ifndef _HELPERS_HPP_
#define _HELPERS_HPP_


#include <string>
#include <sstream>
#include <boost/timer/timer.hpp>
#include <iostream>
#include <fstream>

#include "logger.hpp"


class Timer {
public:
    Timer(const std::string &title = "", bool plain = false, const std::string &color = cform::bold);
};


class RedirectPrintouts {
private:
    std::streambuf *stdout_orig_, *stderr_orig_;
    std::ofstream   stdout_file_,  stderr_file_;

public:
    RedirectPrintouts(const std::string &stdout = "", const std::string &stderr = "");
    ~RedirectPrintouts();
};

#endif
