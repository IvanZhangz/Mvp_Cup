#include "OrderManager.h"
#include "PickWorkflow.h"

OrderManager::OrderManager(PickWorkflow* workflow)
    : m_workflow(workflow)
{
}

bool OrderManager::executeOrder(const PickOrder& order)
{
    if (!m_workflow)
        return false;

    // 目前不做排队和拆单，直接同步执行单杯工作流。
    return m_workflow->executeSingleCupOrder(order);
}
