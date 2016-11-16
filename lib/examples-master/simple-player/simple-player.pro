#
# VLC-Qt Simple Player
# Copyright (C) 2015 Tadej Novak <tadej@tano.si>
#

TEMPLATE = subdirs

SUBDIRS += src


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VLC-Qt_1.1.0_win32_mingw/lib/ -llibVLCQtCore.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VLC-Qt_1.1.0_win32_mingw/lib/ -llibVLCQtCore.dlld
else:unix: LIBS += -L$$PWD/../../VLC-Qt_1.1.0_win32_mingw/lib/ -llibVLCQtCore.dll

INCLUDEPATH += $$PWD/../../VLC-Qt_1.1.0_win32_mingw/include
DEPENDPATH += $$PWD/../../VLC-Qt_1.1.0_win32_mingw/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../VLC-Qt_1.1.0_win32_mingw/lib/ -llibVLCQtWidgets.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../VLC-Qt_1.1.0_win32_mingw/lib/ -llibVLCQtWidgets.dlld
else:unix: LIBS += -L$$PWD/../../VLC-Qt_1.1.0_win32_mingw/lib/ -llibVLCQtWidgets.dll

INCLUDEPATH += $$PWD/../../VLC-Qt_1.1.0_win32_mingw/include
DEPENDPATH += $$PWD/../../VLC-Qt_1.1.0_win32_mingw/include
