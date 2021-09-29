include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++17
CONFIG += thread
CONFIG += qt
CONFIG += thread

QT += core gui charts widgets opengl

# Ova opcija omogucava da se qDebug ne ispisuje u Release modu.
# Nikada ne zelimo da imamo debug poruke u kodu na kojem se mere performanse,
# narocito imajuci u vidu da je kompajler optimizovao dosta ponasanja koda,
# sto nas efektivno onemogucuje da debagujemo program.
# Greske se traze i ispravljaju u debug modu, ne u release modu.
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += \
    tst_ga00_PrimerTest.h \
    tst_ga00_KonveksniOmotac.h \
    tst_ga00_KonveksniOmotac3D.h \
    tst_ga00_PreseciDuzi.h \
    tst_ga00_DCEL.h \
    tst_ga00_Triangulation.h \
    tst_ga03_PointLocation.h \
    tst_ga06_PresekPravougaonika.h \
    tst_ga07_KonturaPragougaonika.h \
    tst_ga09_klasterovanje.h \
    tst_ga10_unitDiskCover.h \
    tst_ga14_CoinsOnShelf.h \
    tst_ga15_CollisionDetection.h \
    tst_ga17_convexhulllineintersections.h \
    tst_ga05_triangulationdq.h \
    tst_ga20_largest_empty_circle.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga03_pointlocation.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga05_pomocneFunkcije.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga05_EdgeDQ.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga05_triangulationdq.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga06_presekPravougaonika.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga07_konturaPravougaonika.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga09_klasterovanje.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga10_unitdiskcover.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga10_unitDiskCover.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga12_closestpair.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga14_coinsOnShelf.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga15_collisiondetection.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga17_convexhulllineintersections.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/convex_hull.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/dcel.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/delaunay.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/graph.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/largest_empty_circle.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/lec.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/utility.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/voronoi.h \
    ../Geometrijski_Algoritmi/pomocnefunkcije.h \
    ../Geometrijski_Algoritmi/algoritambaza.h \
    ../Geometrijski_Algoritmi/animacijanit.h \
    ../Geometrijski_Algoritmi/config.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga00_demoiscrtavanja.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga01_brisucaprava.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga02_3discrtavanje.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga03_konveksniomotac.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga04_konveksni3dDatastructures.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga04_konveksniomotac3d.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga05_datastructures.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga05_preseciduzi.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga06_dcel.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga06_dceldemo.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga07_datastructures.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga07_triangulation.h


INCLUDEPATH += \
    ../Geometrijski_Algoritmi \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/

win32 {
INCLUDEPATH += \
    $$(BOOST_FOLDER_LOCATION)
}

SOURCES += main.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga03_pointlocation.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga06_presekPravougaonika.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga07_konturaPravougaonika.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga09_klasterovanje.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga10_unitDiskCover.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga12_closestpair.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga14_coinsOnShelf.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga15_collisiondetection.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga17_convexhulllineintersections.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/convex_hull.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/dcel.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/delaunay.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/graph.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/largest_empty_circle.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/lec.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/utility.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga20_largest_empty_circle/voronoi.cpp \
    ../Geometrijski_Algoritmi/pomocnefunkcije.cpp \
    ../Geometrijski_Algoritmi/algoritambaza.cpp \
    ../Geometrijski_Algoritmi/animacijanit.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga00_demoiscrtavanja.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga01_brisucaprava.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga02_3discrtavanje.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga03_konveksniomotac.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga04_konveksniomotac3d.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga05_preseciduzi.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga06_dcel.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga06_dceldemo.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga07_triangulation.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga05_triangulationdq.cpp

