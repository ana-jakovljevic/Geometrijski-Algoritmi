#ifndef TST_GA00_TRIANGULATION_H
#define TST_GA00_TRIANGULATION_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga07_triangulation.h"

using namespace testing;

static constexpr auto dat1 = "../Geometrijski_Algoritmi/input_files/ga00_Triangulation/input1.txt";
static constexpr auto dat2 = "../Geometrijski_Algoritmi/input_files/ga00_Triangulation/input2.txt";

TEST(ga07_triangulation, datotekaTest1){
    Triangulation tr(nullptr, 0, false, dat1);
    tr.pokreniNaivniAlgoritam();
    const auto &poly = tr.getNaivePolygon();
    const auto &diag = tr.getNaiveDiagonals();

    /* Unapred poznate vrednosti prema datoteci */
    EXPECT_EQ(diag.size(), 4);
    EXPECT_EQ(poly.vsize(), 7);
    EXPECT_EQ(poly.fsize(), 2);
    EXPECT_EQ(poly.esize(), 14);
}

TEST(ga07_triangulation, datotekaTest2){
    Triangulation tr(nullptr, 0, false, dat2);
    tr.pokreniNaivniAlgoritam();
    const auto &poly = tr.getNaivePolygon();
    const auto &diag = tr.getNaiveDiagonals();

    /* Unapred poznate vrednosti prema datoteci */
    EXPECT_EQ(diag.size(), 12);
    EXPECT_EQ(poly.vsize(), 15);
    EXPECT_EQ(poly.fsize(), 2);
    EXPECT_EQ(poly.esize(), 30);
}

TEST(ga07_triangulation, randomTest1){
    constexpr auto n = 10u;
    Triangulation tr(nullptr, 0, false, "", n);
    tr.pokreniNaivniAlgoritam();
    const auto &poly = tr.getNaivePolygon();
    const auto &diag = tr.getNaiveDiagonals();

    /* Kod random temena ide se ukrug */
    auto i = 2ul;
    for (const auto &d : diag) {
        EXPECT_EQ(d.first, poly.vertex(0));
        EXPECT_EQ(d.second, poly.vertex(i++));
    }
    EXPECT_EQ(i, n-1);

    /* Odnosi velicina prema n */
    EXPECT_EQ(diag.size(), n-3);
    EXPECT_EQ(poly.vsize(), n);
    EXPECT_EQ(poly.fsize(), 2);
    EXPECT_EQ(poly.esize(), 2*n);
}

TEST(ga07_triangulation, randomTest2){
    constexpr auto n = 100u;
    Triangulation tr(nullptr, 0, false, "", n);
    tr.pokreniNaivniAlgoritam();
    const auto &poly = tr.getNaivePolygon();
    const auto &diag = tr.getNaiveDiagonals();

    /* Kod random temena ide se ukrug */
    auto i = 2ul;
    for (const auto &d : diag) {
        EXPECT_EQ(d.first, poly.vertex(0));
        EXPECT_EQ(d.second, poly.vertex(i++));
    }
    EXPECT_EQ(i, n-1);

    /* Odnosi velicina prema n */
    EXPECT_EQ(diag.size(), n-3);
    EXPECT_EQ(poly.vsize(), n);
    EXPECT_EQ(poly.fsize(), 2);
    EXPECT_EQ(poly.esize(), 2*n);
}

TEST(ga07_triangulation, jedanTrougao){
    constexpr auto n = 3u;
    Triangulation tr(nullptr, 0, false, "", n);
    tr.pokreniNaivniAlgoritam();

    /* Prazne dijagonale se uklapaju u gornje
     * razmatranje jer je n-3 == 3-3 == 0 */
    EXPECT_TRUE(tr.getNaiveDiagonals().empty());
}

#endif // TST_GA00_TRIANGULATION_H
