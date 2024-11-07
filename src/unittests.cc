#include <gtest/gtest.h>
#include <string>
#include "models.hpp"


// planarFET model testing suite
namespace {
    namespace dut = Model::Active::Planar;

    class PlanarTest : public ::testing::Test {
    protected:
        double W;
        double Vt;

        void SetUp() override {
            W = 1e-6;
            Vt = dut::Vt;
        }

        void checkAll(double Vgs, double Vds, bool saturation) {
            double dut_Id = dut::getId(W, Vgs, Vds);
            double dut_gm = dut::getGm(W, Vgs, Vds);
            double dut_Cgs = dut::getCgs(W, Vgs, Vds);
            double dut_Cgd = dut::getCgd(W, Vgs, Vds);

            double expect_Id = saturation ? dut::_getId_sat(W, Vgs, Vds) : dut::_getId_lin(W, Vgs, Vds);
            double expect_gm = saturation ? dut::_getGm_sat(W, Vgs, Vds) : dut::_getGm_lin(W, Vgs, Vds);
            double expect_Cgs = saturation ? dut::_getCgs_sat(W) : dut::_getCgs_lin(W);
            double expect_Cgd = saturation ? dut::_getCgd_sat(W) : dut::_getCgd_lin(W);

            EXPECT_NEAR(dut_Id, expect_Id, 1e-6);
            EXPECT_NEAR(dut_gm, expect_gm, 5e-6);  // relaxed tolerance due to less accurate gm model
            EXPECT_NEAR(dut_Cgs, expect_Cgs, 1e-6);
            EXPECT_NEAR(dut_Cgd, expect_Cgd, 1e-6);
        }
    };

    TEST_F(PlanarTest, Cutoff) {
        // cutoff : Vgs < Vt
        double Vgs = Vt - 0.1;
        double Vds = 0.5;

        double dut_Id = dut::getId(W, Vgs, Vds);
        double dut_gm = dut::getGm(W, Vgs, Vds);
        double dut_Cgs = dut::getCgs(W, Vgs, Vds);
        double dut_Cgd = dut::getCgd(W, Vgs, Vds);

        EXPECT_FLOAT_EQ(dut_Id, 0.0);
        EXPECT_FLOAT_EQ(dut_gm, 0.0);
        EXPECT_FLOAT_EQ(dut_Cgs, 0.0);
        EXPECT_FLOAT_EQ(dut_Cgd, 0.0);
    }

    TEST_F(PlanarTest, LinearBarelyOn) {
        // linear : Vds < Vgs - Vt
        // barely on : Vgs ~= Vt
        double Vgs = Vt + 0.01;
        double Vds = 0.01;
        checkAll(Vgs, Vds, false);
    }

    TEST_F(PlanarTest, LinearAlmostSaturation) {
        // linear : Vds < Vgs - Vt
        // almost sat : Vds ~= Vgs - Vt
        double Vgs = Vt + 0.2;
        double Vds = Vgs - Vt - 0.01;
        checkAll(Vgs, Vds, false);
    }

    TEST_F(PlanarTest, BarelySaturation) {
        // saturation : Vds >= Vgs - Vt
        // barely sat : Vds = Vgs - Vt
        double Vgs = Vt + 0.5;
        double Vds = Vgs - Vt;
        checkAll(Vgs, Vds, true);
    }

    TEST_F(PlanarTest, DeepSaturation) {
        // saturation : Vds >= Vgs - Vt
        // deep sat: Vds >> Vgs - Vt
        double Vgs = Vt + 0.7;
        double Vds = Vgs;
        checkAll(Vgs, Vds, true);
    }
}

