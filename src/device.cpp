#include "device.hpp"

Device::Device(std::string &gate, std::string &drain, std::string &source, bool polarity, double size_ratio)
    : pinout_({gate, drain, source}), polarity_(polarity), size_ratio_(size_ratio) {}

Device::~Device() {}

float Device::getDrainVoltage(const float vgate, const float vsource) {
    float vdrain = 0;
}
