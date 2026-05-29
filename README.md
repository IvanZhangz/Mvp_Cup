# Mvp_Cup 取杯机器人 MVP

这是一个基于 **Qt Widgets + QMake + C++17** 的取杯机器人 MVP 工程。当前目标不是直接控制真实硬件，而是先把“下单 → 选库位 → 观察 → 视觉 → 抓取 → 放置 → 更新库存”的软件主链路跑通，后续再把 Mock 设备替换成真实相机、机械臂、底盘、腰部、升降、头/颈和夹爪控制。

## 当前已经具备的能力

1. 点击 Qt 界面中的“开始取杯”按钮后，程序会执行一次模拟单杯订单。
2. 库存从 `config/inventory.json` 读取；如果外部文件不存在，会退回到内置默认配置或代码默认库存。
3. 货架库位、观察位姿、名义抓取位姿从 `config/shelf_config.json` 读取。
4. `resources.qrc` 会把两份 JSON 配置打包进 Qt 资源系统，避免运行目录没有 `config/` 时 MVP 直接失败。
5. `SlotSelector` 会根据订单杯型 `cupType` 和库存状态选择可用库位。
6. `Ros2VisionClient` 当前默认运行在模拟模式，会返回预置的名义抓取位姿；真实 ROS2 视觉接入点已经保留。
7. `MotionCoordinator` 会按顺序调用底盘、腰部、升降、头部、机械臂、夹爪接口，当前实际注入的是全部返回成功的 Mock 设备。
8. 成功完成取放后，库存中的对应库位会被标记为空。

## 目录说明

- `main.cpp` / `mainwindow.*`：Qt 程序入口和 MVP 按钮触发逻辑。
- `model/`：通用数据结构，例如位姿、视觉结果、订单、库位配置。
- `core/`：业务核心，包括库存管理、库位选择、订单管理、取杯流程和运动协调。
- `device/`：硬件抽象接口、Mock 设备和视觉客户端适配层。
- `config/`：MVP 示例配置。JSON 本身不支持传统注释，因此说明性文字放在 `_comment` 字段中。
- `resources.qrc`：Qt 资源清单，用于把配置文件嵌入可执行程序。

## 如何编译运行

1. 使用 Qt Creator 打开 `Mvp_Cup.pro`。
2. 选择带有 Qt Widgets 的 Qt Kit。
3. 构建并运行。
4. 点击“开始取杯”，观察界面日志中的库存加载、库位选择、视觉模拟、抓取、放置和库存更新结果。

命令行环境可使用类似流程：

```bash
qmake Mvp_Cup.pro
make
./Mvp_Cup
```

## 后续接真实硬件的位置

真实设备建议按以下方式接入：

1. 真实相机/视觉：实现或扩展 `device/Ros2VisionClient.*`，在 `Mode::Ros2Bridge` 中发布 `/vision/request` 并接收 `/vision/result`。
2. 真实机械臂：实现 `IArmDevice`，替换 `MockArmDevice`。
3. 真实底盘：实现 `IChassisDevice`，替换 `MockChassisDevice`。
4. 真实腰部：实现 `IWaistDevice`，替换 `MockWaistDevice`。
5. 真实升降：实现 `ILiftDevice`，替换 `MockLiftDevice`。
6. 真实头/颈：实现 `IHeadDevice`，替换 `MockHeadDevice`。
7. 真实夹爪：实现 `IGripperDevice`，替换 `MockGripperDevice`。

接入真实硬件前，建议补充急停、归零、限位、超时、重试、碰撞保护和错误码上报，避免软件流程直接驱动未校准设备。
