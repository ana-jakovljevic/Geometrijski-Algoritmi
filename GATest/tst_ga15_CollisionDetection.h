#ifndef TST_GA15_COLLISIONDETECTION_H
#define TST_GA15_COLLISIONDETECTION_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "./ga15_collisiondetection.h"

using namespace testing;

static constexpr auto inputFile1 = "../Geometrijski_Algoritmi/input_files/ga15_CollisionDetection/input1.txt";
static constexpr auto inputFile2 = "../Geometrijski_Algoritmi/input_files/ga15_CollisionDetection/input1.txt";
static constexpr auto inputFile3 = "../Geometrijski_Algoritmi/input_files/ga15_CollisionDetection/input1.txt";
static constexpr auto inputFile4 = "../Geometrijski_Algoritmi/input_files/ga15_CollisionDetection/input1.txt";
static constexpr auto inputFile5 = "../Geometrijski_Algoritmi/input_files/ga15_CollisionDetection/input1.txt";
static constexpr auto inputFile6 = "../Geometrijski_Algoritmi/input_files/ga15_CollisionDetection/input1.txt";
static constexpr auto inputFile7 = "../Geometrijski_Algoritmi/input_files/ga15_CollisionDetection/input1.txt";


TEST(ga15_CollisionDetection, test1)
{
    CollisionDetection cd(nullptr, 0, false, "", 20);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test2)
{
    CollisionDetection cd(nullptr, 0, false, "", 100);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test3)
{
    CollisionDetection cd(nullptr, 0, false, "", 150);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test4)
{
    CollisionDetection cd(nullptr, 0, false, inputFile1, 0);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test5)
{
    CollisionDetection cd(nullptr, 0, false, inputFile2, 0);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test6)
{
    CollisionDetection cd(nullptr, 0, false, inputFile3, 0);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test7)
{
    CollisionDetection cd(nullptr, 0, false, inputFile4, 0);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test8)
{
    CollisionDetection cd(nullptr, 0, false, inputFile5, 0);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test9)
{
    CollisionDetection cd(nullptr, 0, false, inputFile6, 0);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

TEST(ga15_CollisionDetection, test10)
{
    CollisionDetection cd(nullptr, 0, false, inputFile7, 0);

    cd.pokreniAlgoritam();
    cd.pokreniNaivniAlgoritam();

    EXPECT_DOUBLE_EQ(sqrt(cd.getMinDistance()),
                     sqrt(cd.getMinDistanceNaive()));
}

#endif // TST_GA15_COLLISIONDETECTION_H
