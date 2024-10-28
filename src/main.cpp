#include <iostream>
#include <string>
#include <sstream>

#include "exceptions.h"
#include "logger.h"
#include "argparse.h"

auto Log = logger();


argparse getArgs(int argc, char** argv) {
    std::string description = "This is my description";
    std::string epilog = "This is my epilog";
    argparse ap = argparse(description, epilog);

    std::string args_header = cform::underline + "Arguments" + cform::end;
    po::options_description* arg = ap.add_argument_group(args_header);
    arg->add_options()
        (
            "foobar,f",
            po::value<float>()->value_name("float")->required(),
            "Test argument."
        );

    std::string flags_header = cform::underline + "Flags" + cform::end;
    po::options_description* flg = ap.add_argument_group(flags_header);
    flg->add_options()
        ("verbose,v", "Run in verbose mode.")
        ("help,h", "Print this help message and exit");

    ap.parse_args(argc, argv);
    return ap;
}

int main(int argc, char** argv) {
    argparse args = getArgs(argc, argv);

    try {

    } catch (peaceful_ex& e) {
        return 0;
    } catch (fatal_ex& e) {
        return e.which();
    }

    return 0;
}
