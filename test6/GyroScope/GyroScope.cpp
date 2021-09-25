/**
 * @file GyroScope.cpp
 * @author 赵曦 (535394140@qq.com)
 * @brief 通信协议管理类
 * @version 1.0
 * @date 2021-06-14
 * 
 * @copyright Copyright SCUT RobotLab(c) 2021
 * 
 */

#include "GyroScope.h"

/**
 * @brief 构造函数
 */
GyroScope::GyroScope()
{
    this->isInit = false;
}

/**
 * @brief 构造函数
 * 
 * @param gyroScope 收到的数据向量
 */
GyroData GyroScope::filter(const vector<GyroData> &gyroScope)
{
    // 判断是否初始化
    if (!this->isInit)
    {
        this->gyroData.pitch = other_param.INIT_PITCH;
        this->gyroData.yaw = other_param.INIT_YAW;
        this->gyroData.color_mode = other_param.INIT_COLOR_MODE;
        this->gyroData.shootSpeed = other_param.INIT_SHOOT_SPEED;
        this->gyroData.mode = other_param.INIT_MODE;
        // 更新标志位
        this->isInit = true;
    }
    // 判断是否收到数据
    if (!gyroScope.empty())
    {
        GyroData tmp_data = gyroScope.back();
        // 异常判断
        if ((tmp_data.shootSpeed != 15 && tmp_data.shootSpeed != 18 && tmp_data.shootSpeed != 30) ||
            (tmp_data.mode > 5) || (tmp_data.color_mode != 0 && tmp_data.color_mode != 1))
        {
            return this->gyroData;
        }
        else
        {
            this->gyroData = tmp_data;
        }
    }

    // 手动设置子弹射速
    switch (this->gyroData.shootSpeed)
    {
    case 15:
        this->gyroData.shootSpeed -= strategy_param.SHOOT_SPEED_15_BIAS;
        break;
    case 18:
        this->gyroData.shootSpeed -= strategy_param.SHOOT_SPEED_18_BIAS;
        break;
    case 30:
        this->gyroData.shootSpeed -= strategy_param.SHOOT_SPEED_30_BIAS;
        break;
    default:
        break;
    }
    return this->gyroData;
}
