#include "device.hpp"

Device::Device(const std::string &name, const std::string &type, const std::string &subtype, const std::vector<std::string> &terminals, const std::string &model)
    : Element(name, type, subtype, terminals) {}
