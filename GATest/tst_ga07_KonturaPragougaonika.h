#ifndef TST_GA07_KONTURAPRAVOUGAONIKA_H
#define TST_GA07_KONTURAPRAVOUGAONIKA_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "./ga07_konturaPravougaonika.h"

using namespace testing;

static constexpr auto ulaz1 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/1 jedan.txt";
static constexpr auto ulaz2 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/2 dva odvojena.txt";
static constexpr auto ulaz3 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/3 dva sa presekom.txt";
static constexpr auto ulaz4 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/4 konkavni oblik.txt";
static constexpr auto ulaz5 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/5 oblik s rupom.tx.txt";
static constexpr auto ulaz6 = "../Geometrijski_Algoritmi/input_files/ga07_KonturaPravougaonika/6 preklapanja.txt";

TEST(ga07_KonturaPravougaonika, jedan_pravougaonik)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, ulaz1, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 4);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, dva_odvojena_pravougaonika)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, ulaz2, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 8);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, dva_pravougaonika_sa_presecima)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, ulaz3, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 8);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, konkavni_oblik)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, ulaz4, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 16);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, oblik_s_rupom)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, ulaz5, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 16);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

TEST(ga07_KonturaPravougaonika, pravougaonici_koji_se_prekrivaju)
{
    KonturaPravougaonika *k = new KonturaPravougaonika(nullptr, 0, false, ulaz6, 0);
    k->pokreniAlgoritam();
    auto iviceLP = k->iviceKonture.size();
    k->pokreniNaivniAlgoritam();
    auto iviceBF = k->iviceKonture.size();

    ASSERT_EQ(iviceLP, 6);
    ASSERT_EQ(iviceLP, iviceBF);

    delete k;
}

#endif // TST_GA07_KONTURAPRAVOUGAONIKA_H
