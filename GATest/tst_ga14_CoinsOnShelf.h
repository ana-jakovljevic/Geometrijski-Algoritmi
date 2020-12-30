#ifndef TST_GA14_COINSONSHELF_H
#define TST_GA14_COINSONSHELF_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include <./ga14_coinsOnShelf.h>
using namespace testing;

const string specialCase1File = "../Geometrijski_Algoritmi/input_files/ga14_coinsOnShelf/test5.naive.special";

TEST(ga14_CoinsOnShelf, bothShelfsSameSizeRadnom)
{
    // Arrange
    CoinsOnShelf cos(nullptr, 0, false, "", 4);
    cos.pokreniAlgoritam();
    cos.pokreniNaivniAlgoritam();

    // Act
    int sizeNaive = cos.getNaiveShelfSize();
    int size = cos.getShelfSize();

    // Assert
    EXPECT_EQ(size, sizeNaive);
}

TEST(ga14_CoinsOnShelf, bothShelfsSameFinalSpan)
{
    // Arrange
    CoinsOnShelf cos1(nullptr, 0, false, specialCase1File, 4);
    CoinsOnShelf cos2(nullptr, 0, false, specialCase1File, 4);
    cos1.pokreniAlgoritam();
    cos2.pokreniNaivniAlgoritam();

    // Act
    double span1 = cos1.getSpan();
    double span2 = cos2.getSpanNaive();

    // Assert
    EXPECT_DOUBLE_EQ(span1, span2);
}

#endif // TST_GA14_COINSONSHELF_H
