#-------------------------------------------------
#
# Project created by QtCreator 2017-04-12T20:12:11
#
#-------------------------------------------------

QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TcpClient
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

INCLUDEPATH += ../TcpServer/

SOURCES += main.cpp\
           cls_tcpclient.cpp \
           cls_multiclient.cpp \
           cls_formconnections.cpp \
           cls_serverfinder.cpp \
           ../TcpServer/support.cpp \
           cls_connstatechecker.cpp \
           cls_tcpserverscanner.cpp \
           cls_mdiwindow.cpp

HEADERS += cls_tcpclient.h \
           cls_multiclient.h \
           cls_formconnections.h \
           cls_serverfinder.h \
           commonnetworkconst.h \
           ../TcpServer/support.h \
           cls_connstatechecker.h \
           cls_tcpserverscanner.h \
           cls_mdiwindow.h

FORMS += cls_tcpclient.ui \
         cls_multiclient.ui \
         cls_formconnections.ui \
         cls_serverfinder.ui \
         cls_connstatechecker.ui \
         cls_mdiwindow.ui
