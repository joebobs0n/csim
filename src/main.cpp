#include <iostream>
#include <string>
#include <sstream>
#include <boost/timer/timer.hpp>

#include "argparse.hpp"
#include "logger.hpp"


logger Log = logger();

argparse getArgs(int argc, char** argv) {
    std::string description = "";
    std::string epilog = "";
    argparse ap(description, epilog);

    std::string args_header = cform::underline + "Arguments" + cform::end;
    po::options_description* arg = ap.add_argument_group(args_header);
    arg->add_options();

    std::string flags_header = cform::underline + "Flags" + cform::end;
    po::options_description* flg = ap.add_argument_group(flags_header);
    flg->add_options()
        ("verbose,v", "Run in verbose mode.")
        ("help,h", "Print this help message and exit");

    ap.parse_args(argc, argv);
    return ap;
}

int run(argparse args) {

    return 0;
}

int main(int argc, char** argv) {
    try {
        argparse args = getArgs(argc, argv);
        Log.setVerbosity(args.flag("verbose"));
        boost::timer::auto_cpu_timer t(3, "Runtime: %w sec\n");
        return run(args);
    } catch (peaceful_ex& e) {
        return 0;
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 255;
}
