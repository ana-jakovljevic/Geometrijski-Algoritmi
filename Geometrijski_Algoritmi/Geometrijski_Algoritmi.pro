QT       += core gui opengl charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lglut -lGLU

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    algoritambaza.cpp \
    algoritmi_sa_vezbi/ga00_demoiscrtavanja.cpp \
    algoritmi_sa_vezbi/ga01_brisucaprava.cpp \
    animacijanit.cpp \
    main.cpp \
    mainwindow.cpp \
    oblastcrtanja.cpp \
    oblastcrtanjaopengl.cpp \
    timemeasurementthread.cpp

HEADERS += \
    algoritambaza.h \
    algoritmi_sa_vezbi/ga00_demoiscrtavanja.h \
    algoritmi_sa_vezbi/ga01_brisucaprava.h \
    animacijanit.h \
    config.h \
    mainwindow.h \
    oblastcrtanja.h \
    oblastcrtanjaopengl.h \
    timemeasurementthread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
