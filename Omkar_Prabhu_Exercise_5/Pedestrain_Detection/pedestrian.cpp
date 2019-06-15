//Reference: http://www.magicandlove.com/blog/2011/12/04/people-detection-sample-from-opencv/
//Reference: https://stackoverflow.com/questions/34985196/opencv-using-svm-and-hog-for-person-detection
#include <iostream>
#include <opencv/highgui.h>
#include <opencv2/opencv.hpp>
#include "opencv2/imgproc/imgproc.hpp"
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

int shw = 0;
int save = 0;
int number_people;
int Frame = 1;
 
int main (int argc, const char * argv[])
{   /*waiting to get second arg and check whether it's just to show or save */ 
    int arg_count = 0;
    while(arg_count < argc)
    {
	if(string(argv[arg_count]) == "--show")
	{
		shw = 1;
	}
	else if(string(argv[arg_count]) == "--store")
	{
		save = 1;
	}
	arg_count++;
    }
    string file_name;
    file_name = string(argv[1]);
    
    /*setting width and height of the output video according to input*/
    VideoCapture capture(file_name);
    capture.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);  

    int width = capture.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    /*write video to detection.avi according to width and height*/
    VideoWriter output_video("Detection.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(width, height));
    

    HOGDescriptor hog;
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    
    FILE * file_ptr;
    file_ptr = fopen("count_log.txt", "w"); // creating a log file to store the count
    Mat image;
    while(1)
    {
	file_ptr = fopen("count_log.txt", "a"); //append mode
        capture >> image;
        if (!image.data)
	{
            printf("Error");
	}

	

	if(shw == 1)
	imshow("Resized input", image);
       
        vector<Rect> people_found, people_marked;
        /* performs the full object detection (descriptor extraction and binary classification together and returns the bounding boxes of each candidates*/
        hog.detectMultiScale(image, people_found, 0, Size(8,8), Size(32,32), 1.05, 2);
 
        size_t i, j;
        for (i=0; i<people_found.size(); i++)
        {
            Rect r = people_found[i];
            for (j=0; j< people_found.size(); j++)
                if (j!=i && (r & people_found[j])==r)
                    break;
            if (j==people_found.size())
                people_marked.push_back(r);
        }
	{
        	for (i=0; i<people_marked.size(); i++)
		{
		    Rect r = people_marked[i];
		    r.x += cvRound(r.width*0.1);
		    r.width = cvRound(r.width*0.8);
		    r.y += cvRound(r.height*0.06);
		    r.height = cvRound(r.height*0.9);

		    rectangle(image, r.tl(), r.br(), cv::Scalar(0,255,0), 2);

		    Point center = (r.br() + r.tl())* 0.5;
                    /*printing it on console*/
		    printf("Co-ordinate of the subject x = %d and y = %d \n", center.x, center.y);
		    fprintf(file_ptr, "Frame number: %d \n", Frame);
                    /*printing logs to file*/
		    fprintf(file_ptr, "Pedestrian detected at x = %d and y = %d \n", center.x, center.y);
		}
	}
        imshow("Output", image);
        if(save == 1)
	{
	output_video << image;
	}
	
	number_people = people_marked.size();
	printf("people/frame: %d ",number_people);
        /*printing log*/
	fprintf(file_ptr, "Number of people per frame: %d", number_people);
	printf("\n");
	fprintf(file_ptr, "%s", "\n");

	
	Frame++;
        /*Esc as wait*/
        if (waitKey(20) >= 0)
            break;
	fclose(file_ptr);
	//waitKey();
    }
    return 0;
}
