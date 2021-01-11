#ifndef TST_GA18_SHORTESTPATH_H
#define TST_GA18_SHORTESTPATH_H
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga18_shortestpath.h"


using namespace testing;

static constexpr auto poligoni = "../Geometrijski_Algoritmi/input_files/ga18_ShortestPath/ulaz.txt";

TEST(ga18_shortestPath, jednakost_najkracih_puteva_duzina){
    ShortestPath sp(nullptr, 0, false, "", 10);

    sp.pokreniAlgoritam();
    auto dobijenoAlgoritmom = sp.getNajkraciPut().size();
    sp.pokreniNaivniAlgoritam();
    auto dobijenoNaivnim = sp.getNajkraciPut().size();
    ASSERT_EQ(dobijenoAlgoritmom, dobijenoNaivnim);
}

TEST(ga18_shortestPath, jednakost_najkracih_puteva_vrednost){
    ShortestPath sp(nullptr, 0, false, "", 10);

    sp.pokreniAlgoritam();
    auto dobijenoAlgoritmom = sp.getDuzinaNajkracegPuta();
    sp.pokreniNaivniAlgoritam();
    auto dobijenoNaivnim = sp.getDuzinaNajkracegPuta();
    ASSERT_EQ(dobijenoAlgoritmom, dobijenoNaivnim);
}
#endif // TST_GA18_SHORTESTPATH_H
