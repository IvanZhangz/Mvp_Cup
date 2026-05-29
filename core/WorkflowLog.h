#pragma once

#include <QString>

// 工作流日志接口。
// 核心业务层只调用这个接口，不直接依赖Qt控件、文件日志库或ROS日志，方便后续替换日志后端。
class IWorkflowLogger
{
public:
    virtual ~IWorkflowLogger() = default;

    // 普通流程信息，例如“选择库位”“使用视觉位姿”。
    virtual void info(const QString& message) = 0;
    // 错误信息，例如“移动失败”“缺少库位配置”。
    virtual void error(const QString& message) = 0;
};
