TEMPLATE = app
TARGET = rfsm-light
INCLUDEPATH += .

QT       += core gui
QT 		 += widgets
QT 		 += core5compat

CONFIG += c++11 console debug

# Input
HEADERS += include/nlohmann_json.h \
           transition.h  \
           state.h  \
           fsmIo.h  \
           fsm.h  \
           properties.h \
           stimulus.h \
           stimuli.h \
           command.h \
           imageviewer.h \
           syntaxHighlighters.h \
           compilerPaths.h \
           compilerOption.h \
           compilerOptions.h \
           debug.h \
           mainwindow.h
SOURCES += transition.cpp \
           state.cpp \
           fsmIo.cpp  \
           fsm.cpp \
           properties.cpp \
           stimulus.cpp \
           stimuli.cpp \
           command.cpp \
           syntaxHighlighters.cpp \
           compilerPaths.cpp \
           compilerOption.cpp \
           compilerOptions.cpp \
           imageviewer.cpp \
           debug.cpp \
           main.cpp \
           mainwindow.cpp

RESOURCES += resources.qrc
