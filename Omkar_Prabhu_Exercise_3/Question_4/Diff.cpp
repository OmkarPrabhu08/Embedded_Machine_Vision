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
#include <fstream>  
using namespace std;

using namespace cv;

/*function to write pgm files used char pointer to access Mat variable data and used itoa and saved in format for pgm*/
void pgm(Mat src , FILE * pFile){
        int j,i,sum;
        unsigned char *indata;
        indata = (unsigned char*)(src.data);
	indata = indata +1;
	char buffer[3];
	int c;
	int k=0;
        i =0;
        
              /* reading the pixels in Mat object using char pointer and storing*/
              while(i < src.rows){
              j=0;
                       while(j < src.cols){
                                          
		       c = *(indata);
                       buffer[k] = (c/100);
		       c = c - buffer[k]*100;
		       buffer[k+1] = c/10;
		       c = c - buffer[k+1]*10;
		       buffer[k+2] = c;

		       buffer[k] = buffer[k] +48;
		       buffer[k+1] = buffer[k+1] +48;
		       buffer[k+2] = buffer[k+2] +48;
		       if (buffer[k+1]>48) printf("%c ",buffer[k+2]); 
		       k =0;
                       if(buffer[k] <=48 && buffer[k+1] <=48 && buffer[k+2] <=48){
                       fputc ( '0' , pFile );	       
                       }
		       else {
                       if( buffer[k] >48)     fputc (buffer[k], pFile );
                       if( buffer[k+1] >48)   fputc (buffer[k+1], pFile );
                       else fputc ('0', pFile );
                       if( buffer[k+2] >48)   fputc (buffer[k+2], pFile );
	               else fputc ('0', pFile );	
                       }
                       fputc ( ' ' , pFile );
		       j++;
		       indata = indata+3;
                       }
              fputc ( '\n' , pFile );
              i++; 
              }  
	
}  


int main(){

        char k;
        int frameno=0;
        char file_name[100];
        Mat pgm_img(Size(359, 240), CV_8UC3);
        Mat pgm_img_down(Size(359, 240), CV_8UC3);
        Mat dst(Size(718, 480), CV_8UC3);


        	pgm_img = imread("Bears.ppm");     
                sprintf(file_name, "1.pgm");
		ofstream outfile (file_name); 
		outfile.close();
                FILE * pFile;
                pFile=fopen (file_name,"w+");
                fputc ( 'P' , pFile );	
		fputc ( '2' , pFile );
                fputc ( '\n' , pFile );
                fputc ( '3' , pFile );
                fputc ( '5' , pFile );
                fputc ( '9' , pFile );
                fputc ( ' ' , pFile );
                fputc ( '2', pFile );
                fputc ( '4' , pFile );
                fputc ( '0' , pFile );
                fputc ( '\n' , pFile );
                fputc ( '2' , pFile );
                fputc ( '5' , pFile );
                fputc ( '5' , pFile );
                fputc ( '\n' , pFile );
                pgm (pgm_img, pFile);
    		fclose (pFile);
        	pgm_img = imread("1.pgm");     
                /* upsamples image without losing more information based on Laplacian Pyramid*/
                pyrUp( pgm_img, dst, Size( pgm_img.cols*2, pgm_img.rows*2 ) );
                /* downsamples image */
                pyrDown( dst, pgm_img_down, Size( dst.cols/2, dst.rows/2 ) );
                imwrite("pgm_down.pgm",pgm_img_down);
                /*taking absolute difference*/
                absdiff(pgm_img, pgm_img_down,  pgm_img_down);
                imwrite("diff.pgm",pgm_img_down);

                
               
    		
 		
}



