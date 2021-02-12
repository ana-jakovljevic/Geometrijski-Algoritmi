#ifndef TST_GA14_COINSONSHELF_H
#define TST_GA14_COINSONSHELF_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga14_coinsOnShelf.h"

using namespace testing;

static constexpr auto specialCase1File = "../Geometrijski_Algoritmi/input_files/ga14_coinsOnShelf/test5.naive.special";

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
    CoinsOnShelf cos1(nullptr, 0, false, specialCase1File);
    CoinsOnShelf cos2(nullptr, 0, false, specialCase1File);
    cos1.pokreniAlgoritam();
    cos2.pokreniNaivniAlgoritam();

    // Act
    double span1 = cos1.getSpan();
    double span2 = cos2.getSpanNaive();

    // Assert
    EXPECT_DOUBLE_EQ(span1, span2);
}

TEST(ga14_CoinsOnShelf, footprintsConsecutive)
{
    // Arrange
    CoinsOnShelf cos1(nullptr, 0, false, "", 10);
    cos1.pokreniAlgoritam();

    // Act
    list<Disk*> discs = cos1.getShelfDisks();
    list<Disk*>::iterator fst = discs.begin();
    list<Disk*>::iterator snd= fst;
    snd++;
    list<Disk*>::iterator lst = discs.end();

    while(snd != lst) {
        EXPECT_TRUE((*fst)->footprint() < (*snd)->footprint());
        fst++;
        snd++;
    }
}

#endif // TST_GA14_COINSONSHELF_H
