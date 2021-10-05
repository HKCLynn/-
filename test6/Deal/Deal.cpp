/**
*@file Deal.cpp
*@author 徐英杰（541223130@qq.com）
*/

#include "Deal.h"


using namespace std;
using namespace cv;

/**
*@brief 获得距离
*@param point_one 其中一个点
*@param point_two 另一个点
*@return double 这两个点的距离
*/
double get_distance(Point2f point_one, Point2f point_two)
{
    double distance;
    //得到两点距离
    distance = sqrt(pow(point_one.x - point_two.x, 2) + pow(point_one.y - point_two.y, 2));
    return distance;
}

/**
*@brief 图像处理
*@param src 输入图像
*@return Mat 处理后的图像
*/

Mat imgPreprosses(const Mat src)
{
    //分离得到的通道
    vector<Mat> channels;
    //图像分离通道
    split(src, channels);
    //通道相减后得到的图像
    Mat image;
    if (channels.size() == 3)
        image = channels[0] - channels[2];
    else
        image = src;
    //最后处理得到的图像
    Mat mask;
    threshold(image, mask, 100, 255, THRESH_BINARY); //阈值化
    blur(mask, mask, Size(3, 3));
    medianBlur(mask, mask, 3);
    //创建核
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    //闭运算
    morphologyEx(mask, mask, MORPH_DILATE, kernel, Point(-1, -1), 1);
    return mask;
}
/**
 * @brief 角度转换为弧度
 * 
 * @param deg 角度
 * @return float 弧度
 */
float deg2rad(float deg)
{
    return deg * 3.1415926 / 180.f;
}

/**
 * @brief 弧度转化为角度
 * 
 * @param rad 弧度
 * @return float 角度
 */
float rad2deg(float rad)
{
    return rad * 180.f / 3.1415926;
}

/**
 * @brief 用来获得相机中心相对于装甲板中心的角度
 * 
 * @param cameraMatrix 相机内参
 * @param distCoeff 畸变参数
 * @param center 图像中装甲板中心
 * @return x, y 方向夹角 -- 目标在图像右方，point.x 为正，目标在图像下方，point.y 为正
 */
Point2f calculateRelativeAngle(const Mat &cameraMatrix, const Mat &distCoeff, Point2f center)
{
    Mat tf_point(3, 1, CV_32F);
    Mat cameraMatrix_inverse;
    cameraMatrix.convertTo(cameraMatrix_inverse, CV_32F);
    cameraMatrix_inverse = cameraMatrix_inverse.inv();
    tf_point.at<float>(0) = center.x;
    tf_point.at<float>(1) = center.y;
    tf_point.at<float>(2) = 1;
    // 得到tan角矩阵
    Mat tf_result = cameraMatrix_inverse * tf_point;
    // 从图像坐标系转换成世界坐标系角度
    return Point2f(rad2deg(atan(tf_result.at<float>(0))),
                   rad2deg(atan(tf_result.at<float>(1))));
}


