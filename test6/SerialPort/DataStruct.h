#ifndef __DATASTRUCT_
#define __DATASTRUCT_

#include <bits/types.h>
#include <memory>
#include <opencv2/opencv.hpp>

/**
 * @brief 步兵英雄通信协议结构体 -- 视觉发送给电控
 * @note Flag:       有目标  -- 37
 *                   无目标  -- 72
 *       shoot_mode: 禁止射击 -- 64
 *                   允许射击 -- 76
 */
#pragma pack(1)
struct DataStruct
{
    uint8_t Flag;       // 视觉接管响应的标志位
    uint8_t shoot_mode; // 视觉接管发弹的标志位 (具体发弹策略可由电控进一步调整)
    float yaw;          // yaw 角度   -- 正值表示枪口往右偏移
    float pitch;        // pitch 角度 -- 正值表示枪口往下偏移

    uint8_t End = 0;
};
#pragma pack()

#endif // !__DATASTRUCT_
