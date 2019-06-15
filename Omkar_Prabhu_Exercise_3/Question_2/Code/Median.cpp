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
/* functions to find max min values for filter*/
int min(int arr[], int size){
    int i=0;
     int min=arr[0];
    for (; i< size; i++){
         if ( arr[i]<min) min = arr[i];
    }   
    return min;
}
int max(int arr[], int size){
    int i=0;
    int max=arr[0];
    for (; i< size; i++){
         if ( arr[i]>max) max = arr[i];
    }
    return max;
}
int main(){





	Mat frame1, imagep,bwimg;
        system("ffmpeg -i Dark-Room-Laser-Spot.mpeg -r 0.25 thumb_%04d.png");
	VideoCapture cap("Dark-Room-Laser-Spot.mpeg"); 
        cap >> imagep;
        Mat img(Size(1920, 1280), CV_8UC3);
        int hist[8]= {0};
        int minPixel,maxPixel;
        int i=0,j,l,m,sum=0,b,g,r;
        unsigned char *indata,*outdata;
        char k;
        indata = (unsigned char*)(imagep.data);
        outdata = (unsigned char*)(img.data);
        indata = indata+1;
        outdata = outdata+1;
        sum = 0;
        i =0;
        while(1)
        { 
              /* reading the g band pixels in Mat object using char pointer and storing*/
              while(i < imagep.rows){
              j=1;
                       while(j < imagep.cols){
                       g =   *indata;
                       *outdata = g;                       
                       j = j+1; 
                       indata = indata+3;
                       outdata = outdata+3;
                       }
              i++; 
              }    
              cvtColor(img, bwimg,COLOR_RGB2GRAY); //change g channel to grayscale 
              frame1 = bwimg;
              /*applying filter using pixel min max used neighborhood pixel algorithm*/
      	      while(i < bwimg.rows){
              j=1;
                    while(j < bwimg.cols){
                    hist[0] = bwimg.at<uchar>(i-1,j);
                    hist[1] = bwimg.at<uchar>(i+1,j);
                    hist[2] = bwimg.at<uchar>(i-1,j+1);
                    hist[3] = bwimg.at<uchar>(i+1,j+1);
                    hist[4] = bwimg.at<uchar>(i-1,j-1);
                    hist[5] = bwimg.at<uchar>(i+1,j-1);
                    hist[6] = bwimg.at<uchar>(i,j-1);
                    hist[7] = bwimg.at<uchar>(i,j+1);
                    minPixel = min(hist,8);
                    maxPixel = max(hist,8);
                 	   if(bwimg.at<uchar>(i,j) < minPixel ){
                           frame1.at<uchar>(i,j) = minPixel;
                           }
                           	else if(bwimg.at<uchar>(i,j) > maxPixel) {
                                frame1.at<uchar>(i,j) = maxPixel;
                                }                        
                   j = j+1; 
                   
                   }
             i++; 
             }    
          
	printf("%d\n %d\n", imagep.cols,imagep.rows); 
        imshow("G_Band",img);       
        imshow("Gray-Scale",bwimg);  
	imshow("Median-Filter",frame1);  
	k=waitKey(2);
	if(k==27)
	break;
        }
}



