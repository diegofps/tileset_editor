QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app.cpp \
    appcommands.cpp \
    appstate.cpp \
    appstyles.cpp \
    dialogeditscenes.cpp \
    dialogoptions.cpp \
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
    layoutabsolute.cpp \
    main.cpp \
    mainwindow.cpp \
    model/cell.cpp \
    model/palette.cpp \
    model/project.cpp \
    model/reference.cpp \
    model/scene.cpp \
    model/screenshot.cpp \
    model/tile.cpp \
    model/tileset.cpp \
    servicecontext.cpp \
    tilecache.cpp \
    tilecachehd.cpp \
    tilesgridlist.cpp \
    widgeteditor.cpp \
    widgetitempalette.cpp \
    widgetpalette.cpp \
    widgetpicture.cpp \
    widgetreference.cpp \
    widgettiles.cpp \
    widgetvscrollarea.cpp

HEADERS += \
    app.h \
    appcommands.h \
    appstate.h \
    appstyles.h \
    commands/command.h \
    dialogeditscenes.h \
    dialogoptions.h \
    errors.h \
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
    layoutabsolute.h \
    mainwindow.h \
    model/cell.h \
    model/jsonhelpers.h \
    model/palette.h \
    model/project.h \
    model/reference.h \
    model/scene.h \
    model/screenshot.h \
    model/tile.h \
    model/tileset.h \
    servicecontext.h \
    tilecache.h \
    tilecachehd.h \
    tilesgridlist.h \
    widgeteditor.h \
    widgetitempalette.h \
    widgetpalette.h \
    widgetpicture.h \
    widgetreference.h \
    widgettiles.h \
    widgetvscrollarea.h

FORMS += \
    dialogeditscenes.ui \
    dialogoptions.ui \
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
    mainwindow.ui \
    widgetitempalette.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
