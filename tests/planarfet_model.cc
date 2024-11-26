#include <gtest/gtest.h>
#include <string>
#include <map>

#include "models.hpp"
#include "planar_fet.hpp"

namespace {
    std::map<std::string, int> Condition = {
        {"Cutoff",        0},
        {"Lin_BarelyOn",  1},
        {"Lin_AlmostSat", 2},
        {"Sat_Barely",    3},
        {"Sat_Deep",      4}
    };

    class PlanarTest : public ::testing::Test {
    protected:
        double W;
        double Id_tol, Gm_numeric_tol, Gm_analytic_tol, Cgs_tol, Cgd_tol;
        PlanarFET::Tech tech;
        ModelUtils::DevType N, P;

        PlanarTest() : tech(PlanarFET::t180nm), N(ModelUtils::DevType::N), P(ModelUtils::DevType::P) {}
        // PlanarTest() : tech(PlanarFET::t065nm), N(ModelUtils::DevType::N), P(ModelUtils::DevType::P) {}

        void SetUp() override {
            W = 1e-6;
            Id_tol = Gm_numeric_tol = Cgs_tol = Cgd_tol = 1e-6;
            Gm_analytic_tol = 1.5e-5;  // relaxed tolerance due to approximations in Gm analytic model
        }

        std::pair<double, double> getVolts(int cond) {
            double Vgs, Vds;
            switch (cond) {
            case 0:
                Vgs = tech.Vt - 0.1;
                Vds = 0.5;
                return std::make_pair(Vgs, Vds);

            case 1:
                Vgs = tech.Vt + 0.01;
                Vds = 0.0;
                return std::make_pair(Vgs, Vds);

            case 2:
                Vgs = tech.Vt + 0.2;
                Vds = Vgs - tech.Vt - 0.01;
                return std::make_pair(Vgs, Vds);

            case 3:
                Vgs = tech.Vt + 0.5;
                Vds = Vgs - tech.Vt;
                return std::make_pair(Vgs, Vds);

            case 4:
                Vgs = tech.Vt + 0.7;
                Vds = Vgs;
                return std::make_pair(Vgs, Vds);

            default:
                throw std::runtime_error("received bad condition enum value [" + std::any_cast<std::string>(cond) + "]");
            }
        }
    };

    namespace test_Id {
        TEST_F(PlanarTest, Id_Ntype_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, N);

            EXPECT_FLOAT_EQ(dut_Id, 0.0);
        }

        TEST_F(PlanarTest, Id_Ptype_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, P);

            EXPECT_FLOAT_EQ(dut_Id, 0.0);
        }

        TEST_F(PlanarTest, Id_Ntype_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, N);
            double ref_Id = PlanarFET_ut_friend::_getId_lin(tech, W, Vgs, Vds, N);

            EXPECT_GE(dut_Id, 0.0);
            EXPECT_NEAR(dut_Id, ref_Id, Id_tol);
        }

        TEST_F(PlanarTest, Id_Ptype_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, P);
            double ref_Id = PlanarFET_ut_friend::_getId_lin(tech, W, Vgs, Vds, P);

            EXPECT_LE(dut_Id, 0.0);
            EXPECT_NEAR(dut_Id, ref_Id, Id_tol);
        }

        TEST_F(PlanarTest, Id_Ntype_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, N);
            double ref_Id = PlanarFET_ut_friend::_getId_lin(tech, W, Vgs, Vds, N);

            EXPECT_GE(dut_Id, 0.0);
            EXPECT_NEAR(dut_Id, ref_Id, Id_tol);
        }

        TEST_F(PlanarTest, Id_Ptype_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, P);
            double ref_Id = PlanarFET_ut_friend::_getId_lin(tech, W, Vgs, Vds, P);

            EXPECT_LE(dut_Id, 0.0);
            EXPECT_NEAR(dut_Id, ref_Id, Id_tol);
        }

        TEST_F(PlanarTest, Id_Ntype_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, N);
            double ref_Id = PlanarFET_ut_friend::_getId_sat(tech, W, Vgs, Vds, N);

            EXPECT_GE(dut_Id, 0.0);
            EXPECT_NEAR(dut_Id, ref_Id, Id_tol);
        }

        TEST_F(PlanarTest, Id_Ptype_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, P);
            double ref_Id = PlanarFET_ut_friend::_getId_sat(tech, W, Vgs, Vds, P);

            EXPECT_LE(dut_Id, 0.0);
            EXPECT_NEAR(dut_Id, ref_Id, Id_tol);
        }

        TEST_F(PlanarTest, Id_Ntype_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, N);
            double ref_Id = PlanarFET_ut_friend::_getId_sat(tech, W, Vgs, Vds, N);

            EXPECT_GE(dut_Id, 0.0);
            EXPECT_NEAR(dut_Id, ref_Id, Id_tol);
        }

        TEST_F(PlanarTest, Id_Ptype_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Id = PlanarFET::getId(tech, W, normVgs, normVds, P);
            double ref_Id = PlanarFET_ut_friend::_getId_sat(tech, W, Vgs, Vds, P);

            EXPECT_GE(dut_Id, 0.0);
            EXPECT_NEAR(dut_Id, ref_Id, Id_tol);
        }
    }

    namespace test_Gm_analytic {
        TEST_F(PlanarTest, Gm_Ntype_AnalyticModel_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);

            EXPECT_FLOAT_EQ(dut_Gm, 0.0);
        }

        TEST_F(PlanarTest, Gm_Ptype_AnalyticModel_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);

            EXPECT_FLOAT_EQ(dut_Gm, 0.0);
        }

        TEST_F(PlanarTest, Gm_Ntype_AnalyticModel_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);
            double ref_Gm = PlanarFET_ut_friend::_getGm_lin(tech, W, Vgs, Vds, N);

            EXPECT_GE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_analytic_tol);
        }

        TEST_F(PlanarTest, Gm_Ptype_AnalyticModel_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);
            double ref_Gm = PlanarFET_ut_friend::_getGm_lin(tech, W, Vgs, Vds, P);

            EXPECT_LE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_analytic_tol);
        }

        TEST_F(PlanarTest, Gm_Ntype_AnalyticModel_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);
            double ref_Gm = PlanarFET_ut_friend::_getGm_lin(tech, W, Vgs, Vds, N);

            EXPECT_GE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_analytic_tol);
        }

        TEST_F(PlanarTest, Gm_Ptype_AnalyticModel_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);
            double ref_Gm = PlanarFET_ut_friend::_getGm_lin(tech, W, Vgs, Vds, P);

            EXPECT_LE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_analytic_tol);
        }

        TEST_F(PlanarTest, Gm_Ntype_AnalyticModel_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);
            double ref_Gm = PlanarFET_ut_friend::_getGm_sat(tech, W, Vgs, Vds, N);

            EXPECT_GE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_analytic_tol);
        }

        TEST_F(PlanarTest, Gm_Ptype_AnalyticModel_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);
            double ref_Gm = PlanarFET_ut_friend::_getGm_sat(tech, W, Vgs, Vds, P);

            EXPECT_LE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_analytic_tol);
        }

        TEST_F(PlanarTest, Gm_Ntype_AnalyticModel_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);
            double ref_Gm = PlanarFET_ut_friend::_getGm_sat(tech, W, Vgs, Vds, N);

            EXPECT_GE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_analytic_tol);
        }

        TEST_F(PlanarTest, Gm_Ptype_AnalyticModel_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);
            double ref_Gm = PlanarFET_ut_friend::_getGm_sat(tech, W, Vgs, Vds, P);

            EXPECT_LE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_analytic_tol);
        }
    }

    namespace test_Gm_numeric {
        TEST_F(PlanarTest, Gm_Ntype_NumericModel_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);

            EXPECT_FLOAT_EQ(dut_Gm, 0.0);
        }

        TEST_F(PlanarTest, Gm_Ptype_NumericModel_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);

            EXPECT_FLOAT_EQ(dut_Gm, 0.0);
        }

        TEST_F(PlanarTest, Gm_Ntype_NumericModel_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);
            double ref_Gm = PlanarFET_ut_friend::_getGm_lin(tech, W, Vgs, Vds, N, false);

            EXPECT_GE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_numeric_tol);
        }

        TEST_F(PlanarTest, Gm_Ptype_NumericModel_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);
            double ref_Gm = PlanarFET_ut_friend::_getGm_lin(tech, W, Vgs, Vds, P, false);

            EXPECT_LE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_numeric_tol);
        }

        TEST_F(PlanarTest, Gm_Ntype_NumericModel_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);
            double ref_Gm = PlanarFET_ut_friend::_getGm_lin(tech, W, Vgs, Vds, N, false);

            EXPECT_GE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_numeric_tol);
        }

        TEST_F(PlanarTest, Gm_Ptype_NumericModel_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);
            double ref_Gm = PlanarFET_ut_friend::_getGm_lin(tech, W, Vgs, Vds, P, false);

            EXPECT_LE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_numeric_tol);
        }

        TEST_F(PlanarTest, Gm_Ntype_NumericModel_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);
            double ref_Gm = PlanarFET_ut_friend::_getGm_sat(tech, W, Vgs, Vds, N, false);

            EXPECT_GE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_numeric_tol);
        }

        TEST_F(PlanarTest, Gm_Ptype_NumericModel_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);
            double ref_Gm = PlanarFET_ut_friend::_getGm_sat(tech, W, Vgs, Vds, P, false);

            EXPECT_LE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_numeric_tol);
        }

        TEST_F(PlanarTest, Gm_Ntype_NumericModel_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, N);
            double ref_Gm = PlanarFET_ut_friend::_getGm_sat(tech, W, Vgs, Vds, N, false);

            EXPECT_GE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_numeric_tol);
        }

        TEST_F(PlanarTest, Gm_Ptype_NumericModel_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Gm = PlanarFET::getGm(tech, W, normVgs, normVds, P);
            double ref_Gm = PlanarFET_ut_friend::_getGm_sat(tech, W, Vgs, Vds, P, false);

            EXPECT_LE(dut_Gm, 0.0);
            EXPECT_NEAR(dut_Gm, ref_Gm, Gm_numeric_tol);
        }
    }

    namespace test_Cgs {
        TEST_F(PlanarTest, Cgs_Ntype_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, N);

            EXPECT_FLOAT_EQ(dut_Cgs, tech.Covl);
        }
        TEST_F(PlanarTest, Cgs_Ptype_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, P);

            EXPECT_FLOAT_EQ(dut_Cgs, tech.Covl);
        }

        TEST_F(PlanarTest, Cgs_Ntype_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, N);
            double ref_Cgs = PlanarFET_ut_friend::_getCgs_lin(tech, W);

            EXPECT_GT(dut_Cgs, tech.Covl);
            EXPECT_NEAR(dut_Cgs, ref_Cgs, Cgs_tol);
        }

        TEST_F(PlanarTest, Cgs_Ptype_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, P);
            double ref_Cgs = PlanarFET_ut_friend::_getCgs_lin(tech, W);

            EXPECT_GT(dut_Cgs, tech.Covl);
            EXPECT_NEAR(dut_Cgs, ref_Cgs, Cgs_tol);
        }

        TEST_F(PlanarTest, Cgs_Ntype_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, N);
            double ref_Cgs = PlanarFET_ut_friend::_getCgs_lin(tech, W);

            EXPECT_GT(dut_Cgs, tech.Covl);
            EXPECT_NEAR(dut_Cgs, ref_Cgs, Cgs_tol);
        }

        TEST_F(PlanarTest, Cgs_Ptype_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, P);
            double ref_Cgs = PlanarFET_ut_friend::_getCgs_lin(tech, W);

            EXPECT_GT(dut_Cgs, tech.Covl);
            EXPECT_NEAR(dut_Cgs, ref_Cgs, Cgs_tol);
        }

        TEST_F(PlanarTest, Cgs_Ntype_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, N);
            double ref_Cgs = PlanarFET_ut_friend::_getCgs_sat(tech, W);

            EXPECT_GT(dut_Cgs, tech.Covl);
            EXPECT_NEAR(dut_Cgs, ref_Cgs, Cgs_tol);
        }

        TEST_F(PlanarTest, Cgs_Ptype_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, P);
            double ref_Cgs = PlanarFET_ut_friend::_getCgs_sat(tech, W);

            EXPECT_GT(dut_Cgs, tech.Covl);
            EXPECT_NEAR(dut_Cgs, ref_Cgs, Cgs_tol);
        }

        TEST_F(PlanarTest, Cgs_Ntype_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, N);
            double ref_Cgs = PlanarFET_ut_friend::_getCgs_sat(tech, W);

            EXPECT_GT(dut_Cgs, tech.Covl);
            EXPECT_NEAR(dut_Cgs, ref_Cgs, Cgs_tol);
        }

        TEST_F(PlanarTest, Cgs_Ptype_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgs = PlanarFET::getCgs(tech, W, normVgs, normVds, P);
            double ref_Cgs = PlanarFET_ut_friend::_getCgs_sat(tech, W);

            EXPECT_GT(dut_Cgs, tech.Covl);
            EXPECT_NEAR(dut_Cgs, ref_Cgs, Cgs_tol);
        }
    }

    namespace test_Cgd {
        TEST_F(PlanarTest, Cgd_Ntype_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, N);

            EXPECT_FLOAT_EQ(dut_Cgd, tech.Covl);
        }
        TEST_F(PlanarTest, Cgd_Ptype_Cutoff) {
            auto [Vgs, Vds] = getVolts(Condition["Cutoff"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, P);

            EXPECT_FLOAT_EQ(dut_Cgd, tech.Covl);
        }

        TEST_F(PlanarTest, Cgd_Ntype_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, N);
            double ref_Cgd = PlanarFET_ut_friend::_getCgd_lin(tech, (W - tech.Lovl)) + tech.Covl;

            EXPECT_GT(dut_Cgd, tech.Covl);
            EXPECT_NEAR(dut_Cgd, ref_Cgd, Cgd_tol);
        }

        TEST_F(PlanarTest, Cgd_Ptype_Lin_BarelyOn) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_BarelyOn"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, P);
            double ref_Cgd = PlanarFET_ut_friend::_getCgd_lin(tech, (W - tech.Lovl)) + tech.Covl;

            EXPECT_GT(dut_Cgd, tech.Covl);
            EXPECT_NEAR(dut_Cgd, ref_Cgd, Cgd_tol);
        }

        TEST_F(PlanarTest, Cgd_Ntype_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, N);
            double ref_Cgd = PlanarFET_ut_friend::_getCgd_lin(tech, (W - tech.Lovl)) + tech.Covl;

            EXPECT_GT(dut_Cgd, tech.Covl);
            EXPECT_NEAR(dut_Cgd, ref_Cgd, Cgd_tol);
        }

        TEST_F(PlanarTest, Cgd_Ptype_Lin_AlmostSat) {
            auto [Vgs, Vds] = getVolts(Condition["Lin_AlmostSat"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, P);
            double ref_Cgd = PlanarFET_ut_friend::_getCgd_lin(tech, (W - tech.Lovl)) + tech.Covl;

            EXPECT_GT(dut_Cgd, tech.Covl);
            EXPECT_NEAR(dut_Cgd, ref_Cgd, Cgd_tol);
        }

        TEST_F(PlanarTest, Cgd_Ntype_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, N);
            double ref_Cgd = PlanarFET_ut_friend::_getCgd_sat(tech, (W - tech.Lovl)) + tech.Covl;

            EXPECT_GT(dut_Cgd, tech.Covl);
            EXPECT_NEAR(dut_Cgd, ref_Cgd, Cgd_tol);
        }

        TEST_F(PlanarTest, Cgd_Ptype_Sat_Barely) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Barely"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, P);
            double ref_Cgd = PlanarFET_ut_friend::_getCgd_sat(tech, (W - tech.Lovl)) + tech.Covl;

            EXPECT_GT(dut_Cgd, tech.Covl);
            EXPECT_NEAR(dut_Cgd, ref_Cgd, Cgd_tol);
        }

        TEST_F(PlanarTest, Cgd_Ntype_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, N);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, N);
            double ref_Cgd = PlanarFET_ut_friend::_getCgd_sat(tech, (W - tech.Lovl)) + tech.Covl;

            EXPECT_GT(dut_Cgd, tech.Covl);
            EXPECT_NEAR(dut_Cgd, ref_Cgd, Cgd_tol);
        }

        TEST_F(PlanarTest, Cgd_Ptype_Sat_Deep) {
            auto [Vgs, Vds] = getVolts(Condition["Sat_Deep"]);
            auto [normVgs, normVds] = PlanarFET_ut_friend::_normalizeVoltages(Vgs, Vds, P);
            double dut_Cgd = PlanarFET::getCgd(tech, W, normVgs, normVds, P);
            double ref_Cgd = PlanarFET_ut_friend::_getCgd_sat(tech, (W - tech.Lovl)) + tech.Covl;

            EXPECT_GT(dut_Cgd, tech.Covl);
            EXPECT_NEAR(dut_Cgd, ref_Cgd, Cgd_tol);
        }
    }

    namespace test_edge_cases {
    }
}

