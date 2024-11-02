#pragma once
#ifndef _DEVICE_HPP_
#define _DEVICE_HPP_

#include <string>

template <typename T>
struct FET {
    const T gate;
    const T drain;
    const T source;
};

class Device {
private:
    const FET<std::string> pinout_;
    const bool polarity_;  //? True: N-type | False: P-type (to emulate gate when conducting)
    const double width_;  // as in w/l --> l (to be) found in process tech file

public:
    Device(std::string &gate, std::string &drain, std::string &source, bool polarity, double width);
    ~Device();
};

#endif
