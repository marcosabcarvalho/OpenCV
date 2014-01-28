#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

Mat edge(Mat &);
Mat contour(Mat &);
void corners(Mat &, Mat &);

int main(int argc, char *argv[])
{
//Variable Declaration	
	Mat img, canny, bw;
	namedWindow("Digit Model", CV_WINDOW_NORMAL);
	namedWindow("Number",CV_WINDOW_NORMAL);
	char ch;

//Read Image
	if(argc!=2)
	{
		cout<<"\nUsage: ./digit.cpp <Your Image>\n";
		return -1;
	}

	img = imread(argv[1]);
	if(!img.data)
	{
		cout<<"\n Not a Valid Image\n";
		return -1;
	}

//edge 
	canny = edge(img);
	

//contour 
	bw = contour(canny);
	

//corners 
	corners(bw, img);
	
 	return 0;
}

Mat edge(Mat &img)
{
	Mat gray, blur, canny;
	cvtColor(img, gray, CV_BGR2GRAY);
	GaussianBlur(gray, blur, Size(3,3), 0);
	Canny(blur, canny, 120, 360, 3);

	return canny;
}

Mat contour(Mat &canny)
{
	Mat bw;
	canny.copyTo(bw);
	vector<vector<Point> > contours;
	vector<Vec4i> h;
	findContours(canny, contours, h, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));
	for(int i=0; i<contours.size(); i++)
	{
		drawContours(bw, contours, i, Scalar(255,255,255), 2, 8, h, 0, Point());
	}

	return bw;
}

void corners(Mat &bw, Mat &img)
{
	Mat dst, dst_norm, dst_norm_scaled;
	char ch;

  	dst = Mat::zeros(bw.size(), CV_32FC1);

   	int blockSize = 2;
  	int apertureSize = 3, thresh=100;
  	double k = 0.04;
  
  	cornerHarris(bw, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

  	normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
 	convertScaleAbs(dst_norm, dst_norm_scaled);

 	Mat small;
 	Point corner;

  	for(int i=0; i<dst_norm.rows; i++)
 	{
 		for(int j=0; j<dst_norm.cols; j++)
 		{
 			if( (int) dst_norm.at<float>(i,j) > thresh)
 			{
 				corner = Point(j-2,i-2);
				rectangle(img, corner, Point(corner.x+4, corner.y+4), Scalar(255,255,255), 3, 8, 0);	
				Rect ROI = Rect(corner.x, corner.y, 4, 4);
				small=bw(ROI);
 				circle(dst_norm_scaled, Point(j,i), 5, Scalar(255,255,255), 2, 8, 0);			

 				for(int i=0; i<small.rows; i++)
				{
					printf("\n");
					for(int j=0; j<small.cols; j++)
					{
						printf("%d ", (int)small.at<uchar>(i,j)/255);
					}	
				}	

				printf("\n===================================================\n");
 				imshow("Number", dst_norm_scaled);
 				ch=waitKey();
 				if(ch==27)
 					return ;
 			}	
 		}	
 	}
		 	
}


