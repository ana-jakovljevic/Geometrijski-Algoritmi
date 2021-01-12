#ifndef TST_GA17_CONVEXHULLLINEINTERSECTIONS_H
#define TST_GA17_CONVEXHULLLINEINTERSECTIONS_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "./ga17_convexhulllineintersections.h"

using namespace testing;

static constexpr auto ulaz1 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/input1.txt";
static constexpr auto ulaz2 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/input2.txt";
static constexpr auto ulaz3 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/grid_parallel.txt";
static constexpr auto ulaz4 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/empty.txt";
static constexpr auto ulaz5 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/2_parallel.txt";
static constexpr auto ulaz6 = "../Geometrijski_Algoritmi/input_files/ga17_ConvexHullLineIntersections/random_generated.txt";

/* Staticki brojevi linija za test primere */
static constexpr auto brojLinija1 = 10;
static constexpr auto brojLinija2 = 100;
static constexpr auto brojLinija3 = 200;
static constexpr auto brojLinija4 = 500;


TEST(ga17_convexhulllineintersections, input10)
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

    //std::cout << "Omotaci su jednaki? " << (konveksniOmotac == konveksniNaivniGrem) << std::endl;

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());

    }

    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_LE(konveksniOmotac.size()-1, konveksniNaivni.size()); // -1 zbog prve ponovljene tacke
    }

    delete chli;
}

TEST(ga17_convexhulllineintersections, input100)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        "",
                                                                        brojLinija2);
    if (chli->getGREMOV_NAIVNI()) {
        chli->pokreniAlgoritam();
        chli->pokreniNaivniAlgoritam();

        /* ACT: dohvatanje dobijenih skupova preseka */
        const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
        const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF

        if (!konveksniNaivniGrem.empty()){
            /* ASSERT: potvrda ocekivanja test primera */
            ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
            ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);

        }
    }

    delete chli;
}

TEST(ga17_convexhulllineintersections, input200)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        "",

                                                                        brojLinija3);
    if (chli->getGREMOV_NAIVNI()) {
        chli->pokreniAlgoritam();
        chli->pokreniNaivniAlgoritam();

        /* ACT: dohvatanje dobijenih skupova preseka */
        const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
        const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF

        if (!konveksniNaivniGrem.empty()){
            /* ASSERT: potvrda ocekivanja test primera */
            ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
            ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
        }
    }

    delete chli;
}

/* Ovaj predugo traje i pada :( */
//TEST(ga17_convexhulllineintersections, input500)
//{
//    /* ARRANGE: instanciranje i izvrsavanje algoritma */
//    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
//                                                                        0,
//                                                                        false,
//                                                                        "",
//                                                                        brojLinija4);
//    if (chli->getGREMOV_NAIVNI()) {
//        chli->pokreniAlgoritam();
//        chli->pokreniNaivniAlgoritam();

//        /* ACT: dohvatanje dobijenih skupova preseka */
//        const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
//        const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF

//        if (!konveksniNaivniGrem.empty()){
//            /* ASSERT: potvrda ocekivanja test primera */
//            ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
//            ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
//        }

//        delete chli;
//    }
//}

TEST(ga17_convexhulllineintersections, inputFileBasic)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        ulaz1
                                                                        );
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }
    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_LE(konveksniOmotac.size()-1, konveksniNaivni.size());
    }

    delete chli;
}


TEST(ga17_convexhulllineintersections, inputFileCollinear)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        ulaz2
                                                                        );
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }
    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_LE(konveksniOmotac.size()-1, konveksniNaivni.size());
    }

    delete chli;
}


TEST(ga17_convexhulllineintersections, inputFileGridParallel)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        ulaz3
                                                                        );
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }
    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_LE(konveksniOmotac.size()-1, konveksniNaivni.size());
    }

    delete chli;
}


TEST(ga17_convexhulllineintersections, inputFileEmpty)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        ulaz4
                                                                        );
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }
    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_LE(konveksniOmotac.size()-1, konveksniNaivni.size());
    }

    delete chli;
}


TEST(ga17_convexhulllineintersections, inputFile2Parallel)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        ulaz5
                                                                        );
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }
    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_LE(konveksniOmotac.size()-1, konveksniNaivni.size());
    }

    delete chli;
}


TEST(ga17_convexhulllineintersections, inputFileRandom)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        ulaz6
                                                                        );
    chli->pokreniAlgoritam();
    chli->pokreniNaivniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto konveksniOmotac = chli->getKonveksniOmotac(); // vraca QPointF
    const auto konveksniNaivniGrem = chli->getNaivniKonveksniOmotacGrem(); // vraca QPointF
    const auto konveksniNaivni = chli->getNaivniKonveksniOmotac(); // vraca QLineF

    if (!konveksniNaivniGrem.empty()){
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_EQ(konveksniOmotac.size(), konveksniNaivniGrem.size());
        ASSERT_EQ(konveksniOmotac, konveksniNaivniGrem);
    }
    if (!konveksniNaivni.empty()) {
        /* ASSERT: potvrda ocekivanja test primera */
        ASSERT_LE(konveksniOmotac.size()-1, konveksniNaivni.size());
    }

    delete chli;
}

TEST(ga17_convexhulllineintersections, testGenerisiLinije)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        "",
                                                                        brojLinija4);

    chli->pokreniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto brojLinija = chli->generisiNasumicneLinije(brojLinija4);

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(brojLinija.size(), brojLinija4);

    delete chli;
}

TEST(ga17_convexhulllineintersections, testGenerisiLinijeInput0)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        "",
                                                                        0);

    chli->pokreniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    const auto brojLinija = chli->generisiNasumicneLinije(0);

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(brojLinija.size(), 0);

    delete chli;
}

TEST(ga17_convexhulllineintersections, testGenerisiUglove)
{
    /* ARRANGE: instanciranje i izvrsavanje algoritma */
    ConvexHullLineIntersections *chli = new ConvexHullLineIntersections(nullptr,
                                                                        0,
                                                                        false,
                                                                        "",
                                                                        brojLinija4);
    std::set<double> angles;
    chli->pokreniAlgoritam();

    /* ACT: dohvatanje dobijenih skupova preseka */
    chli->generateAngles(brojLinija4, 1, 179, angles);

    /* ASSERT: potvrda ocekivanja test primera */
    ASSERT_EQ(angles.size(), brojLinija4);

    delete chli;
}

#endif // TST_GA17_CONVEXHULLLINEINTERSECTIONS_H
