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
const int overFlowLimit = 100;

// Boundries
const int upperXThreshold = (cameraWidth - 2 * xOffset) / 3 * 2 + xOffset;
const int lowerXThreshold = (cameraWidth - 2 * xOffset) / 3 + xOffset;
const int upperYThreshold = (cameraHeight - 2 * yOffset) / 3 * 2 + yOffset;
const int lowerYThreshold = (cameraHeight - 2 * yOffset) / 3 + yOffset;

Vision::Vision(std::shared_ptr<PlayerComponent> iPlayerComponent)
{
    // Initing cascade and setting components
    playerComponent = iPlayerComponent;
    faceCascade.load("Resources/haarcascade_frontalface_default.xml");
    if (faceCascade.empty()) { std::cout << "XML file not loaded" << std::endl; }

    std::cout << "Started vision regon" << std::endl;
    // Booting up regonision thread
    running = true;
    cvThread = std::thread(&Vision::visionRoutine, this);
    cvThread.detach();
}

Vision::~Vision()
{
    running = false;
    if (cvThread.joinable())
    cvThread.join();
}

// Usefull methods.
void Vision::checkForFaces()
{
    // Does the image reginiosion.
    const double scale = 1.2;
    const int minNeighbors = 10;

    cap.read(camImage);
    faceCascade.detectMultiScale(camImage, faces, scale, minNeighbors, 0, Size(50, 50));

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
    // Returning when not needed
    if (faces.size() > 1 || faces.size() == 0) return;
    

    int x = faces[0].x + (faces[0].width / 2);
    int y = faces[0].y + (faces[0].height / 2);

    lockInputQueues.lock();

    // Checking if there is now input overflow
    if (xInputQueue.size() < overFlowLimit) {
        if (x < lowerXThreshold)  xInputQueue.emplace(xPosition::RIGHT);
        else if (x > upperXThreshold)  xInputQueue.emplace(xPosition::LEFT);
        else if (x > lowerXThreshold && x < upperXThreshold) xInputQueue.emplace(xPosition::CENTER);
    }
    // Checking if there is now input overflow
    if (yInputQueue.size() < overFlowLimit) {
        if (y < lowerYThreshold) yInputQueue.emplace(yPosition::JUMP);
        else if (y > upperYThreshold) yInputQueue.emplace(yPosition::DUCK);
        else if (y > lowerYThreshold && y < upperYThreshold)  yInputQueue.emplace(yPosition::STAND);
    }

    lockInputQueues.unlock();
}

void Vision::setNewPlayer(std::shared_ptr<PlayerComponent> iPlayerComponent){
    playerComponent = iPlayerComponent;
}

cv::Mat Vision::getImage() {
    // Returns the image given.
    return camImage;
}



void Vision::visionRoutine()
{
    std::cout << "Started vision regon" << std::endl;
    while (running) {
        checkForFaces();

        // Debug window if needed
        #ifdef VIS_DEBUG
        debugWindow();
        #endif 

        checkResult();

        // Freeing time for other threads
        std::this_thread::sleep_for(std::chrono::microseconds(500));
    }
}

void Vision::visionUpdate() {
    lockInputQueues.lock();

    // Checking x-axis inputs
    if (!xInputQueue.empty()) {
        if (xInputQueue.front() == xPosition::RIGHT && xPos != xPosition::RIGHT) { playerComponent->moveLeft();xPos = xPosition::RIGHT;}
        else if (xInputQueue.front() == xPosition::LEFT && xPos != xPosition::LEFT) { playerComponent->moveRight(); xPos = xPosition::LEFT;}
        else if (xInputQueue.front() == xPosition::CENTER && xPos != xPosition::CENTER) { playerComponent->moveCenter(); xPos = xPosition::CENTER; }
        xInputQueue.pop();
    }

    // Checking y-axis inputs
    if (!yInputQueue.empty()) {
        if (yInputQueue.front() == yPosition::DUCK && yPos != yPosition::DUCK) { playerComponent->crouch(); yPos = yPosition::DUCK; }
        else if (yInputQueue.front() == yPosition::JUMP && yPos != yPosition::JUMP) { playerComponent->jump(); yPos = yPosition::JUMP; }
        else if (yInputQueue.front() == yPosition::STAND && yPos != yPosition::STAND) { yPos = yPosition::STAND; }
        yInputQueue.pop();
    }
    
    lockInputQueues.unlock();
}

