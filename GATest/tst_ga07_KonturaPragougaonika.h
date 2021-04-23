#ifndef TST_GA07_KONTURAPRAVOUGAONIKA_H
#define TST_GA07_KONTURAPRAVOUGAONIKA_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "./ga07_konturaPravougaonika.h"

using namespace testing;

static constexpr auto KPulaz1 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/1 jedan.txt";
static constexpr auto KPulaz2 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/2 dva odvojena.txt";
static constexpr auto KPulaz3 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/3 dva sa presekom.txt";
static constexpr auto KPulaz4 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/4 konkavni oblik.txt";
static constexpr auto KPulaz5 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/5 oblik s rupom.tx.txt";
static constexpr auto KPulaz6 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/6 preklapanja.txt";

TEST(ga07_KonturaPravougaonika, jedan_pravougaonik)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, KPulaz1, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 4ul);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, dva_odvojena_pravougaonika)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, KPulaz2, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 8ul);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, dva_pravougaonika_sa_presecima)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, KPulaz3, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 8ul);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, konkavni_oblik)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, KPulaz4, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 16ul);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, oblik_s_rupom)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, KPulaz5, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 16ul);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, pravougaonici_koji_se_prekrivaju)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, KPulaz6, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 6ul);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

#endif // TST_GA07_KONTURAPRAVOUGAONIKA_H
