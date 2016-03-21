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
    IplImage *src = cvLoadImage("a.jpg",1) ;  
    IplImage *desc_NN ;  
    IplImage *desc_LINEAR ;  
    IplImage *desc_AREA ;  
    IplImage *desc_CUBIC ;  
    IplImage *desc_LANCZOS4 ;  
    CvSize sz ;  
    double scale = 0.2 ;
    if(src)  
    {  
        sz.width = src->width*scale ;  
        sz.height = src->height*scale ;  
        desc_NN     = cvCreateImage( sz , src->depth , src->nChannels ) ;  
        desc_LINEAR = cvCreateImage( sz , src->depth , src->nChannels ) ;  
        desc_AREA   = cvCreateImage( sz , src->depth , src->nChannels ) ;  
        desc_CUBIC  = cvCreateImage( sz , src->depth , src->nChannels ) ;  
        desc_LANCZOS4  = cvCreateImage( sz , src->depth , src->nChannels ) ;  
        // cvResize(src,desc,CV_INTER_CUBIC) ;  
        cvResize( src , desc_NN       , CV_INTER_NN ) ; 
        cvResize( src , desc_LINEAR   , CV_INTER_LINEAR ) ; 
        cvResize( src , desc_AREA     , CV_INTER_AREA ) ; 
        cvResize( src , desc_CUBIC    , CV_INTER_CUBIC ) ;  
        cvResize( src , desc_LANCZOS4 , CV_INTER_LANCZOS4 ) ;  
        // cvNamedWindow("src",CV_WINDOW_AUTOSIZE) ;  
        cvNamedWindow( "desc_NN      " , CV_WINDOW_AUTOSIZE ) ;  
        cvNamedWindow( "desc_LINEAR  " , CV_WINDOW_AUTOSIZE ) ;  
        cvNamedWindow( "desc_AREA    " , CV_WINDOW_AUTOSIZE ) ;  
        cvNamedWindow( "desc_CUBIC   " , CV_WINDOW_AUTOSIZE ) ;  
        cvNamedWindow( "desc_LANCZOS4" , CV_WINDOW_AUTOSIZE ) ;  
        // cvShowImage("src",src) ;  
        cvShowImage( "desc_NN      " , desc_NN     ) ;  
        cvShowImage( "desc_LINEAR  " , desc_LINEAR ) ;  
        cvShowImage( "desc_AREA    " , desc_AREA   ) ;  
        cvShowImage( "desc_CUBIC   " , desc_CUBIC  ) ;  
        cvShowImage( "desc_LANCZOS4" , desc_LANCZOS4  ) ;  
        cvWaitKey( 0 ) ;  
  
        cvReleaseImage( &src ) ;  
        cvReleaseImage( &desc_NN        ) ;  
        cvReleaseImage( &desc_LINEAR    ) ;  
        cvReleaseImage( &desc_AREA      ) ;  
        cvReleaseImage( &desc_CUBIC     ) ;  
        cvReleaseImage( &desc_LANCZOS4  ) ;  
        // cvDestroyWindow("src") ;  
        cvDestroyWindow( "desc_NN      " ) ;  
        cvDestroyWindow( "desc_LINEAR  " ) ;  
        cvDestroyWindow( "desc_AREA    " ) ;  
        cvDestroyWindow( "desc_CUBIC   " ) ;  
        cvDestroyWindow( "desc_LANCZOS4" ) ;  
        return 0 ;  
    }  
    return -1 ;  
} 