#ifndef TST_GA03_POINTLOCATION_H
#define TST_GA03_POINTLOCATION_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga03_pointlocation.h"

using namespace testing;

TEST(ga03_pointlocationTests, input1){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input1.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}
TEST(ga03_pointlocationTests, input2){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input2.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

TEST(ga03_pointlocationTests, input3){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input3.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

TEST(ga03_pointlocationTests, input4){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input4.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

TEST(ga03_pointlocationTests, input5){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input5.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

TEST(ga03_pointlocationTests, input6){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input6.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

TEST(ga03_pointlocationTests, input7){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input7.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

TEST(ga03_pointlocationTests, input8){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input8.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

TEST(ga03_pointlocationTests, input9){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input9.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

TEST(ga03_pointlocationTests, input10){
    PointLocation pl(nullptr, 0,false, "../Geometrijski_Algoritmi/input_files/ga03_PointLocation/input10.txt");
    pl.pokreniAlgoritam();
    Trapez* ocekivani=pl.gettTrapez();
    pl.pokreniNaivniAlgoritam();
    Trapez* dobijeni=pl.gettTrapez();
    //std::cout<<ocekivani->toString();
    ASSERT_EQ(ocekivani,dobijeni);
}

#endif // TST_GA03_POINTLOCATION_H
