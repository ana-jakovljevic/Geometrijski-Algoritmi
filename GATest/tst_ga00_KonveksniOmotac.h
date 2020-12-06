#ifndef TST_GA00_KONVEKSNIOMOTAC_H
#define TST_GA00_KONVEKSNIOMOTAC_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "./ga03_konveksniomotac.h"

using namespace testing;

TEST(ga02_konveksniomotac, firstRandomTest)
{
    konveksniomotac ch1(nullptr, 0, nullptr, "", 30);
    ch1.pokreniAlgoritam();
    ch1.pokreniNaivniAlgoritam();
    EXPECT_EQ(ch1.get_konveksni_omotac().size() - 1, ch1.get_naivni_konveksni_omotac().size());
}


TEST(ga02_konveksniomotac, secondRandomTest)
{
    konveksniomotac ch1(nullptr, 0, nullptr, "", 100);
    ch1.pokreniAlgoritam();
    ch1.pokreniNaivniAlgoritam();
    EXPECT_EQ(ch1.get_konveksni_omotac().size() - 1, ch1.get_naivni_konveksni_omotac().size());
}

TEST(ga02_konveksniomotac, thirdRandomTest)
{
    konveksniomotac ch1(nullptr, 0, nullptr, "", 200);
    ch1.pokreniAlgoritam();
    ch1.pokreniNaivniAlgoritam();
    EXPECT_EQ(ch1.get_konveksni_omotac().size() - 1, ch1.get_naivni_konveksni_omotac().size());
}

#endif // TST_GA00_KONVEKSNIOMOTAC_H
