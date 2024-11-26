#include <iostream>
#include <string>
#include <sstream>
#include <filesystem>

#include "argparse.hpp"
#include "logger.hpp"
#include "helpers.hpp"

#include "models.hpp"

namespace po = boost::program_options;
namespace fs = std::filesystem;

static logger Log;

argparse getArgs(int argc, char** argv) {
    std::stringstream description;
    description << cform::green << "C++ Circuit Simulator (CCS)" << cform::end;
    description << " is an attempt at creating a netlist (spice) circuit simulator from scratch.";

    std::stringstream epilog;

    argparse ap(description.str(), epilog.str());

    std::string args_header = cform::underline + "Arguments" + cform::end;
    po::options_description* arg = ap.add_argument_group(args_header);
    arg->add_options()
        (
            "design,d",
            po::value<fs::path>()
                ->value_name("path"),
            "Path to design/circuit netlist."
        )
        (
            "techfile,t",
            po::value<fs::path>()
                ->value_name("path")
                ->default_value("../src/models.hpp"),
            "Path to device models file."
        );

    std::string flags_header = cform::underline + "Flags" + cform::end;
    po::options_description* flg = ap.add_argument_group(flags_header);
    flg->add_options()
        ("verbose,V", "Run in verbose mode.")
        ("quiet,Q", "Run in quiet mode.")
        ("help,h", "Print this help messagem and exit");

    po::variables_map vm = ap.parse_args(argc, argv);
    return ap;
}

int run(argparse args) {

    return 0;
}

int main(int argc, char** argv) {
    try {
        argparse args = getArgs(argc, argv);
        Timer(cform::green + "Runtime" + cform::end);

        Log = logger(fs::path(__FILE__).stem(), args.flag("verbose"));
        RedirectPrintouts rp(
            args.flag("quiet") ? "/dev/null" : "",
            args.flag("quiet") ? "/dev/null" : ""
        );

        return run(args);
    } catch (peaceful_exception &e) {
        return 0;
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0xFF;
}
