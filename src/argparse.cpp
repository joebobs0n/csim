#include "argparse.hpp"

peaceful_ex::peaceful_ex()
    : message_("peaceful exception thrown") {}

peaceful_ex::peaceful_ex(const std::string& message, const int& err_code = 1)
    : message_(message) {}

peaceful_ex::~peaceful_ex() {}

std::string& peaceful_ex::what() { return message_; }


argparse::argparse(const std::string& description, const std::string& epilog)
    : description_(description), epilog_(epilog) {}

argparse::~argparse() {}

void argparse::add_description(std::string& message) {
    description_ = message;
}

void argparse::add_epilog(std::string& message) {
    epilog_ = message;
}

po::options_description* argparse::add_argument_group(std::string& name) {
    po::options_description new_group = po::options_description(name);
    argument_groups_.push_back(new_group);
    return &argument_groups_[argument_groups_.size() - 1];
}

void argparse::print_help(char** argv) {
    if (description_ != "") std::cout << std::endl << description_ << std::endl;
    std::cout << all_options;
    if (epilog_ != "") std::cout << std::endl << epilog_ << std::endl;
    throw peaceful_ex();
}

po::variables_map* argparse::parse_args(int argc, char** argv) {
    for (auto ag : argument_groups_) all_options.add(ag);
    po::store(po::parse_command_line(argc, argv, all_options), vm);
    if (vm.count("help")) print_help(argv);
    po::notify(vm);
    return &vm;
}
