#include <gtest/gtest.h>

#include "tst_ga00_PrimerTest.h"
#include "tst_ga00_KonveksniOmotac.h"
#include "tst_ga00_KonveksniOmotac3D.h"
#include "tst_ga00_PreseciDuzi.h"
#include "tst_ga00_DCEL.h"
#include "tst_ga00_Triangulation.h"

#include "tst_ga06_PresekPravougaonika.h"
#include "tst_ga17_convexhulllineintersections.h"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
