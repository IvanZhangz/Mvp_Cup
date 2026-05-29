#include "mainwindow.h"

#include "core/InventoryManager.h"
#include "core/MotionCoordinator.h"
#include "core/OrderManager.h"
#include "core/PickWorkflow.h"
#include "core/SlotConfigManager.h"
#include "core/WorkflowLog.h"
#include "device/MockDevices.h"
#include "device/Ros2VisionClient.h"

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

namespace
{
// 配置查找顺序：
// 1. 可执行程序同级的config目录，适合部署后的配置覆盖；
// 2. 当前工作目录下的config目录，适合Qt Creator或命令行从仓库根目录运行；
// 3. Qt资源系统中的内置配置，确保MVP在没复制配置文件时也能跑起来。
QString resolveConfigPath(const QString& fileName)
{
    const QString appPath = QCoreApplication::applicationDirPath() + "/config/" + fileName;
    if (QFileInfo::exists(appPath))
        return appPath;

    const QString cwdPath = QDir::currentPath() + "/config/" + fileName;
    if (QFileInfo::exists(cwdPath))
        return cwdPath;

#ifdef PROJECT_SOURCE_DIR
    const QString sourcePath = QStringLiteral(PROJECT_SOURCE_DIR) + "/config/" + fileName;
    if (QFileInfo::exists(sourcePath))
        return sourcePath;
#endif

    return ":/config/" + fileName;
}

// 简单的Qt界面日志适配器：把核心流程中的info/error消息追加到QTextEdit。
// 这样核心层只依赖IWorkflowLogger接口，不需要知道Qt控件的存在。
class QTextEditWorkflowLogger : public IWorkflowLogger
{
public:
    explicit QTextEditWorkflowLogger(QTextEdit* output)
        : m_output(output)
    {
    }

    void info(const QString& message) override
    {
        append("[INFO] " + message);
    }

    void error(const QString& message) override
    {
        append("[ERROR] " + message);
    }

private:
    void append(const QString& message)
    {
        if (m_output)
            m_output->append(message);
    }

    QTextEdit* m_output;
};
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    // 构造一个最小可用界面：上方按钮，下方日志。
    QWidget* center = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(center);

    m_startButton = new QPushButton("开始取杯", this);
    m_logEdit = new QTextEdit(this);
    m_logEdit->setReadOnly(true);

    layout->addWidget(m_startButton);
    layout->addWidget(m_logEdit);
    setCentralWidget(center);

    connect(m_startButton, &QPushButton::clicked,
            this, &MainWindow::onStartPickClicked);
}

void MainWindow::onStartPickClicked()
{
    m_logEdit->append("收到Qt界面取杯指令");

    // 1. 加载库存。库存决定哪些库位还有杯子可取。
    InventoryManager inventory;
    const QString configPath = resolveConfigPath("inventory.json");

    if (inventory.loadFromFile(configPath))
        m_logEdit->append("库存配置加载成功: " + configPath);
    else
    {
        // 库存失败时允许使用代码默认库存，保证MVP演示不被配置文件阻塞。
        inventory.loadDefault();
        m_logEdit->append("库存配置加载失败，使用默认库存");
    }

    // 2. 加载货架库位位姿。真实抓取不能缺少位姿，因此加载失败时直接停止。
    SlotConfigManager slotConfig;
    const QString shelfConfigPath = resolveConfigPath("shelf_config.json");
    if (slotConfig.loadFromFile(shelfConfigPath))
        m_logEdit->append("货架位姿配置加载成功: " + shelfConfigPath);
    else
        m_logEdit->append("货架位姿配置加载失败，视觉失败时将使用零位姿兜底");
    {
        m_logEdit->append("货架位姿配置加载失败，停止执行以避免使用未知抓取位姿");
        return;
    }

    // 3. 当前MVP注入Mock设备；后续真实硬件接入时，只需要替换这些对象。
    MockArmDevice arm;
    MockChassisDevice chassis;
    MockHeadDevice head;
    MockWaistDevice waist;
    MockLiftDevice lift;
    MockGripperDevice gripper;

    // 4. 运动协调器负责把业务动作拆成底盘/腰/升降/头/机械臂/夹爪动作。
    MotionCoordinator motion(&arm, &chassis, &head, &waist, &lift, &gripper);

    // 5. 视觉客户端使用模拟模式，并把每个库位的名义抓取位姿作为模拟识别结果。
    Ros2VisionClient vision(Ros2VisionClient::Mode::Simulated);
    for (const QString& slotId : slotConfig.slotIds())
        vision.setSimulatedPose(slotId, slotConfig.configOf(slotId).nominalPickPose);

    // 6. 组装核心工作流。核心层只依赖接口，便于后续替换真实硬件实现。
    QTextEditWorkflowLogger logger(m_logEdit);
    PickWorkflow workflow(&inventory, &slotConfig, &motion, &vision, &logger);
    OrderManager orderManager(&workflow);

    // 7. MVP先固定生成一张单杯订单；后续可改为接收真实订单系统输入。
    PickOrder order;
    order.orderId = "mvp_order_001";
    order.cupType = "default";
    order.placePose.x = 0.1;
    order.placePose.y = -0.2;
    order.placePose.z = 0.25;
    order.placePose.yaw = 0.0;

    const bool ok = orderManager.executeOrder(order);

    if (ok)
        m_logEdit->append("MVP取杯流程执行成功");
    else
        m_logEdit->append("MVP取杯流程执行失败");
}
