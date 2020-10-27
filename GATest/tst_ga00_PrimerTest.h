#ifndef TST_GA00_PRIMERTEST_H
#define TST_GA00_PRIMERTEST_H

#include<gtest/gtest.h>
#include<gmock/gmock-matchers.h>
// #include"../Geometrijski_Algoritmi/algoritmi_sa_vezbi/

using namespace testing;

TEST(ga00_PrimerTests, firstRandomTest)
{
    int i = 3;
    EXPECT_EQ(i, 3);
}

#endif // TST_GA00_PRIMERTEST_H
