#pragma once

#include <QMainWindow>

class QPushButton;
class QTextEdit;

// 主窗口只承担MVP演示入口职责：
// 1. 展示“开始取杯”按钮；
// 2. 展示流程日志；
// 3. 在按钮点击时组装Mock设备、配置管理器和业务工作流。
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    // Qt槽函数：用户点击“开始取杯”按钮后执行一次模拟单杯取杯流程。
    void onStartPickClicked();

private:
    // 触发MVP流程的按钮。
    QPushButton* m_startButton;
    // 只读日志窗口，用于观察库存加载、选库位、视觉、抓取和放置状态。
    QTextEdit* m_logEdit;
};
