QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++17

# To fix error while building QCustomPlot
QMAKE_CXXFLAGS += -Wa,-mbig-obj

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Add pybind
INCLUDEPATH += \
    C:\Users\miguel\AppData\Local\Programs\Python\Python312\Lib\site-packages\pybind11\include \
    C:\\Users\\Miguel\\AppData\\Local\\Programs\\Python\\Python312\\include

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    python_binder.cpp \
    qcustomplot.cpp \
    utilities.cpp \
    wkv.cpp

HEADERS += \
    mainwindow.h \
    python_binder.h \
    qcustomplot.h \
    utilities.h \
    wkv.h
    pybind11\pybind11.h
    pybind11\stl.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
