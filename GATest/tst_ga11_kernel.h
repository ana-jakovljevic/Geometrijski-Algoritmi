#ifndef TST_GA11_KERNEL_H
#define TST_GA11_KERNEL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga11_kernel.h"


using namespace testing;

namespace GA11_tst_inputs {
class Ga11Test: public ::testing::Test {
    protected:
        void SetUp() override
        {
            testPoligon1.push_back(QPointF(-2.74, 1.16));
            testPoligon1.push_back(QPointF(0, 2.48));
            testPoligon1.push_back(QPointF(0.46, 4.84));
            testPoligon1.push_back(QPointF(-2.16, 6.82));
            testPoligon1.push_back(QPointF(-4.76, 6.84));
            testPoligon1.push_back(QPointF(-5.94, 3.2));
        }
        QPolygonF testPoligon1;

};

} //GA11_tst_inputs

using namespace GA11_tst_inputs;
TEST_F(Ga11Test, binarnaPretragaTest1)
{
    QPointF referentaTacka(-5.74, 1.5);
    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka, testPoligon1, true);
    EXPECT_EQ(result, testPoligon1.end());
}

TEST_F(Ga11Test, binarnaPretragaTest2)
{
    QPointF referentaTacka2(-10.28, 1.14);
    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka2, testPoligon1, true);
    EXPECT_EQ(result, testPoligon1.end()-1);
}


#endif // TST_GA11_KERNEL_H
