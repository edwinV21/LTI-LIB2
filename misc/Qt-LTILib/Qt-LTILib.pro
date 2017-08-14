# -------------------------------------------------
# Project created by QtCreator 2012-10-24T18:10:59
# -------------------------------------------------

# if you only have a local version of the ltilib, then ensure this line
# provides the proper path to your ltilib-2 directory
LTICFG = $$system(/bin/echo "$HOME/ltilib-2/linux/lti-local-config")

# if you want to use the global installation of the ltilib-2 uncomment the
# following line.  Please ensure the lti-config script is available
# LTICFG =$$system(/bin/echo "lti-config")

QT += opengl \
    xml \
    core \
    gui

TARGET = Qt-LTILib
TEMPLATE = app
CONFIG += warn_on \
    thread \
    qt

SOURCES += main.cpp \
    mainwindow.cpp \
    ltiQt.cpp

HEADERS += mainwindow.h \
    ltiQt.h

FORMS += mainwindow.ui

LTILIBDFLAGS = $$system($$LTICFG --libs debug nogtk)
LTILIBFLAGS = $$system($$LTICFG --libs nogtk)

CONFIG(debug, debug|release) {
  LIBS += $$LTILIBDFLAGS
}

CONFIG(release, debug|release) {
  LIBS += $$LTILIBFLAGS
}

QMAKE_CXXFLAGS_RELEASE += $$system($${LTICFG} --cxxflags nogtk)
QMAKE_CXXFLAGS_DEBUG += $$system($${LTICFG} --cxxflags debug nogtk)

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
