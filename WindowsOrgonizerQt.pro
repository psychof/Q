QT += core gui widgets sql


CONFIG += c++17

RC_ICONS = \
   pictures/Orgonizer.ico \
   pictures/CloseEye.ico   \
   pictures/OpenEye.ico

SOURCES += \
    CustomLineEdit.cpp \
    database.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindowmenu.cpp \
    registrationwindow.cpp \



HEADERS += \
    CustomLineEdit.h \
    MainWindow.h \
    database.h \
    loginwindow.h \
    mainwindowmenu.h \
    registrationwindow.h \
    sessionmanager.h \



FORMS += \
    mainwindow.ui





qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
