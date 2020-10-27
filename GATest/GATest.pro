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

HEADERS +=     \
    ../Geometrijski_Algoritmi/algoritambaza.h \
    ../Geometrijski_Algoritmi/animacijanit.h \
    ../Geometrijski_Algoritmi/config.h \
    ../Geometrijski_Algoritmi/oblastcrtanja.h \
    ../Geometrijski_Algoritmi/timemeasurementthread.h \


INCLUDEPATH += ../Geometrijski_Algoritmi
win32 {
INCLUDEPATH += \
    $$(BOOST_FOLDER_LOCATION)
}
SOURCES +=     main.cpp \
    ../Geometrijski_Algoritmi/algoritambaza.cpp \
    ../Geometrijski_Algoritmi/animacijanit.cpp \
    ../Geometrijski_Algoritmi/oblastcrtanja.cpp \
    ../Geometrijski_Algoritmi/timemeasurementthread.cpp \
