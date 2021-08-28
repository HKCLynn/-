#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "DealWith.h"

using namespace std;
using namespace cv;

class Identifier
{
public:
    Mat picture;
    Mat frame;
    Point2f cen[4];
    Point2f rec[4];
    RotatedRect rec_rec;
    RotatedRect cen_rec;
    //所有轮廓
    vector<vector<Point>> contours_all;
    //轮廓信息
    vector<Vec4i> hierarchy_all;
    void identify();
    Identifier(Mat picture,Mat frame);
};
double get_distance(Point2f point_one, Point2f point_two);
void write_rec(Point2f point[], Mat &frame);
