#include "SlotSelector.h"

#include "InventoryManager.h"
#include "SlotConfigManager.h"

SlotSelector::SlotSelector(InventoryManager* inventory, SlotConfigManager* slotConfig)
    : m_inventory(inventory)
    , m_slotConfig(slotConfig)
{
}

QString SlotSelector::selectSlot(const PickOrder& order) const
{
    if (!m_inventory)
        return QString();

    // 先从库存中拿到所有可用库位，避免选择空库位或失败库位。
    const QStringList slots = m_inventory->availableSlots();

    // 如果订单没有杯型要求，或者还没加载货架配置，则退化为第一个可用库位。
    if (order.cupType.isEmpty() || !m_slotConfig)
        return slots.isEmpty() ? QString() : slots.first();

    // 有杯型要求时，必须同时满足库存可用和货架配置杯型一致。
    for (const QString& slotId : slots)
    {
        if (!m_slotConfig->hasSlot(slotId))
            continue;

        const SlotConfig config = m_slotConfig->configOf(slotId);
        if (config.cupType == order.cupType)
            return slotId;
    }

    return QString();
}
