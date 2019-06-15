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
 CvCapture* capture = cvCreateCameraCapture(0);
 IplImage* frame;
 int r;

for(r=1;r<=3000;r++)
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
  int i;
  i = 1;
  int j;
  int k;
           int o =0;
 
 while(i < binary.rows){
              j=1;
           while(j < binary.cols){
           int a,b;
           int pix[20]; 
           int *pix1; 
           pix1 = pix;      
           a =  binary.at<uchar>(i-1,j)+binary.at<uchar>(i+1,j)+binary.at<uchar>(i-1,j+1)+binary.at<uchar>(i-1,j+1)+binary.at<uchar>(i+1,j+1)+ binary.at<uchar>(i-1,j-1)+binary.at<uchar>(i+1,j-1)  +binary.at<uchar>(i,j-1)+binary.at<uchar>(i,j+1);

          
           *pix1 = binary.at<uchar>(i-1,j-1);
           pix1++;
           *pix1 = binary.at<uchar>(i-1,j);
           pix1++;
           *pix1 = binary.at<uchar>(i-1,j+1);
           pix1++;
           *pix1 = binary.at<uchar>(i,j-1);
           pix1++;
           *pix1 = binary.at<uchar>(i,j);
           pix1++;
           *pix1 = binary.at<uchar>(i,j+1);
           pix1++;
           *pix1 = binary.at<uchar>(i+1,j-1);
           pix1++;
           *pix1 = binary.at<uchar>(i+1,j);
           pix1++;
            *pix1 = binary.at<uchar>(i+1,j+1);
           pix1 = pix1 - 8;
          
           for ( k=0;k<=8;k++)
           {
           int l;
           l = pix[k+1]-pix[k];
           if(l == 255)
           {
            o++;
           } 
           }
           if((binary.at<uchar>(i,j)==255)&&(510<a<1530)&&(o==1)&&((pix[6])*(pix[2])*(pix[4])*(pix[8])==0))
           {
            binary.at<uchar>(i,j) = 0;
           }
       
          
                               
                   j = j+1; 
                   
                   }
           printf("%d",o);
             i++; 
             }   

while(i < binary.rows){
              j=1;
           while(j < binary.cols){
           int a,b;
           int pix[20]; 
           int *pix1; 
           pix1 = pix;      
           a =  binary.at<uchar>(i-1,j)+binary.at<uchar>(i+1,j)+binary.at<uchar>(i-1,j+1)+binary.at<uchar>(i-1,j+1)+binary.at<uchar>(i+1,j+1)+ binary.at<uchar>(i-1,j-1)+binary.at<uchar>(i+1,j-1)  +binary.at<uchar>(i,j-1)+binary.at<uchar>(i,j+1);

          
           *pix1 = binary.at<uchar>(i-1,j-1);
           pix1++;
           *pix1 = binary.at<uchar>(i-1,j);
           pix1++;
           *pix1 = binary.at<uchar>(i-1,j+1);
           pix1++;
           *pix1 = binary.at<uchar>(i,j-1);
           pix1++;
           *pix1 = binary.at<uchar>(i,j);
           pix1++;
           *pix1 = binary.at<uchar>(i,j+1);
           pix1++;
           *pix1 = binary.at<uchar>(i+1,j-1);
           pix1++;
           *pix1 = binary.at<uchar>(i+1,j);
           pix1++;
            *pix1 = binary.at<uchar>(i+1,j+1);
           pix1 = pix1 - 8;
          
           for ( k=0;k<=8;k++)
           {
           int l;
           l = pix[k+1]-pix[k];
           if(l == 255)
           {
            o++;
           } 
           }
           if((binary.at<uchar>(i,j)==255)&&(510<a<1530)&&(o==1)&&((pix[6])*(pix[2])*(pix[4])*(pix[8])==0))
           {
            binary.at<uchar>(i,j) = 0;
           }
       
          
                               
                   j = j+1; 
                   
                   }
           printf("%d",o);
             i++; 
             }   


 imshow("detected lines", binary);
 char filename[100];
 sprintf(filename, "../Screenshots/Skeleton%0d.jpg", i);
 imwrite( filename , binary );
        
    }

    cvReleaseCapture(&capture);
    cvDestroyWindow("Capture Example");
    

 



 



 
 
 
 
 return 0;
}


