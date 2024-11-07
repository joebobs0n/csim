#pragma once
#ifndef _MODELS_HPP_
#define _MODELS_HPP_

#define VAL const static double

#include <cmath>

namespace Model {
    VAL EPSo = 8.854e-12;      // [F/m] permittivity of free space
    VAL ETAox = 3.9;           // [const] relative permittivity of silicon oxide
    VAL EPSox = EPSo * ETAox;  // [F/m] permittivity of silicon oxide (~34.53e-12 F/m)

    namespace Active {
        double sigmoid(double Vgs, double Vds, double Vt) {
            double BETA = 100;  // [V^-1] linear->saturation smoothing constant
            return 1.0 / (1.0 + std::exp(-BETA * (Vds - Vgs + Vt)));
        }

        //? Planar FET Model (traditionally called MOSFET)
        inline namespace Planar {
            VAL L = 180e-9;         // [m] channel length
            VAL Tox = 5e-9;         // [m] gate oxide thickness
            VAL Vt = 0.4;           // [V] threshold voltage
            VAL MUn = 35e-3;        // [m^2/V*s] electron mobility
            VAL MUp = 15e-3;        // [m^2/V*s] hole mobility
            VAL LAMBDA =  0.015;    // [V^-1] channel length modulation
            VAL Cox = EPSox / Tox;  // [F/m^2] gate capacitance per area (~6.9e-3 F/m^2)

            bool _isConducting(double Vgs) {
                return (std::abs(Vgs) >= Vt);
            }

            bool _inSaturation(double Vgs, double Vds) {
                return _isConducting(Vgs) ? (std::abs(Vds) > std::abs(Vgs) - Vt) : false;
            }

            double _getId_lin(double W, double Vgs, double Vds) {
                auto mu = (Vgs > 0) ? MUn : MUp;
                return _isConducting(Vgs) ? mu * Cox * (W/L) * ((Vgs - Vt) * Vds - (std::pow(Vds, 2) / 2)) * (1 + LAMBDA * Vds) : 0.0;
            }

            double _getId_sat(double W, double Vgs, double Vds) {
                auto mu = (Vgs > 0) ? MUn : MUp;
                return _isConducting(Vgs) ? 0.5 * mu * Cox * (W/L) * std::pow(Vgs - Vt, 2) * (1 + LAMBDA * Vds) : 0.0;
            }

            double getId(double W, double Vgs, double Vds) {
                auto mu = (Vgs > 0) ? MUn : MUp;
                auto alpha = sigmoid(Vgs, Vds, Vt);
                auto Id_lin = _getId_lin(W, Vgs, Vds);
                auto Id_sat = _getId_sat(W, Vgs, Vds);
                return alpha * Id_sat + (1 - alpha) * Id_lin;
            }

            double _getGm_lin(double W, double Vgs, double Vds) {
                auto mu = (Vgs > 0) ? MUn : MUp;
                return _isConducting(Vgs) ? mu * Cox * (W/L) * Vds : 0.0;
            }

            double _getGm_sat(double W, double Vgs, double Vds) {
                auto mu = (Vgs > 0) ? MUn : MUp;
                return _isConducting(Vgs) ? mu * Cox * (W/L) * (Vgs - Vt) : 0.0;
            }

            double getGm(double W, double Vgs, double Vds) {
                auto alpha = sigmoid(Vgs, Vds, Vt);
                auto gm_lin = _getGm_lin(W, Vgs, Vds);
                auto gm_sat = _getGm_sat(W, Vgs, Vds);
                return alpha * gm_sat + (1 - alpha) * gm_lin;
            }

            double _getCgs_lin(double W) {
                return 0.5 * Cox * W * L;
            }

            double _getCgd_lin(double W) {
                return 0.5 * Cox * W * L;
            }

            double _getCgs_sat(double W) {
                return (2.0 / 3.0) * Cox * W * L;
            }

            double _getCgd_sat(double W) {
                return (1.0 / 3.0) * Cox * W * L;
            }

            double getCgs(double W, double Vgs, double Vds) {
                double Cgs = 0.0;
                if (_isConducting(Vgs)) {
                    auto alpha = sigmoid(Vgs, Vds, Vt);
                    Cgs = alpha * _getCgs_sat(W) + (1 - alpha) * _getCgs_lin(W);
                }
                return Cgs;
            }

            double getCgd(double W, double Vgs, double Vds) {
                double Cgd = 0.0;
                if (_isConducting(Vgs)) {
                    auto alpha = sigmoid(Vgs, Vds, Vt);
                    Cgd = alpha * _getCgd_sat(W) + (1 - alpha) * _getCgd_lin(W);
                }
                return Cgd;
            }

            double getTransientCurrent(double W, double Vgs, double Vds, double dVgs_dt, double dVds_dt) {
                auto Id = getId(W, Vgs, Vds);
                auto Cgs = getCgs(W, Vgs, Vds);
                auto Cgd = getCgd(W, Vgs, Vds);
                return Id + Cgs * dVgs_dt + Cgd * dVds_dt;
            }

            double getInstantaneousPower(double W, double Vgs, double Vds) {
                return Vds * getId(W, Vgs, Vds);
            }
        }
    }

    namespace Passive {
        namespace R {}
        namespace C {}
        namespace L {}
    }
}

#endif

