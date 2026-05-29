#include "mainwindow.h"

#include <QApplication>

// 程序入口：创建Qt应用对象、显示主窗口，然后进入Qt事件循环。
// MVP所有业务动作都由MainWindow中的“开始取杯”按钮触发。
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow w;
    w.show();

    return app.exec();
}
