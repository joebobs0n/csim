#pragma once
#ifndef _LOGGER_HPP_
#define _LOGGER_HPP_

#define CTYPE std::string
#define COLOR const static CTYPE

#include <string>
#include <sstream>
#include <iostream>

namespace cform {
    COLOR bullet =  "•";         COLOR clear =      "\033c";     COLOR white =       "\033[097m"; COLOR white_bkg =       "\033[107m";
    COLOR end =     "\033[000m"; COLOR underline =  "\033[004m"; COLOR gray =        "\033[037m"; COLOR gray_bkg =        "\033[047m";
    COLOR bold =    "\033[001m"; COLOR blink =      "\033[005m"; COLOR dark_gray =   "\033[090m"; COLOR dark_gray_bkg =   "\033[100m";
    COLOR italics = "\033[003m"; COLOR invert =     "\033[007m"; COLOR black =       "\033[030m"; COLOR black_bkg =       "\033[040m";

    COLOR red =     "\033[091m"; COLOR red_bkg =    "\033[101m"; COLOR dark_red =    "\033[031m"; COLOR dark_red_bkg =    "\033[041m";
    COLOR yellow =  "\033[093m"; COLOR yellow_bkg = "\033[103m"; COLOR dark_yellow = "\033[033m"; COLOR dark_yellow_bkg = "\033[043m";
    COLOR green =   "\033[092m"; COLOR green_bkg =  "\033[102m"; COLOR dark_green =  "\033[032m"; COLOR dark_green_bkg =  "\033[042m";
    COLOR cyan =    "\033[096m"; COLOR cyan_bkg =   "\033[106m"; COLOR dark_cyan =   "\033[036m"; COLOR dark_cyan_bkg =   "\033[046m";
    COLOR blue =    "\033[094m"; COLOR blue_bkg =   "\033[104m"; COLOR dark_blue =   "\033[034m"; COLOR dark_blue_bkg =   "\033[044m";
    COLOR purple =  "\033[095m"; COLOR purple_bkg = "\033[105m"; COLOR dark_purple = "\033[035m"; COLOR dark_purple_bkg = "\033[045m";
}

class logger {
public:
    logger(const std::string &scriptname = "unset", const bool vflag = false);
    ~logger();

    void info(const std::string &message);
    void verbose(const std::string &message);
    void warning(const std::string &message);
    void fatal(const std::string &message, int err_code);

private:
    std::string scriptname_;
    bool vflag_;

    void base_(const std::string &pre, const std::string &message, std::ostream &stream);
};

#endif
