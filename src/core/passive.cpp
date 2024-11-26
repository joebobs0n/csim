#include "passive.hpp"

Passive::Passive(std::string &name, const std::string &type, const std::string &subtype, std::vector<std::string> &terminals)
    : Element(name, type, subtype, terminals) {}
