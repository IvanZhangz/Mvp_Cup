#pragma once

#include <QString>

// 通用六自由度位姿。
// x/y/z表示空间位置，roll/pitch/yaw表示姿态角。
// MVP阶段先用简单double字段，后续如果接入ROS2 tf2、Eigen或厂商SDK Pose类型，可以在适配层转换。
struct Pose
{
    double x = 0.0;      // X方向位置，单位由具体硬件适配层约定，MVP示例按米理解。
    double y = 0.0;      // Y方向位置。
    double z = 0.0;      // Z方向位置或升降高度。
    double roll = 0.0;   // 绕X轴旋转角。
    double pitch = 0.0;  // 绕Y轴旋转角。
    double yaw = 0.0;    // 绕Z轴旋转角，当前也用于腰部朝向示例。
};

// 视觉识别得到的杯子位姿结果。
// valid=false表示本次没有可用视觉结果，工作流会退回到货架配置的名义抓取位姿。
struct CupPose
{
    bool valid = false;  // 视觉结果是否可信可用。
    QString slotId;      // 视觉结果对应的库位，便于排查识别是否串位。
    Pose pose;           // 杯子的实际抓取位姿。
};

// MVP订单：当前只处理单杯订单。
// 后续接真实订单系统时，可继续增加数量、优先级、目标出杯口、订单状态等字段。
struct PickOrder
{
    QString orderId;     // 订单号，用于日志追踪。
    QString cupType;     // 杯型，用于SlotSelector匹配货架库位。
    Pose placePose;      // 放置位姿，描述取到杯子后放到哪里。
};
