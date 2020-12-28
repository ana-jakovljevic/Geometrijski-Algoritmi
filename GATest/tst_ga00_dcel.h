#ifndef TST_GA00_DCEL_H
#define TST_GA00_DCEL_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga06_dcel.h"

using namespace testing;
const auto dcel = DCEL( "../Geometrijski_Algoritmi/input_files/ga00_DCELDemo/mushroom.off", 2, 2);
TEST(ga06_dcel, konstruktorTest){
    EXPECT_EQ(226, dcel.vertices().size());
    //mushroom.off ima 240 strana plus jednu spoljasnju
    EXPECT_EQ(241, dcel.fields().size());
    EXPECT_EQ(928, dcel.edges().size());
}
TEST(ga06_dcel, twinEdgeTest){
   for(auto edge: dcel.edges()){
       EXPECT_EQ(edge->origin()->x(), edge->twin()->next()->origin()->x());
       EXPECT_EQ(edge->origin()->y(), edge->twin()->next()->origin()->y());
   }
}

TEST(ga06_dcel, next_prevEdgeTest1){
   for(auto edge: dcel.edges()){
       EXPECT_EQ(edge->next()->origin()->x(), edge->next()->next()->prev()->origin()->x());
       EXPECT_EQ(edge->next()->origin()->y(), edge->next()->next()->prev()->origin()->y());

   }
}
TEST(ga06_dcel, next_prevEdgeTest2){
   for(auto edge: dcel.edges()){
       EXPECT_EQ(edge->origin()->x(), edge->next()->prev()->origin()->x());
       EXPECT_EQ(edge->origin()->y(), edge->next()->prev()->origin()->y());

   }
}

#endif // TST_GA00_DCEL_H
