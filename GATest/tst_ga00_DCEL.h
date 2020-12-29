#ifndef TST_GA00_DCEL_H
#define TST_GA00_DCEL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga06_dcel.h"

using namespace testing;
static constexpr auto dat = "../Geometrijski_Algoritmi/input_files/ga00_DCELDemo/mushroom.off";

TEST(ga06_dcel, konstruktorTest){
    DCEL mushroom(dat, 2, 2);
    EXPECT_EQ(226, mushroom.vsize());
    //mushroom.off ima 240 strana plus jednu spoljasnju
    EXPECT_EQ(241, mushroom.fsize());
    EXPECT_EQ(928, mushroom.esize());
}
TEST(ga06_dcel, twinEdgeTest){
   DCEL mushroom(dat, 2, 2);
   for (auto edge: mushroom.edges()){
       EXPECT_EQ(edge->origin()->x(), edge->twin()->next()->origin()->x());
       EXPECT_EQ(edge->origin()->y(), edge->twin()->next()->origin()->y());
   }
}

TEST(ga06_dcel, next_prevEdgeTest1){
   DCEL mushroom(dat, 2, 2);
   for (auto edge: mushroom.edges()){
       EXPECT_EQ(edge->next()->origin()->x(), edge->next()->next()->prev()->origin()->x());
       EXPECT_EQ(edge->next()->origin()->y(), edge->next()->next()->prev()->origin()->y());

   }
}
TEST(ga06_dcel, next_prevEdgeTest2){
   DCEL mushroom(dat, 2, 2);
   for (auto edge: mushroom.edges()){
       EXPECT_EQ(edge->origin()->x(), edge->next()->prev()->origin()->x());
       EXPECT_EQ(edge->origin()->y(), edge->next()->prev()->origin()->y());
   }
}

#endif // TST_GA00_DCEL_H
