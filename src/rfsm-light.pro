!include(../config) { error("Cannot open config file. Run configure script in top directory") }

QT       += core widgets gui

QMAKE_PROJECT_NAME = ssde
QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.6

TARGET = rfsm-light
TEMPLATE = app

equals(USE_QGV,"yes") {
message("Building with QGV support")
message($$QGVDIR)
INCLUDEPATH += $$QGVLIBDIR
LIBS += -L$$QGVLIBDIR -lQGVCore
DEPENDPATH += $$QGVLIBDIR
QMAKE_CXXFLAGS += -DUSE_QGV
} else {
message("Building without QGV support")
}


!include(./GraphViz.pri) { error("Cannot open GraphViz.pri file") }

HEADERS += include/nlohmann_json.h \
           transition.h  \
           state.h  \
           iov.h  \
           model.h  \
           commandExec.h \
           compiler.h \
           fragmentChecker.h \
           dynamicPanel.h \
           stateValuations.h \
           stateProperties.h \
           transitionGuards.h \
           transitionActions.h \
           transitionProperties.h \
           modelIovs.h \
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
           compiler.cpp \
           fragmentChecker.cpp \
           dynamicPanel.cpp \
           stateValuations.cpp \
           stateProperties.cpp \
           transitionGuards.cpp \
           transitionActions.cpp \
           transitionProperties.cpp \
           modelIovs.cpp \
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
