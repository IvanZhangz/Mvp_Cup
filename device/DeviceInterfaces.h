#pragma once

#include "model/CommonTypes.h"

// 机械臂接口。
// 后续真实机械臂SDK只要实现这三个动作，就能被MotionCoordinator调用。
class IArmDevice
{
public:
    virtual ~IArmDevice() = default;
    virtual bool moveToPrePickPose(const Pose& pose) = 0; // 移动到抓取前的安全预备位。
    virtual bool moveToPickPose(const Pose& pose) = 0;    // 移动到实际抓取位。
    virtual bool moveToPlacePose(const Pose& pose) = 0;   // 移动到放置位。
};

// 底盘接口。
// moveToObservePosition提供按库位编号移动的粗定位入口，moveToPose提供按标定位姿移动的细定位入口。
class IChassisDevice
{
public:
    virtual ~IChassisDevice() = default;
    virtual bool moveToObservePosition(const QString& slotId) = 0;
    virtual bool moveToPose(const Pose& pose) = 0;
};

// 头部/颈部接口，用于让相机或传感器看向指定库位。
class IHeadDevice
{
public:
    virtual ~IHeadDevice() = default;
    virtual bool lookAtSlot(const QString& slotId) = 0;
};

// 腰部接口，用于上半身或机械臂基座朝向调整。
class IWaistDevice
{
public:
    virtual ~IWaistDevice() = default;
    virtual bool rotateTo(double angle) = 0;
};

// 升降接口，用于把相机、机械臂或躯干调整到指定高度。
class ILiftDevice
{
public:
    virtual ~ILiftDevice() = default;
    virtual bool moveToHeight(double height) = 0;
};

// 夹爪接口，MVP只需要开/合两个动作。
class IGripperDevice
{
public:
    virtual ~IGripperDevice() = default;
    virtual bool open() = 0;
    virtual bool close() = 0;
};

// 视觉客户端接口。
// requestDetect触发识别，getLatestResult读取最近一次识别结果；真实ROS2和模拟视觉都实现该接口。
class IVisionClient
{
public:
    virtual ~IVisionClient() = default;
    virtual bool requestDetect(const QString& slotId) = 0;
    virtual bool getLatestResult(CupPose& result) = 0;
};
