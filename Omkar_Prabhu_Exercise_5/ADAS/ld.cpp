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
	Mat frame,gray, hsv_img, gauss, dst,can_edge, mask_img;
	VideoCapture cap(argv[1]);
	

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
		
		//resize(frame, frame, Size(), 0.4, 0.4, INTER_LINEAR);
		
		
		//inRange(frame, Scalar(140, 140, 140), Scalar(255, 255, 255), frame);

		cvtColor(frame, gray, CV_BGR2GRAY);
		cvtColor(frame, hsv_img, CV_BGR2HSV);


		int rows = gray.rows;
		int rows2 = gray.rows;
		int columns = gray.cols;
		int columns2 = gray.cols;

		/*for(int i=3*rows/5; i>=0; i--)
		{
				for(int j=0; j <= columns; j++)
				{
					gray.at<ushort>(i, j) = 0;
				}
		}*/	

		/*for(int i=rows2-1; i>=4*rows2/5; i--)
		{
				for(int j=0; j <= columns2; j++)
				{
					gray.at<ushort>(i, j) = 0;
				}
		}*/	 

		GaussianBlur(gray, gauss, Size(5, 5), 0, 0 );

		Canny(gauss,can_edge, 20, 150, 3);

		Mat cropped;
 		cropped = can_edge(Rect(can_edge.cols/3,3*can_edge.rows/5,can_edge.cols/3,can_edge.rows/5));
 		//imshow("Cropped", cropped);
		//waitKey();
		
		vector<Vec4i> lines;
		HoughLinesP(cropped, lines, 1, CV_PI/180, 40, 10, 5);
		for( size_t i = 0; i < lines.size(); i++ )
		{
		    Vec4i l = lines[i];
		    Point first = Point((l[0]+can_edge.cols/3), (l[1]+3*can_edge.rows/5));
		    Point second = Point((l[2]+can_edge.cols/3), (l[3]+3*can_edge.rows/5));

		    double slope = (second.y - first.y) / (double)(second.x - first.x);	
		    cout << second.y -first.y << endl;
		    //line( frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(255,0,0), 3, CV_AA);
		    //if(slope < 1)
		    if(abs(first.y - second.y) < 40)
		    {}
	 	    else
		    {
		    	line( frame, Point((l[0]+can_edge.cols/3), (l[1]+3*can_edge.rows/5)), Point((l[2]+can_edge.cols/3), (l[3]+3*can_edge.rows/5)), Scalar(0,0,255), 3, CV_AA);
		    }
		}
		
		imshow("source", frame);
		
		char c=cvWaitKey(30);
		if(c == 27)
			break;
	}
}
