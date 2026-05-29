#pragma once

#include "device/DeviceInterfaces.h"

// 以下Mock设备全部直接返回true，用于在没有真实硬件时验证软件主流程。
// 接真实设备时，用真实适配类替换MainWindow中创建的这些Mock对象即可。
class MockArmDevice : public IArmDevice
{
public:
    bool moveToPrePickPose(const Pose&) override { return true; }
    bool moveToPickPose(const Pose&) override { return true; }
    bool moveToPlacePose(const Pose&) override { return true; }
};

class MockChassisDevice : public IChassisDevice
{
public:
    bool moveToObservePosition(const QString&) override { return true; }
    bool moveToPose(const Pose&) override { return true; }
};

class MockHeadDevice : public IHeadDevice
{
public:
    bool lookAtSlot(const QString&) override { return true; }
};

class MockWaistDevice : public IWaistDevice
{
public:
    bool rotateTo(double) override { return true; }
};

class MockLiftDevice : public ILiftDevice
{
public:
    bool moveToHeight(double) override { return true; }
};

class MockGripperDevice : public IGripperDevice
{
public:
    bool open() override { return true; }
    bool close() override { return true; }
};
