/*@file test5.cpp*/
/*author 徐英杰（541223130@qq.com）*/

#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <string>
using namespace std;
using namespace cv;

int main()
{
    //读取分类文件
    string classesFile = "/home/lynn/test5/coco.names";
    ifstream ifs(classesFile);
    //创建分类容器，插入所有分类标签
    vector<string> classes;
    string classLine;
    while (getline(ifs, classLine))
    {
        classes.push_back(classLine);
    }

    //获得权重文件和cfg配置文件
    string modelWeights = "/home/lynn/test5/yolov3.weights";
    string modelCfg = "/home/lynn/test5/yolov3.cfg";

    //加载yolov3模型
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(modelCfg, modelWeights);

    //需要识别的图片
    Mat mat;
    //转换成可以进行识别的图
    Mat blob;

    //获得神经网络的所有层
    vector<String> layerNames = net.getLayerNames();
    //获得神经网络的所有索引
    int lastLayerId = net.getLayerId(layerNames[layerNames.size() - 1]);
    //获得最后一层
    Ptr<cv::dnn::Layer> lastLayer = net.getLayer(cv::dnn::DictValue(lastLayerId));

    //未连接的输出层
    vector<String> outPutNames;
    //得到未连接的三个输出层的索引
    vector<int> outLayers = net.getUnconnectedOutLayers();
    //将未连接的输出层插入到容器中
    for (int index = 0; index < outLayers.size(); index++)
    {
        outPutNames.push_back(layerNames[outLayers[index] - 1]);
    }

    while (1)
    {
        //获得被识别的图片
        mat = imread("/home/lynn/test5/view4.jpg");
        //若图片无法识别，则显示
        if (!mat.data)
        {
            cout << "Fail to read the image" << endl;
            return 0;
        }
        //将获得的图片转换成可以被神经网络识别的图片
        cv::dnn::blobFromImage(mat,
                               blob,
                               1.0f / 1023,
                               Size(320, 320),
                               Scalar(0, 0, 0),
                               true,
                               false);

        //设置神经网络输入
        net.setInput(blob);

        //三个层对应的输出结果
        vector<Mat> probs;

        //进行推理
        net.forward(probs, outPutNames);

        //矩形框容器
        vector<Rect>bboxes;
        //标签容器
        vector<string>labels;
        //置信度容器
        vector<float>confidences;
        
        //对得到的预测结果进行遍历
        for (int index = 0; index < probs.size(); index++)
        {
            for (int row = 0; row < probs[index].rows; row++)
            {
                //获得每个元素对应的分数并求出分数最高的的元素
                Mat scores = probs[index].row(row).colRange(5, probs[index].cols);
                Point classIdPoint;
                double confidence;
                minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > 0.6)
                {
                    //获得矩形框需要画的坐标存入矩形框容器中
                    int centerX = (int)(probs.at(index).at<float>(row, 0) * mat.cols);
                    int centerY = (int)(probs.at(index).at<float>(row, 1) * mat.rows);
                    int width = (int)(probs.at(index).at<float>(row, 2) * mat.cols);
                    int height = (int)(probs.at(index).at<float>(row, 3) * mat.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;
                    Rect objectRect(left, top, width, height);
                    bboxes.push_back(objectRect);
                    //将标签也按顺序存入容器中
                    String label = format("%s:%.4f",
                                                  classes[classIdPoint.x].data(),
                                                  confidence);
                    labels.push_back(label);
                    //将置信度存入容器中
                    confidences.push_back((float)confidence);
                }
            }

        }
        //创建非极大值抑制索引容器
        std::vector<int>indices;
        
    
        //非极大值抑制
        dnn::NMSBoxes(bboxes,confidences,0.6,0.5,indices);
        //画出置信度最高的方框
        for(int j=0;j<indices.size();j++)
        {
            rectangle(mat,bboxes[indices[j]],Scalar(255,0,0),3);
            putText(mat,labels[indices[j]],Point(bboxes[indices[j]].x,bboxes[indices[j]].y-5),FONT_HERSHEY_COMPLEX,0.4,Scalar(0,0,255));
        }

        imshow("windows", mat);
        waitKey(30);
    }
}