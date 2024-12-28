# 单片机期末作业课程项目

## 文件夹层次介绍

- Code：51代码

- Hardware：硬件设计
  - stc8g.eprj      核心板
  - Step-down.eprj  扩展模块（包含降压，电流测量，锂电池模块）
  - term_work.eprj  Demo 0.1，放弃的版本

- Structure：结构设计

- Report：报告与演示文档
- Software：烧录软件与串口助手

## 代码使用说明

本项目采用stc8g芯片，其烧录需要单独配置。具体操作如下：

使用 Keil 之前需要先安装 STC 的仿真驱动。首先开 STC 的 ISP 下载软件，然后软件右边功能区的“Keil 仿真设置”页面中点击“添加型号和头文件到 Keil 中 添加 STC 仿真器驱动到 Keil 中”按钮：

![Step1](Photo\Add_Header_To_Keil.png)

按下后会出现如下画面：

![Step2](Photo\Choose_file.png)

将目录定位到 Keil 软件的安装目录，然后确定。安装成功后会弹出如下的提示框：

![Step3](Photo\success.png)

即表示驱动正确安装了。
