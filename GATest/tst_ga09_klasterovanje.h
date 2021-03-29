#ifndef TST_GA09_KLASTEROVANJE_H
#define TST_GA09_KLASTEROVANJE_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga09_klasterovanje.h"

static constexpr auto datoteka1klasterovanje = "../Geometrijski_Algoritmi/input_files/ga09_klasterovanje/input1.txt";
static constexpr auto datoteka2klasterovanje = "../Geometrijski_Algoritmi/input_files/ga09_klasterovanje/input2.txt";
static constexpr auto datoteka3klasterovanje = "../Geometrijski_Algoritmi/input_files/ga09_klasterovanje/input3.txt";
static constexpr auto datoteka4klasterovanje = "../Geometrijski_Algoritmi/input_files/ga09_klasterovanje/input4.txt";

using namespace testing;

TEST(ga09_klasterovanje, prviNasumicniTest)
{
    Klasterovanje klasterovanje(nullptr, 0, false, "", 10);
    klasterovanje.pokreniAlgoritam();
    klasterovanje.pokreniNaivniAlgoritam();
    bool condition = abs(klasterovanje.maksimalnoRastojanje() - klasterovanje.maksimalnoRastojanjeNaivnog() ) <= 0.0001;
    EXPECT_TRUE(condition);
}

TEST(ga09_klasterovanje, drugiNasumicniTest)
{
    Klasterovanje klasterovanje(nullptr, 0, false, "", 20);
    klasterovanje.pokreniAlgoritam();
    klasterovanje.pokreniNaivniAlgoritam();
    bool condition = abs(klasterovanje.maksimalnoRastojanje() - klasterovanje.maksimalnoRastojanjeNaivnog() ) <= 0.0001;
    EXPECT_TRUE(condition);
}

TEST(ga09_klasterovanje, treciNasumicniTest)
{
    Klasterovanje klasterovanje(nullptr, 0, false, "", 30);
    klasterovanje.pokreniAlgoritam();
    klasterovanje.pokreniNaivniAlgoritam();
    bool condition = abs(klasterovanje.maksimalnoRastojanje() - klasterovanje.maksimalnoRastojanjeNaivnog() ) <= 0.0001;
    EXPECT_TRUE(condition);
}

TEST(ga09_klasterovanje, datoteka1)
{
    Klasterovanje klasterovanje(nullptr, 0, false, datoteka1klasterovanje);
    klasterovanje.pokreniAlgoritam();
    klasterovanje.pokreniNaivniAlgoritam();
    bool condition = abs(klasterovanje.maksimalnoRastojanje() - klasterovanje.maksimalnoRastojanjeNaivnog() ) <= 0.0001;
    EXPECT_TRUE(condition);
}

TEST(ga09_klasterovanje, datoteka2)
{
    Klasterovanje klasterovanje(nullptr, 0, false, datoteka2klasterovanje);
    klasterovanje.pokreniAlgoritam();
    klasterovanje.pokreniNaivniAlgoritam();
    bool condition = abs(klasterovanje.maksimalnoRastojanje() - klasterovanje.maksimalnoRastojanjeNaivnog() ) <= 0.0001;
    EXPECT_TRUE(condition);
}

TEST(ga09_klasterovanje, datoteka3)
{
    Klasterovanje klasterovanje(nullptr, 0, false, datoteka3klasterovanje);
    klasterovanje.pokreniAlgoritam();
    klasterovanje.pokreniNaivniAlgoritam();
    bool condition = abs(klasterovanje.maksimalnoRastojanje() - klasterovanje.maksimalnoRastojanjeNaivnog() ) <= 0.0001;
    EXPECT_TRUE(condition);
}

TEST(ga09_klasterovanje, datoteka4)
{
    Klasterovanje klasterovanje(nullptr, 0, false, datoteka4klasterovanje);
    klasterovanje.pokreniAlgoritam();
    klasterovanje.pokreniNaivniAlgoritam();
    bool condition = abs(klasterovanje.maksimalnoRastojanje() - klasterovanje.maksimalnoRastojanjeNaivnog() ) <= 0.0001;
    EXPECT_TRUE(condition);
}

TEST(ga09_klasterovanje, nedovoljnoTacaka)
{
    Klasterovanje klasterovanje(nullptr, 0, false, "", 3);
    klasterovanje.pokreniAlgoritam();
    klasterovanje.pokreniNaivniAlgoritam();
    bool condition1 = abs(klasterovanje.maksimalnoRastojanje()) <= 0.0001;
    bool condition2 = abs(klasterovanje.maksimalnoRastojanjeNaivnog() ) <= 0.0001;
    EXPECT_TRUE(condition1);
    EXPECT_TRUE(condition2);
}

TEST(ga09_klasterovanje, rastojanjeTackeOdDuzi)
{
    Klasterovanje klasterovanje(nullptr, 0, false, "", 0);
    QPoint tacka = QPoint(7,1);
    QPoint tackaDuzi1 = QPoint(3,3);
    QPoint tackaDuzi2 = QPoint(9,5);
    QPoint trazenaTacka;
    double rastojanje = klasterovanje.rastojanjeTackeOdDuzi(tacka, tackaDuzi1, tackaDuzi2, trazenaTacka);
    double ocekivanoRastojanje = 10;
    QPoint ocekivanaTrazenaTacka = QPoint(6,4);
    bool condition1 = abs(rastojanje - ocekivanoRastojanje) <= 0.0001;
    bool condition2 = trazenaTacka.x() == ocekivanaTrazenaTacka.x() && trazenaTacka.y() == ocekivanaTrazenaTacka.y();
    EXPECT_TRUE(condition1);
    EXPECT_TRUE(condition2);
    EXPECT_EQ(ocekivanoRastojanje, rastojanje);
}

TEST(ga09_klasterovanje, rastojanjeTackeOdDuziParalelneYOsi)
{
    Klasterovanje klasterovanje(nullptr, 0, false, "", 0);
    QPoint tacka = QPoint(7,6);
    QPoint tackaDuzi1 = QPoint(4,5);
    QPoint tackaDuzi2 = QPoint(4,8);
    QPoint trazenaTacka;
    double rastojanje = klasterovanje.rastojanjeTackeOdDuzi(tacka, tackaDuzi1, tackaDuzi2, trazenaTacka);
    double ocekivanoRastojanje = 9;
    QPoint ocekivanaTrazenaTacka = QPoint(4,6);
    bool condition1 = abs(rastojanje - ocekivanoRastojanje) <= 0.0001;
    bool condition2 = trazenaTacka.x() == ocekivanaTrazenaTacka.x() && trazenaTacka.y() == ocekivanaTrazenaTacka.y();
    EXPECT_TRUE(condition1);
    EXPECT_TRUE(condition2);
    EXPECT_EQ(ocekivanoRastojanje, rastojanje);
}

TEST(ga09_klasterovanje, rastojanjeTackeOdDuziNaKojuNemaNormalu)
{
    Klasterovanje klasterovanje(nullptr, 0, false, "", 0);
    QPoint tacka = QPoint(3,10);
    QPoint tackaDuzi1 = QPoint(3,7);
    QPoint tackaDuzi2 = QPoint(9,3);
    QPoint trazenaTacka;
    double rastojanje = klasterovanje.rastojanjeTackeOdDuzi(tacka, tackaDuzi1, tackaDuzi2, trazenaTacka);
    double ocekivanoRastojanje = 9;
    QPoint ocekivanaTrazenaTacka = QPoint(3,7);
    bool condition1 = abs(rastojanje - ocekivanoRastojanje) <= 0.0001;
    bool condition2 = trazenaTacka.x() == ocekivanaTrazenaTacka.x() && trazenaTacka.y() == ocekivanaTrazenaTacka.y();
    EXPECT_TRUE(condition1);
    EXPECT_TRUE(condition2);
    EXPECT_EQ(ocekivanoRastojanje, rastojanje);
}

#endif // TST_GA09_KLASTEROVANJE_H

