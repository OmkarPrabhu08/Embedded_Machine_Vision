#include <opencv/highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>
#include "opencv2/opencv.hpp"
#include <time.h>

using namespace std;
using namespace cv;
long double timestamp[2000];
long double diff[2000];
long double total_exe = 0;
long double avg_exe;
struct timespec curr_time;
int main(int argc,char *argv[])
{
	Mat frame,gray, hsv_img, gauss, dst,can_edge, mask_img, y_frame, w_frame, yw_frame, new_frame,hls_img, b_frame, and_frame;
	VideoCapture cap(argv[1]);
	Point close, close2, close3, close4;
	double close_x1, close_y1, close_x2, close_y2 = 0;
	double close_x3, close_y3, close_x4, close_y4 = 0;

	if(!cap.isOpened())
	{
		cout<<"Error in opening the file"<<endl;	
	}
	
for(int i = 0; i<=1000;)
	{ 		
 		if(!cap.read(frame))
		{
 			cout<<"Error in reading the file"<<endl;
 			break;
 		}
                clock_gettime(CLOCK_REALTIME, &curr_time);
                timestamp[i] = curr_time.tv_sec*1000000000+curr_time.tv_nsec;
		cvtColor(frame, gray, CV_BGR2GRAY);
		cvtColor(frame, hsv_img, CV_BGR2HSV);
		cvtColor(frame, hls_img, CV_BGR2HLS);
		inRange(gray, 120, 255, w_frame);
		inRange(hls_img, Scalar(0, 63, 48), Scalar(62, 74, 57), y_frame);
                bitwise_or(y_frame, w_frame, yw_frame);
                bitwise_and(gray, yw_frame, and_frame);

		int rows = frame.rows;
		int rows2 = gray.rows;
		int columns = frame.cols;
		int columns2 = gray.cols;	 

		GaussianBlur(and_frame, gauss, Size(1, 1), 0, 0 );
		Canny(gauss,can_edge, 20, 60, 3);

		Mat cropped;
 		cropped = can_edge(Rect(can_edge.cols/4,3*can_edge.rows/5,2*can_edge.cols/5,can_edge.rows/5));

		Point third = Point(0, 3*rows/5+ 25);
	 	Point fourth = Point(columns, 3*rows/5+ 25);

		Point fifth = Point(0, 7*rows/10);
	 	Point sixth = Point(columns, 7*rows/10);
		
		vector<Vec4i> lines;
		HoughLinesP(cropped, lines, 1, CV_PI/180, 40, 20, 60);

		Point cent = Point(columns/2 - 15, 3*rows/5 + 25);
		Point min_left, min_right;
	 	int left_detected = 0;
		int right_detected = 0;
		
		double test_x1 = 0;
		double test_x2 = columns;
		double old_test_x1;
		double old_test_x2;

		double test_x3 = 0;
		double test_x4 = columns;
		double old_test_x3;
		double old_test_x4;

		for( size_t i = 0; i < lines.size(); i++ )
		{
		    Vec4i l = lines[i];
		    Point first = Point((l[0]+can_edge.cols/3), (l[1]+3*can_edge.rows/5));
		    Point second = Point((l[2]+can_edge.cols/3), (l[3]+3*can_edge.rows/5));

		    double slope = (second.y - first.y) / (double)(second.x - first.x);

		    if(abs(first.y - second.y) < 40)
		    {}
	 	    else
		    {
			    double a1 = second.y - first.y;
			    double b1 = first.x - second.x;
			    double c1 = a1 * (first.x) + b1 * (first.y);

			    double a2 = fourth.y - third.y;
			    double b2 = third.x - fourth.x;
			    double c2 = a2 * (third.x) + b2 * (third.y);

			    double d = a1*b2 - a2*b1;
			    double x1,y1;

			    if(d != 0)
			    {
				x1 = (b2 * c1 - b1 * c2)/d;
				y1 = (a1 * c2 - a2 * c1)/d;
			    }

			    if(x1 > 0 && x1 < 700)
			    {
				if(x1 > test_x1)
				{
					test_x1 = x1;
					close_x1 = x1;
					close_y1 = y1;
					line(frame, Point(x1- 100, y1), Point(x1 - 100, y1 +10), Scalar(0,0,0), 3, CV_AA);
					line(frame, Point((l[0]+can_edge.cols/4), (l[1]+3*can_edge.rows/5)), Point((l[2]+can_edge.cols/4), (l[3]+3*can_edge.rows/5)), Scalar(0,0,255), 3, CV_AA);
				}
			    }

			    if(x1 > 700 && x1 < columns)
			    {
				if(x1 < test_x2)
				{
					test_x2 = x1;
					close_x2 = x1;
					close_y2 = y1;
					line(frame, Point(x1- 100, y1), Point(x1 - 100, y1 +10), Scalar(0,0,0), 3, CV_AA);
					line(frame, Point((l[0]+can_edge.cols/4), (l[1]+3*can_edge.rows/5)), Point((l[2]+can_edge.cols/4), (l[3]+3*can_edge.rows/5)), Scalar(0,0,255), 3, CV_AA);
				}
			    }

			    old_test_x1 = test_x1;
			    old_test_x2 = test_x2;

			    double a3 = second.y - first.y;
			    double b3 = first.x - second.x;
			    double c3 = a3* (first.x) + b3 * (first.y);

			    double a4 = sixth.y - fifth.y;
			    double b4 = fifth.x - sixth.x;
			    double c4 = a4 * (fifth.x) + b4 * (fifth.y);

			    double d1 = a3*b4 - a4*b3;
			    double x2,y2;

			    if(d1 != 0)
			    {
				x2 = (b4 * c3 - b3 * c4)/d1;
				y2 = (a3 * c4 - a4 * c3)/d1;
			    }

			    if(x2 > 0 && x2 < 700)
			    {
				if(x2 > test_x3)
				{
					test_x3 = x2;
					close_x3 = x2;
					close_y3 = y2;
					line(frame, Point(x2- 100, y2), Point(x2 - 100, y2 +10), Scalar(0,0,0), 3, CV_AA);
					//line(frame, Point((l[0]+can_edge.cols/4), (l[1]+3*can_edge.rows/5)), Point((l[2]+can_edge.cols/4), (l[3]+3*can_edge.rows/5)), Scalar(0,0,255), 3, CV_AA);
				}
			    }

			    if(x2 > 700 && x2 < columns)
			    {
				if(x2 < test_x4)
				{
					test_x4 = x2;
					close_x4 = x2;
					close_y4 = y2;
					line(frame, Point(x2- 100, y2), Point(x2 - 100, y2 +10), Scalar(0,0,0), 3, CV_AA);
					//line(frame, Point((l[0]+can_edge.cols/4), (l[1]+3*can_edge.rows/5)), Point((l[2]+can_edge.cols/4), (l[3]+3*can_edge.rows/5)), Scalar(0,0,255), 3, CV_AA);
				}
			    }

			    old_test_x3 = test_x3;
			    old_test_x4 = test_x4;
			}
		
		}
		test_x1 = 0;
		close = Point(close_x1, close_y1);
		close.x = close.x - 100;
		circle(frame, close, 50, Scalar(0,0,255),0);

		test_x2 = 0;
		close2 = Point(close_x2, close_y2);
		close2.x = close2.x - 100;
		circle(frame, close2, 50, Scalar(0,0,255),0);

		test_x3 = 0;
		close3 = Point(close_x3, close_y3);
		close3.x = close3.x - 100;
		circle(frame, close3, 50, Scalar(0,0,255),0);

		test_x4 = 0;
		close4 = Point(close_x4, close_y4);
		close4.x = close4.x - 100;
		circle(frame, close4, 50, Scalar(0,0,255),0);

		Point midpoint;
		Point midpoint2;

		if(close.x != -100 && close2.x != -100)
			midpoint = Point((close.x + close2.x)/2, 3*rows/5+ 25);
		else
			midpoint = Point(0, 0);

		if(close3.x != -100 && close4.x != -100)
			midpoint2 = Point((close3.x + close4.x)/2, 3*rows/5+ 25);
		else if(close2.x == -100 && close4.x == -100)
		{
			midpoint = Point(close2.x - columns/7, 3*rows/5 + 15);
			midpoint2 = Point(close4.x - columns/7, 3*rows/5 + 15); 
		}
		else
			midpoint2 = Point(0, 0);
			

		/*if(midpoint.x > (columns/2))
			putText(frame,"Turn left", cvPoint(100,50),FONT_HERSHEY_COMPLEX_SMALL, 2 ,cvScalar(255, 0, 0));
		else if(midpoint.x > 0 && midpoint.x < (columns/2 - 20))
			putText(frame, "Turn right",cvPoint(900,50),FONT_HERSHEY_COMPLEX_SMALL, 2,cvScalar(255, 0, 0));
		else if(midpoint.x == 0 && midpoint.y == 0)
			putText(frame, "Stay on course",cvPoint(500,50),FONT_HERSHEY_COMPLEX_SMALL, 2, cvScalar(255, 0, 0));
		else
			putText(frame, "Stay on course",cvPoint(500,50),FONT_HERSHEY_COMPLEX_SMALL, 2, cvScalar(255, 0, 0));*/

		if(midpoint.x > midpoint2.x + 10 && midpoint.x < midpoint2.x + 20)
			putText(frame,"Turn right by 10 degree", cvPoint(800,50),FONT_HERSHEY_COMPLEX_SMALL, 2 ,cvScalar(255, 0, 0));
		else if(midpoint.x > midpoint2.x + 10 && midpoint.x < midpoint2.x + 30)
			putText(frame,"Turn right by 20 degree", cvPoint(800,50),FONT_HERSHEY_COMPLEX_SMALL, 2 ,cvScalar(255, 0, 0));
		else if(midpoint.x > midpoint2.x + 10 && midpoint.x < midpoint2.x + 40)
			putText(frame,"Turn right by 30 degree", cvPoint(800,50),FONT_HERSHEY_COMPLEX_SMALL, 2 ,cvScalar(255, 0, 0));
		else if(midpoint.x > midpoint2.x + 10 && midpoint.x < midpoint2.x + 50)
			putText(frame,"Turn right by 40 degree", cvPoint(800,50),FONT_HERSHEY_COMPLEX_SMALL, 2 ,cvScalar(255, 0, 0));
		else if(midpoint.x > midpoint2.x + 10 && midpoint.x < midpoint2.x + 60)
			putText(frame,"Turn right by 50 degree", cvPoint(800,50),FONT_HERSHEY_COMPLEX_SMALL, 2 ,cvScalar(255, 0, 0));
		else if(midpoint.x > midpoint2.x + 10 && midpoint.x < midpoint2.x + 70)
			putText(frame,"Turn right by 60 degree", cvPoint(800,50),FONT_HERSHEY_COMPLEX_SMALL, 2 ,cvScalar(255, 0, 0));
		else if(midpoint.x > 0 && midpoint.x < midpoint2.x - 10 && midpoint.x > midpoint2.x -20)
			putText(frame, "Turn left by 10 degree",cvPoint(100,50),FONT_HERSHEY_COMPLEX_SMALL, 2,cvScalar(255, 0, 0));
		else if(midpoint.x > 0 && midpoint.x < midpoint2.x - 10 && midpoint.x > midpoint2.x -30)
			putText(frame, "Turn left by 20 degree",cvPoint(100,50),FONT_HERSHEY_COMPLEX_SMALL, 2,cvScalar(255, 0, 0));
		else if(midpoint.x > 0 && midpoint.x < midpoint2.x - 10 && midpoint.x > midpoint2.x -40)
			putText(frame, "Turn left by 30 degree",cvPoint(100,50),FONT_HERSHEY_COMPLEX_SMALL, 2,cvScalar(255, 0, 0));
		else if(midpoint.x > 0 && midpoint.x < midpoint2.x - 10 && midpoint.x > midpoint2.x -50)
			putText(frame, "Turn left by 40 degree",cvPoint(100,50),FONT_HERSHEY_COMPLEX_SMALL, 2,cvScalar(255, 0, 0));
		else if(midpoint.x > 0 && midpoint.x < midpoint2.x - 10 && midpoint.x > midpoint2.x -60)
			putText(frame, "Turn left by 50 degree",cvPoint(100,50),FONT_HERSHEY_COMPLEX_SMALL, 2,cvScalar(255, 0, 0));
		else if(midpoint.x > 0 && midpoint.x < midpoint2.x - 10 && midpoint.x > midpoint2.x -70)
			putText(frame, "Turn left by 60 degree",cvPoint(100,50),FONT_HERSHEY_COMPLEX_SMALL, 2,cvScalar(255, 0, 0));
		else if(midpoint.x == 0 && midpoint.y == 0)
			putText(frame, "Stay on course",cvPoint(500,50),FONT_HERSHEY_COMPLEX_SMALL, 2, cvScalar(255, 0, 0));
		else
			putText(frame, "Stay on course",cvPoint(500,50),FONT_HERSHEY_COMPLEX_SMALL, 2, cvScalar(255, 0, 0));	

		//line(frame, Point(columns/2-10, 3*rows/5+ 20), Point(columns/2-10, 3*rows/5 + 30), Scalar(0, 255, 255), 2, CV_AA);
		line(frame, Point(0, 3*rows/5+ 25), Point(columns, 3*rows/5+ 25), Scalar(255,255,0), 1, CV_AA);
		line(frame, Point(0, 7*rows/10), Point(columns, 7*rows/10), Scalar(255,255,0), 1, CV_AA);

		if(close2.x == -100 || close2.y == 0)
		{
			line(frame, Point(close.x + rows/7, 3*rows/5+15), Point(close.x + rows/7, 3*rows/5+35), Scalar(255,120,255), 4, CV_AA);
		}
		else if(close.x == -100 || close.y == 0)
		{
			line(frame, Point(close.x - rows/7, 3*rows/5+15), Point(close.x - rows/7, 3*rows/5+35), Scalar(255,120,255), 4, CV_AA);	
		}
		else
		line(frame, Point(midpoint.x, 3*rows/5+15), Point(midpoint.x, 3*rows/5+35), Scalar(255,120,255), 4, CV_AA);

		if(close4.x == -100 || close4.y == 0)
		{
			line(frame, Point(close3.x + rows/7, 7*rows/10+5), Point(close3.x + rows/7, 7*rows/10+25), Scalar(255,120,255), 4, CV_AA);
		}
		else if(close3.x == -100 || close3.y == 0)
		{
			line(frame, Point(close3.x - rows/7, 7*rows/10+5), Point(close3.x - rows/7, 7*rows/10+25), Scalar(255,120,255), 4, CV_AA);	
		}
		else
		line(frame, Point(midpoint2.x, 7*rows/10 + 10), Point(midpoint2.x, 7*rows/10-10), Scalar(255,180,255), 4, CV_AA);
		
		imshow("source", frame);
		
		close_x1 = close_y1 = close_x2 = close_y2 = 0;
		close_x3 = close_y3 = close_x4 = close_y4 = 0;
		if (i >= 1){
                diff[i] = timestamp[i] - timestamp[i-1];
                }
                total_exe = total_exe+diff[i];
               
		char c=cvWaitKey(1);
		if(c == 27)
			break;
                i++;
	}
 avg_exe = 1000000000000/total_exe;
                printf("fps : %LG\n",avg_exe);
}

