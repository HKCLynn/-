/**
 * @file Visioncontrol.cpp
 * @author 徐英杰 (541223130@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-10-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Visioncontrol.h"

using namespace std;

void Visioncontrol::setCamera()
{
    // 获取相机模式
    uint8_t tmp_camId;
    // 判断是否切换相机
    switch (this->__mode)
    {
    case 0U:
    case 1U:
        tmp_camId = NORMAL_CAM;
        break;
    case 2U:
    case 3U:
        tmp_camId = RUNE_CAM;
        break;
    default:
        tmp_camId = NORMAL_CAM;
        break;
    }
    if (this->__cameraId != tmp_camId)
    {
        this->capture.reset();
        this->initCamera(tmp_camId);
        this->__cameraId = tmp_camId;
    }
}

bool Visioncontrol::run()
{
    // 读取数据
    if (!this->port.isOpen())
    {
        cout << "串口异常" << endl;
        ;
    }
    gyro_data = port_manager.filter(this->port.readStruct<GyroData>(0x44, 0x55));

    // 根据收到的陀螺仪数据设置模式
    this->setId(gyro_data.mode);
    // 选择相机模式
    this->setCamera();
    // 视觉处理
    if (!this->capture->read(this->frame))
    {
        // 不能正常打开相机
        cout << "相机异常" << endl;
        ;
        return false;
    }
    // 根据收到的陀螺仪数据选择识别模式
    this->setDetector();
    // 获取当前时间戳
    float record_time = getTickCount();
    // 处理程序
    try
    {
        this->send_data = this->detector->get_data(this->frame);
    }
    catch (...)
    {
        this->send_data.pitch = 0.f;
        this->send_data.yaw = 0.f;
        cout << "数据发送异常" << endl;
        ;
    }

    // 发送数据
    this->port.writeStruct(send_data);
    return true;
}

void Visioncontrol::initCamera()
{
    CameraParam camera_param;
    if (path.empty())
    {
        this->capture = unique_ptr<RMVideoCapture>(new RMVideoCapture());
        if (this->capture->isOpened())
        {
            this->capture->set(CAP_PROP_EXPOSURE, camera_param.cam_exposure);
            // init - 对比度
            this->capture->set(CAP_PROP_CONTRAST, camera_param.cam_contrast);
            // init - Gamma
            this->capture->set(CAP_PROP_GAMMA, camera_param.cam_gamma);
            // init - 设置颜色增益
            this->capture->set(CAP_PROP_XI_WB_KB, camera_param.cam_Bgain);
            this->capture->set(CAP_PROP_XI_WB_KG, camera_param.cam_Ggain);
            this->capture->set(CAP_PROP_XI_WB_KR, camera_param.cam_Rgain);
            // init - 启用颜色增益
            this->capture->set(CAP_PROP_AUTO_WB, 0);
        }
    }
}