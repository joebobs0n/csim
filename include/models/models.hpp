#pragma once
#ifndef _MODELS_HPP_
#define _MODELS_HPP_

#define EXP static constexpr double
#define VAL static const double

#include <cmath>

class SiConstants {
public:
    EXP EPSo = 8.854e-12;      // [F/m] permittivity of free space
    EXP ETAox = 3.9;           // [const] relative permittivity of silicon oxide
    EXP EPSox = EPSo * ETAox;  // [F/m] permittivity of silicon oxide (~34.53e-12 F/m)
};

class ModelUtils {
public:
    enum class DevType { N, P };

    static double sigmoid(double beta, double gamma);
    static double fx_smooth(double beta, double gamma, double fx1, double fx2);
};

#endif
