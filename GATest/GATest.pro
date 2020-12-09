include(gtest_dependency.pri)

TEMPLATE = app
CONFIG += console c++14
CONFIG += thread
CONFIG += qt

QT += core gui charts widgets opengl

# Ova opcija omogucava da se qDebug ne ispisuje u Release modu.
# Nikada ne zelimo da imamo debug poruke u kodu na kojem se mere performanse,
# narocito imajuci u vidu da je kompajler optimizovao dosta ponasanja koda,
# sto nas efektivno onemogucuje da debagujemo program.
# Greske se traze i ispravljaju u debug modu, ne u release modu.
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

HEADERS += \
    tst_ga00_KonveksniOmotac.h \
    tst_ga00_KonveksniOmotac3D.h \
    tst_ga00_PrimerTest.h \
    tst_ga06_presekPravougaonika.h \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga06_presekPravougaonika.h \
    ../Geometrijski_Algoritmi/pomocnefunkcije.h \
    ../Geometrijski_Algoritmi/algoritambaza.h \
    ../Geometrijski_Algoritmi/animacijanit.h \
    ../Geometrijski_Algoritmi/config.h \
    ../Geometrijski_Algoritmi/oblastcrtanja.h \
    ../Geometrijski_Algoritmi/timemeasurementthread.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga00_demoiscrtavanja.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga01_brisucaprava.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga02_3discrtavanje.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga03_konveksniomotac.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga04_konveksniomotac3d.h \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga05_preseciduzi.h

INCLUDEPATH += \
    ../Geometrijski_Algoritmi \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/

win32 {
INCLUDEPATH += \
    $$(BOOST_FOLDER_LOCATION)
}

SOURCES += main.cpp \
    ../Geometrijski_Algoritmi/algoritmi_studentski_projekti/ga06_presekPravougaonika.cpp \
    ../Geometrijski_Algoritmi/pomocnefunkcije.cpp \
    ../Geometrijski_Algoritmi/algoritambaza.cpp \
    ../Geometrijski_Algoritmi/animacijanit.cpp \
    ../Geometrijski_Algoritmi/oblastcrtanja.cpp \
    ../Geometrijski_Algoritmi/timemeasurementthread.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga00_demoiscrtavanja.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga01_brisucaprava.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga02_3discrtavanje.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga03_konveksniomotac.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga04_konveksniomotac3d.cpp \
    ../Geometrijski_Algoritmi/algoritmi_sa_vezbi/ga05_preseciduzi.cpp
