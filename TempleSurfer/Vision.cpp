#include "Vision.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

#include "PlayerComponent.h"

using namespace cv;

VideoCapture cap(0);
Mat img;
CascadeClassifier faceCascade;
std::vector<Rect> faces;

std::shared_ptr<PlayerComponent> playerComponent;

int frames = 0;

Vision::Vision()
{
}

Vision::Vision(std::shared_ptr<PlayerComponent> iPlayerComponent)
{
    playerComponent = iPlayerComponent;
    faceCascade.load("Resources/haarcascade_frontalface_default.xml");
    if (faceCascade.empty()) { std::cout << "XML file not loaded" << std::endl; }
}

Vision::~Vision()
{
}

void Vision::visionUpdate() {

    cap.read(img);

    if (frames % 10 == 0)
    {
        faceCascade.detectMultiScale(img, faces, 1.2, 10, 0, Size(50, 50));
    }
    
    if (faces.size() == 0)
    {
        return;
    }


#define VIS_DEBUG
#ifdef VIS_DEBUG
    //Show all detected faces
    for (int i = 0; i < faces.size(); i++)
    {
        int x = faces[i].x + (faces[i].width / 2);
        int y = faces[i].y + (faces[i].height / 2);
        line(img, Point(x, y), Point(x, y), Scalar(0, 0, 255), 50);
    }

    //Get coordinates of main face
    int xd = faces[0].x + (faces[0].width / 2);
    int yd = faces[0].y + (faces[0].height / 2);

    //Draw point on middle of face
    line(img, Point(xd, yd), Point(xd, yd), Scalar(0, 255, 0), 50);

    //Debug for face coordinates
    std::cout << "X:" << xd << " Y:" << yd << std::endl;
#endif // VIS_DEBUG

    imshow("Image", img);
    waitKey(1);
    frames++;

    if (faces.size() > 1)
    {
        return;
    }

    int x = faces[0].x + (faces[0].width / 2);
    int y = faces[0].y + (faces[0].height / 2);

    if (xPos != RIGHT && x < 225)
    {
        playerComponent->moveRight();
        xPos = RIGHT;
    }
    else if (xPos != LEFT && x > 415)
    {
        playerComponent->moveLeft();
        xPos = LEFT;
    }
    else if (xPos != CENTER && x > 225 && x < 415)
    {
        playerComponent->moveCenter();
        xPos = CENTER;
    }

    if (yPos != DUCK && y < 175)
    {
        playerComponent->crouch();
        yPos = DUCK;
    }
    else if (yPos != JUMP && y > 290)
    {
        playerComponent->jump();
        yPos = JUMP;
    }
    else if (yPos != STAND && y > 175 && y < 290)
    {
        yPos = STAND;
    }

}

