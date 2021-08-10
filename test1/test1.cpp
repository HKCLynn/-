/*@file test1.cpp*/
/*author 徐英杰（541223130@qq.com）*/




#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>


using namespace std;
using namespace cv;


int main(int argc, char *argv[])
{

    VideoCapture video("/home/lynn/text1.mp4");

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
        //下边界
        Scalar scalarL = Scalar(100, 43, 46);
        //上边界
        Scalar scalarH = Scalar(125, 255, 255);
        //色彩空间转换之后的图片
        Mat imgHsv;
        cvtColor(image, imgHsv, COLOR_BGR2HSV);//色彩空间转换
        //二值化之后的图片
        Mat mask;
        inRange(imgHsv, scalarL, scalarH, mask);//二值化

        //创建核
        Mat element;
        //膨胀之后的图片
        Mat dstImageDilate;
        //腐蚀之后最终的图片
        Mat dstImage;
        element= getStructuringElement(MORPH_RECT, Size(4,4));
        dilate(mask, dstImageDilate, element);
        erode(dstImageDilate, dstImage, element);//进行闭运算
        
        findContours(dstImage, contours_all, hierarchy_all, RETR_TREE, CHAIN_APPROX_NONE, Point());//查找轮廓


        vector<vector<Point>> contours_poly(contours_all.size());//取值分割
        for (int i = 0; i < contours_all.size(); i++)
        {   //获得所有轮廓的最小外接四边形
             RotatedRect r= minAreaRect(contours_all[i]);
            if (hierarchy_all[i][0] == -1 && 
                 hierarchy_all[i][1] == -1 &&
                 abs((r.size.height/r.size.width)-(r.size.width/r.size.height))<=2)//对符合的轮廓进行筛选
                 
                {
                approxPolyDP(Mat(contours_all[i]), contours_poly[i], 5, true);//使轮廓更加直
                drawContours(frame, contours_poly, i, Scalar(10, 255, 255), 2, 8);//画出符合的轮廓
                }

        }

        imshow("windows", dstImage);//放出二值化，闭运算之后的图像
        imshow("frame", frame);//放出原图像
        waitKey(30);
    }
}
