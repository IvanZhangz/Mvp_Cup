#pragma once

#include "model/SlotConfig.h"

#include <QMap>
#include <QString>
#include <QStringList>

// 货架配置管理器：读取每个库位的杯型、观察位姿和名义抓取位姿。
// 该配置是后续标定数据的入口，真实设备接入后应由现场标定结果生成或更新。
class SlotConfigManager
{
public:
    // 从JSON文件或Qt资源路径加载货架配置。
    bool loadFromFile(const QString& path);
    // 判断某个库位是否存在配置。
    bool hasSlot(const QString& slotId) const;
    // 获取库位配置；调用前建议先hasSlot，避免拿到默认空配置。
    SlotConfig configOf(const QString& slotId) const;
    // 返回所有已配置库位，用于初始化模拟视觉结果或调试展示。
    QStringList slotIds() const;

private:
    // key为slotId，value为该库位完整配置。
    QMap<QString, SlotConfig> m_slots;
};
