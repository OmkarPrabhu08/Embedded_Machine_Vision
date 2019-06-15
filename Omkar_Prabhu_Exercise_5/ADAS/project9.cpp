#include <unistd.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <semaphore.h>
#include <pthread.h>
#include <opencv/highgui.h>

#include <linux/videodev2.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#define FRAME_COUNT 10

using namespace cv;
using namespace std;

char filename[] = "imoge_0000.ppm";
char filesave[] = "image_0000.jpg";
char jitter[] = "plot.csv";

char username[20];
char host_name[20];

struct utsname systemname;
struct timespec start,end1,abs_start,abs_end,start2,end2,nano;

struct timespec timing[FRAME_COUNT];

Mat fm;
int frame_count = 0;
int save_count = 0;
double diff=0.00;
double diff2=0.00, diff3 = 0.00, diff4 = 0.00;
CvCapture * capture;

IplImage* frame;

pthread_t captureImage_thread;
pthread_t saveImage_thread;
pthread_t saveVideo_thread;

/*Thread Attributes*/
pthread_attr_t captureImage_attr;
pthread_attr_t saveImage_attr;
pthread_attr_t saveVideo_attr;
pthread_attr_t main_thread_attr;

struct sched_param captureImage_param;
struct sched_param saveImage_param;
struct sched_param saveVideo_param;

/*Semaphores*/
sem_t sem1, sem2, sem3;

pthread_mutex_t rsrcA;

//This function saves the IplImage as a ppm file and adds timestamp to its header
void savetemp(IplImage * mat_frame, int count)
{
	char *p = (char *)mat_frame->imageData;

	char data [307200*3];
	char header [33];
	char bucket;
	long long int i=0, j = 0;

	//swapping of elements as frame captured is BGR and ppm saves RGB
	for(i = 0; i<640*480*3; i=i+3)
	{
	//check for NULL elements
	if(*(p+i)=='\0')
	*(p+i)=0;
	if(*(p+i+2)=='\0')
	*(p+i+2)=0;
	
	bucket = *(p+i);
	*(p+i) = *(p+i+2);
	*(p+i+2) = bucket;
	
	data[j++] = *p;
	data[j++] = *(p+1);
	data[j++] = *(p+2);
	
	}

	//create a header with timestamp
	sprintf(header , "#%010d sec %010d nsec\n", timing[count].tv_sec, timing[count].tv_nsec);

	ofstream tempfile;

	header [32] = '\0';
	
	data [j] = '\0';

	//write data and header to new ppm file
	tempfile.open(filename);
	tempfile << "P6\n640 480\n255\n";
	tempfile << data;
	tempfile.close();
}


void *captureImage(void *threadp)
{	
	//request for a initial test frame
	frame=cvQueryFrame(capture);
//	fm=Mat(frame);
	Mat mat_frame(cvarrToMat(frame));
	//create a csv file to dump timing
	ofstream jitterplot;	
	jitterplot.open(jitter);
	jitterplot << "Frame,Jitter\r\n";	

	clock_gettime(CLOCK_REALTIME,&start);

	while(frame_count<FRAME_COUNT)
	{
	sem_wait(&sem1);
	
	//polling for 1 sec time between start and stop time 
	while(diff<1.000)
	{
	clock_gettime(CLOCK_REALTIME,&end1);
	diff=(end1.tv_sec - start.tv_sec)+((double)(end1.tv_nsec-start.tv_nsec)/1000000000.0d);
	}


	//this module uses nanosleep to generate the required 1 sec delay
	//It helps in reducing CPU utilization
/*	clock_gettime(CLOCK_REALTIME,&end);
 diff=(end.tv_sec - start.tv_sec)+((double)(end.tv_nsec-start.tv_nsec)/1000000000.0d);
	nano.tv_sec = 0;
	nano.tv_nsec = (0.1 - diff)*1000000000;	
	
	nanosleep(&nano,NULL);
	clock_gettime(CLOCK_REALTIME,&end);
 diff=(end.tv_sec - start.tv_sec)+((double)(end.tv_nsec-start.tv_nsec)/1000000000.0d);
*/
	//capture frame
	frame=cvQueryFrame(capture);

	//enter the time in the csv file
	if(!frame) break;
	jitterplot << frame_count;
	jitterplot << ",";
	jitterplot << diff;
	jitterplot << "\r\n";


	cout<<"Frame : "<<frame_count<<" Time: "<<diff<<endl;
	
	diff=0.00;
	
	//set start = end so that next time is calculate from this instant 
	//thus taking other execution time into account
	start = end1; 

	//save timstamp in the array used later to add timestamp
	timing[frame_count] = end1; 

//	fm=Mat(frame);
	Mat mat_frame(cvarrToMat(frame));
	frame_count++;
	
	//Pass control to thread 2
	sem_post(&sem2);
	}

	clock_gettime(CLOCK_REALTIME,&abs_end);

 diff=(abs_end.tv_sec - abs_start.tv_sec)+((double)(abs_end.tv_nsec-abs_start.tv_nsec)/1000000000.0d);

	//close the csv file
	jitterplot.close();	

	//Display the execution time
//sprintf(" Realtime thread time : %f  ", diff);
         cout<<"Realtime thread Time : ";
 cout<<diff;
	sem_post(&sem3);

}

//This function saves frames as jpg files
void *saveImage(void *threadp)
{
	VideoWriter writer("video1.avi",CV_FOURCC('M','J','P','G'),20,Size(640,480));
	int jpg[3]={CV_IMWRITE_JPEG_QUALITY, 100, 0};
	int ppm = 1;
	char a[35],b[35];
	int j = 0;
	int j1 = 0;

	while(j<2000)
	{
	sem_wait(&sem2);	

	clock_gettime(CLOCK_REALTIME, &start2);
	
	//manipulate filename to save seperate jpg files

/*4
	filename[15]=char((j/1000)+48);
        filename[16]=char(((j%1000)/100)+48);
        filename[17]=char(((j%100)/10)+48);
        filename[18]=char((j%10)+48);
*/
	sprintf(a,"Img%d.ppm",j);
	
        
	
	filesave[15]=char((j/1000)+48);
	filesave[16]=char(((j%1000)/100)+48);
	filesave[17]=char(((j%100)/10)+48);
	filesave[18]=char((j%10)+48);
//	imwrite(filesave ,frame);
	sprintf(b,"IMG%d.jpg",j);
	Mat mat_frame(cvarrToMat(frame));
	//save jpg
	imshow( "Display window", mat_frame );
	char abc=waitKey(33);
	if(abc=='q')
	{
		printf("Forced exit");		
		exit(0);
	} 
//	cvSaveImage(filesave, frame, jpg);
	imwrite(b,mat_frame);
	imwrite(a,mat_frame);
//	waitKey(1);

	writer.write(mat_frame);
	j++;

	clock_gettime(CLOCK_REALTIME, &end2);
	diff2 = diff2 + (end2.tv_sec - start2.tv_sec) + (double)((end2.tv_nsec - start2.tv_nsec)/1000000000);
	
	//pass control back to thread1 for next frame captue
	sem_post(&sem1);
	}
	//pass control to thread 3
	sem_post(&sem3);
}

//	*///This function reads jpg files and saves as ppm with timestamp and add frames to video
void *saveVideo(void *threadp)
{ 
	FILE * File;

	int i = 0;

	getlogin_r(username,20);
	gethostname(host_name,20);

	Mat temp;
	IplImage * tempframe;	

	//initializa videowriter
	VideoWriter writer("video1.avi",CV_FOURCC('M','J','P','G'),20,Size(640,480));

	//wait for semaphore
	sem_wait(&sem3);	

	while(i<FRAME_COUNT)
	{

	clock_gettime(CLOCK_REALTIME, &start2);	
	
	//manipulate jpg and ppm file names for each image	
	filesave[15]=char((i/1000)+48);
	filesave[16]=char(((i%1000)/100)+48);
	filesave[17]=char(((i%100)/10)+48);
	filesave[18]=char((i%10)+48);

	filename[15]=char((i/1000)+48);
 	filename[16]=char(((i%1000)/100)+48);
	filename[17]=char(((i%100)/10)+48);
 	filename[18]=char((i%10)+48); 

	temp = imread(filesave);

	//write the Mat frame to video
	writer.write(temp);

// v this block v uses custom function to save ppm files

//	tempframe = cvLoadImage(filesave);

//	savetemp(tempframe,i);	

	//save file as ppm and add timestamp information to the image
	if(i<5 || i > (FRAME_COUNT-5))
	{
	printf("raj");
	imwrite(filename,temp);

	File=fopen(filename,"a");
	fprintf(File,"\n");
	fprintf(File,"#Uname:%s\n",username);
	fprintf(File,"#Host name:%s\n",host_name);
	fprintf(File,"#%d sec ",timing[i].tv_sec);
	fprintf(File, "%d nsec",timing[i].tv_nsec);

	fclose(File);
	}
	clock_gettime(CLOCK_REALTIME, &end2);
	
	diff3 = diff3 + (end2.tv_sec - start2.tv_sec) + ((end2.tv_nsec - start2.tv_nsec)/1000000000); 

	cout<<"Saving Video "<<i<<"th frame"<<endl;

	i++;
	}

	sem_post(&sem1);

}

int main ()
{
	//Initializa Camera Capture
	capture = cvCreateCameraCapture(0);
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH,640);
	cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT,480);

	int rc = -1,scope;
	int rt_max_prio, rt_min_prio;
	cpu_set_t cpuset;
 CPU_SET(1, &cpuset);	
	
	/*Initialize Semaphore*/
	sem_init(&sem1, 0, 1);
	sem_init(&sem2, 0 ,0);
	sem_init(&sem3, 0 ,0);	
	
 // Set default protocol for mutex
 pthread_mutex_init(&rsrcA, NULL);
 
	/*Knowing Priority Values*/
	rt_max_prio = sched_get_priority_max(SCHED_FIFO);
	rt_min_prio = sched_get_priority_min(SCHED_FIFO);
	printf("rt_max_prio=%d\n", rt_max_prio);
	printf("rt_min_prio=%d\n", rt_min_prio);

	captureImage_param.sched_priority = rt_max_prio;
	saveImage_param.sched_priority = rt_max_prio - 1;
	saveVideo_param.sched_priority = rt_max_prio - 2;

	/*Initialize Sceduling Attributes*/
	pthread_attr_init(&captureImage_attr);
	pthread_attr_init(&saveImage_attr);
	pthread_attr_init(&saveVideo_attr);	
	
	/*Assigning Scheduling policy of threads*/
	pthread_attr_setinheritsched(&captureImage_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&captureImage_attr, SCHED_FIFO);
	rc=pthread_attr_setaffinity_np(&captureImage_attr, sizeof(cpu_set_t), &cpuset);
	if(rc<0)
	{
	perror("pthread_setaffinity_np");
	}

	pthread_attr_setinheritsched(&saveImage_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&saveImage_attr, SCHED_FIFO);
	rc=pthread_attr_setaffinity_np(&saveImage_attr, sizeof(cpu_set_t), &cpuset);
	if(rc<0)
	{
	perror("pthread_setaffinity_np");
	}
	
	pthread_attr_setinheritsched(&saveVideo_attr, PTHREAD_EXPLICIT_SCHED);
	pthread_attr_setschedpolicy(&saveVideo_attr, SCHED_FIFO);	
	rc=pthread_attr_setaffinity_np(&saveVideo_attr, sizeof(cpu_set_t), &cpuset);
	if(rc<0)
	{
	perror("pthread_setaffinity_np");
	}
	
	pthread_attr_setschedparam(&captureImage_attr, &captureImage_param);
	pthread_attr_setschedparam(&saveImage_attr, &saveImage_param);	
	pthread_attr_setschedparam(&saveVideo_attr, &saveVideo_param);
	
	/*Changing Schedular Type*/
	rc=sched_setscheduler(getpid(), SCHED_FIFO, &captureImage_param);
	if(rc < 0) perror("nrt_param");
	
 /*Knowing the scope of Main*/
 pthread_attr_getscope(&main_thread_attr, &scope);
 if(scope == PTHREAD_SCOPE_SYSTEM)
 printf("PTHREAD SCOPE SYSTEM\n");
 else if (scope == PTHREAD_SCOPE_PROCESS)
 printf("PTHREAD SCOPE PROCESS\n");
 else
 printf("PTHREAD SCOPE UNKNOWN\n");

	/*Create Threads*/
	rc = pthread_create( &captureImage_thread ,NULL, captureImage ,NULL ) ;
	if (rc)
 {
 printf("ERROR; pthread_create() 1 rc is %d\n", rc);
 perror(NULL);
 exit(-1);
 }
	rc = pthread_create( &saveImage_thread ,NULL, saveImage,NULL) ;
	if (rc)
 {
 printf("ERROR; pthread_create() 2 rc is %d\n", rc);
 perror(NULL);
 exit(-1);
 }
	rc = pthread_create( &saveVideo_thread , &saveVideo_attr, saveVideo, ( void * ) 0 ) ;
	if (rc)
 {
 printf("ERROR; pthread_create() 3 rc is %d\n", rc);
 perror(NULL);
	exit(-1);
 }	

 /*Join Threads to Main*/
	
	clock_gettime(CLOCK_REALTIME,&abs_start);
	
 pthread_join ( captureImage_thread, NULL ) ;
 pthread_join ( saveImage_thread, NULL ) ;
 pthread_join ( saveVideo_thread, NULL ) ;
 clock_gettime(CLOCK_REALTIME,&abs_end);
 diff=(abs_end.tv_sec - abs_start.tv_sec)+((double)(abs_end.tv_nsec-abs_start.tv_nsec)/1000000000.0d);

	cout<<endl<<"Total Time :"<<diff;
	cout<<endl<<"Time to Save Image "<<diff2/FRAME_COUNT;
	cout<<endl<<"Time to Compress and Make Video"<<diff3/FRAME_COUNT;
 

};
