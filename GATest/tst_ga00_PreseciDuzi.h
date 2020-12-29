#ifndef TST_GA00_PRESECIDUZI_H
#define TST_GA00_PRESECIDUZI_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "./ga05_preseciduzi.h"

using namespace testing;

static constexpr auto duzi = "../Geometrijski_Algoritmi/input_files/ga00_PreseciDuzi/input1.txt";

TEST(ga05_preseciduzi, test1)
{
    PreseciDuzi pd(nullptr, 0, false, "", 5);
    pd.pokreniAlgoritam();
    pd.pokreniNaivniAlgoritam();
    EXPECT_LE(pd.getPreseci().size(), pd.getNaivniPreseci().size());
}

TEST(ga05_preseciduzi, test2)
{
    PreseciDuzi pd(nullptr, 0, false, "", 150);
    pd.pokreniAlgoritam();
    pd.pokreniNaivniAlgoritam();
    EXPECT_LE(pd.getPreseci().size(), pd.getNaivniPreseci().size());
}

TEST(ga05_preseciduzi, test3)
{
    PreseciDuzi pd(nullptr, 0, false, "", 1000);
    pd.pokreniAlgoritam();
    pd.pokreniNaivniAlgoritam();
    EXPECT_LE(pd.getPreseci().size(), pd.getNaivniPreseci().size());
}

TEST(ga05_preseciduzi, test4)
{
    PreseciDuzi pd(nullptr, 0, false, duzi, 0);
    pd.pokreniAlgoritam();
    pd.pokreniNaivniAlgoritam();
    EXPECT_LE(pd.getPreseci().size(), pd.getNaivniPreseci().size());
}

#endif // TST_GA00_PRESECIDUZI_H
