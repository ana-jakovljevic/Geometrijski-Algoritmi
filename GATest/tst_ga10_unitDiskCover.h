#ifndef TST_GA10_UNITDISKCOVER_H
#define TST_GA10_UNITDISKCOVER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga10_unitDiskCover.h"

using namespace testing;

static constexpr auto inputFile = "../Geometrijski_Algoritmi/input_files/ga10_unitDiskCover/file1";

/*** BLMS ***/

/* no points */
TEST(ga10_unitDiskCoverTests, BLMS1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, AlgorithmType::BLMS);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, BLMS2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, AlgorithmType::BLMS);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 4);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, BLMS3)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, AlgorithmType::BLMS);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}

/*** GHS ***/

/* no points */
TEST(ga10_unitDiskCoverTests, GHS1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, AlgorithmType::GHS);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, GHS2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, AlgorithmType::GHS);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, GHS3)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, AlgorithmType::GHS);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}

/*** LL ***/

/* no points */
TEST(ga10_unitDiskCoverTests, LL1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, AlgorithmType::LL);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, LL2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, AlgorithmType::LL);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, LL3)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, AlgorithmType::LL);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}

/*** G ***/

/* no points */
TEST(ga10_unitDiskCoverTests, G1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, AlgorithmType::G);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, G2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, AlgorithmType::G);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, G3)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, AlgorithmType::G);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}


#endif // TST_GA10_UNITDISKCOVER_H

