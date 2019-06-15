#include <opencv/highgui.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/video/background_segm.hpp>
#include <string>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <stdio.h>
#include <string.h>
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

int main(int argc,char *argv[])
{
	Mat frame,gray, hsv_img, gauss, dst,can_edge, mask_img, y_frame, w_frame, yw_frame, new_frame,hls_img,b_frame;
	VideoCapture cap(argv[1]);
	Point close, close2;
	

	if(!cap.isOpened())
	{
		cout<<"Error in opening the file"<<endl;	
	}
	
	while(1)
	{ 		
 		if(!cap.read(frame))
		{
 			cout<<"Error in reading the file"<<endl;
 			break;
 		}
                //frame = imread("2.png");
		
	//	resize(frame, frame, Size(), 0.7, 0.7, INTER_LINEAR);
		
		
		
		//bitwise_and(yw_frame, frame, new_frame);

		cvtColor(frame, gray, CV_BGR2GRAY);
		cvtColor(frame, hsv_img, CV_BGR2HSV);
		cvtColor(frame, hls_img, CV_BGR2HLS);
	//	inRange(hsv_img, Scalar(20, 20, 100), Scalar(30, 255, 255), y_frame);
	//	inRange(frame, Scalar(0, 30, 30), Scalar(30, 255, 255), y_frame);
		inRange(gray, 120, 255, w_frame);
		//bitwise_or(y_frame, w_frame, yw_frame);
		//dilate(y_frame, y_frame, 9);*/
                //inRange(frame, Scalar(0, 20, 20), Scalar(0, 255, 255), y_frame);
		inRange(hls_img, Scalar(0, 63, 48), Scalar(62, 74, 57), y_frame);
		inRange(hls_img, Scalar(0, 0, 0), Scalar(180, 98, 35), b_frame);
	//	inRange(hls_img, Scalar(0, 115, 0), Scalar(255, 255, 255), w_frame);
                bitwise_or(y_frame, w_frame, yw_frame);
         //       bitwise_and(yw_frame, frame, frame);
	//	inRange(gray, 130, 255, w_frame);
               
		//bitwise_or(y_frame, w_frame, yw_frame);
            /* dilate(yw_frame, yw_frame, 9);
                               dilate(yw_frame, yw_frame, 9);
                dilate(yw_frame, yw_frame, 9);
                dilate(yw_frame, yw_frame, 9);*/

		int rows = frame.rows;
		int rows2 = gray.rows;
		int columns = frame.cols;
		int columns2 = gray.cols;

		/*for(int i=rows; i>=0; i--)
		{
				for(int j=0; j <= columns; j++)
				{
					if(frame.at<Vec3b>(i,j)[0] = frame.at<Vec3b>(i,j)[1] = frame.at<Vec3b>(i,j)[2])		
					{}
					else
					{
						frame.at<ushort>(i, j) = 0;
					}
				}
		}
		imshow("source", frame);*/

		/*for(int i=rows2-1; i>=4*rows2/5; i--)
		{
				for(int j=0; j <= columns2; j++)
				{
					gray.at<ushort>(i, j) = 0;
				}
		}*/	 

		GaussianBlur(yw_frame, gauss, Size(1, 1), 0, 0 );
		//GaussianBlur(b_frame, gauss, Size(1, 1), 0, 0 );
		Canny(gauss,can_edge, 20, 60, 3);


		Mat cropped;
 		cropped = can_edge(Rect(can_edge.cols/4,3*can_edge.rows/5,can_edge.cols/2,can_edge.rows/5));
		//line(cropped, Point(11*can_edge/24, (2*can_edge.rows/5)), Point(11*can_edge/24, (2*can_edge.rows/4)), Scalar(0,255,255), 1, CV_AA); 
 		//imshow("Cropped", cropped);
		//waitKey();

		Point third = Point(0, 3*rows/5+ 25);
	 	Point fourth = Point(columns, 3*rows/5+ 25);
		
		vector<Vec4i> lines;
		HoughLinesP(cropped, lines, 1, CV_PI/180, 35, 20, 30);

		Point cent = Point(columns/2 - 15, 3*rows/5 + 25);
		Point min_left, min_right;
	 	int left_detected = 0;
		int right_detected = 0;
		double close_x1, close_y1, close_x2, close_y2 = 0;
		double test_x1 = 0;
		double test_x2 = columns;

		for( size_t i = 0; i < lines.size(); i++ )
		{
		    Vec4i l = lines[i];
		    Point first = Point((l[0]+can_edge.cols/3), (l[1]+3*can_edge.rows/5));
		    Point second = Point((l[2]+can_edge.cols/3), (l[3]+3*can_edge.rows/5));

		    double slope = (second.y - first.y) / (double)(second.x - first.x);	
		    //cout << second.y -first.y << endl;
		    //line( frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
		    //if(slope < 1)


		    /*if(x1 < cent.x)
		    {
			if(left_detected == 0)
			{
				min_left.x = x1;
				min_left.y = y1;	
				left_detected = 1;			
			}
			else
			{
				if(x1 > min_left.x)
				{
					min_left.x = x1;
					min_left.y = y1;
				}				
			}
		    }

		    if(x1 > cent.x)
		    {
			if(right_detected == 0)
			{
				min_right.x = x1;
				min_right.y = y1;	
				right_detected = 1;			
			}
			else
			{
				if(x1 > min_left.x)
				{
					min_right.x = x1;
					min_right.y = y1;
				}				
			}
		    }*/

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

			//Point close = Point(x1, y1);
		//close.x = close.x - 100;

		//Point close2 = Point(close_x2, close_y2);
		//close2.x = close2.x - 100;
		//if(lines.size() != 0)
		//circle(frame, close2, 10, Scalar(255,255, 0),0);

		//if(lines.size() != 0)
		//circle(frame, close, 10, Scalar(255,255, 0),0);

		//cout << x1 << endl;
		//cout << (columns/2 - 15) << endl;
			if(x1 > 0 && x1 < 700)
			    {
				if(x1 > test_x1)
				{
					test_x1 = x1;
					close_x1 = x1;
					close_y1 = y1;
				}
			    }

			if(x1 > 700 && x1 < columns)
			    {
				if(x1 < test_x2)
				{
					test_x2 = x1;
					close_x2 = x1;
					close_y2 = y1;
				}
			    }
			//cout << close_x2 << endl;
			//cout << close_y2 << endl;

			if(x1 > 500 && x1 < 850)
		    	line(frame, Point((l[0]+can_edge.cols/4), (l[1]+3*can_edge.rows/5)), Point((l[2]+can_edge.cols/4), (l[3]+3*can_edge.rows/5)), Scalar(0,0,255), 3, CV_AA);
			line(frame, Point(x1- 100, y1), Point(x1 - 100, y1 +10), Scalar(0,0,0), 3, CV_AA);
			
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

		Point midpoint;

		midpoint = Point((close.x + close2.x)/2, 3*rows/5+ 25);

		cout << close.x << " " << close.y << " " << close2.x << " " << close2.y << endl;		

		line(frame, Point(columns/2-15, 3*rows/5+ 20), Point(columns/2-15, 3*rows/5 + 30), Scalar(0, 255, 255), 2, CV_AA);
		line(frame, Point(0, 3*rows/5+ 25), Point(columns, 3*rows/5+ 25), Scalar(255,255,0), 1, CV_AA);

		if(close2.x == -100 && close2.y == 0)
		{
			line(frame, Point(close.x + rows/7, 3*rows/5+15), Point(close.x + rows/7, 3*rows/5+35), Scalar(255,120,255), 4, CV_AA);
		}
		else
		line(frame, Point(midpoint.x, 3*rows/5+15), Point(midpoint.x, 3*rows/5+35), Scalar(255,120,255), 4, CV_AA);

		/*if(midpoint.x > (columns/2 - 15))
			cvPutText(frame, "Turn Left", Point(100, 50), FONT_HERSHEY_PLAIN, 3, Scalar(0,0,255), 2, CV_AA);
		if(midpoint.x < (columns/2 - 15))
			cvPutText(frame, "Turn Right", Point(900, 50), FONT_HERSHEY_PLAIN, 3, Scalar(0,0,255), 2, CV_AA);*/
		
		imshow("source", frame);
		
		char c=cvWaitKey(30);
		if(c == 27)
			break;
	}
}
