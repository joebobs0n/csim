#include "device.hpp"

Device::Device(std::string &gate, std::string &drain, std::string &source, bool polarity, double width)
    : pinout_({gate, drain, source}), polarity_(polarity), width_(width) {}

Device::~Device() {}
