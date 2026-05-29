#pragma once

#include "model/CommonTypes.h"

class PickWorkflow;

// 订单管理器：当前MVP只做一层薄封装，把订单交给PickWorkflow执行。
// 后续可以在这里加入订单队列、订单取消、重试、状态上报等能力。
class OrderManager
{
public:
    explicit OrderManager(PickWorkflow* workflow);

    // 执行一张单杯取杯订单。
    bool executeOrder(const PickOrder& order);

private:
    PickWorkflow* m_workflow;
};
