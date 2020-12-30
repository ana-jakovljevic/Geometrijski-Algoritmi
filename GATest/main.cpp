#include <gtest/gtest.h>

#include "tst_ga00_PrimerTest.h"
#include "tst_ga00_KonveksniOmotac.h"
#include "tst_ga00_KonveksniOmotac3D.h"

#include "tst_ga06_presekPravougaonika.h"
#include "tst_ga14_CoinsOnShelf.h"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
