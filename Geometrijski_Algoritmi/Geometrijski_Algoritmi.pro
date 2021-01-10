QT += core gui opengl charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lglut -lGLU

CONFIG += c++14

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
    animacijanit.cpp \
    main.cpp \
    mainwindow.cpp \
    oblastcrtanja.cpp \
    oblastcrtanjaopengl.cpp \
    pomocnefunkcije.cpp \
    timemeasurementthread.cpp \
    algoritmi_studentski_projekti/ga06_presekPravougaonika.cpp \
    algoritmi_studentski_projekti/ga17_convexhulllineintersections.cpp

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
    animacijanit.h \
    config.h \
    mainwindow.h \
    oblastcrtanja.h \
    oblastcrtanjaopengl.h \
    pomocnefunkcije.h \
    timemeasurementthread.h \
    algoritmi_studentski_projekti/ga06_presekPravougaonika.h \
    algoritmi_studentski_projekti/ga17_convexhulllineintersections.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
