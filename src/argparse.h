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


class argparse {
public:
    argparse();
    argparse(const std::string& description, const std::string& epilog = "");
    ~argparse();

    void add_description(std::string& message);
    void add_epilog(std::string& message);
    po::options_description* add_argument_group(std::string& name);

    void print_usage(char** argv);
    void print_help(char** argv);
    po::variables_map parse_args(int argc, char** argv);

private:
    std::string description_;
    std::string epilog_;
    std::vector<po::options_description> argument_groups_;
    po::options_description all_options;
};

#endif
