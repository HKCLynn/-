#pragma once

class LightParam
{
public:
    float MAX_RATIO = 7;        //灯条最大长宽比
    float MIN_RATIO = 1.5;      //灯条最小长宽比
    float MIN_AREA_RATIO = 0.6; //灯条最小占比
    float MAX_ANGLE = 45;       //灯条倾斜最大角度
    int MIN_AREA = 10;          //灯条最小面积
};

class ArmorParam
{
public:
    float MAX_CROSS_RATIO = 0.115;  //对角线和装甲板高度的比值
    float MAX_DETLA_LENGTH = 20;    //两灯条高度的最大差值
    float MAX_LENGTH_RATIO;         //两灯条高度的最大比值
    float MAX_RATIO = 3.3;          //装甲板最大长宽比
    float MIN_RATIO = 1.5;          //装甲板最小长宽比
    float MAX_ANGLE = 5.5;          //两灯条倾斜角度的差值
    float MIN_X_LENGTH_RATIO = 0.5; //两灯条中心点x的差值和高度的比值
    float MAX_Y_LENGTH_RATIO = 0.7; //亮灯条中心点y的差值和高度的比值
};

// 相机参数
struct CameraParam
{
    Mat cameraMatrix;        // 相机内参
    Mat distCoeff;           // 畸变参数
    int image_width = 1280;  // 相机宽度和高度
    int image_height = 1024; // 相机宽度和高度
    int cam_exposure = 300;  // 相机曝光
    int cam_gamma = 100;     // 相机 Gamma 值
    int cam_contrast = 100;  // 相机对比度
    int cam_Bgain = 180;     // 相机蓝色增益
    int cam_Ggain = 141;     // 相机绿色增益
    int cam_Rgain = 100;     // 相机红色增益

};