#include "Vision.h"
#include "PlayerComponent.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <thread>

using namespace cv;

// Debug define for show debug window
//#define VIS_DEBUG

// Variables needed
VideoCapture cap(0);            // Camera id
CascadeClassifier faceCascade;  // Cascade used for regonizing
std::vector<Rect> faces;        // The locations of faces

Mat camImage;

std::shared_ptr<PlayerComponent> playerComponent;

int frames = 0;

// Settings
const int cameraWidth = 650;
const int cameraHeight = 500;
const int xOffset = 25;
const int yOffset = 70;

// Boundries
const int upperXThreshold = (cameraWidth - 2 * xOffset) / 3 * 2 + xOffset;
const int lowerXThreshold = (cameraWidth - 2 * xOffset) / 3 + xOffset;
const int upperYThreshold = (cameraHeight - 2 * yOffset) / 3 * 2 + yOffset;
const int lowerYThreshold = (cameraHeight - 2 * yOffset) / 3 + yOffset;

// Constructors and destructors
Vision::Vision(std::shared_ptr<PlayerComponent> iPlayerComponent)
{
    // Initing cascade and setting components
    playerComponent = iPlayerComponent;
    faceCascade.load("Resources/haarcascade_frontalface_default.xml");
    if (faceCascade.empty()) { std::cout << "XML file not loaded" << std::endl; }

    // Booting up regonision thread
    std::thread(visionRoutine);
}

Vision::~Vision()
{
}

// Usefull methods.
void Vision::checkForFaces()
{
    // Does the image reginiosion.
    if (frames % 10 == 0)
    {
        const double scale = 1.2;
        const int minNeighbors = 10;

        cap.read(camImage);
        faceCascade.detectMultiScale(camImage, faces, scale, minNeighbors, 0, Size(50, 50));
    }
    frames++;
}

void Vision::debugWindow()
{
    //Show all detected faces
    for (int i = 0; i < faces.size(); i++)
    {
        int x = faces[i].x + (faces[i].width / 2);
        int y = faces[i].y + (faces[i].height / 2);
        line(camImage, Point(x, y), Point(x, y), Scalar(0, 0, 255), 50);
    }

    //Get coordinates of main face
    int xd = faces[0].x + (faces[0].width / 2);
    int yd = faces[0].y + (faces[0].height / 2);

    //Draw point on middle of face
    Mat img = camImage;
    line(img, Point(xd, yd), Point(xd, yd), Scalar(0, 255, 0), 50);

    //Debug for face coordinates
    //std::cout << "X:" << xd << " Y:" << yd << std::endl;

    //Drawing boundary lines
    //Horizontal
    line(img, Point(0, lowerYThreshold), Point(cameraWidth, lowerYThreshold), Scalar(0, 0, 255), 5);
    line(img, Point(0, upperYThreshold), Point(cameraWidth, upperYThreshold), Scalar(0, 0, 255), 5);

    //Vertical
    line(img, Point(lowerXThreshold, 0), Point(lowerXThreshold, cameraHeight), Scalar(0, 0, 255), 5);
    line(img, Point(upperXThreshold, 0), Point(upperXThreshold, cameraHeight), Scalar(0, 0, 255), 5);

    imshow("Debug", img);
    waitKey(1);
}

void Vision::checkResult() {
    if (faces.size() > 1)
    {
        return;
    }

    int x = faces[0].x + (faces[0].width / 2);
    int y = faces[0].y + (faces[0].height / 2);

    if (xPos != xPosition::RIGHT && x < lowerXThreshold)
    {
        playerComponent->moveRight();
        xPos = RIGHT;
    }
    else if (xPos != LEFT && x > upperXThreshold)
    {
        playerComponent->moveLeft();
        xPos = LEFT;
    }
    else if (xPos != CENTER && x > lowerXThreshold && x < upperXThreshold)
    {
        playerComponent->moveCenter();
        xPos = CENTER;
    }

    if (yPos != JUMP && y < lowerYThreshold)
    {
        playerComponent->jump();
        yPos = JUMP;
    }
    else if (yPos != DUCK && y > upperYThreshold)
    {
        playerComponent->crouch();
        yPos = DUCK;
    }
    else if (yPos != STAND && y > lowerYThreshold && y < upperYThreshold)
    {
        yPos = STAND;
    }
}

cv::Mat Vision::getImage() {
    return camImage;
}



void Vision::visionRoutine()
{
    while (true) {
        checkForFaces();

        // Debug window if needed
        #ifdef VIS_DEBUG
        debugWindow();
        #endif 

        checkResult();
    }
}

void Vision::visionUpdate() {

  

}

