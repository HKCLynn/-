/*@file head.cpp*/
/*author 徐英杰（541223130@qq.com）*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include "head.hpp"

using namespace std;
using namespace cv;


/*@确定左右灯条*/
/*@画出某一灯条的上下两个点*/
/*@确定某一灯条的上下两个点*/
/*@测距并且输出距离*/



void left_right(Point2f left[], Point2f right[])
{
    if (right[0].x < left[0].x)
    {
        Point2f temp[4];
        for (int i = 0; i < 4; i++)
        {
            temp[i] = right[i];
        }
        for (int i = 0; i < 4; i++)
        {
            right[i] = left[i];
        }
        for (int i = 0; i < 4; i++)
        {
            left[i] = temp[i];
        }
    }
}

void write_circle(Point2f led[], Mat frame)
{
    for (int i = 0; i <= 3; i++)
    {
        for (int j = 0; j <= 3; j++)
        {
            if (pow((led[i].x - led[j].x), 2) + pow((led[i].y - led[j].y), 2) < 690 && i != j)//取出外接矩形符合条件的两个角点画出其中点作为灯条上点
                circle(frame, (led[i] + led[j]) / 2, 5, Scalar(153, 43, 46), -1);
        }
    }
}



void top_bottom(Point2f &top, Point2f &bottom, Point2f *input_led)
{
    Point2f led[4];
    for (int i = 0; i < 4; i++)
    {
        led[i] =input_led[i];
    }
    sort(begin(led1), end(led1), [](Point2f &a, Point2f &b) -> bool
         { return a.y < b.y; });//进行排序，前两个一定是灯条上方的点，后两个一定是灯条下方的点
    top = (led[0] + led[1]) / 2;
    bottom = (le1[2] + led[3]) / 2;
}

void range_finding(float length, float weight, Point2f bottom_left, Point2f bottom_right, Point2f top_right, Point2f top_left)
{   
    //存放装甲板三位坐标的容器
    vector<Point3f> m_markerCorners3d;
    m_markerCorners3d.push_back(cv::Point3f(-length / 2, -weight / 2, 0));//以装甲板的中心为原点按顺序插入四个角点三维坐标
    m_markerCorners3d.push_back(cv::Point3f(+length / 2, -weight / 2, 0));
    m_markerCorners3d.push_back(cv::Point3f(+length / 2, +weight / 2, 0));
    m_markerCorners3d.push_back(cv::Point3f(-length / 2, +weight / 2, 0));

    //存放装甲板二维坐标的容器
    vector<Point2f> mimage;
    mimage.push_back(bottom_left);//插入四个二维像素坐标
    mimage.push_back(bottom_right);
    mimage.push_back(top_right);
    mimage.push_back(top_left);
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

    invert(rotMat, rotMat_in, DECOMP_LU);//求逆矩阵

    p_oc = -rotMat_in * tVec;//得到相机的世界坐标（以装甲板中心坐标为原点）

    //取出p_oc的三个元素（x,y,z坐标）
    double first = p_oc.at<double>(0, 0);
    double second = p_oc.at<double>(1, 0);
    double third = p_oc.at<double>(2, 0);
    //求出距离
    cout << "distance=" << pow(pow(first, 2) + pow(second, 2) + pow(third, 2), 0.5) << endl;
}
