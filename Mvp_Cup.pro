# Qt模块：core提供基础类型/容器/JSON，gui和widgets提供桌面窗口界面。
QT += core gui widgets

# MVP统一使用C++17，方便后续引入更现代的SDK适配代码。
CONFIG += c++17
# 关闭macOS app bundle形式，便于当前MVP在开发目录中直接运行和查找文件。
CONFIG -= app_bundle

# qmake工程类型和最终生成的程序名。
TEMPLATE = app
TARGET = Mvp_Cup

# 把仓库根目录加入头文件搜索路径，因此源码中可以使用 "core/xxx.h" 这类相对路径。
INCLUDEPATH += $$PWD
DEFINES += PROJECT_SOURCE_DIR=\\\"$$PWD\\\"

# 源文件列表：按UI、核心业务、设备适配层组织。
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    core/InventoryManager.cpp \
    core/SlotSelector.cpp \
    core/OrderManager.cpp \
    core/PickWorkflow.cpp \
    core/MotionCoordinator.cpp \
    core/SlotConfigManager.cpp \
    device/Ros2VisionClient.cpp

# 头文件列表：Qt Creator会据此在项目树中展示对应接口和数据结构。
HEADERS += \
    mainwindow.h \
    model/CommonTypes.h \
    model/SlotConfig.h \
    model/InventoryTypes.h \
    device/DeviceInterfaces.h \
    device/MockDevices.h \
    device/Ros2VisionClient.h \
    core/InventoryManager.h \
    core/SlotSelector.h \
    core/OrderManager.h \
    core/PickWorkflow.h \
    core/MotionCoordinator.h \
    core/SlotConfigManager.h

# 随源码分发的外部配置文件，便于开发者直接查看和修改。
DISTFILES += \
    config/shelf_config.json \
    config/inventory.json

# Qt资源文件：把配置文件嵌入程序，作为外部config目录缺失时的兜底配置来源。
RESOURCES += \
    resources.qrc
