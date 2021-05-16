#ifndef TST_GA11_KERNEL_H
#define TST_GA11_KERNEL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga11_kernel.h"
#include "pomocnefunkcije.h"

using namespace testing;

namespace GA11_tst_inputs {
class ga11_kernelTests: public ::testing::Test {
    protected:
        void SetUp() override
        {
            testPoligon1.push_back(QPointF(-2.74, 1.16));
            testPoligon1.push_back(QPointF(0, 2.48));
            testPoligon1.push_back(QPointF(0.46, 4.84));
            testPoligon1.push_back(QPointF(-2.16, 6.82));
            testPoligon1.push_back(QPointF(-4.76, 6.84));
            testPoligon1.push_back(QPointF(-5.94, 3.2));

            testTrougao.push_back(QPointF(-5, -3));
            testTrougao.push_back(QPointF(-6.94, -4.42));
            testTrougao.push_back(QPointF(-6.44, -0.44));

        }
        QPolygonF testPoligon1;
        QPolygonF testTrougao;
};

} //GA11_tst_inputs

using namespace GA11_tst_inputs;
TEST_F(ga11_kernelTests, binarnaPretragaTest1)
{
    QPointF referentaTacka(-5.74, 1.5);

    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka, testPoligon1.begin(),
                                                                 testPoligon1.end(),
                                                                 pomocneFunkcije::TipBinarnePretrage::MAX);
    EXPECT_EQ(result, testPoligon1.end()-1);
}

TEST_F(ga11_kernelTests, binarnaPretragaTest2)
{
    QPointF referentaTacka2(-10.28, 1.14);
    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka2, testPoligon1.begin(),
                                                                 testPoligon1.end(),
                                                                 pomocneFunkcije::TipBinarnePretrage::MAX);
    EXPECT_EQ(result, testPoligon1.end()-1);
}

TEST_F(ga11_kernelTests, binarnaPretragaTest3)
{
    QPolygonF copyPoligon(testPoligon1);
    copyPoligon[3] = QPointF(-2.82, 10.33);
    QPointF referentaTacka3(-14.4, 5.44);
    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka3, copyPoligon.begin(),
                                                                 copyPoligon.end(),
                                                                 pomocneFunkcije::TipBinarnePretrage::MAX);
    EXPECT_EQ(result, copyPoligon.end()-2);
}

TEST_F(ga11_kernelTests, binarnaPretragaTest4)
{
    QPointF referentaTacka4(2.28, 1.08);
    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka4, testPoligon1.begin(),
                                                                 testPoligon1.end(),
                                                                 pomocneFunkcije::TipBinarnePretrage::MIN);
    EXPECT_EQ(result, testPoligon1.begin()+2);
}

TEST_F(ga11_kernelTests, binarnaPretragaTest5)
{
    QPointF referentaTacka5(5.3, 2.68);
    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka5, testPoligon1.begin(),
                                                                 testPoligon1.end(),
                                                                 pomocneFunkcije::TipBinarnePretrage::MIN);
    EXPECT_EQ(result, testPoligon1.begin()+3);
}

TEST_F(ga11_kernelTests, binarnaPretragaTestNule1)
{
    QPolygonF copyPoligon(testPoligon1);
    std::rotate(copyPoligon.begin(), copyPoligon.begin()+2, copyPoligon.end());
    QPointF referentaTacka6(-9, 4);
    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka6, copyPoligon.begin(),
                                                                 copyPoligon.end(),
                                                                 pomocneFunkcije::TipBinarnePretrage::NULA);

    EXPECT_EQ(result, copyPoligon.begin() + 2);
}

TEST_F(ga11_kernelTests, binarnaPretragaTestNule2)
{
    QPolygonF copyPoligon(testPoligon1);
    std::rotate(copyPoligon.begin(), copyPoligon.begin()+1, copyPoligon.end());
    QPointF referentaTacka6(8, 2);
    auto result = pomocneFunkcije::binarnaPretragaUglovaPoligona(referentaTacka6, copyPoligon.begin(),
                                                                 copyPoligon.end(),
                                                                 pomocneFunkcije::TipBinarnePretrage::NULA);

//    EXPECT_EQ(result, copyPoligon.end()-1);
}

TEST_F(ga11_kernelTests, binarnaPretragaZavrsno1RasteOpada)
{
    QPointF referentaTacka(-6, 2);
    auto result = pomocneFunkcije::tackeOslonca(referentaTacka, testPoligon1);
    EXPECT_EQ(result.first, testPoligon1.begin());
    EXPECT_EQ(result.second, testPoligon1.end()-1);

}

TEST_F(ga11_kernelTests, binarnaPretragaZavrsno2OpadaRaste)
{
    QPointF referentaTacka(-6, 2);
    QPolygonF copyPoligon(testPoligon1);
    std::rotate(copyPoligon.begin(), copyPoligon.begin()+5, copyPoligon.end());
    auto result = pomocneFunkcije::tackeOslonca(referentaTacka, copyPoligon);
    EXPECT_EQ(result.first, copyPoligon.begin());
    EXPECT_EQ(result.second, copyPoligon.begin()+1);

}

#include <QDebug>
TEST_F(ga11_kernelTests, binarnaPretragaZavrsno3RasteOpadaRaste)
{
    QPointF referentaTacka(-8, 6);
    auto result = pomocneFunkcije::tackeOslonca(referentaTacka, testPoligon1);
//    qInfo() << *(result.first) << "===============" << *(result.second);
    EXPECT_EQ((result.first), (testPoligon1.begin()+4));
    EXPECT_EQ((result.second), (testPoligon1.begin()+5));

}

TEST_F(ga11_kernelTests, binarnaPretragaZavrsno4OpadaRasteOpada)
{
    QPointF referentaTacka(-2, -2);
    auto result = pomocneFunkcije::tackeOslonca(referentaTacka, testPoligon1);
//    qInfo() << *(result.first) << "===============" << *(result.second);
    EXPECT_EQ((result.first), (testPoligon1.begin()+1));
    EXPECT_EQ((result.second), (testPoligon1.begin()+5));

}


#endif // TST_GA11_KERNEL_H
