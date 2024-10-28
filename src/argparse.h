#pragma once
#ifndef ARGPARSE_H
#define ARGPARSE_H

#include <iostream>
#include <string>
#include <memory>
#include <regex>
#include <boost/program_options.hpp>

#include "exceptions.h"

namespace po = boost::program_options;


class argument_group {
public:
    po::options_description arg_group;

    argument_group();
    argument_group(std::string& name, bool required = false, bool exclusive = false);
    ~argument_group();

    void set_name(std::string& name);
    void set_required(bool required);
    void set_exclusive(bool exclusive);

    auto get_name();
    bool get_required();
    bool get_exclusive();

private:
    std::string name_;
    bool required_;
    bool exclusive_;
};


class argparse {
public:
    argparse();
    argparse(const std::string& description, const std::string& epilog = NULL);
    ~argparse();

    void add_description(std::string& message);
    void add_epilog(std::string& message);
    std::shared_ptr<argument_group> add_argument_group(std::string& name, bool required = false, bool exclusive = false);

    void print_usage(char** argv);
    void print_help(char** argv);
    po::variables_map parse_args(int argc, char** argv);

private:
    std::string description_;
    std::string epilog_;
    std::vector<std::shared_ptr<argument_group>> argument_groups_;
    po::options_description all_options;
};

#endif
