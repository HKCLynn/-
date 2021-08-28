#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "Identifier.h"


using namespace std;
using namespace cv;

Identifier::Identifier(Mat picture, Mat frame)
{
    this->picture = picture;
    this->frame = frame;
}

void Identifier::identify()
{
    findContours(picture, contours_all, hierarchy_all, RETR_TREE, CHAIN_APPROX_NONE, Point()); //查找轮廓
    for (int i = 0; i < contours_all.size(); i++)
    { //获得所有轮廓的最小外接四边形
        RotatedRect r = minAreaRect(contours_all[i]);
        if (contourArea(contours_all[i]) > 20 &&
            hierarchy_all[i][0] == -1 &&
            hierarchy_all[i][1] == -1 &&
            abs((r.size.height / r.size.width) - (r.size.width / r.size.height)) <= 2 &&
            abs((r.size.height / r.size.width) - (r.size.width / r.size.height)) >= 1 &&
            contourArea(contours_all[i]) / r.size.area() > 0.7) //对符合的轮廓进行筛选

        {
            rec_rec = minAreaRect(contours_all[i]);
            rec_rec.points(rec);
            circle(frame, r.center, 2, Scalar(0, 0, 255), -1);
            write_rec(rec, frame);
        }
        if (contourArea(contours_all[i]) > 10 &&
            abs((r.size.height / r.size.width) - (r.size.width / r.size.height)) <= 0.5 &&
            contourArea(contours_all[i]) / r.size.area() > 0.7 &&
            hierarchy_all[i][2] == -1 &&
            hierarchy_all[i][3] == -1)
        {
            cen_rec = minAreaRect(contours_all[i]);
            cen_rec.points(cen);
            write_rec(cen, frame);
        }
    }
}

