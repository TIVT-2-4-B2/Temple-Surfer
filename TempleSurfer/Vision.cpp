#include "Vision.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include<windows.h>


using namespace cv;

VideoCapture cap(0);
Mat img;
CascadeClassifier faceCascade;
std::vector<Rect> faces;

Vision::Vision()
{
    faceCascade.load("Resources/haarcascade_frontalface_default.xml");
    if (faceCascade.empty()) { std::cout << "XML file not loaded" << std::endl; }
}

Vision::~Vision()
{
}

void Vision::visionUpdate() {

    cap.read(img);
    faceCascade.detectMultiScale(img, faces, 1.2, 10, 0, Size(50, 50));
    for (int i = 0; i < faces.size(); i++)
    {
        int x = faces[i].x + (faces[i].width / 2);
        int y = faces[i].y + (faces[i].height / 2) + 10;

        line(img, Point(x, y), Point(x, y), Scalar(0, 0, 255), 50);
    }
    imshow("Image", img);
    waitKey(1);
}

