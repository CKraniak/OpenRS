TEMPLATE = app
CONFIG += console
CONFIG -= qt

INCLUDEPATH += "D:/Programming/boost/boost_1_59_0"

SOURCES += main.cpp \
    world.cpp \
    terrain.cpp \
    playercharacter.cpp \
    localgrid.cpp \
    item.cpp \
    gamescreen.cpp \
    gameoptionset.cpp \
    gamemanager.cpp \
    districtmapinfo.cpp \
    district.cpp \
    dispatcher.cpp \
    command_aliases.cpp \
    camera.cpp \
    actor.cpp \
    behavior_alisases.cpp

OTHER_FILES += \
    plan.txt \
    ../README.txt \
    makefile

HEADERS += \
    world.h \
    terrain.h \
    playercharacter.h \
    localgrid.h \
    item.h \
    gamescreen.h \
    gameoptionset.h \
    gamemanager.h \
    districtmapinfo.h \
    district.h \
    dispatcher.h \
    defines.h \
    command_aliases.h \
    camera.h \
    actor.h \
    behavior_alisases.h

