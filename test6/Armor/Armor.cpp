/**
*@file Armor.cpp
*@author 徐英杰（541223130@qq.com）
*/



#include "Armor.h"

using namespace std;
using namespace cv;
using namespace Eigen;

/**
*@brief 初始化
*@param frame 传入未处理的图
*@param mask 传入二值化等图像处理后的图
*/

FindArmor::FindArmor(Mat frame, Mat mask)
{
    this->frame = frame;
    this->mask = mask;
}

/**
*@brief 灯条寻找并配对
*@param centers 配对后将装甲板中心点存入的序列
*/

void FindArmor::lights_pair(vector<Point2f> &centers, ArmorTracker &Tracker)
{
    //筛选灯条的参数
    LightParam light_param;
    //筛选装甲板的参数
    ArmorParam armor_param;
    //在二值化等图像处理后的图片中寻找轮廓
    findContours(mask, contours_all, hierarchy_all, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
    //存放每个灯条四个点坐标的容器
    vector<RotatedRect> light_info;
    //存放灯条的容器
    vector<Light> lights;
    for (int i = 0; i < contours_all.size(); i++)
    {
        //椭圆拟合
        RotatedRect r = fitEllipse(contours_all[i]);
        //统一角度
        r.angle = (r.angle > 90 ? r.angle - 180 : r.angle);
        //对于每一个轮廓进行筛选得到灯条
        if (r.size.height / r.size.width < light_param.MAX_RATIO &&
            r.size.height / r.size.width > light_param.MIN_RATIO &&
            contourArea(contours_all[i]) > light_param.MIN_AREA &&
            contourArea(contours_all[i]) / r.size.area() > light_param.MIN_AREA_RATIO &&
            r.center.x - r.size.width > 0 &&
            r.center.y - r.size.height > 0 &&
            abs(r.angle) < light_param.MAX_ANGLE)
        {
            //灯条的四个点信息
            Point2f point[4];
            r.points(point);
            //插入容器中
            light_info.push_back(r);
            //创建灯条对象
            Light input_light(point, contours_all[i], r.center);
            //将筛选出的灯条插入容器中
            lights.push_back(input_light);
            //画出灯条（用于测试）
            drawContours(frame, contours_all, i, (255, 255, 255), 10);
        }
    }
    //装甲板容器
    vector<Armor> armors;
    //对装甲板进行排序
    sort(lights.begin(), lights.end(),
         [&](const Light left, Light right) -> bool
         {
             return left.center.x < right.center.x;
         });
    //对装甲板对应的轮廓信息进行排序
    sort(light_info.begin(), light_info.end(),
         [&](const RotatedRect left, RotatedRect right) -> bool
         {
             return left.center.x < right.center.x;
         });
    //对装甲板进行筛选
    for (int i = 0; i < lights.size(); i++)
    {
        for (int j = i + 1; j < lights.size(); j++)
        {
            //未筛选的“装甲板”
            Armor input_armor;
            //水平宽度
            input_armor.middleWidth = get_distance(light_info[i].center, light_info[j].center);
            //平均高度
            input_armor.middleHeight = (lights[i].height + lights[j].height) / 2;
            //任意两个灯条y坐标的偏差
            double distanceY = lights[i].top.y - lights[j].top.y;
            //两条对角线的长度
            double oppositeOne = get_distance(lights[i].top, lights[j].bottom);
            double oppositeTwo = get_distance(lights[j].top, lights[i].bottom);
            //对灯条进行配对
            if (abs(light_info[i].center.y - light_info[j].center.y) / ((lights[i].height + lights[j].height) / 2) > armor_param.MAX_Y_LENGTH_RATIO)
                continue;
            if (abs(light_info[i].center.x - light_info[j].center.x) / ((lights[i].height + lights[j].height) / 2) < armor_param.MIN_X_LENGTH_RATIO)
                continue;
            if (abs(light_info[i].angle - light_info[j].angle) > armor_param.MAX_ANGLE)
                continue;
            if (input_armor.middleWidth / input_armor.middleHeight > armor_param.MAX_RATIO ||
                input_armor.middleWidth / input_armor.middleHeight < armor_param.MIN_RATIO)
                continue;

            if (abs(lights[i].height - lights[j].height) > armor_param.MAX_DETLA_LENGTH)
                continue;
            if (abs(oppositeOne - oppositeTwo) / input_armor.middleWidth > armor_param.MAX_CROSS_RATIO)
                continue;

            //配对成功后分别储存这两个灯条的四个点信息
            light_info[i].points(lights[i].points);
            light_info[j].points(lights[j].points);
            //消除上下定点因为椭圆拟合而得到的误差
            lights[i].getTruePoint();
            lights[j].getTruePoint();
            //存入装甲板的左右灯条
            input_armor.left_light = lights[i];
            input_armor.right_light = lights[j];
            //得到装甲板中心点坐标
            input_armor.center = (light_info[i].center + light_info[j].center) / 2;
            //得到pnp信息
            input_armor.get_pnp();
            //对装甲板进行卡尔曼滤波初始化
            input_armor.kf_init();
            //插入装甲板进容器里
            armors.push_back(input_armor);
            //如果是第一帧
            if (Tracker.start == 0)
            {
                //将第一帧的装甲板当做上一帧的装甲板
                Tracker.last_armors.push_back(input_armor);
            }
            else if (Tracker.start == 1)
            { //将第二帧的装甲板当做当前帧的装甲板
                Tracker.now_armors.push_back(input_armor);
            }
        }
    }
    //如果是第二帧之后（可迭代更新）
    if (Tracker.start > 1)
    {
        //将当前装甲板容器清空
        Tracker.now_armors.clear();
        //插入当前帧的装甲板
        for (auto i = 0; i < armors.size(); i++)
        {
            Tracker.now_armors.push_back(armors[i]);
        }
        //通过和上一帧装甲板的关系进行更新，
        Tracker.update_armors();
    }
    Tracker.start++;
}

/**
 * @brief 更新装甲板信息
 * 
 */
void ArmorTracker::update_armors()
{
    //将上一帧装甲板和这一帧装甲板进行匹配（删除装甲板）
    for (int i = 0; i < this->last_armors.size(); i++)
    {
        //匹配成功标志位
        bool is_match = false;
        for (int j = 0; j < this->now_armors.size(); j++)
        {
            //如果距离足够小
            if (get_distance(last_armors[i].center, now_armors[j].center) < 100)
            {
                //匹配成功
                is_match = true;
                //保留初始化更新之后的装甲板的卡尔曼滤波的信息
                now_armors[j].kf = last_armors[i].kf;
                //进行迭代
                last_armors[i] = now_armors[j];
                //消失帧数清零
                last_armors[i].dis_count = 0;
            }
        }
        //如果匹配不成功
        if (is_match == false)
        {
            //消失帧数加1
            last_armors[i].dis_count++;
            now_armors.push_back(last_armors[i]);
        }
    }
    //再次遍历（产生新装甲板）
    for (int j = 0; j < this->now_armors.size(); j++)
    {
        //匹配成功标志位
        int is_match = false;

        for (int i = 0; i < this->last_armors.size(); i++)
        {
            //如果匹配成功
            if (get_distance(last_armors[i].center, now_armors[j].center) < 100)
            {
                is_match = true;
            }
        }
        //如果该帧的装甲板和之前的所有装甲板都不配对
        if (is_match == false)
        {
            //视为新装甲板插入
            last_armors.push_back(now_armors[j]);
        }
    }
    //清算
    for (int i = 0; i < last_armors.size(); i++)
    {
        //连续消失帧数过多
        if (last_armors[i].dis_count > 2)
        {
            //删除该装甲板
            last_armors.erase(last_armors.begin() + i);
        }
    }
}

/**
 * @brief 获得pnp数据
 * 
 */
void Armor::get_pnp()
{
    //存放装甲板三位坐标的容器
    vector<Point3f> m_markerCorners3d;
    m_markerCorners3d.push_back(cv::Point3f(-67.5 / 2, -26.5 / 2, 0)); //以装甲板的中心为原点按顺序插入四个角点三维坐标
    m_markerCorners3d.push_back(cv::Point3f(+67.5 / 2, -26.5 / 2, 0));
    m_markerCorners3d.push_back(cv::Point3f(+67.5 / 2, +26.5 / 2, 0));
    m_markerCorners3d.push_back(cv::Point3f(-67.5 / 2, +26.5 / 2, 0));

    //存放装甲板二维坐标的容器
    vector<Point2f> mimage;
    mimage.push_back(left_light.bottom); //插入四个二维像素坐标
    mimage.push_back(right_light.bottom);
    mimage.push_back(right_light.top);
    mimage.push_back(left_light.top);
    //相机内参矩阵
    Mat camMatrix = (Mat_<double>(3, 3) << 1.2853517927598091e+03, 0., 3.1944768628958542e+02,
                     0., 1.2792339468697937e+03, 2.3929354061292258e+02, 0., 0., 1.);

    //相机畸变矩阵
    Mat distCoeff = (Mat_<double>(5, 1) << -6.3687295852461456e-01, -1.9748008790347320e+00,
                     3.0970703651800782e-02, 2.1944646842516919e-03, 0.);
    //旋转向量
    Mat rVec = cv::Mat::zeros(3, 1, CV_64FC1);
    //平移向量
    Mat tVec = cv::Mat::zeros(3, 1, CV_64FC1);
    //最终相机的世界坐标
    Mat p_oc = cv::Mat::zeros(3, 3, CV_64FC1);
    //姿态解算
    solvePnP(m_markerCorners3d, mimage, camMatrix, distCoeff, rVec, tVec, SOLVEPNP_ITERATIVE);

    //旋转向量Rodrigue后得到的旋转矩阵
    Mat_<double> rotMat(3, 3);
    //旋转矩阵的逆矩阵
    Mat_<double> rotMat_in(3, 3);

    Rodrigues(rVec, rotMat);
    rotMat.convertTo(rotMat, CV_64FC1);
    tVec.convertTo(tVec, CV_64FC1);
    Matrix3d rotated_matrix;  // 旋转矩阵
    Vector3d transfer_vector; // 平移向量

    // 类型转换
    cv2eigen(rotMat, rotated_matrix);
    cv2eigen(tVec, transfer_vector);

    Vector3d euler_angles = rotated_matrix.eulerAngles(0, 1, 2);

    PnP_data.pitch = rad2deg(euler_angles[0]);
    PnP_data.yaw = rad2deg(euler_angles[1]);
    PnP_data.roll = rad2deg(euler_angles[2]);

    invert(rotMat, rotMat_in, DECOMP_LU); //求逆矩阵

    p_oc = -rotMat_in * tVec; //得到相机的世界坐标（以装甲板中心坐标为原点）

    //取出p_oc的三个元素（x,y,z坐标）
    double x = p_oc.at<double>(0, 0);
    double y = p_oc.at<double>(1, 0);
    double z = p_oc.at<double>(2, 0);
    //求出距离
    PnP_data.distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));

    tVec.convertTo(tVec, CV_32FC1);

    PnP_data.rotat_vec = rVec;
    PnP_data.rotat_vec = tVec;
}

/**
 * @brief 位置卡尔曼滤波初始化
 * 
 */
void Armor::kf_init()
{
    KalmanFilter new_kf(4, 2, 0);
    RNG rng;
    new_kf.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0,
                               0, 1, 0, 1,
                               0, 0, 1, 0,
                               0, 0, 0, 1);                     //转移矩阵A
    setIdentity(new_kf.measurementMatrix);                      //测量矩阵H
    setIdentity(new_kf.processNoiseCov, Scalar::all(1e-5));     //系统噪声方差矩阵Q
    setIdentity(new_kf.measurementNoiseCov, Scalar::all(1e-7)); //测量噪声方差矩阵R
    setIdentity(new_kf.errorCovPost, Scalar::all(1));
    //后验错误估计协方差矩阵P
    rng.fill(new_kf.statePost, RNG::UNIFORM, 0, 0); //初始状态值x(0)
    this->kf = new_kf;
    //初始化标志位为真
    this->kf_inited = true;
}

/**
 * @brief 位置卡尔曼滤波的更新
 * 
 */
void Armor::update_kfer()
{
    //观测量
    Mat measurement = Mat::zeros(2, 1, CV_32F);
    measurement.at<float>(0) = (float)this->center.x;
    measurement.at<float>(1) = (float)this->center.y;
    //更新进滤波器中
    this->kf.correct(measurement);
    //将这一帧的点进行预测，并保存预测点
    Mat prediction = this->kf.predict();
    //得到预测点
    this->pre_center = Point2f(prediction.at<float>(0), prediction.at<float>(1));
}

Point2f Armor::get_pre_angle()
{
    //相机内参矩阵
    Mat camMatrix = (Mat_<double>(3, 3) << 1.2853517927598091e+03, 0., 3.1944768628958542e+02,
                     0., 1.2792339468697937e+03, 2.3929354061292258e+02, 0., 0., 1.);

    //相机畸变矩阵
    Mat distCoeff = (Mat_<double>(5, 1) << -6.3687295852461456e-01, -1.9748008790347320e+00,
                     3.0970703651800782e-02, 2.1944646842516919e-03, 0.);
    return calculateRelativeAngle(camMatrix, distCoeff, pre_center);
}