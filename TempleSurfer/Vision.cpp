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

    //while (true)
    //{
        cap.read(img);
        faceCascade.detectMultiScale(img, faces, 1.1, 10);
        for (int i = 0; i < faces.size(); i++)
        {
            rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 0), 3);
        }
        imshow("Image", img);
        waitKey(1);
    //}

}

