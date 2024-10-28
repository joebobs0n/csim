#include "argparse.h"


argparse::argparse()
    : description_(""), epilog_("") {}

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
    std::cout << all_options << std::endl;
    if (epilog_ != "") {
        std::cout << epilog_ << std::endl;
    }
    throw peaceful_ex();
}

po::variables_map argparse::parse_args(int argc, char** argv) {
    po::variables_map vm;

    for (auto ag : argument_groups_) { all_options.add(ag); }
    po::store(po::parse_command_line(argc, argv, all_options), vm);
    po::notify(vm);
    if (vm.count("help")) { print_help(argv); }

    return vm;
}
