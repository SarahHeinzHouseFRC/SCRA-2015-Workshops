#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
	
	int lowH = 0;
	int highH = 179;

	int lowS = 0; 
	int highS = 255;

	int lowV = 0;
	int highV = 255;
	int expo = 0;

	namedWindow("Control", CV_WINDOW_AUTOSIZE);
	
	cvCreateTrackbar("LowH", "Control", &lowH, 179);
	cvCreateTrackbar("HighH", "Control", &highH, 179);

	cvCreateTrackbar("LowS", "Control", &lowS, 255);
	cvCreateTrackbar("HighS", "Control", &highS, 255);

	cvCreateTrackbar("LowV", "Control", &lowV, 255);
	cvCreateTrackbar("HighV", "Control", &highV, 255);
	cvCreateTrackbar("EXPOSURE","Control",&expo,1);
	
	VideoCapture capture(0);
	
	if(!capture.isOpened())
	{
		cout << "Capture failed to open";
		return -1;
	}
		
	while(true)
	{
		Mat currentFrame;
		Mat binaryFrame;
		
		if(!capture.read(currentFrame))
		{
			cout << "Failed to read video";
			break;
		}
		
		cvtColor(currentFrame, binaryFrame, COLOR_BGR2HSV);
		inRange(binaryFrame, Scalar(lowH, lowS, lowV), Scalar(highH, highS, highV), binaryFrame);
		
		int best_countour = 0;
        Rect bounding_box;
        vector<vector<Point> > all_contours;
        vector<Vec4i> hierarchy;
        findContours(binaryFrame, all_contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE);
        double largest_area = 0;
        int largest_contour_index = -1;
        for( int i = 0; i < all_contours.size(); i++)
        {
            double area = contourArea(all_contours[i],false);
            if(area > largest_area && area > 100){

                largest_area =area;
                largest_contour_index = i;
                bounding_box = boundingRect(all_contours[i]);
            }

        }
        Scalar color(255,255,255);

        drawContours(currentFrame, all_contours,largest_contour_index, color, CV_FILLED,8,hierarchy);
        rectangle(currentFrame, bounding_box, Scalar(0,255,0),2, 8,0);
        putText(currentFrame, "Best Color Match", Point(bounding_box.x, bounding_box.y), FONT_HERSHEY_PLAIN, 1.0, CV_RGB(0,255,0), 2.0);
		
		
		imshow("Capture",currentFrame);
		
		if(waitKey(30) == 27)
       {
                cout << "esc key is pressed by user" << endl;
		capture.release(); 
                break; 
       }	
	}
    return 0;
}
