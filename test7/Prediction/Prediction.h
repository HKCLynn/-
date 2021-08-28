#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;

class Prediction
{
public:
    KalmanFilter kf;
    Point aft_point;
    Point center;
    Mat measurement= Mat::zeros(1, 1, CV_32F);

    float r;
    float angle;
    Prediction(KalmanFilter &kf);
    void kf_initialization(KalmanFilter &kf);
    void kf_prediction(Point2f &rec_center, Point2f &cen_center, Mat &frame);
};
double get_distance(Point2f point_one, Point2f point_two);
void write_rec(Point2f point[], Mat &frame);
