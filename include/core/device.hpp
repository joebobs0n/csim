#pragma once
#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

#include <string>
#include <vector>

#include "element.hpp"


class Device : public Element {
protected:

public:
    Device(const std::string &name, const std::string &type, const std::string &subtype, const std::vector<std::string> &terminals, const std::string &model);
    virtual ~Device() = default;

    // virtual double getId(double Vgs, double Vds) const = 0;
    // virtual double getGm(double Vgs, double Vds) const = 0;
    // virtual double getCgs(double Vgs, double Vds) const = 0;
    // virtual double getCgd(double Vgs, double Vds) const = 0;
    // ...
};

#endif
