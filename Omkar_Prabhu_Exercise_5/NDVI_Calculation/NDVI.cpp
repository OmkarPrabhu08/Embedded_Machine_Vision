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


/*function to encode image according VARI index alternative to NDVI for RGB images since NDVI works on NIR*/
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
                       float VARI;
                       float l;
                       float m;
                       /*formula to calculate VARI*/
                       m = *indata2+*indata1-*indata3;
                       l = *indata2-*indata1;
                       VARI = l/m;     
                       /*just mapping between green and red hence B value 0*/
                       /*More Green-More Healthy*/                       
                       *outdata = 0; 
		       outdata++;
                       if ( VARI<0){
                       *(outdata) = ((-1)*VARI/2)*(255); 
                       outdata++;
                       *(outdata) = (1+VARI/2)*(255);
                       }  
                       else{
                       *(outdata) = (1+VARI/2)*(255); 
                       outdata++;
                       *(outdata) = ((-1)*VARI/2)*(255);
                       }                                                                  
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
        Mat src = imread("House.jpeg");
        Mat dst; 
        imshow("source", src);
        Mat r(Size(800, 533), CV_8UC3);
        Mat g(Size(800, 533), CV_8UC3);
        Mat b(Size(800, 533), CV_8UC3);
        Mat pr(Size(800, 533), CV_8UC3);
        Mat pg(Size(800, 533), CV_8UC3);
        Mat pb(Size(800, 533), CV_8UC3);
        Mat re_encode(Size(800, 533), CV_8UC3); 	       
      	decode(src,r,g,b);  // seprating bands 
        encode(r,g,b,re_encode); //encoding again to get VARI
        imshow("Vegetation-Index", re_encode);
        waitKey(0);
        	
}



