QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app.cpp \
    appstate.cpp \
    appstyles.cpp \
    fragmentcontextclosed.cpp \
    fragmentcontextopen.cpp \
    fragmenteditor.cpp \
    fragmentpalettes.cpp \
    fragmentpreview.cpp \
    fragmentreferences.cpp \
    fragmenttilepreview.cpp \
    fragmenttiles.cpp \
    fragmenttilesetproperties.cpp \
    fragmenttilesets.cpp \
    main.cpp \
    mainwindow.cpp \
    tilesgridlist.cpp \
    widgetcolorpalette.cpp

HEADERS += \
    app.h \
    appstate.h \
    appstyles.h \
    fragmentcontextclosed.h \
    fragmentcontextopen.h \
    fragmenteditor.h \
    fragmentpalettes.h \
    fragmentpreview.h \
    fragmentreferences.h \
    fragmenttilepreview.h \
    fragmenttiles.h \
    fragmenttilesetproperties.h \
    fragmenttilesets.h \
    mainwindow.h \
    tilesgridlist.h \
    widgetcolorpalette.h

FORMS += \
    fragmentcontextclosed.ui \
    fragmentcontextopen.ui \
    fragmenteditor.ui \
    fragmentpalettes.ui \
    fragmentpreview.ui \
    fragmentreferences.ui \
    fragmenttilepreview.ui \
    fragmenttiles.ui \
    fragmenttilesetproperties.ui \
    fragmenttilesets.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
