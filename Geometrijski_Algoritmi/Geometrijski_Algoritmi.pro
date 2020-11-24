QT += core gui opengl charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lglut -lGLU

CONFIG += c++14

INCLUDEPATH += $$PWD/algoritmi_sa_vezbi

SOURCES += \
    algoritambaza.cpp \
    algoritmi_sa_vezbi/ga00_demoiscrtavanja.cpp \
    algoritmi_sa_vezbi/ga01_brisucaprava.cpp \
    algoritmi_sa_vezbi/ga02_3discrtavanje.cpp \
    algoritmi_sa_vezbi/ga04_konveksniomotac3d.cpp \
    animacijanit.cpp \
    main.cpp \
    mainwindow.cpp \
    oblastcrtanja.cpp \
    oblastcrtanjaopengl.cpp \
    pomocnefunkcije.cpp \
    timemeasurementthread.cpp \
    algoritmi_sa_vezbi/ga03_konveksniomotac.cpp \
    algoritmi_sa_vezbi/ga05_preseciduzi.cpp

HEADERS += \
    algoritambaza.h \
    algoritmi_sa_vezbi/ga00_demoiscrtavanja.h \
    algoritmi_sa_vezbi/ga01_brisucaprava.h \
    algoritmi_sa_vezbi/ga02_3discrtavanje.h \
    algoritmi_sa_vezbi/ga04_konveksni3dDatastructures.h \
    algoritmi_sa_vezbi/ga04_konveksniomotac3d.h \
    animacijanit.h \
    config.h \
    mainwindow.h \
    oblastcrtanja.h \
    oblastcrtanjaopengl.h \
    pomocnefunkcije.h \
    timemeasurementthread.h \
    algoritmi_sa_vezbi/ga03_konveksniomotac.h \
    algoritmi_sa_vezbi/ga05_preseciduzi.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
