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
/*function to separate bands*/
void decode(Mat src, Mat r, Mat g, Mat b){
        int j,i;
        unsigned char *indata,*outdata1,*outdata2,*outdata3;
	indata = (unsigned char*)(src.data);
        outdata1 = (unsigned char*)(b.data);
	outdata2 = (unsigned char*)(g.data);
	outdata3 = (unsigned char*)(r.data);
        outdata2 = outdata2+1;
        outdata3 = outdata3+2;
        i =0;
        
              /* reading the pixels in Mat object using char pointer and storing*/
              while(i < src.rows){
              j=0;
                       while(j < src.cols){
                       *outdata1 = *indata;  
                       *outdata2 = *(indata+1);                       
                       *outdata3 = *(indata+2);                                            
                       j = j+1; 
                       indata = indata+3;
                       outdata1 = outdata1+3;
                       outdata2 = outdata2+3;
                       outdata3 = outdata3+3;
                       }
              i++; 
              }  
	
}  

void encode(Mat r,Mat g,Mat b,Mat dst){
        int j,i;
        unsigned char *indata1,*indata2,*indata3,*outdata;
	indata1 = (unsigned char*)(r.data);
	indata2 = (unsigned char*)(g.data);
	indata3 = (unsigned char*)(b.data);
        outdata = (unsigned char*)(dst.data);
        indata2 = indata2+1;
        indata1 = indata1+2;
        i =0;
        
              /* reading the pixels in Mat object using char pointer and storing*/
              while(i < dst.rows){
              j=0;
                       while(j < dst.cols){
                       
                       *outdata = *indata3*0.6; 
		       outdata++;
                       *(outdata) = *indata2*0.3; 
                       outdata++;
                       *(outdata) = *indata1*1.3;                                                                  
                       j = j+1; 
                       indata1 = indata1+3;
                       indata2 = indata2+3;
                       indata3 = indata3+3;
                       outdata = outdata+1;
                       }
              i++; 
              }  
	
}  


int main(){
	Mat prev,curr;
        char k;
        VideoWriter video("Output.avi", CV_FOURCC('M','J','P','G'), 10, Size(1920,1280));
        Mat r(Size(1920, 1280), CV_8UC3);
        Mat g(Size(1920, 1280), CV_8UC3);
        Mat b(Size(1920, 1280), CV_8UC3);
        Mat pr(Size(1920, 1280), CV_8UC3);
        Mat pg(Size(1920, 1280), CV_8UC3);
        Mat pb(Size(1920, 1280), CV_8UC3);
        Mat re_encode(Size(1920, 1280), CV_8UC3); 
	VideoCapture cap("Dark-Room-Laser-Spot-with-Clutter.mpeg");         
        	while(1){        
        	cap >> prev;        
        	decode(prev,pr,pg,pb);  // seprating bands for prev frame
        	cap >> curr;        // seprating bands for curr frame
                decode(curr,r,g,b);
        	r = r-pr;
        	g = g-pg;
        	b = b-pb;      
        	imshow("Red",r);       
        	imshow("Green",g);  
		imshow("Blue",b); 
                encode(r,g,b,re_encode); //encoding again to get video 
                video.write(re_encode);		
		k=waitKey(2);
		if(k==27)
		break;
        	}
}



