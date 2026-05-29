#pragma once

#include "device/DeviceInterfaces.h"
#include "model/CommonTypes.h"

#include <QMap>

// 视觉客户端适配层。
// 类名保留Ros2VisionClient，是因为真实阶段计划通过ROS2对接相机/视觉节点；
// 但当前MVP默认使用Simulated模式，不依赖ROS2库，保证普通Qt环境也能编译运行。
class Ros2VisionClient : public IVisionClient
{
public:
    enum class Mode
    {
        Simulated,  // 模拟模式：从m_simulatedPoses中返回预置识别结果。
        Ros2Bridge  // 真实ROS2桥接模式：后续在cpp中接入topic/service/action。
    };

    explicit Ros2VisionClient(Mode mode = Mode::Simulated);

    // 给模拟模式设置某个库位的“识别结果”。MVP中使用货架名义抓取位姿作为模拟结果。
    void setSimulatedPose(const QString& slotId, const Pose& pose);

    // IVisionClient接口：触发识别请求。
    bool requestDetect(const QString& slotId) override;
    // IVisionClient接口：读取最近一次识别结果。
    bool getLatestResult(CupPose& result) override;

private:
    Mode m_mode;                         // 当前视觉运行模式。
    QString m_requestedSlotId;           // 最近一次请求识别的库位。
    QMap<QString, Pose> m_simulatedPoses; // 模拟模式下的库位->杯子位姿映射。
};
