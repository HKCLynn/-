#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

Mat imgPreprosses(const Mat src);
double get_distance(Point2f point_one, Point2f point_two);
void write_rec(Point2f point[], Mat &frame);