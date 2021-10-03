/**
 * @file GyroScope.h
 * @author 赵曦 (535394140@qq.com)
 * @brief 通信协议管理类头文件
 * @version 1.0
 * @date 2021-06-14
 * 
 * @copyright Copyright SCUT RobotLab(c) 2021
 * 
 */

#pragma once

#include <opencv2/opencv.hpp>
#include "Parameter.h"

using namespace std;

/**
 * @brief 步兵通信协议结构体 -- 电控发送给视觉
 * 
 * @note 如果没接受到电控发来的数据，视觉沿用上一次收到的数据，
 *       若第一次没有收到，视觉将使用缺省值
 *       color_mode: 0 -- 己方为红
 *                   1 -- 己方为蓝
 *       shootSpeed: 15 (默认)
 *                   18
 *                   30
 *       mode:       0 -- 普通模式：击打地面目标 (默认)
 *                   1 -- 哨兵模式：击打哨兵
 *                   2 -- 神符模式：打符
 *                   3 -- 干扰模式：干扰敌方打符
 */
struct GyroData
{
    float pitch;        // 陀螺仪 - pitch 角度
    float yaw;          // 陀螺仪 - yaw 角度
    uint8_t color_mode; // 己方颜色
    uint8_t shootSpeed; // 枪管射速
    uint8_t mode;       // 击打策略

} __attribute__((packed)); // packed 参数用来减小结构占用的空间，也就是取消对齐，防止读取到错误的数据

class GyroScope
{
public:
    GyroScope();                               // 构造函数
    GyroData filter(const vector<GyroData> &); // 从 vector 中提取陀螺仪参数

    int wrong_collect = 0;

private:
    bool isInit;       // 初始化标志
    GyroData gyroData; // 陀螺仪参数
};


struct DataStruct
{
    uint8_t Flag;       // 视觉接管响应的标志位
    uint8_t shoot_mode; // 视觉接管发弹的标志位 (具体发弹策略可由电控进一步调整)
    float yaw;          // yaw 角度   -- 正值表示枪口往右偏移
    float pitch;        // pitch 角度 -- 正值表示枪口往下偏移

    uint8_t End = 0;
};
#pragma pack()
