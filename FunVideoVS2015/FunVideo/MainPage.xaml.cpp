//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

//opencv include
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2\imgcodecs\imgcodecs.hpp>
#include <opencv2\core\core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/videoio/cap_winrt.hpp>


using namespace FunVideo;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace cv;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}

cv::VideoCapture cam;
int seuil = 10;
int nbPixel = 0;

void Compare(Mat f, Mat oldF)
{
	if (oldF.rows == 0)
		return;

	nbPixel = 0;


	for (int i = 0; i < f.rows; i++)
	{
		for (int j = 0; j < f.cols; j++)
		{
			if (abs(f.at<cv::Vec3b>(i, j)[2] - oldF.at<cv::Vec3b>(i, j)[2]) >seuil &&
				abs(f.at<cv::Vec3b>(i, j)[0] - oldF.at<cv::Vec3b>(i, j)[0]) >seuil&&
				abs(f.at<cv::Vec3b>(i, j)[1] - oldF.at<cv::Vec3b>(i, j)[1]) > seuil)
			{
				f.at<cv::Vec3b>(i, j)[2] = 255;
				f.at<cv::Vec3b>(i, j)[1] = 0;
				f.at<cv::Vec3b>(i, j)[0] = 0;
				nbPixel++;
			}
		}
	}

	//auto tmp2 = nbPixel.ToString();
	//cv::String cvStr = "some text";
	//cvStr.operator std::string() = tmp2->Data();

	//cv::putText(f, cvStr, cv::Point(10, 30), FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar::all(255), 2, 8);
}



void cvVideoTask()
{
	
	cv::Mat frame, oldFrame, tmp;
	cam.open(0);
	while (1)
	{
		// get a new frame from camera - this is non-blocking per spec
		cam >> frame;
		if (!cam.grab()) continue;

		frame.copyTo(tmp);
		Compare(frame, oldFrame);
		tmp.copyTo(oldFrame);



		winrt_imshow();
	}
}

void FunVideo::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cv::winrt_setFrameContainer(imgCV);
	cv::winrt_startMessageLoop(cvVideoTask);

}
