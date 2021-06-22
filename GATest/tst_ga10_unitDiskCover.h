#ifndef TST_GA10_UNITDISKCOVER_H
#define TST_GA10_UNITDISKCOVER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga10_unitDiskCover.h"

using namespace testing;

static constexpr auto inputFile = "../Geometrijski_Algoritmi/input_files/ga10_unitDiskCover/file1.txt";


/****** G1991 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, G1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, AlgorithmType::G1991);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, G2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, AlgorithmType::G1991);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, G3)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, AlgorithmType::G1991);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}


/****** LL2014 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, LL1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, AlgorithmType::LL2014);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, LL2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, AlgorithmType::LL2014);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, LL3)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, AlgorithmType::LL2014);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}


/****** BLMS2017 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, BLMS1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, AlgorithmType::BLMS2017);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, BLMS2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, AlgorithmType::BLMS2017);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 4);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, BLMS3)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, AlgorithmType::BLMS2017);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}


/****** GHS2019 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, GHS1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, AlgorithmType::GHS2019);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, GHS2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, AlgorithmType::GHS2019);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, GHS3)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, AlgorithmType::GHS2019);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}




#endif // TST_GA10_UNITDISKCOVER_H

