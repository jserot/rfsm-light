TEMPLATE = app
TARGET = rfsm-light
INCLUDEPATH += .

QT       += core gui
QT 		 += widgets
greaterThan(QT_MAJOR_VERSION,5) {
QT 		 += core5compat
}

CONFIG += c++11 console debug

# Input
HEADERS += include/nlohmann_json.h \
           transition.h  \
           state.h  \
           iov.h  \
           model.h  \
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
           iov.cpp  \
           model.cpp \
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
