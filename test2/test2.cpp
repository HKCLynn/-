

/*@file test2.cpp*/
/*author 徐英杰（541223130@qq.com）*/


#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;


//画出某个灯条的上下两个点
void write_circle(Point2f led[], Mat frame)
{
    for (int i = 0; i <= 3; i++)
    {
        for (int j = 0; j <= 3; j++)
        {
            if (pow((led[i].x - led[j].x), 2) + pow((led[i].y - led[j].y), 2) < 690 && i != j)
                circle(frame, (led[i] + led[j]) / 2, 5, Scalar(153, 43, 46), -1);//筛选画点
        }
    }
}

int main(int argc, char *argv[])
{

    VideoCapture video("/home/lynn/text2.avi");

    while (1)
    {
        //摄像头每一帧的图片
        Mat frame;
        video >> frame;//传入摄像头每一帧图片
        
        //高斯滤波之后的图片
        Mat image;

        GaussianBlur(frame, image, Size(5, 5), 0); //高斯滤波

        //所有轮廓
        vector<vector<Point>> contours_all;
        //轮廓信息
        vector<Vec4i> hierarchy_all;

        //色彩空间转换之后的图片
        Mat imgHsv;
        cvtColor(image, imgHsv, COLOR_RGB2GRAY);//色彩空间变换
        
        //阈值化之后的图片
        Mat mask;
        threshold(imgHsv, mask, 100, 255, THRESH_BINARY); //阈值化

        findContours(mask, contours_all, hierarchy_all, RETR_TREE, CHAIN_APPROX_NONE, Point());//查找轮廓

        for (int i = 0; i < contours_all.size(); i++)
        {
            //取每一个轮廓的最小外接四边形
            RotatedRect one = minAreaRect(contours_all[i]);
            for (int j = 0; j < contours_all.size(); j++)
            {
                //取每一个轮廓的最小外接四边形
                RotatedRect two = minAreaRect(contours_all[j]);
                if (abs((one.size.height / one.size.width) - (one.size.width / one.size.height)) >= 3.5 &&
                    abs((two.size.height / two.size.width) - (two.size.width / two.size.height)) >= 3.5 &&
                    contourArea(contours_all[i])>200&&contourArea(contours_all[j])>200&&
                    i != j)//对轮廓进行筛选，同时使之前两个轮廓分开，不是同一个轮廓
                {
                    //其中一个灯条的轮廓
                    RotatedRect boundRectOne = one;
                    //另一个灯条的轮廓
                    RotatedRect boundRectTwo = two;
                    //其中灯条的四个点（不分左右）
                    Point2f left[4];
                    //另一个灯条的四个点（不分左右）
                    Point2f right[4];

                    boundRectOne.points(left);
                    boundRectTwo.points(right);

                    circle(frame, (boundRectOne.center + boundRectTwo.center) / 2, 5, Scalar(153, 43, 46), -1);//画出装甲板的中心点
                    
                    write_circle(left, frame);//画出装甲板一边的两个角点
                    
                    write_circle(right, frame);//画出装甲板另一边的两个角点

                }
            }
        }
        imshow("windows", mask);//放出阈值化之后的图像
        imshow("frame", frame);//放出原图像
        waitKey(30);
    }
}