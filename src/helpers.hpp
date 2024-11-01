#pragma once
#ifndef _HELPERS_HPP_
#define _HELPERS_HPP_

#include <string>
#include <sstream>
#include <boost/timer/timer.hpp>

#include "logger.hpp"

class Timer {
public:
    Timer(const std::string &title = "", bool plain = false, const std::string &color = cform::bold) {
        std::stringstream formatted;
        if (!plain) formatted << std::endl << title << ": ";
        formatted << color << "%w" << cform::end << std::endl;
        static auto t_ = boost::timer::auto_cpu_timer(6, formatted.str());
    }
};

#endif
