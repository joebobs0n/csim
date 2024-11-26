#include "planar_fet.hpp"

const PlanarFET::Tech PlanarFET::t180nm = {180e-9, 5e-9, 15e-9, 0.4, 35e-3, 15e-3, 0.015, 100};
const PlanarFET::Tech PlanarFET::t065nm = {65e-9, 2.5e-9, 10e-9, 0.25, 45e-3, 25e-3, 0.02, 200};

double PlanarFET::_getGamma(const Tech &tech, double Vgs, double Vds, ModelUtils::DevType devType) {
    // sigmoid(x) value
    return (Vds - Vgs + tech.Vt);
}

std::pair<double, double> PlanarFET::_normalizeVoltages(double Vgs, double Vds, ModelUtils::DevType devType) {
        // invert Vgs and Vds values for p-type devices
        return (devType == ModelUtils::DevType::N) ? std::make_pair(Vgs, Vds) : std::make_pair(-Vgs, -Vds);
    }

bool PlanarFET::_isConducting(const Tech &tech, double Vgs, ModelUtils::DevType devType) {
    // determine if device is conducting under given conditions
    // assumes Vgs and Vds values are pre-negated for p-type devices
    return (devType == ModelUtils::DevType::N) ? Vgs >= tech.Vt : Vgs <= -tech.Vt;
}

bool PlanarFET::_inSaturation(const Tech &tech, double Vgs, double Vds, ModelUtils::DevType devType) {
    // determine if device is in saturation under given conditions
    // assumes Vgs and Vds values are pre-negated for p-type devices
    bool isSaturation = (devType == ModelUtils::DevType::N) ? Vds >= Vgs - tech.Vt : Vds <= Vgs - tech.Vt;
    return _isConducting(tech, Vgs, devType) && isSaturation;
}

double PlanarFET::_getId_lin(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType) {
    // get current through device in linear mode
    // assumes Vgs and Vds values are pre-negated for p-type devices
    auto mu = (devType == ModelUtils::DevType::N) ? tech.MUn : tech.MUp;
    return _isConducting(tech, Vgs, devType) ? mu * tech.Cox * (W/tech.L) * ((Vgs - tech.Vt) * Vds - (std::pow(Vds, 2) / 2)) * (1 + tech.LAMBDA * Vds) : 0.0;
}

double PlanarFET::_getId_sat(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType) {
    // get current through device in saturation mode
    // assumes Vgs and Vds values are pre-negated for p-type devices
    auto mu = (devType == ModelUtils::DevType::N) ? tech.MUn : tech.MUp;
    return _isConducting(tech, Vgs, devType) ? 0.5 * mu * tech.Cox * (W/tech.L) * std::pow(Vgs - tech.Vt, 2) * (1 + tech.LAMBDA * Vds) : 0.0;
}

double PlanarFET::_getGm_lin(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType, bool useAnalyticModel) {
    // get transconductance of device in linear mode
    // assumes Vgs and Vds values are pre-negated for p-type devices
    auto mu = (devType == ModelUtils::DevType::N) ? tech.MUn : tech.MUp;
    double Gm = 0.0;
    if (_isConducting(tech, Vgs, devType)) {
        if (useAnalyticModel) {
            Gm = mu * tech.Cox * (W/tech.L) * Vds;
        } else {
            double Vgs_delta = std::max(0.01 * tech.Vt, 1e-3);
            auto Id_0 = _getId_lin(tech, W, Vgs, Vds, devType);
            auto Id_1 = _getId_lin(tech, W, Vgs + Vgs_delta, Vds, devType);
            Gm = (Id_1 - Id_0) / Vgs_delta;
        }
    }
    return Gm;
}

double PlanarFET::_getGm_sat(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType, bool useAnalyticModel) {
    // get transconductance of device in saturation mode
    // assumes Vgs and Vds values are pre-negated for p-type devices
    auto mu = (devType == ModelUtils::DevType::N) ? tech.MUn : tech.MUp;
    double Gm = 0.0;
    if (_isConducting(tech, Vgs, devType)) {
        if (useAnalyticModel) {
            Gm = mu * tech.Cox * (W/tech.L) * (Vgs - tech.Vt);
        } else {
            double Vgs_delta = std::max(0.01 * tech.Vt, 1e-3);
            auto Id_0 = _getId_sat(tech, W, Vgs, Vds, devType);
            auto Id_1 = _getId_sat(tech, W, Vgs + Vgs_delta, Vds, devType);
            Gm = (Id_1 - Id_0) / Vgs_delta;
        }
    }
    return Gm;
}

double PlanarFET::_getCgs_lin(const Tech &tech, double W) {
    // get channel cap value between gate and source in linear mode
    // assumes Vgs and Vds values are pre-negated for p-type devices
    return 0.5 * tech.Cox * W * tech.L;
}

double PlanarFET::_getCgs_sat(const Tech &tech, double W) {
    // get channel cap value between gate and source in saturation mode
    // assumes Vgs and Vds values are pre-negated for p-type devices
    return (2.0 / 3.0) * tech.Cox * W * tech.L;
}

double PlanarFET::_getCgd_lin(const Tech &tech, double W) {
    // get channel cap value between gate and drain in linear mode
    // assumes Vgs and Vds values are pre-negated for p-type devices
    return 0.5 * tech.Cox * W * tech.L;
}

double PlanarFET::_getCgd_sat(const Tech &tech, double W) {
    // get channel cap value between gate and drain in saturation mode
    // assumes Vgs and Vds values are pre-negated for p-type devices
    return (1.0 / 3.0) * tech.Cox * W * tech.L;
}

double PlanarFET::getId(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType) {
    // get current through device with smoothing between linear and saturation modes
    // performs Vgs and Vds negation for p-type devices
    auto [normVgs, normVds] = _normalizeVoltages(Vgs, Vds, devType);
    auto Id_lin = _getId_lin(tech, W, normVgs, normVds, devType);
    auto Id_sat = _getId_sat(tech, W, normVgs, normVds, devType);
    auto gamma = _getGamma(tech, normVgs, normVds, devType);
    return ModelUtils::fx_smooth(tech.BETA, gamma, Id_sat, Id_lin);
}

double PlanarFET::getGm(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType, bool useAnalyticModel) {
    // get transconductance of device with smoothing between linear and saturation modes
    // performs Vgs and Vds negation for p-type devices
    double Gm = 0.0;
    if (_isConducting(tech, Vgs, devType)) {
        if (useAnalyticModel) {
            auto [normVgs, normVds] = _normalizeVoltages(Vgs, Vds, devType);
            auto gm_lin = _getGm_lin(tech, W, normVgs, normVds, devType);
            auto gm_sat = _getGm_sat(tech, W, normVgs, normVds, devType);
            auto gamma = _getGamma(tech, normVgs, normVds, devType);
            Gm = ModelUtils::fx_smooth(tech.BETA, gamma, gm_sat, gm_lin);
        } else {
            // dynamically choosing Vgs so that smaller devices are not over impacted
            // choosing a minimum value of 100nV to maintain numerical stability
            double Vgs_delta = std::max(0.01 * tech.Vt, 1e-3);
            if (devType == ModelUtils::DevType::P) Vgs_delta *= -1;
            auto Id_0 = getId(tech, W, Vgs, Vds, devType);
            auto Id_1 = getId(tech, W, Vgs + Vgs_delta, Vds, devType);
            Gm = (Id_1 - Id_0) / Vgs_delta;
        }
    }
    return Gm;
}

double PlanarFET::getCgs(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType) {
    // get cap value between gate and source with smoothing between linear and saturation modes
    // performs Vgs and Vds negation for p-type devices
    double Cgs = tech.Covl;
    if (_isConducting(tech, Vgs, devType)) {
        auto [normVgs, normVds] = _normalizeVoltages(Vgs, Vds, devType);
        auto Cgs_lin = _getCgs_lin(tech, (W - tech.Lovl)) + tech.Covl;
        auto Cgs_sat = _getCgs_sat(tech, (W - tech.Lovl)) + tech.Covl;
        auto gamma = _getGamma(tech, normVgs, normVds, devType);
        Cgs = ModelUtils::fx_smooth(tech.BETA, gamma, Cgs_sat, Cgs_lin);
    }
    return Cgs;
}

double PlanarFET::getCgd(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType) {
    // get cap value between gate and drain with smoothing between linear and saturation modes
    // performs Vgs and Vds negation for p-type devices
    double Cgd = tech.Covl;
    if (_isConducting(tech, Vgs, devType)) {
        auto [normVgs, normVds] = _normalizeVoltages(Vgs, Vds, devType);
        auto Cgd_lin = _getCgd_lin(tech, (W - tech.Lovl)) + tech.Covl;
        auto Cgd_sat = _getCgd_sat(tech, (W - tech.Lovl)) + tech.Covl;
        auto gamma = _getGamma(tech, normVgs, normVds, devType);
        Cgd = ModelUtils::fx_smooth(tech.BETA, gamma, Cgd_sat, Cgd_lin);
    }
    return Cgd;
}

double PlanarFET::getTransientCurrent(const Tech &tech, double W, double Vgs, double Vds, double dVgs_dt, double dVds_dt, ModelUtils::DevType devType) {
    // get instantaneous current
    // values perform p-type negation as needed
    auto Id = getId(tech, W, Vgs, Vds, devType);
    auto Cgs = getCgs(tech, W, Vgs, Vds, devType);
    auto Cgd = getCgd(tech, W, Vgs, Vds, devType);
    return Id + Cgs * dVgs_dt + Cgd * dVds_dt;
}

double PlanarFET::getInstantaneousPower(const Tech &tech, double W, double Vgs, double Vds, ModelUtils::DevType devType) {
    // get instantaneous power
    // getId() handles p-type negation
    return (devType == ModelUtils::DevType::N ? Vds : -Vds) * getId(tech, W, Vgs, Vds, devType);
}
