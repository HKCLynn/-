/**
*@file Light.cpp
*@author 徐英杰（541223130@qq.com）
*/
#include "Light.h"


using namespace std;
using namespace cv;

/**
 * @brief 构造函数初始化
 * 
 * @param input_led 传入的灯条的四个点
 * @param contours 传入灯条的轮廓
 * @param center 传入灯条的中心点
 */

Light::Light(Point2f input_led[], vector<Point> contours, Point2f center)
{
    //中间传输的灯条四个点信息
    for (int i = 0; i < 4; i++)
    {
        this->points[i] = input_led[i];
    }
    //排序
    sort(begin(this->points), end(this->points), [](Point2f &a, Point2f &b) -> bool
         { return a.y < b.y; });
    //灯条的上顶点
    top = (this->points[0] + this->points[1]) / 2;
    //灯条的下顶点
    bottom = (this->points[2] + this->points[3]) / 2;
    //灯条的高
    height = sqrt(pow(top.x - bottom.x, 2) + pow(top.y - bottom.y, 2));
    this->contours = contours;
    this->center = center;
}
/**
 * @brief 消除椭圆拟合的误差
 * 
 */
void Light::getTruePoint()
{
    //如果是近处的装甲板
    if (contourArea(this->contours) > 30)
    {
        float min_y = this->center.y;
        float max_y = this->center.y;
        Point2f new_bottom = Point(-1, -1);
        Point2f new_top = Point(-1, -1);
        float min_distance_top = MAXFLOAT;
        float min_distance_botoom = MAXFLOAT;
        //对轮廓的每一个点进行遍历
        for (auto point : contours)
        {
            //如果是上半部分的点
            if (point.y < min_y)
            {
                // 找出离原来上顶点最近的点作新上顶点
                float tmp_distance = get_distance(point, this->top);
                if (tmp_distance < min_distance_top)
                {
                    min_distance_top = tmp_distance;
                    new_top = point;
                }
            }
            //如果是下半部分的点
            if (point.y > max_y)
            {
                // 找出离原来下顶点最近的点作新下顶点
                float tmp_distance = get_distance(point, this->bottom);
                if (tmp_distance < min_distance_botoom)
                {
                    min_distance_botoom = tmp_distance;
                    new_bottom = point;
                }
            }
        }
        //上顶点的坐标和下顶点的坐标都改变了
        if (new_bottom != Point2f(-1, -1) || new_top != Point2f(-1, -1))
        {
            //上顶点的偏移量
            Point2f dta_top_location=new_top-this->top;
            //下顶点的偏移量
            Point2f dta_bottom_location=new_bottom-this->bottom;
            //替换上顶点
            this->top = new_top;
            //替换下顶点
            this->bottom = new_bottom;
            //替换新的中间坐标
            this->center = (new_top + new_bottom) / 2.f;
            //替换新的高
            this->height = get_distance(this->top, this->bottom);
            //同时将四个顶点的坐标也做替换（通过偏移量）
            for(int i=0;i<4;i++)
            {
                if(this->points[i].y<this->center.y)
                this->points[i]=this->points[i]+dta_top_location;

                if(this->points[i].y>this->center.y)
                this->points[i]=this->points[i]+dta_bottom_location;
            }
        }
    }
}
