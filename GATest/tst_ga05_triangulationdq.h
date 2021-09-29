#ifndef TST_GA05_TRIANGULATIONDQ_H
#define TST_GA05_TRIANGULATIONDQ_H

#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>

#include "ga05_triangulationdq.h"

using namespace testing;
static constexpr auto input3 = "../Geometrijski_Algoritmi/input_files/ga05_TriangulationDQ/input3.txt";
static constexpr auto input2 = "../Geometrijski_Algoritmi/input_files/ga05_TriangulationDQ/input2.txt";
static constexpr auto input1 = "../Geometrijski_Algoritmi/input_files/ga05_TriangulationDQ/input1.txt";

TEST(ga05_triangulationDQ, input2){
    triangulationDQ trougao(nullptr,0, false, input2);
    trougao.pokreniAlgoritam();
    auto edges = trougao.getEdges();
    auto vertices = trougao.getVertices();
    EXPECT_EQ(vertices.size(), 3);
    // 3*n-3-h (h number of points on convex hull, h=3)
    EXPECT_EQ(edges.size(), 3);
    EXPECT_TRUE(trougao.checkSym());

}
TEST(ga05_triangulationDQ, input1){
    triangulationDQ triangulation(nullptr,0, false, input1);
    triangulation.pokreniAlgoritam();
    auto edges = triangulation.getEdges();
    auto vertices = triangulation.getVertices();
    EXPECT_EQ(vertices.size(), 10);
    // 3*n-3-h (h number of points on convex hull, h=6)
    EXPECT_EQ(edges.size(), 21);
    EXPECT_TRUE(triangulation.checkSym());

}
TEST(ga05_triangulationDQ, input3){
    triangulationDQ triangulation(nullptr,0, false, input3);
    triangulation.pokreniAlgoritam();
    auto edges = triangulation.getEdges();
    auto vertices = triangulation.getVertices();
    EXPECT_EQ(vertices.size(), 12);
    // 3*n-3-h (h number of points on convex hull, h=7)
    EXPECT_EQ(edges.size(), 26);
    EXPECT_TRUE(triangulation.checkSym());
}
TEST(ga05_triangulationDQ, ThreePoints){
    // input is 3 points, check if triangulation calls TrianglePrimitive
    triangulationDQ triangle(nullptr, 0, false, input2);
    auto primitive = triangle.TrianglePrimitive(triangle.getVertices());
    auto triangulation = triangle.triangulate(triangle.getVertices());
    EXPECT_EQ(*(std::get<0>(primitive)[0]), *(std::get<0>(triangulation)[0]));
    EXPECT_EQ(*(std::get<1>(primitive)[0]), *(std::get<1>(triangulation)[0]));
}
TEST(ga05_triangulationDQ, TwoPoints){
    // input is 2 points, check if triangulation calls LinePrimitive
    triangulationDQ line(nullptr, 0, false, "" ,2);
    auto primitive = line.LinePrimitive(line.getVertices());
    auto triangulation = line.triangulate(line.getVertices());
    EXPECT_EQ(*(std::get<0>(primitive)[0]), *(std::get<0>(triangulation)[0]));
    EXPECT_EQ(*(std::get<1>(primitive)[0]), *(std::get<1>(triangulation)[0]));
    // line must have to symetric edges
    EXPECT_EQ(*(std::get<0>(primitive)[0]), *(std::get<1>(primitive)[0]->Sym()));
}
TEST(ga05_triangulationDQ, IsSorted100){
    triangulationDQ triangulation(nullptr, 0, false, "" ,100);
    EXPECT_TRUE(triangulation.isSorted());
}

TEST(ga05_triangulationDQ, IsSorted1000){
    triangulationDQ triangulation(nullptr, 0, false, "" ,1000);
    EXPECT_TRUE(triangulation.isSorted());
}
TEST(ga05_triangulationDQ, Random){
    triangulationDQ triangulation(nullptr,0, false, "", 1000);
    triangulation.pokreniAlgoritam();
    EXPECT_TRUE(triangulation.checkSym());
}

#endif // TST_GA05_TRIANGULATIONDQ_H
