#include "models.hpp"

double ModelUtils::sigmoid(double beta, double gamma) {
    gamma = gamma + 0.04;
    return 1.0 / (1.0 + std::exp(-beta * gamma));
}

double ModelUtils::fx_smooth(double beta, double gamma, double fx1, double fx2) {
    auto alpha = sigmoid(beta, gamma);
    return alpha * fx1 + (1 - alpha) * fx2;
}
