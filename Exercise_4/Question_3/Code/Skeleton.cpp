#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/opencv.hpp"
#include <iostream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/core/core.hpp>
#include <string>
#include <stdio.h>
#include <opencv/highgui.h>
#include <opencv2/video/background_segm.hpp>
using namespace std;

using namespace cv;


void help()
{
 cout << "\nThis program demonstrates a skeletal transform.\n"
         "Usage:\n"
         "./skeletal <image_name>, Default is pic1.jpg\n" << endl;
}

int main(int argc, char** argv)
{


 Mat gray, binary, mfblur;


 cvNamedWindow("Skeleton", CV_WINDOW_AUTOSIZE);
 CvCapture* capture = cvCreateCameraCapture(1);
 IplImage* frame;
 int i;

for(i=1;i<=3000;i++)
{
        frame=cvQueryFrame(capture);
        if(!frame) break;
        Mat img = cvarrToMat(frame); 

        cvtColor(img, gray, CV_BGR2GRAY);
        imshow("detected lines", img);
        // To improve, compute a histogram here and set threshold to first peak
 //
 // For now, histogram analysis was done with GIMP
 //
 threshold(gray, binary, 70, 255, CV_THRESH_BINARY);
 binary = 255 - binary;

 

 // To remove median filter, just replace blurr value with 1
 medianBlur(binary, mfblur, 1);

 
 // This section of code was adapted from the following post, which was
 // based in turn on the Wikipedia description of a morphological skeleton
 //
 // http://felix.abecassis.me/2011/09/opencv-morphological-skeleton/
 //
 Mat skel(mfblur.size(), CV_8UC1, Scalar(0));
 Mat temp;
 Mat eroded;
 Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3));
 bool done;
 int iterations=0;
 

 do
 {
   erode(mfblur, eroded, element);
   dilate(eroded, temp, element);
   subtract(mfblur, temp, temp);
   bitwise_or(skel, temp, skel);
   eroded.copyTo(mfblur);

   done = (countNonZero(mfblur) == 0);
   iterations++;
 
 } while (!done && (iterations < 100));
 imshow("detected lines", skel);
 char filename[100];
 sprintf(filename, "../Screenshots/Skeleton%0d.jpg", i);
 imwrite( filename , skel );
        
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("Capture Example");
    

 



 



 
 
 
 
 return 0;
}

