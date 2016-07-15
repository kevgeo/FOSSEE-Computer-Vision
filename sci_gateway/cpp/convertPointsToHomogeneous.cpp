#include <numeric>
#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
	Mat1f Thomogeneous(4, 1); 
	Thomogeneous(0) = -70;
	Thomogeneous(1) = -95;
	Thomogeneous(2) = -120;
	Thomogeneous(3) = 1;

	// Reshape to 4 channel matrix
	Mat Th = Thomogeneous.reshape(4);

	cv::Mat T;
	cv::convertPointsToHomogeneous(Th, T);
	for(int i=0;i<5;i++)
		cout<<T.at<float>(i)<<endl;

	return 0;
}