#include "InventoryManager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

bool InventoryManager::loadDefault()
{
    // 默认给三个库位都标记为可用，方便没有配置文件时快速跑通MVP。
    m_states.clear();
    m_states["slot_1"] = SlotState::Available;
    m_states["slot_2"] = SlotState::Available;
    m_states["slot_3"] = SlotState::Available;
    return true;
}

bool InventoryManager::loadFromFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject())
        return false;

    const QJsonArray array = doc.object().value("inventory").toArray();
    if (array.isEmpty())
        return false;

    // 只要quantity>0就认为可用；数量扣减目前由状态Empty表达，后续可扩展为真实数量管理。
    m_states.clear();
    for (const QJsonValue& value : array)
    {
        const QJsonObject obj = value.toObject();
        const QString slotId = obj.value("slotId").toString();
        const int quantity = obj.value("quantity").toInt(0);
        if (!slotId.isEmpty())
            m_states[slotId] = quantity > 0 ? SlotState::Available : SlotState::Empty;
    }

    return !m_states.isEmpty();
}

bool InventoryManager::hasAvailableSlot() const
{
    return !firstAvailableSlot().isEmpty();
}

QString InventoryManager::firstAvailableSlot() const
{
    const QStringList slotArray = availableSlots();
    return slotArray.isEmpty() ? QString() : slotArray.first();
}

QStringList InventoryManager::availableSlots() const
{
    QStringList slotArray;
    for (auto it = m_states.begin(); it != m_states.end(); ++it)
    {
        if (it.value() == SlotState::Available)
            slotArray.append(it.key());
    }
    return slotArray;
}

SlotState InventoryManager::stateOf(const QString& slotId) const
{
    // 未知库位按Failed处理，避免工作流误把不存在的库位当作可用。
    return m_states.value(slotId, SlotState::Failed);
}

void InventoryManager::setState(const QString& slotId, SlotState state)
{
    m_states[slotId] = state;
}
