QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authorizeduser.cpp \
    basetreenode.cpp \
    configwindow.cpp \
    detailwidget.cpp \
    groupdialog.cpp \
    ldapcatalogue.cpp \
    ldapserver.cpp \
    loginwidget.cpp \
    main.cpp \
    main_window.cpp \
    modifydialog.cpp \
    modifypwd.cpp \
    muldialogou.cpp \
    multipledialog.cpp \
    organizationtree.cpp \
    serverconnect.cpp \
    widget.cpp

HEADERS += \
    authorizeduser.h \
    basetreenode.h \
    configwindow.h \
    detailwidget.h \
    groupdialog.h \
    ldapcatalogue.h \
    ldapserver.h \
    loginwidget.h \
    main_window.h \
    modifydialog.h \
    modifypwd.h \
    muldialogou.h \
    multipledialog.h \
    organizationtree.h \
    serverconnect.h \
    widget.h

FORMS += \
    detailwidget.ui \
    groupdialog.ui \
    loginwidget.ui \
    main_window.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/'../../../Windows Kits/10/Lib/10.0.18362.0/um/x64/' -lWldap32

INCLUDEPATH += $$PWD/'../../../Windows Kits/10/Lib/10.0.18362.0/um/x64'
DEPENDPATH += $$PWD/'../../../Windows Kits/10/Lib/10.0.18362.0/um/x64'

DISTFILES +=
