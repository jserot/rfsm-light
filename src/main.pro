TEMPLATE = app
TARGET = rfsm-light

QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.6

QT       += core gui
QT 		 += widgets
greaterThan(QT_MAJOR_VERSION,5) {
QT 		 += core5compat
}

include(../platform)

equals(USE_QGV,"yes") {
message("Building with QGV support")
message($$QGVDIR)
INCLUDEPATH += $$QGVDIR/QGVCore
LIBS += -L$$QGVDIR/lib -lQGVCore
DEPENDPATH += $$QGVDIR//QGVCore
include(./GraphViz.pri)
QMAKE_CXXFLAGS += -DUSE_QGV
} else {
message("Building without QGV support")
}

CONFIG += c++11 console debug

# Input
HEADERS += include/nlohmann_json.h \
           transition.h  \
           state.h  \
           iov.h  \
           model.h  \
           commandExec.h \
           syntaxChecker.h \
           dynamicPanel.h \
           stateValuations.h \
           stateProperties.h \
           transitionGuards.h \
           transitionActions.h \
           transitionProperties.h \
           modelVars.h \
           modelProperties.h \
           stimulus.h \
           stimuli.h \
           command.h \
           imageviewer.h \
           textviewer.h \
           syntaxHighlighters.h \
           compilerPaths.h \
           compilerOption.h \
           compilerOptions.h \
           debug.h \
           mainwindow.h
SOURCES += transition.cpp \
           state.cpp \
           iov.cpp  \
           model.cpp \
           commandExec.cpp \
           syntaxChecker.cpp \
           dynamicPanel.cpp \
           stateValuations.cpp \
           stateProperties.cpp \
           transitionGuards.cpp \
           transitionActions.cpp \
           transitionProperties.cpp \
           modelVars.cpp \
           modelProperties.cpp \
           stimulus.cpp \
           stimuli.cpp \
           command.cpp \
           syntaxHighlighters.cpp \
           compilerPaths.cpp \
           compilerOption.cpp \
           compilerOptions.cpp \
           textviewer.cpp \
           imageviewer.cpp \
           debug.cpp \
           main.cpp \
           mainwindow.cpp
equals(USE_QGV,"yes") {
HEADERS += dotviewer.h
SOURCES += dotviewer.cpp
}

RESOURCES += resources.qrc
