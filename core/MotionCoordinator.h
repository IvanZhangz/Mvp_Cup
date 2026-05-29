#pragma once

#include "model/CommonTypes.h"
#include "model/SlotConfig.h"

class IArmDevice;
class IChassisDevice;
class IHeadDevice;
class IWaistDevice;
class ILiftDevice;
class IGripperDevice;

// 运动协调器：把“观察/抓取/放置”这些业务动作拆解成多个硬件接口调用。
// 它不关心具体设备品牌；真实设备和Mock设备都通过DeviceInterfaces.h中的接口接入。
class MotionCoordinator
{
public:
    MotionCoordinator(IArmDevice* arm,
                      IChassisDevice* chassis,
                      IHeadDevice* head,
                      IWaistDevice* waist,
                      ILiftDevice* lift,
                      IGripperDevice* gripper);

    // 移动到某个库位的观察姿态，主要驱动底盘、腰、升降和头部。
    bool moveToObserve(const SlotConfig& slotConfig);
    // 根据视觉位姿或名义抓取位姿执行抓取动作。
    bool executePick(const Pose& pickPose);
    // 把已抓取的杯子移动到订单指定放置位并松开夹爪。
    bool executePlace(const Pose& placePose = Pose());

private:
    IArmDevice* m_arm;
    IChassisDevice* m_chassis;
    IHeadDevice* m_head;
    IWaistDevice* m_waist;
    ILiftDevice* m_lift;
    IGripperDevice* m_gripper;
};
