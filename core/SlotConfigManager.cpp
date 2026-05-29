#include "SlotConfigManager.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

// 把JSON对象解析为Pose。缺失字段会按0处理，适合MVP默认值；真实项目中可增加严格校验。
static Pose parsePose(const QJsonObject& obj)
{
    Pose pose;
    pose.x = obj.value("x").toDouble();
    pose.y = obj.value("y").toDouble();
    pose.z = obj.value("z").toDouble();
    pose.roll = obj.value("roll").toDouble();
    pose.pitch = obj.value("pitch").toDouble();
    pose.yaw = obj.value("yaw").toDouble();
    return pose;
}

bool SlotConfigManager::loadFromFile(const QString& path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    const QJsonArray slotArray = doc.object().value("slots").toArray();

    m_slots.clear();

    for (const auto& value : slotArray)
    {
        const QJsonObject obj = value.toObject();

        SlotConfig config;
        config.slotId = obj.value("slotId").toString();
        config.cupType = obj.value("cupType").toString();
        config.observePose = parsePose(obj.value("observePose").toObject());
        config.nominalPickPose = parsePose(obj.value("nominalPickPose").toObject());

        if (!config.slotId.isEmpty())
            m_slots[config.slotId] = config;
    }

    return !m_slots.isEmpty();
}

bool SlotConfigManager::hasSlot(const QString& slotId) const
{
    return m_slots.contains(slotId);
}

SlotConfig SlotConfigManager::configOf(const QString& slotId) const
{
    return m_slots.value(slotId);
}

QStringList SlotConfigManager::slotIds() const
{
    return m_slots.keys();
}
