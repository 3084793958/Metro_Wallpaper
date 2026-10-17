#插件只能以Debug模式编译,不能以Release模式编译
#若需优化,使用strip命令
QT += widgets core
TEMPLATE = lib
CONFIG += plugin c++11
TARGET = Metro_Wallpaper

DISTFILES += \
    Metro_Wallpaper.json

HEADERS += \
    metro_wallpaper.h \
    interfaces/easy_desktop_class.h \
    interfaces/wallpaper/wallpaper_info_def.h \
    interfaces/wallpaper/wallpaper_plugin.h

SOURCES += \
    metro_wallpaper.cpp \
    interfaces/easy_desktop_class.cpp \
    interfaces/wallpaper/wallpaper_info_def.cpp \
    interfaces/wallpaper/wallpaper_plugin.cpp

RESOURCES += \
    metro_pic.qrc
