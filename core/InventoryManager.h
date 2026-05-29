#pragma once

#include <QMap>
#include <QString>
#include <QStringList>

// 库位状态机。
// Available：可被新订单选择；Reserved：当前订单已锁定；Empty：杯子已取走；Failed：该库位流程失败，需要人工或上层策略处理。
enum class SlotState
{
    Available,
    Reserved,
    Empty,
    Failed
};

// 库存管理器：负责从配置加载库位库存，并维护MVP运行期间的库位状态。
// 注意：当前状态只保存在内存中，程序退出后不会回写JSON；真实项目中应接数据库或库存服务。
class InventoryManager
{
public:
    // 加载代码内置默认库存，主要用于配置缺失时让MVP仍可演示。
    bool loadDefault();
    // 从JSON文件或Qt资源路径加载库存。
    bool loadFromFile(const QString& path);

    // 是否存在至少一个Available库位。
    bool hasAvailableSlot() const;
    // 返回第一个Available库位，保留给简单场景或调试使用。
    QString firstAvailableSlot() const;
    // 返回所有Available库位，供SlotSelector按杯型进一步筛选。
    QStringList availableSlots() const;

    // 查询和更新单个库位状态。
    SlotState stateOf(const QString& slotId) const;
    void setState(const QString& slotId, SlotState state);

private:
    // key为slotId，value为当前库位状态。QMap会按key排序，选择结果稳定可复现。
    QMap<QString, SlotState> m_states;
};
