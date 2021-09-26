#ifndef TST_GA12_CLOSESTPAIR_H
#define TST_GA12_CLOSESTPAIR_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga12_closestpair.h"

using namespace testing;

static constexpr auto two_points_file = "../Geometrijski_Algoritmi/input_files/ga12_closestpair/input1.txt";
static constexpr auto f_20_points = "../Geometrijski_Algoritmi/input_files/ga12_closestpair/input2.txt";

TEST(ga12_closestpair, test_two_points){
    ClosestPair cp(nullptr, 0, false, two_points_file);
    cp.pokreniAlgoritam();

    /* Unapred poznate vrednosti prema datoteci */
    EXPECT_LT(abs(cp.min_dst() - 141.4213), 0.0001);
}

TEST(ga12_closestpair, test_two_points_naive){
    ClosestPair cp(nullptr, 0, false, two_points_file);
    cp.pokreniNaivniAlgoritam();

    /* Unapred poznate vrednosti prema datoteci */
    EXPECT_LT(abs(cp.min_dst_naive() - 141.4213), 0.0001);
}

TEST(ga12_closestpair, optimized_equals_file_input){
    ClosestPair cp(nullptr, 0, false, f_20_points);
    cp.pokreniAlgoritam();

    /* Unapred poznate vrednosti prema datoteci */
    EXPECT_LT(abs(cp.min_dst() - 39.6232), 0.0001);

    QPoint p1 = cp.best1();
    QPoint p2 = cp.best2();

    EXPECT_EQ(p2.x(), 850);
    EXPECT_EQ(p2.y(), 330);
    EXPECT_EQ(p1.x(), 889);
    EXPECT_EQ(p1.y(), 323);
}

TEST(ga12_closestpair, naive_equals_file_input){
    ClosestPair cp(nullptr, 0, false, f_20_points);
    cp.pokreniNaivniAlgoritam();

    /* Unapred poznate vrednosti prema datoteci */
    EXPECT_LT(abs(cp.min_dst_naive() - 39.6232), 0.0001);

    QPoint p1_n = cp.best1_naive();
    QPoint p2_n = cp.best2_naive();

    EXPECT_EQ(p1_n.x(), 850);
    EXPECT_EQ(p1_n.y(), 330);
    EXPECT_EQ(p2_n.x(), 889);
    EXPECT_EQ(p2_n.y(), 323);
}

TEST(ga12_closestpair, naive_optimized_equals_big_random_input){
    ClosestPair cp(nullptr, 0, false, "", 250);
    cp.pokreniAlgoritam();
    cp.pokreniNaivniAlgoritam();

    /* Unapred poznate vrednosti prema datoteci */
    EXPECT_EQ(std::round(cp.min_dst()*1000), std::round(cp.min_dst_naive()*1000));

    QPoint p1 = cp.best1();
    QPoint p2 = cp.best2();

    QPoint p1_n = cp.best1_naive();
    QPoint p2_n = cp.best2_naive();

    // We can guarantee that the points are the same but, their ordering can be arbitrary
    if (p1.x() == p1_n.x()) {
        EXPECT_EQ(p1.x(), p1_n.x());
        EXPECT_EQ(p1.y(), p1_n.y());
        EXPECT_EQ(p2.x(), p2_n.x());
        EXPECT_EQ(p2.y(), p2_n.y());
    } else {
        EXPECT_EQ(p1.x(), p2_n.x());
        EXPECT_EQ(p1.y(), p2_n.y());
        EXPECT_EQ(p2.x(), p1_n.x());
        EXPECT_EQ(p2.y(), p1_n.y());
    }
}


#endif // TST_GA12_CLOSESTPAIR_H
