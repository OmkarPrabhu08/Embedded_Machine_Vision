/*
 *
 *  Example by Sam Siewert 
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <getopt.h>             /* getopt_long() */

#include <fcntl.h>              /* low-level i/o */
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include <linux/videodev2.h>

#include <time.h>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <opencv/highgui.h> 

using namespace cv;
using namespace std;
Mat src, src_gray;
Mat dst, detected_edges;
Mat grad;
char* window_name;

  int scale = 1;
  int delta = 0;
  int ddepth = CV_16S;

  int c;
int edgeThresh = 1;
int lowThreshold;
int const max_lowThreshold = 100;
int ratio = 3;
int kernel_size = 3;
long timestamp[2000];  //to record time in secs
long timestampn[2000];  //to record time in nsecs
long  diff[2001]; // to calculate jitter
signed int jitter[2000] = {0}; // to store jitters and calculate avg jitter no negative jitter is considered hence intialized to 0 
 struct timespec curr_time; // records time while capturing 
    int frame_count =0;
    double total_diff = 0; 
    double avg_rate; // average fps of the camera
    long total_jitter = 0;
    long avg_jitter = 0;
   
    long wce= 0; // worst case execution 



   

/**
 * @function CannyThreshold
 * @brief Trackbar callback - Canny thresholds input with a ratio 1:3
 */
void CannyThreshold(int, void*)
{
  /// Reduce noise with a kernel 3x3
  blur( src_gray, detected_edges, Size(3,3) );

  /// Canny detector
  Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );

  /// Using Canny's output as a mask, we display our result
  dst = Scalar::all(0);

  src.copyTo( dst, detected_edges);
  imshow( window_name, dst );
 }


int main( int argc, char** argv )
{   clock_gettime(CLOCK_REALTIME, &curr_time); 
timestamp[0] = curr_time.tv_sec; //storing time in sec
timestampn[0] = curr_time.tv_nsec; //storing time in nsecs

    cvNamedWindow("Capture Example", CV_WINDOW_AUTOSIZE);
    CvCapture* capture = cvCreateCameraCapture(1);
    IplImage* frame;

    for (;frame_count<=1000;frame_count++)
        {       clock_gettime(CLOCK_REALTIME, &curr_time); 
            if (frame_count>0)
            {
            timestamp[frame_count] = curr_time.tv_sec; //storing time in sec
            timestampn[frame_count] = curr_time.tv_nsec; //storing time in nsecs
            diff[frame_count]= timestamp[frame_count]*1000000000+timestampn[frame_count]-(timestamp[frame_count-1]*1000000000+timestampn[frame_count-1]);// calculating difference between      
            }
           
           if(diff[frame_count]>= 38000000)  // to calculate positive jitter we are considering only ones who are greater than avg 
            {
            jitter[frame_count] = diff[frame_count] - 38000000; 
            
            } 
          total_diff = total_diff+diff[frame_count]; // to calculate avg rate that is fps   
          total_jitter = total_jitter+jitter[frame_count]; // to calculate avg jitter
        frame=cvQueryFrame(capture);
     
        if(!frame) break;
        Mat frame_mat=cvarrToMat(frame); // converting to mat to use to process
        cvShowImage("Capture Example", frame);

       

    

     char d = cvWaitKey(33);

         if( d == 's') // if s then select sobel 
{          cvDestroyWindow("Canny Edge Detector");
window_name = "Sobel Edge Detector";
  /// Load an image
  src = frame_mat;
   
  if( !src.data )
  { return -1; }

  GaussianBlur( src, src, Size(3,3), 0, 0, BORDER_DEFAULT );

  /// Convert it to gray
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Create window
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_x, abs_grad_x );

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel( src_gray, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );
  convertScaleAbs( grad_y, abs_grad_y );

  /// Total Gradient (approximate)
  addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad );

  imshow( window_name, grad );

}
else if( d == 'c')
{        cvDestroyWindow("Sobel Edge Detector");
window_name = "Canny Edge Detector";
src = frame_mat;

  if( !src.data )
  { return -1; }

  /// Create a matrix of the same type and size as src (for dst)
  dst.create( src.size(), src.type() );

  /// Convert the image to grayscale
  cvtColor( src, src_gray, CV_BGR2GRAY );

  /// Create a window
  namedWindow( window_name, CV_WINDOW_AUTOSIZE );

  /// Create a Trackbar for user to enter threshold
  createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );

  /// Show the image
  CannyThreshold(0, 0);
 }
else if (d == 27)
{    cvReleaseCapture(&capture);
     cvDestroyWindow("Canny Edge Detector");
     cvDestroyWindow("Sobel Edge Detector");
break;
}
    }
  for (frame_count =2;frame_count<=1000;frame_count++) // sorting to find wce 
{
if (wce< diff[frame_count])
          {
            wce = diff[frame_count];
          }
}
avg_rate = 1/(total_diff/1000000000000); //dividing by total frames*(sec to nanosec convertor) taking inverse to calculate frames per seconds
avg_jitter = total_jitter/1000;
printf("\nfps=%lf\n", avg_rate);
printf("avg_jitter=%ld\n", avg_jitter);
printf("worst case execution time=%ld\n", wce);

};

