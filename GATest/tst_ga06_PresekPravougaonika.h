#ifndef TST_GA06_PRESEKPRAVOUGAONIKA_H
#define TST_GA06_PRESEKPRAVOUGAONIKA_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga06_presekPravougaonika.h"

using namespace testing;

/* Staticki podrazumevani parametri algoritma */
static constexpr auto bezCrtanja = nullptr;
static constexpr auto bezPauze = 0;
static constexpr auto naivnostNebitna = false;

/* Staticki nazivi datoteka sa test primerima */
static constexpr auto datoteka1 = "../Geometrijski_Algoritmi/input_files/ga06_PresekPravougaonika/input1.txt";
static constexpr auto datoteka2 = "../Geometrijski_Algoritmi/input_files/ga06_PresekPravougaonika/input2.txt";
static constexpr auto datoteka3 = "../Geometrijski_Algoritmi/input_files/ga06_PresekPravougaonika/input3.txt";

/* Testiranje prve pripremljene datoteke */
TEST(ga06_presekPravougaonikaTests, datotekaTest1)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    PresekPravougaonika algoritam(bezCrtanja, bezPauze,
                                  naivnostNebitna, datoteka1);
    algoritam.pokreniAlgoritam();
    algoritam.pokreniNaivniAlgoritam();
    algoritam.pokreniAlgoritamGrubeSile();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto ocekivanaVelicina = 3ul;
    const auto glavniPreseci = algoritam.getGlavni();
    const auto naivniPreseci = algoritam.getNaivni();
    const auto grubaPreseci = algoritam.getGruba();
    const auto dobijenaVelicina = glavniPreseci.size();

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(dobijenaVelicina, ocekivanaVelicina);
    ASSERT_EQ(glavniPreseci, naivniPreseci);
    ASSERT_EQ(glavniPreseci, grubaPreseci);
}

/* Testiranje druge pripremljene datoteke */
TEST(ga06_presekPravougaonikaTests, datotekaTest2)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    PresekPravougaonika algoritam(bezCrtanja, bezPauze,
                                  naivnostNebitna, datoteka2);
    algoritam.pokreniAlgoritam();
    algoritam.pokreniNaivniAlgoritam();
    algoritam.pokreniAlgoritamGrubeSile();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto ocekivanaVelicina = 2ul;
    const auto glavniPreseci = algoritam.getGlavni();
    const auto naivniPreseci = algoritam.getNaivni();
    const auto grubaPreseci = algoritam.getGruba();
    const auto dobijenaVelicina = glavniPreseci.size();

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(dobijenaVelicina, ocekivanaVelicina);
    ASSERT_EQ(glavniPreseci, naivniPreseci);
    ASSERT_EQ(glavniPreseci, grubaPreseci);
}

/* Testiranje trece pripremljene datoteke */
TEST(ga06_presekPravougaonikaTests, datotekaTest3)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    PresekPravougaonika algoritam(bezCrtanja, bezPauze,
                                  naivnostNebitna, datoteka3);
    algoritam.pokreniAlgoritam();
    algoritam.pokreniNaivniAlgoritam();
    algoritam.pokreniAlgoritamGrubeSile();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto ocekivanaVelicina = 10ul;
    const auto glavniPreseci = algoritam.getGlavni();
    const auto naivniPreseci = algoritam.getNaivni();
    const auto grubaPreseci = algoritam.getGruba();
    const auto dobijenaVelicina = glavniPreseci.size();

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(dobijenaVelicina, ocekivanaVelicina);
    ASSERT_EQ(glavniPreseci, naivniPreseci);
    ASSERT_EQ(glavniPreseci, grubaPreseci);
}

/* Staticki brojevi pravougaonika za test primere */
static constexpr auto randomPravougaonici = "";
static constexpr auto brojPravougaonika1 = 10;
static constexpr auto brojPravougaonika2 = 100;
static constexpr auto brojPravougaonika3 = 1000;

/* Testiranje sa prvim slucajnim pravougaonicima */
TEST(ga06_presekPravougaonikaTests, randomTest1)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    PresekPravougaonika algoritam(bezCrtanja, bezPauze,
                                  naivnostNebitna,
                                  randomPravougaonici,
                                  brojPravougaonika1);
    algoritam.pokreniAlgoritam();
    algoritam.pokreniNaivniAlgoritam();
    algoritam.pokreniAlgoritamGrubeSile();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto glavniPreseci = algoritam.getGlavni();
    const auto naivniPreseci = algoritam.getNaivni();
    const auto grubaPreseci = algoritam.getGruba();

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(glavniPreseci, naivniPreseci);
    ASSERT_EQ(glavniPreseci, grubaPreseci);
}

/* Testiranje sa drugim slucajnim pravougaonicima */
TEST(ga06_presekPravougaonikaTests, randomTest2)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    PresekPravougaonika algoritam(bezCrtanja, bezPauze,
                                  naivnostNebitna,
                                  randomPravougaonici,
                                  brojPravougaonika2);
    algoritam.pokreniAlgoritam();
    algoritam.pokreniNaivniAlgoritam();
    algoritam.pokreniAlgoritamGrubeSile();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto glavniPreseci = algoritam.getGlavni();
    const auto naivniPreseci = algoritam.getNaivni();
    const auto grubaPreseci = algoritam.getGruba();

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(glavniPreseci, naivniPreseci);
    ASSERT_EQ(glavniPreseci, grubaPreseci);
}

/* Testiranje sa trecim slucajnim pravougaonicima */
TEST(ga06_presekPravougaonikaTests, randomTest3)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    PresekPravougaonika algoritam(bezCrtanja, bezPauze,
                                  naivnostNebitna,
                                  randomPravougaonici,
                                  brojPravougaonika3);
    algoritam.pokreniAlgoritam();
    algoritam.pokreniNaivniAlgoritam();
    algoritam.pokreniAlgoritamGrubeSile();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto glavniPreseci = algoritam.getGlavni();
    const auto naivniPreseci = algoritam.getNaivni();
    const auto grubaPreseci = algoritam.getGruba();

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(glavniPreseci, naivniPreseci);
    ASSERT_EQ(glavniPreseci, grubaPreseci);
}

/* Staticki brojevi pravougaonika za granicne slucajeve */
static constexpr auto nemaPravougaonika = 0;
static constexpr auto jedanPravougaonik = 1;

/* Testiranje kada nema pravougaonika */
TEST(ga06_presekPravougaonikaTests, nemaPravougaonika)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    PresekPravougaonika algoritam(bezCrtanja, bezPauze,
                                  naivnostNebitna,
                                  randomPravougaonici,
                                  nemaPravougaonika);
    algoritam.pokreniAlgoritam();
    algoritam.pokreniNaivniAlgoritam();
    algoritam.pokreniAlgoritamGrubeSile();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto glavniPreseci = algoritam.getGlavni();
    const auto naivniPreseci = algoritam.getNaivni();
    const auto grubaPreseci = algoritam.getGruba();

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_TRUE(glavniPreseci.empty());
    ASSERT_TRUE(naivniPreseci.empty());
    ASSERT_TRUE(grubaPreseci.empty());
}

/* Testiranje kada je samo jedan pravougaonik */
TEST(ga06_presekPravougaonikaTests, jedanPravougaonik)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    PresekPravougaonika algoritam(bezCrtanja, bezPauze,
                                  naivnostNebitna,
                                  randomPravougaonici,
                                  jedanPravougaonik);
    algoritam.pokreniAlgoritam();
    algoritam.pokreniNaivniAlgoritam();
    algoritam.pokreniAlgoritamGrubeSile();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto glavniPreseci = algoritam.getGlavni();
    const auto naivniPreseci = algoritam.getNaivni();
    const auto grubaPreseci = algoritam.getGruba();

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_TRUE(glavniPreseci.empty());
    ASSERT_TRUE(naivniPreseci.empty());
    ASSERT_TRUE(grubaPreseci.empty());
}

#endif // TST_GA06_PRESEKPRAVOUGAONIKA_H
