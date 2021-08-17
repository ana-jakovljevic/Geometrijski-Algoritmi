QT += core gui opengl charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lglut -lGLU

CONFIG += c++17

INCLUDEPATH += \
    $$PWD/algoritmi_sa_vezbi \
    $$PWD/algoritmi_studentski_projekti

SOURCES += \
    algoritambaza.cpp \
    algoritmi_sa_vezbi/ga00_demoiscrtavanja.cpp \
    algoritmi_sa_vezbi/ga01_brisucaprava.cpp \
    algoritmi_sa_vezbi/ga02_3discrtavanje.cpp \
    algoritmi_sa_vezbi/ga03_konveksniomotac.cpp \
    algoritmi_sa_vezbi/ga04_konveksniomotac3d.cpp \
    algoritmi_sa_vezbi/ga05_preseciduzi.cpp \
    algoritmi_sa_vezbi/ga06_dcel.cpp \
    algoritmi_sa_vezbi/ga06_dceldemo.cpp \
    algoritmi_sa_vezbi/ga07_triangulation.cpp \
    algoritmi_studentski_projekti/ga03_pointlocation.cpp \
    algoritmi_studentski_projekti/ga10_unitDiskCover.cpp \
    animacijanit.cpp \
    main.cpp \
    mainwindow.cpp \
    oblastcrtanja.cpp \
    oblastcrtanjaopengl.cpp \
    pomocnefunkcije.cpp \
    timemeasurementthread.cpp \
    algoritmi_studentski_projekti/ga06_presekPravougaonika.cpp \
    algoritmi_studentski_projekti/ga07_konturaPravougaonika.cpp \
    algoritmi_studentski_projekti/ga09_klasterovanje.cpp \
    algoritmi_studentski_projekti/ga14_coinsOnShelf.cpp \
    algoritmi_studentski_projekti/ga15_collisiondetection.cpp \
    algoritmi_studentski_projekti/ga17_convexhulllineintersections.cpp \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/convex_hull.cpp \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/dcel.cpp \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/delaunay.cpp \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/graph.cpp \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/largest_empty_circle.cpp \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/lec.cpp \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/utility.cpp \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/voronoi.cpp

HEADERS += \
    algoritambaza.h \
    algoritmi_sa_vezbi/ga00_demoiscrtavanja.h \
    algoritmi_sa_vezbi/ga01_brisucaprava.h \
    algoritmi_sa_vezbi/ga02_3discrtavanje.h \
    algoritmi_sa_vezbi/ga03_konveksniomotac.h \
    algoritmi_sa_vezbi/ga04_konveksni3dDatastructures.h \
    algoritmi_sa_vezbi/ga04_konveksniomotac3d.h \
    algoritmi_sa_vezbi/ga05_datastructures.h \
    algoritmi_sa_vezbi/ga05_preseciduzi.h \
    algoritmi_sa_vezbi/ga06_dcel.h \
    algoritmi_sa_vezbi/ga06_dceldemo.h \
    algoritmi_sa_vezbi/ga07_datastructures.h \
    algoritmi_sa_vezbi/ga07_triangulation.h \
    algoritmi_studentski_projekti/ga03_pointlocation.h \
    algoritmi_studentski_projekti/ga10_unitDiskCover.h \
    animacijanit.h \
    config.h \
    mainwindow.h \
    oblastcrtanja.h \
    oblastcrtanjaopengl.h \
    pomocnefunkcije.h \
    timemeasurementthread.h \
    algoritmi_studentski_projekti/ga06_presekPravougaonika.h \
    algoritmi_studentski_projekti/ga07_konturaPravougaonika.h \
    algoritmi_studentski_projekti/ga09_klasterovanje.h \
    algoritmi_studentski_projekti/ga14_coinsOnShelf.h \
    algoritmi_studentski_projekti/ga15_collisiondetection.h \
    algoritmi_studentski_projekti/ga15_datastructures.h \
    algoritmi_studentski_projekti/ga17_convexhulllineintersections.h \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/convex_hull.h \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/dcel.h \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/delaunay.h \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/graph.h \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/largest_empty_circle.h \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/lec.h \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/utility.h \
    algoritmi_studentski_projekti/ga20_largest_empty_circle/voronoi.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
