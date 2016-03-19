// #include "highgui.h"
// #include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <cv.h>  
#include <cxcore.h>  
#include <highgui.h>  
  
#pragma comment(lib, "cv.lib")  
#pragma comment(lib, "cxcore.lib")  
#pragma comment(lib, "highgui.lib")  
  
int main( int argc, char** argv )  
{     
    IplImage *src = cvLoadImage("a.jpg",1);  
    IplImage *desc;  
    CvSize sz;  
    double scale = 0.3;  
    if(src)  
    {  
        sz.width = src->width*scale;  
        sz.height = src->height*scale;  
        desc = cvCreateImage(sz,src->depth,src->nChannels);  
        // cvResize(src,desc,CV_INTER_CUBIC);  
        cvResize(src,desc,CV_INTER_AREA);  
        // cvNamedWindow("src",CV_WINDOW_AUTOSIZE);  
        cvNamedWindow("desc",CV_WINDOW_AUTOSIZE);  
        // cvShowImage("src",src);  
        cvShowImage("desc",desc);  
        cvWaitKey(0);  
  
        cvReleaseImage(&src);  
        cvReleaseImage(&desc);  
        // cvDestroyWindow("src");  
        cvDestroyWindow("desc");  
        return 0;  
    }  
    return -1;  
} 