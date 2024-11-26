#pragma once
#ifndef _PLANAR_FET_HPP_
#define _PLANAR_FET_HPP_

#include "models.hpp"

class PlanarFET {
public:
    struct Tech {
        double L;           // [m] channel length
        double Tox;         // [m] gate oxide thickness
        double Lovl;        // [m] drain/source gate overlap
        double Vt;          // [V] threshold voltage
        double MUn;         // [m^2/V*s] electron mobility
        double MUp;         // [m^2/V*s] hole mobility
        double LAMBDA;      // [V^-1] channel length modulation
        double Cox;         // [F/m^2] gate capacitance per area
        double Covl;        // [F]
        double BETA;        // [V^-1] linear->saturation smoothing constant

        Tech(double L, double Tox, double Lovl, double Vt, double MUn, double MUp, double LAMBDA, double BETA)
            : L(L), Tox(Tox), Lovl(Lovl), Vt(Vt), MUn(MUn), MUp(MUp), LAMBDA(LAMBDA), Cox(SiConstants::EPSox / Tox), Covl(SiConstants::EPSox * Lovl / Tox), BETA(BETA) {}
    };

    static const Tech t180nm;
    static const Tech t065nm;

    static double getId(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType);
    static double getGm(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType, bool useAnalyticModel = true);
    static double getCgs(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType);
    static double getCgd(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType);
    static double getTransientCurrent(const Tech &tech, double W, double Vgs, double Vds, double dVgs_dt, double dVds_dt, ModelUtils::DevType devType);
    static double getInstantaneousPower(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType);

private:
    static double _getGamma(const Tech &tech, double Vgs, double Vds, ModelUtils::DevType devType);
    static std::pair<double, double> _normalizeVoltages(double Vgs, double Vds, ModelUtils::DevType devType);
    static bool _isConducting(const Tech &tech, double Vgs, ModelUtils::DevType devType);
    static bool _inSaturation(const Tech &tech, double Vgs, double Vds, ModelUtils::DevType devType);
    static double _getId_lin(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType);
    static double _getId_sat(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType);
    static double _getGm_lin(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType, bool useAnalyticModel = true);
    static double _getGm_sat(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType, bool useAnalyticModel = true);
    static double _getCgs_lin(const Tech &tech, double W);
    static double _getCgs_sat(const Tech &tech, double W);
    static double _getCgd_lin(const Tech &tech, double W);
    static double _getCgd_sat(const Tech &tech, double W);

    friend class PlanarFET_ut_friend;
};

class PlanarFET_ut_friend {
public:
    static double _getGamma(const PlanarFET::Tech &tech, double Vgs, double Vds, ModelUtils::DevType devType) { return PlanarFET::_getGamma(tech, Vgs, Vds, devType); }
    static std::pair<double, double> _normalizeVoltages(double Vgs, double Vds, ModelUtils::DevType devType) { return PlanarFET::_normalizeVoltages(Vgs, Vds, devType); }
    static bool _isConducting(const PlanarFET::Tech &tech, double Vgs, ModelUtils::DevType devType) { return PlanarFET::_isConducting(tech, Vgs, devType); }
    static bool _inSaturation(const PlanarFET::Tech &tech, double Vgs, double Vds, ModelUtils::DevType devType) { return PlanarFET::_inSaturation(tech, Vgs, Vds, devType); }
    static double _getId_lin(const PlanarFET::Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType) { return PlanarFET::_getId_lin(tech, W, Vgs, Vds, devType); }
    static double _getId_sat(const PlanarFET::Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType) { return PlanarFET::_getId_sat(tech, W, Vgs, Vds, devType); }
    static double _getGm_lin(const PlanarFET::Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType, bool useAnalyticModel = true) { return PlanarFET::_getGm_lin(tech, W, Vgs, Vds, devType, useAnalyticModel); }
    static double _getGm_sat(const PlanarFET::Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType, bool useAnalyticModel = true) { return PlanarFET::_getGm_sat(tech, W, Vgs, Vds, devType, useAnalyticModel); }
    static double _getCgs_lin(const PlanarFET::Tech &tech, double W) { return PlanarFET::_getCgs_lin(tech, W); }
    static double _getCgs_sat(const PlanarFET::Tech &tech, double W) { return PlanarFET::_getCgs_sat(tech, W); }
    static double _getCgd_lin(const PlanarFET::Tech &tech, double W) { return PlanarFET::_getCgd_lin(tech, W); }
    static double _getCgd_sat(const PlanarFET::Tech &tech, double W) { return PlanarFET::_getCgd_sat(tech, W); }
};

#endif
