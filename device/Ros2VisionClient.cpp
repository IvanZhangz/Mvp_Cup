#include "Ros2VisionClient.h"

Ros2VisionClient::Ros2VisionClient(Mode mode)
    : m_mode(mode)
{
}

void Ros2VisionClient::setSimulatedPose(const QString& slotId, const Pose& pose)
{
    // 空库位编号没有意义，直接忽略，避免污染模拟结果表。
    if (!slotId.isEmpty())
        m_simulatedPoses[slotId] = pose;
}

bool Ros2VisionClient::requestDetect(const QString& slotId)
{
    if (slotId.isEmpty())
        return false;

    // 记录最近一次请求，getLatestResult会根据该库位返回结果。
    m_requestedSlotId = slotId;

    if (m_mode == Mode::Ros2Bridge)
    {
        // 真实硬件阶段在这里发布ROS2识别请求Topic，例如 /vision/request。
        // 当前代码刻意不直接依赖ROS2库，保证MVP在纯Qt环境可编译运行。
        return true;
    }

    // 模拟模式下，请求总是立即成功。
    return true;
}

bool Ros2VisionClient::getLatestResult(CupPose& result)
{
    // 先清空输出，避免调用方误用上一次残留结果。
    result = CupPose();

    if (m_requestedSlotId.isEmpty())
        return false;

    if (m_mode == Mode::Ros2Bridge)
    {
        // 真实硬件阶段在这里订阅/等待ROS2识别结果Topic，例如 /vision/result，并填充CupPose。
        return false;
    }

    // 模拟模式下，如果没有为该库位预置位姿，就视为视觉无结果。
    if (!m_simulatedPoses.contains(m_requestedSlotId))
        return false;

    result.valid = true;
    result.slotId = m_requestedSlotId;
    result.pose = m_simulatedPoses.value(m_requestedSlotId);
    return true;
}
