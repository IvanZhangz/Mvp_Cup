#pragma once

#include "model/CommonTypes.h"

class InventoryManager;
class MotionCoordinator;
class SlotConfigManager;
class IVisionClient;
class IWorkflowLogger;

// 取杯工作流：串联选库位、库存锁定、观察、视觉、抓取、放置和库存更新。
// 这是MVP的核心业务编排层，不直接依赖Qt界面或具体硬件SDK。
class PickWorkflow
{
public:
    PickWorkflow(InventoryManager* inventory,
                 SlotConfigManager* slotConfig,
                 MotionCoordinator* motion,
                 IVisionClient* vision,
                 IWorkflowLogger* logger = nullptr);

    // 执行单杯订单；成功返回true，任一步失败都会标记库位失败并返回false。
    bool executeSingleCupOrder(const PickOrder& order);

private:
    InventoryManager* m_inventory;
    SlotConfigManager* m_slotConfig;
    MotionCoordinator* m_motion;
    IVisionClient* m_vision;
    IWorkflowLogger* m_logger;
};
