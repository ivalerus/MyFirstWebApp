QT -= gui
QT += network
QT += xml

CONFIG += c++11 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
      src/cookietestcontroller.cpp \
      src/datatemplatecontroller.cpp \
      src/global.cpp \
      src/helloworldcontroller.cpp \
      src/listdatacontroller.cpp \
      src/logincontroller.cpp \
      src/main.cpp \
      src/requestmapper.cpp \
      src/xmlcontroller.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(../QtWebApp/QtWebApp/httpserver/httpserver.pri)
OTHER_FILES += etc/webapp1.ini

include(../QtWebApp/QtWebApp/templateengine/templateengine.pri)
include(../QtWebApp/QtWebApp/logging/logging.pri)

HEADERS += \
    src/cookietestcontroller.h \
    src/datatemplatecontroller.h \
    src/global.h \
    src/helloworldcontroller.h \
    src/listdatacontroller.h \
    src/logincontroller.h \
    src/requestmapper.h \
    src/xmlcontroller.h
