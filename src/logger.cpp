#include "logger.hpp"

logger::logger(const std::string &scriptname, const bool vflag)
    : scriptname_(scriptname), vflag_(vflag) {}

logger::~logger() {}

void logger::info(const std::string &message) {
    std::string pre = cform::green + "-I-" + cform::end;
    base_(pre, message, std::cout);
}

void logger::verbose(const std::string &message) {
    if (vflag_ == true) {
        std::string pre = cform::purple + "-V-" + cform::end;
        base_(pre, message, std::cout);
    }
}

void logger::warning(const std::string &message) {
    std::string pre = cform::yellow + "-W-" + cform::end;
    base_(pre, message, std::cerr);
}

void logger::fatal(const std::string &message, int err_code = 1) {
    std::string pre = cform::red + "-F-" + cform::end;
    base_(pre, message, std::cerr);
    std::exit(err_code);
}

void logger::base_(const std::string &pre, const std::string &message, std::ostream &stream) {
    std::string sname = cform::dark_gray + "[ " + scriptname_ + " ]" + cform::end;
    stream << pre << " " << sname << " " << message << std::endl;
}
