#include "exceptions.h"

fatal_ex::fatal_ex()
    : message_("fatal exception thrown"), err_code_(1) {}

fatal_ex::fatal_ex(const std::string& message, const int& err_code = 1)
    : message_(message), err_code_(err_code) {}

fatal_ex::~fatal_ex() {}

std::string& fatal_ex::what() { return message_; }

int fatal_ex::which() { return err_code_; }


peaceful_ex::peaceful_ex()
    : message_("peaceful exception thrown") {}

peaceful_ex::peaceful_ex(const std::string& message, const int& err_code = 1)
    : message_(message) {}

peaceful_ex::~peaceful_ex() {}

std::string& peaceful_ex::what() { return message_; }
