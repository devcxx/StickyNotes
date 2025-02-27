#-------------------------------------------------
#
# Project created by QtCreator 2017-07-25T11:48:20
#
#-------------------------------------------------

QT       += core gui sql concurrent network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StickyNotes
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

Release:DESTDIR = $$PWD/bin
Debug:DESTDIR = $$PWD/bin_debug

RC_FILE = StickyNotes.rc

TRANSLATIONS += \
    $$PWD/translations/zh_CN.ts \
    $$PWD/translations/qt_zh_CN.ts \

SOURCES += \
    editbar.cpp \
    main.cpp \
    mainwindow.cpp \
    stickywindow.cpp \
    styleoptionbar.cpp \
    titlebar.cpp \
    textedit.cpp \
    vscrollbar.cpp \
    imagelib.cpp \
    notedata.cpp \
    dbmanager.cpp \
    noteview.cpp \
    notewidgetdelegate.cpp \
    notemodel.cpp \
    basebutton.cpp \
    singleinstance.cpp \

HEADERS += \
    editbar.h \
    mainwindow.h \
    stickywindow.h \
    styleoptionbar.h \
    titlebar.h \
    textedit.h \
    vscrollbar.h \
    imagelib.h \
    filedialog.h \
    notedata.h \
    dbmanager.h \
    noteview.h \
    notewidgetdelegate.h \
    notemodel.h \
    basebutton.h \
    singleinstance.h \

RESOURCES += \
    assets.qrc

win32 {
    QMAKE_POST_LINK += $$quote($$[QT_INSTALL_BINS]/windeployqt --no-translations --no-system-d3d-compiler --no-compiler-runtime --no-opengl-sw  \"$${DESTDIR}/$${TARGET}.exe\"$$escape_expand(\\n\\t))
}
