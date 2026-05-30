#include "PickWorkflow.h"

#include "InventoryManager.h"
#include "MotionCoordinator.h"
#include "SlotConfigManager.h"
#include "device/Ros2VisionClient.h"
#include "SlotSelector.h"
#include "WorkflowLog.h"
#include "device/DeviceInterfaces.h"

PickWorkflow::PickWorkflow(InventoryManager* inventory,
                           SlotConfigManager* slotConfig,
                           MotionCoordinator* motion,
                           IVisionClient* vision,
                           IWorkflowLogger* logger)
    : m_inventory(inventory)
    , m_slotConfig(slotConfig)
    , m_motion(motion)
    , m_vision(vision)
    , m_logger(logger)
{
}

bool PickWorkflow::executeSingleCupOrder(const PickOrder& order)
{
    if (!m_inventory)
        return false;

    // 1. 按库存状态和订单杯型选择库位。
    SlotSelector selector(m_inventory, m_slotConfig);
    const QString slotId = selector.selectSlot(order);
    if (slotId.isEmpty())
    {
        if (m_logger) m_logger->error("没有匹配订单杯型的可用库位: " + order.cupType);
        return false;
    }

    // 2. 库位必须有位姿配置，否则真实机器人无法安全观察和抓取。
    if (!m_slotConfig || !m_slotConfig->hasSlot(slotId))
    {
        if (m_logger) m_logger->error("缺少库位位姿配置: " + slotId);
        return false;
    }

    const SlotConfig slotConfig = m_slotConfig->configOf(slotId);
    if (m_logger) m_logger->info("选择库位: " + slotId + ", 杯型: " + slotConfig.cupType);

    // 3. 先锁定库位，避免同一库位被后续订单重复选择。
    m_inventory->setState(slotId, SlotState::Reserved);

    // 4. 移动到观察位，为视觉识别创造稳定视角。
    if (m_motion && !m_motion->moveToObserve(slotConfig))
    {
        if (m_logger) m_logger->error("移动到观察位失败: " + slotId);
        m_inventory->setState(slotId, SlotState::Failed);
        return false;
    }

    // 5. 触发视觉识别。模拟模式会立即准备好结果；真实ROS2模式可在这里发布请求。
    if (m_vision && !m_vision->requestDetect(slotId))
    {
        if (m_logger) m_logger->error("视觉识别请求失败: " + slotId);
        m_inventory->setState(slotId, SlotState::Failed);
        return false;
    }

    // 6. 优先使用视觉位姿；如果没有有效视觉结果，则使用标定名义抓取位姿兜底。
    Pose pickPose = slotConfig.nominalPickPose;
    CupPose cupPose;
    if (m_vision && m_vision->getLatestResult(cupPose) && cupPose.valid)
    {
        pickPose = cupPose.pose;
        if (m_logger) m_logger->info("视觉返回有效杯子位姿，使用视觉位姿抓取");
    }
    else
    {
        if (m_logger) m_logger->info("视觉未返回有效位姿，使用货架名义抓取位姿兜底");
    }

    // 7. 执行抓取动作。
    if (m_motion && !m_motion->executePick(pickPose))
    {
        if (m_logger) m_logger->error("抓取动作失败: " + slotId);
        m_inventory->setState(slotId, SlotState::Failed);
        return false;
    }

    // 8. 执行放置动作，放置位姿来自订单。
    if (m_motion && !m_motion->executePlace(order.placePose))
    {
        if (m_logger) m_logger->error("放置动作失败: " + slotId);
        m_inventory->setState(slotId, SlotState::Failed);
        return false;
    }

    // 9. 全流程成功后，将库位标记为空。
    m_inventory->setState(slotId, SlotState::Empty);
    if (m_logger) m_logger->info("订单完成，库位已更新为空: " + slotId);
    return true;
}
