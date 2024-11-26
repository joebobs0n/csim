#pragma once
#ifndef _ARGPARSE_HPP_
#define _ARGPARSE_HPP_

#include <iostream>
#include <string>
#include <boost/program_options.hpp>

#include "logger.hpp"

namespace po = boost::program_options;

class peaceful_exception : public std::exception {
private:
    std::string message_;

public:
    peaceful_exception();
    peaceful_exception(const std::string& message, const int& err_code);
    ~peaceful_exception();

    std::string& what();
};

class argparse {
private:
    std::string description_;
    std::string epilog_;
    std::vector<po::options_description> argument_groups_;
    po::options_description all_options;
    po::variables_map vm;

public:
    argparse(const std::string& description = "", const std::string& epilog = "");
    ~argparse();

    void add_description(std::string& message);
    void add_epilog(std::string& message);
    po::options_description* add_argument_group(std::string& name);

    void print_help(char** argv);
    po::variables_map* parse_args(int argc, char** argv);

    template <typename T>
    T get(const std::string &key) { return vm[key].as<T>(); }
    bool flag(const std::string &key) { return bool(vm.count(key)); }
};

#endif
