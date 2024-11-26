#include "element.hpp"

Element::Element(const std::string &name, const std::string &type, const std::string &subtype, const std::vector<std::string> &terminals)
    : _name(name), _terminals(terminals), _type(type), _subtype(subtype) {}

std::string Element::getName() {
    return std::string(_name);
}

std::pair<std::string, std::string> Element::getType() {
    return std::make_pair(_type, _subtype);
}

std::vector<std::string> Element::getTerminals() {
    return _terminals;
}
