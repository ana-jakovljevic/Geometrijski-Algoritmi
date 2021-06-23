#ifndef TST_GA10_UNITDISKCOVER_H
#define TST_GA10_UNITDISKCOVER_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga10_unitDiskCover.h"

using namespace testing;

static constexpr auto inputFile = "../Geometrijski_Algoritmi/input_files/ga10_unitDiskCover/file1.txt";
int diskRadius = 10;

/****** G1991 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, NoPointsG1991)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, diskRadius, AlgorithmType::G1991);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, EqualPointsG1991)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, diskRadius, AlgorithmType::G1991);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, RandomInput1G1991)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, diskRadius, AlgorithmType::G1991);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}
TEST(ga10_unitDiskCoverTests, RandomInput2G1991)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 500, diskRadius, AlgorithmType::G1991);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}


/****** LL2014 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, NoPointsLL2014)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, diskRadius, AlgorithmType::LL2014);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, EqualPointsLL2014)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, diskRadius, AlgorithmType::LL2014);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, RandomInput1LL2014)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, diskRadius, AlgorithmType::LL2014);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}

TEST(ga10_unitDiskCoverTests, RandomInput2LL2014)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 500, diskRadius, AlgorithmType::LL2014);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}

/****** BLMS2017 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, NoPointsBLMS2017)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, diskRadius, AlgorithmType::BLMS2017);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, EqualPointsBLMS2017)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, diskRadius, AlgorithmType::BLMS2017);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 4);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, RandomInput1BLMS2017)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, diskRadius, AlgorithmType::BLMS2017);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}

TEST(ga10_unitDiskCoverTests, RandomInput2BLMS2017)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 500, diskRadius, AlgorithmType::BLMS2017);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}


/****** GHS2019 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, NoPointsGHS2019)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, diskRadius, AlgorithmType::GHS2019);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, EqualPointsGHS2019)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, diskRadius, AlgorithmType::GHS2019);

    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
    ASSERT_EQ(algorithm.coverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, RandomInput1GHS2019)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, diskRadius, AlgorithmType::GHS2019);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}

TEST(ga10_unitDiskCoverTests, RandomInput2GHS2019)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 500, diskRadius, AlgorithmType::GHS2019);
    algorithm.pokreniAlgoritam();

    EXPECT_TRUE(algorithm.checkCoverage());
}

/****** RandomAlgorithm1 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, NoPointsRandom1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, diskRadius, AlgorithmType::RANDOM1);
    algorithm.pokreniNaivniAlgoritam();

    EXPECT_TRUE(algorithm.checkNaiveCoverage());
    ASSERT_EQ(algorithm.naiveCoverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, EqualPointsRandom1)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, diskRadius, AlgorithmType::RANDOM1);

    algorithm.pokreniNaivniAlgoritam();

    EXPECT_TRUE(algorithm.checkNaiveCoverage());
    ASSERT_EQ(algorithm.naiveCoverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, RandomInput1Random1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, diskRadius, AlgorithmType::RANDOM1);
    algorithm.pokreniNaivniAlgoritam();

    EXPECT_TRUE(algorithm.checkNaiveCoverage());
}

TEST(ga10_unitDiskCoverTests, RandomInput2Random1)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 50, diskRadius, AlgorithmType::RANDOM1);
    algorithm.pokreniNaivniAlgoritam();

    EXPECT_TRUE(algorithm.checkNaiveCoverage());
}


/****** RandomAlgorithm2 ******/

/* no points */
TEST(ga10_unitDiskCoverTests, NoPointsRandom2)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 0, diskRadius, AlgorithmType::RANDOM2);
    algorithm.pokreniNaivniAlgoritam();

    EXPECT_TRUE(algorithm.checkNaiveCoverage());
    ASSERT_EQ(algorithm.naiveCoverSize(), 0);
}

/* more points on same position */
TEST(ga10_unitDiskCoverTests, EqualPointsRandom2)
{
    UnitDiskCover algorithm(nullptr, 0, false, inputFile, 1, diskRadius, AlgorithmType::RANDOM2);

    algorithm.pokreniNaivniAlgoritam();

    EXPECT_TRUE(algorithm.checkNaiveCoverage());
    ASSERT_EQ(algorithm.naiveCoverSize(), 1);
}

/* generate random points and check if every point is covered */
TEST(ga10_unitDiskCoverTests, RandomInput1Random2)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 20, diskRadius, AlgorithmType::RANDOM2);
    algorithm.pokreniNaivniAlgoritam();

    EXPECT_TRUE(algorithm.checkNaiveCoverage());
}

TEST(ga10_unitDiskCoverTests, RandomInput2Random2)
{
    UnitDiskCover algorithm(nullptr, 0, false, "", 50, diskRadius, AlgorithmType::RANDOM2);
    algorithm.pokreniNaivniAlgoritam();

    EXPECT_TRUE(algorithm.checkNaiveCoverage());
}






#endif // TST_GA10_UNITDISKCOVER_H

