#include "argparse.h"


argument_group::argument_group()
    : name_("arguments_group"), required_(false), exclusive_(false) {}

argument_group::argument_group(std::string& name, bool required, bool exclusive)
    : name_(name), required_(required), exclusive_(exclusive) {}

argument_group::~argument_group() {}

void argument_group::set_name(std::string& name) {
    name_ = name;
}

void argument_group::set_required(bool required) {
    required_ = required;
}

void argument_group::set_exclusive(bool exclusive) {
    exclusive_ = exclusive;
}

auto argument_group::get_name() {
    return name_;
}

bool argument_group::get_required() {
    return required_;
}

bool argument_group::get_exclusive() {
    return exclusive_;
}


argparse::argparse()
    : description_(NULL), epilog_(NULL) {}

argparse::argparse(const std::string& description, const std::string& epilog)
    : description_(description), epilog_(epilog) {}

argparse::~argparse() {}

void argparse::add_description(std::string& message) {
    description_ = message;
}

void argparse::add_epilog(std::string& message) {
    epilog_ = message;
}

std::shared_ptr<argument_group> argparse::add_argument_group(std::string& name, bool required, bool exclusive) {
    auto retval = std::make_shared<argument_group>(name, required, exclusive);
    argument_groups_.push_back(retval);
    return retval;
}

void argparse::print_usage(char** argv) {
    std::cout << "usage: " << argv[0] << " ";
    for (auto arg : all_options.options()) {
        bool is_required = arg->semantic()->is_required();
        std::cout << arg->format_parameter() << std::endl;
    }
}

void argparse::print_help(char** argv) {
    print_usage(argv);
    if (description_ != "") {
        std::cout << description_ << std::endl << std::endl;
    }
    for (std::shared_ptr<argument_group> ag : argument_groups_) {
        std::cout << ag->get_name() << std::endl;
        std::cout << ag->arg_group << std::endl;
    }
    if (epilog_ != "") {
        std::cout << epilog_ << std::endl;
    }
    throw peaceful_ex();
}

po::variables_map argparse::parse_args(int argc, char** argv) {
    po::variables_map vm;

    for (auto ag : argument_groups_) { all_options.add(ag->arg_group); }
    po::store(po::parse_command_line(argc, argv, all_options), vm);
    po::notify(vm);
    if (vm.count("help")) { print_help(argv); }

    return vm;
}
