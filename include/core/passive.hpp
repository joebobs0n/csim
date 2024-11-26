#pragma once
#ifndef _PASSIVE_HPP_
#define _PASSIVE_HPP_

#include <string>
#include <vector>

#include "element.hpp"

class Passive : public Element {
protected:
    std::string _passiveType;

public:
    Passive(std::string &name, const std::string &type, const std::string &subtype, std::vector<std::string> &terminals);
    virtual ~Passive() = default;
};

#endif
