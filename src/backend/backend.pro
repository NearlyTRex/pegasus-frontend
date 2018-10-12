TEMPLATE = lib

QT += qml gamepad sql
CONFIG += c++11 staticlib warn_on exceptions_off object_parallel_to_source


SOURCES += \
    Api.cpp \
    AppContext.cpp \
    Backend.cpp \
    ConfigFile.cpp \
    FrontendLayer.cpp \
    GamepadAxisNavigation.cpp \
    PegasusAssets.cpp \
    ProcessLauncher.cpp \
    ScriptRunner.cpp \
    Paths.cpp \
    AppSettings.cpp \
    AssetTypes.cpp

HEADERS += \
    Api.h \
    AppCloseType.h \
    AppContext.h \
    Backend.h \
    ConfigFile.h \
    FrontendLayer.h \
    GamepadAxisNavigation.h \
    PegasusAssets.h \
    ProcessLauncher.h \
    ScriptRunner.h \
    LocaleUtils.h \
    Paths.h \
    AppSettings.h \
    AssetTypes.h

include(configfiles/configfiles.pri)
include(platform/platform.pri)
include(providers/providers.pri)
include(model/model.pri)
include(modeldata/modeldata.pri)
include(utils/utils.pri)


include($${TOP_SRCDIR}/src/deployment_vars.pri)

DEFINES *= \
    $${COMMON_DEFINES} \
    INSTALL_DATADIR=\\\"$${INSTALL_DATADIR}\\\"
