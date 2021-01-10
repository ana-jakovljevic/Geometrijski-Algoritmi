#ifndef TST_GA17_CONVEXHULLLINEINTERSECTIONS_H
#define TST_GA17_CONVEXHULLLINEINTERSECTIONS_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "./ga17_convexhulllineintersections.h"

using namespace testing;

static constexpr auto ulaz1 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/input1.txt";
static constexpr auto ulaz2 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/input2.txt";
static constexpr auto ulaz3 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/andja.txt";


/* Staticki brojevi pravougaonika za test primere */
static constexpr auto randomLinije = "";
static constexpr auto brojLinija1 = 10;
static constexpr auto brojLinija2 = 100;
static constexpr auto brojLinija3 = 1000;

TEST(ga17_convexhulllineintersections, test1)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        "",
                                                                        brojLinija1);
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }
    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivni.size());
    }

    delete chli;
}

TEST(ga17_convexhulllineintersections, test2)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        "",
                                                                        brojLinija2);
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }

    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivni.size());
    }

    delete chli;
}

TEST(ga17_convexhulllineintersections, test3)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        "",
                                                                        brojLinija3);
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }
    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivni.size());
    }

    delete chli;
}

#endif // TST_GA17_CONVEXHULLLINEINTERSECTIONS_H
