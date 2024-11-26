#pragma once
#ifndef _ELEMENT_HPP_
#define _ELEMENT_HPP_

#include <string>
#include <vector>

class Element {
protected:
    std::string _name;
    std::vector<std::string> _terminals;
    std::string _type, _subtype;

public:
    Element(const std::string &name, const std::string &type, const std::string &subtype, const std::vector<std::string> &terminals);
    virtual ~Element() = default;

    std::string getName();
    std::pair<std::string, std::string> getType();
    std::vector<std::string> getTerminals();

    virtual std::string getElementType() const = 0;
    virtual std::vector<std::string>getTerminals() const = 0;
};

#endif
