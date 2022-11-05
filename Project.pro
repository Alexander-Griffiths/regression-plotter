QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addnewdata.cpp \
    changethickness.cpp \
    graphoptions.cpp \
    helpdialog.cpp \
    main.cpp \
    graphwindow.cpp \
    qgraphicsaxisitem.cpp \
    tablemodel.cpp

HEADERS += \
    addnewdata.h \
    changethickness.h \
    graphoptions.h \
    graphwindow.h \
    helpdialog.h \
    qgraphicsaxisitem.h \
    tablemodel.h

FORMS += \
    addnewdata.ui \
    changethickness.ui \
    graphoptions.ui \
    graphwindow.ui \
    helpdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../windows-gsl-32 bit/src/gsl/1.8/gsl-1.8/VC8/libgsl/Debug-DLL/' -llibgsl_dll_
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../windows-gsl-32 bit/src/gsl/1.8/gsl-1.8/VC8/libgsl/Debug-DLL/' -llibgsl_dll_d

INCLUDEPATH += $$PWD/'../windows-gsl-32 bit/src/gsl/1.8/gsl-1.8/'
DEPENDPATH += $$PWD/'../windows-gsl-32 bit/src/gsl/1.8/gsl-1.8/VC8/libgsl/Debug-DLL'

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../windows-gsl-32 bit/src/gsl/1.8/gsl-1.8/VC8/libgslcblas/Debug-DLL/' -llibgslcblas_dll_
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../windows-gsl-32 bit/src/gsl/1.8/gsl-1.8/VC8/libgslcblas/Debug-DLL/' -llibgslcblas_dll_d

INCLUDEPATH += $$PWD/'../windows-gsl-32 bit/src/gsl/1.8/gsl-1.8/'
DEPENDPATH += $$PWD/'../windows-gsl-32 bit/src/gsl/1.8/gsl-1.8/VC8/libgslcblas/Debug-DLL'

RESOURCES += \
    helpimgs.qrc \
    icons.qrc
