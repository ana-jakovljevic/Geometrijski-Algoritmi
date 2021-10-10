#ifndef TST_GA18_SHORTESTPATH_H
#define TST_GA18_SHORTESTPATH_H
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga18_shortestpath.h"


using namespace testing;

static constexpr auto poligoni = "../Geometrijski_Algoritmi/input_files/ga18_ShortestPath/ulaz.txt";
static constexpr auto poligoni1 = "../Geometrijski_Algoritmi/input_files/ga18_ShortestPath/ulaz1.txt";

TEST(ga18_shortestPathTests, jednakost_najkracih_puteva_duzina){
    ShortestPath sp(nullptr, 0, false, "", 10);

    sp.pokreniAlgoritam();
    auto dobijenoAlgoritmom = sp.getNajkraciPut().size();
    auto duzinaDobijenogAlgoritma = sp.getDuzinaNajkracegPuta();
    sp.pokreniNaivniAlgoritam();
    auto dobijenoNaivnim = sp.getNajkraciPut().size();
    auto duzinaNaivnogAlgoritma = sp.getDuzinaNajkracegPuta();
    ASSERT_EQ(dobijenoAlgoritmom, dobijenoNaivnim);
    ASSERT_EQ(duzinaDobijenogAlgoritma, duzinaNaivnogAlgoritma);
}

TEST(ga18_shortestPathTests, jednakost_najkracih_puteva_vrednost){
    ShortestPath sp(nullptr, 0, false, "", 20);

    sp.pokreniAlgoritam();
    auto dobijenoAlgoritmom = sp.getNajkraciPut().size();
    auto duzinaDobijenogAlgoritma = sp.getDuzinaNajkracegPuta();
    sp.pokreniNaivniAlgoritam();
    auto dobijenoNaivnim = sp.getNajkraciPut().size();
    auto duzinaNaivnogAlgoritma = sp.getDuzinaNajkracegPuta();
    ASSERT_EQ(dobijenoAlgoritmom, dobijenoNaivnim);
    ASSERT_EQ(duzinaDobijenogAlgoritma, duzinaNaivnogAlgoritma);
}

TEST(ga18_shortestPathTests, jednakost_najkracih_puteva_iz_datoteke_vrednost){
    ShortestPath sp(nullptr, 0, false, poligoni, 0);

    sp.pokreniAlgoritam();
    auto dobijenoAlgoritmom = sp.getDuzinaNajkracegPuta();
    sp.pokreniNaivniAlgoritam();
    auto dobijenoNaivnim = sp.getDuzinaNajkracegPuta();
    ASSERT_EQ(dobijenoAlgoritmom, dobijenoNaivnim);
}

TEST(ga18_shortestPathTests, jednakost_najkracih_puteva_iz_datoteke_duzina){
    ShortestPath sp(nullptr, 0, false, poligoni, 0);

    sp.pokreniAlgoritam();
    auto dobijenoAlgoritmom = sp.getNajkraciPut().size();
    sp.pokreniNaivniAlgoritam();
    auto dobijenoNaivnim = sp.getNajkraciPut().size();
    ASSERT_EQ(dobijenoAlgoritmom, dobijenoNaivnim);
}

TEST(ga18_shortestPathTests, jednakost_najkracih_puteva_iz_datoteke1_vrednost){
    ShortestPath sp(nullptr, 0, false, poligoni1, 0);

    sp.pokreniAlgoritam();
    auto dobijenoAlgoritmom = sp.getDuzinaNajkracegPuta();
    sp.pokreniNaivniAlgoritam();
    auto dobijenoNaivnim = sp.getDuzinaNajkracegPuta();
    ASSERT_EQ(dobijenoAlgoritmom, dobijenoNaivnim);
}

TEST(ga18_shortestPathTests, jednakost_najkracih_puteva_iz_datoteke1_duzina){
    ShortestPath sp(nullptr, 0, false, poligoni1, 0);

    sp.pokreniAlgoritam();
    auto dobijenoAlgoritmom = sp.getNajkraciPut().size();
    sp.pokreniNaivniAlgoritam();
    auto dobijenoNaivnim = sp.getNajkraciPut().size();
    ASSERT_EQ(dobijenoAlgoritmom, dobijenoNaivnim);
}
#endif // TST_GA18_SHORTESTPATH_H
