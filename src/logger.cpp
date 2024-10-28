#include "logger.h"
#include "exceptions.h"

logger::logger() : vflag_(false) {}

logger::logger(bool& vflag) : vflag_(vflag) {}

logger::~logger() {}

void logger::info(const std::string& message) {
    std::cout << cform::green << "-I- " << cform::end << message << std::endl;
}

void logger::verbose(const std::string& message) {
    if (vflag_ == true) {
        std::cout << cform::purple << "-V- " << cform::end << message << std::endl;
    }
}

void logger::warning(const std::string& message) {
    std::cerr << cform::yellow << "-W- " << cform::end << message << std::endl;
}

void logger::fatal(const std::string& message, int err_code = 1) {
    std::cerr << cform::red << "-F- " << cform::end << message << std::endl;
    throw fatal_ex(message, err_code);
}

void logger::setVerbosity(bool &vflag) {
    vflag_ = vflag;
}
