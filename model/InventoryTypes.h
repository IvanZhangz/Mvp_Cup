#pragma once

#include <QString>

// 库存原始记录结构。
// 当前InventoryManager内部只使用slotId和quantity推导状态；cupType保留给后续库存系统或配置合并使用。
struct InventoryRecord
{
    QString slotId;   // 库位编号，例如slot_1。
    QString cupType;  // 该库位杯型，当前主要由SlotConfig提供。
    int quantity = 0; // 剩余数量，大于0表示可用。
};
