#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "DealWith.h"

using namespace std;
using namespace cv;

Mat imgPreprosses(const Mat src)
{
    //高斯滤波之后的图片
    Mat image;
    GaussianBlur(src, image, Size(5, 5), 0); //高斯滤波

    //下边界
    Scalar scalarL = Scalar(100, 43, 46);
    //上边界
    Scalar scalarH = Scalar(125, 255, 255);
    //色彩空间转换之后的图片
    Mat imgHsv;
    cvtColor(image, imgHsv, COLOR_BGR2HSV); //色彩空间转换
    //二值化之后的图片
    Mat mask;
    inRange(imgHsv, scalarL, scalarH, mask); //二值化

    //创建核
    Mat element;
    //膨胀之后的图片
    Mat dstImageDilate;
    //腐蚀之后最终的图片
    Mat dstImage;
    element = getStructuringElement(MORPH_RECT, Size(4, 4));
    dilate(mask, dstImageDilate, element);
    erode(dstImageDilate, dstImage, element); //进行闭运算
    return dstImage;
}

double get_distance(Point2f point_one, Point2f point_two)
{
    double distance;
    distance = sqrt(pow(point_one.x - point_two.x, 2) + pow(point_one.y - point_two.y, 2));
    return distance;
}

void write_rec(Point2f point[], Mat &frame)
{
    for (int j = 0; j <= 3; j++)
    {
        line(frame, point[j], point[(j + 1) % 4], Scalar(0, 0, 255), 2);
    }
}