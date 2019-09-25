TEMPLATE = app
TARGET = rfsm-light
INCLUDEPATH += .

QT       += core gui
QT 		 += widgets

CONFIG += c++11 console

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
           imageViewer.h \
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
           imageViewer.cpp \
           debug.cpp \
           main.cpp \
           mainwindow.cpp

RESOURCES += resources.qrc
