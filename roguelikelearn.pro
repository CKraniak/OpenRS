TEMPLATE = app
CONFIG += console
#CONFIG -= qt

SOURCES += main.cpp \
    actor.cpp \
    camera.cpp \
    gamescreen.cpp \
    gamemanager.cpp \
    item.cpp \
    terrain.cpp \
    world.cpp \
    district.cpp \
    gameoptionset.cpp \
    districtmapinfo.cpp \
    localgrid.cpp \
    playercharacter.cpp \
    dispatcher.cpp \
    command_aliases.cpp

HEADERS += \
    actor.h \
    camera.h \
    gamescreen.h \
    gamemanager.h \
    item.h \
    terrain.h \
    world.h \
    district.h \
    gameoptionset.h \
    districtmapinfo.h \
    localgrid.h \
    playercharacter.h \
    dispatcher.h \
    command_aliases.h \
    defines.h

OTHER_FILES += \
    plan.txt

