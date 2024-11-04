#include "helpers.hpp"

Timer::Timer(const std::string &title, bool plain, const std::string &color) {
    std::stringstream formatted;
    if (!plain) formatted << std::endl << title << ": ";
    formatted << color << "%w" << cform::end << std::endl;
    static auto t_ = boost::timer::auto_cpu_timer(6, formatted.str());
}


RedirectPrintouts::RedirectPrintouts(const std::string &stdout, const std::string &stderr)
    : stdout_orig_(std::cout.rdbuf()), stderr_orig_(std::cerr.rdbuf())
{
    if (!stdout.empty()) {
        stdout_file_.open(stdout);
        if (stdout_file_.is_open()) {
            std::cout.rdbuf(stdout_file_.rdbuf());
        } else {
            throw std::runtime_error("Failed to open file [ " + stdout + " ]");
        }
    }
    if (!stderr.empty()) {
        stderr_file_.open(stderr);
        if (stderr_file_.is_open()) {
            std::cerr.rdbuf(stderr_file_.rdbuf());
        } else {
            throw std::runtime_error("Failed to open file [ " + stderr + " ]");
        }
    }
}

RedirectPrintouts::~RedirectPrintouts() {
    std::cout.rdbuf(stdout_orig_);
    std::cerr.rdbuf(stderr_orig_);
}
