//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"







using namespace VideoModern;

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

//using namespace System::Runtime::InteropServices;

using namespace cv;

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
}

cv::VideoCapture cam;
int seuil = 10;
int nbPixel = 0;
cv::String tmp; 


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

int option = 0;
const int MaxOption = 2;


void cvVideoTask()
{


	auto ret = cam.open(0);
	Mat edges;
	Mat frame, oldFrame,tmp;

	
	// process frames
	while (1)
	{
		// get a new frame from camera - this is non-blocking per spec
		cam >> frame;
			if (!cam.grab()) continue;

		if (option == 0)
		{
			cvtColor(frame, edges, COLOR_RGB2GRAY);
			GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
			Canny(edges, edges, 0, 30, 3);
			cvtColor(edges, frame, COLOR_GRAY2RGB);
		}

		if (option == 1)
		{
			frame.copyTo(tmp);
			Compare(frame,oldFrame);
			tmp.copyTo(oldFrame);
		}
		if (option == 2)
		{
			medianBlur(frame, frame, 13);
			cv::Laplacian(frame, frame, CV_8U, 7);
			cv::Mat mask;
			threshold(frame, frame, 10, 55, cv::THRESH_BINARY_INV);
		}
		winrt_imshow();
	}
}






void VideoModern::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cv::winrt_setFrameContainer(ImgCtl);
	cv::winrt_startMessageLoop(cvVideoTask);
}


void VideoModern::MainPage::slider1_ValueChanged(Platform::Object^ sender, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^ e)
{
	if (slider1 == nullptr)
		return;
	seuil = slider1->Value;
}

void VideoModern::MainPage::ImgCtl_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	option++;
	if (option > MaxOption)
		option = 0;
}
