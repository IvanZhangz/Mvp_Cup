#pragma once

#include "model/CommonTypes.h"
#include <QString>

// 单个货架库位的配置。
// observePose用于把机器人移动到适合观察该库位的位置；nominalPickPose用于视觉不可用时的兜底抓取。
struct SlotConfig
{
    QString slotId;          // 库位编号，必须与库存配置中的slotId一致。
    QString cupType;         // 该库位存放的杯型，用于订单匹配。
    Pose observePose;        // 观察位姿：底盘/腰/升降/头部会参考该位姿。
    Pose nominalPickPose;    // 名义抓取位姿：标定后的默认抓取点。
};
