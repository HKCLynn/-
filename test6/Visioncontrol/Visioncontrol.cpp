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
    if (this->__cameraId == 0U)
    {
        this->capture.reset();
        this->initCamera();
        __cameraId = 1U;
    }
}

bool Visioncontrol::run(ArmorTracker &tracker, Strategy &strategy)
{
    // 读取数据
    if (!this->port.isOpen())
    {
        cout << "串口异常" << endl;
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
        return false;
    }
    // 获取当前时间戳
    float record_time = getTickCount();
    // 处理程序
    try
    {
        this->send_data = this->detector->get_data(this->frame, tracker, strategy ,record_time ,gyro_data);
        cout << "yaw=" << send_data.yaw << endl;
        cout << "pitch=" << send_data.pitch << endl;
    }
    catch (...)
    {
        this->send_data.pitch = 0.f;
        this->send_data.yaw = 0.f;
        cout << "数据发送异常" << endl;
    }

    // 发送数据
    this->port.writeStruct(send_data);
    //放出效果图
    namedWindow("windows", WINDOW_NORMAL);
    resizeWindow("windows", Size(640, 512));
    imshow("windows", frame);
    if (27 == waitKey(1))
    {
        if (27 == waitKey(0))
        {
            exit(0);
        }
    }
    return true;
}

void Visioncontrol::initCamera()
{
    CameraParam camera_param("/home/lynn/桌面/-/test6/setting_file/CameraParam.yml");
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
