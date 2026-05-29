#pragma once

#include "model/CommonTypes.h"

#include <QString>

class InventoryManager;
class SlotConfigManager;

// 库位选择器：把“库存是否可用”和“订单杯型是否匹配”两个条件合并。
// 后续可以在这里继续增加先进先出、距离最短、失败库位避让、优先级等策略。
class SlotSelector
{
public:
    SlotSelector(InventoryManager* inventory, SlotConfigManager* slotConfig);

    // 返回最适合该订单的库位；如果没有匹配库位，返回空字符串。
    QString selectSlot(const PickOrder& order) const;

private:
    InventoryManager* m_inventory;
    SlotConfigManager* m_slotConfig;
};
