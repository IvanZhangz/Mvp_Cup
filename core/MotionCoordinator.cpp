#include "MotionCoordinator.h"
#include "device/DeviceInterfaces.h"

MotionCoordinator::MotionCoordinator(IArmDevice* arm,
                                     IChassisDevice* chassis,
                                     IHeadDevice* head,
                                     IWaistDevice* waist,
                                     ILiftDevice* lift,
                                     IGripperDevice* gripper)
    : m_arm(arm)
    , m_chassis(chassis)
    , m_head(head)
    , m_waist(waist)
    , m_lift(lift)
    , m_gripper(gripper)
{
}

bool MotionCoordinator::moveToObserve(const SlotConfig& slotConfig)
{
    // observePose来自货架标定配置，当前把z用于升降高度、yaw用于腰部朝向。
    const Pose& observePose = slotConfig.observePose;

    // 先让底盘靠近库位，再按标定位姿细调；随后调整腰、升降和头部视线。
    if (m_chassis && !m_chassis->moveToObservePosition(slotConfig.slotId)) return false;
    if (m_chassis && !m_chassis->moveToPose(observePose)) return false;
    if (m_waist && !m_waist->rotateTo(observePose.yaw)) return false;
    if (m_lift && !m_lift->moveToHeight(observePose.z)) return false;
    if (m_head && !m_head->lookAtSlot(slotConfig.slotId)) return false;
    return true;
}

bool MotionCoordinator::executePick(const Pose& pickPose)
{
    // 预抓取点在抓取点上方10cm，避免机械臂直接横向撞杯或撞货架。
    Pose prePick = pickPose;
    prePick.z += 0.1;

    // 简化顺序：对准腰部和升降高度 -> 打开夹爪 -> 到预抓取 -> 下探抓取 -> 闭合夹爪 -> 抬升撤出。
    if (m_waist && !m_waist->rotateTo(pickPose.yaw)) return false;
    if (m_lift && !m_lift->moveToHeight(prePick.z)) return false;
    if (m_gripper && !m_gripper->open()) return false;
    if (m_arm && !m_arm->moveToPrePickPose(prePick)) return false;
    if (m_lift && !m_lift->moveToHeight(pickPose.z)) return false;
    if (m_arm && !m_arm->moveToPickPose(pickPose)) return false;
    if (m_gripper && !m_gripper->close()) return false;
    if (m_lift && !m_lift->moveToHeight(prePick.z)) return false;
    return true;
}

bool MotionCoordinator::executePlace(const Pose& placePose)
{
    // 放置阶段移动到订单指定放置位，然后打开夹爪释放杯子。
    if (m_waist && !m_waist->rotateTo(placePose.yaw)) return false;
    if (m_lift && !m_lift->moveToHeight(placePose.z)) return false;
    if (m_arm && !m_arm->moveToPlacePose(placePose)) return false;
    if (m_gripper && !m_gripper->open()) return false;
    return true;
}
