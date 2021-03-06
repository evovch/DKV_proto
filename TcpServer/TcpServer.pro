#-------------------------------------------------
#
# Project created by QtCreator 2017-04-12T16:54:40
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpServer
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

QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic

INCLUDEPATH += ../TcpClient/

SOURCES += main.cpp\
           cls_tcpserver.cpp \
           cls_broadcastreceiver.cpp \
           support.cpp \
           cls_bonecamera.cpp \
           cls_boneservermainwindow.cpp \
    cls_lvloop.cpp

HEADERS += cls_tcpserver.h \
           cls_broadcastreceiver.h \
           ../TcpClient/commonnetworkconst.h \
           support.h \
           cls_bonecamera.h \
           cls_boneservermainwindow.h \
    cls_lvloop.h

FORMS += cls_tcpserver.ui \
         cls_boneservermainwindow.ui
