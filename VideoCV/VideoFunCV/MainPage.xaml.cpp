//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include "opencv2\video.hpp"
#include <opencv2/videoio.hpp>
#include <opencv2/videoio/cap_winrt.hpp>

#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\imgproc\types_c.h>
#include <opencv2\imgcodecs\imgcodecs.hpp>
#include <opencv2\core\core.hpp>





using namespace VideoFunCV;

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

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
}

/// <summary>
/// Invoked when this page is about to be displayed in a Frame.
/// </summary>
/// <param name="e">Event data that describes how this page was reached.  The Parameter
/// property is typically used to configure the page.</param>
void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
	(void) e;	// Unused parameter

	// TODO: Prepare page for display here.

	// TODO: If your application contains multiple pages, ensure that you are
	// handling the hardware Back button by registering for the
	// Windows::Phone::UI::Input::HardwareButtons.BackPressed event.
	// If you are using the NavigationHelper provided by some templates,
	// this event is handled for you.
}

cv::VideoCapture cam;


int seuil = 10;
void Compare(Mat f, Mat oldF)
{
	if (oldF.rows == 0)
		return;

	for (int i = 0; i < f.rows; i++)
	{
		for (int j = 0; j < f.cols; j++)
		{
			if (abs(f.at<cv::Vec3b>(i, j)[2] - oldF.at<cv::Vec3b>(i, j)[2]) > seuil &&
				abs(f.at<cv::Vec3b>(i, j)[0] - oldF.at<cv::Vec3b>(i, j)[0]) > seuil &&
				abs(f.at<cv::Vec3b>(i, j)[1] - oldF.at<cv::Vec3b>(i, j)[1]) > seuil)
			{
				f.at<cv::Vec3b>(i, j)[2] = 255;
				f.at<cv::Vec3b>(i, j)[1] = 0;
				f.at<cv::Vec3b>(i, j)[0] = 0;
			}
		}
	}

}

int option = 0;
const int OptionMax = 2;

void cvVideoTask2()
{
	cv::Mat frame, oldFrame, tmp;
	cam.open(0);
	while (1)
	{
		// get a new frame from camera - this is non-blocking per spec
		cam >> frame;
		if (!cam.grab()) continue;



		if (option == 0)
		{
			frame.copyTo(tmp);
			Compare(frame, oldFrame);

			tmp.copyTo(oldFrame);
		}

		if (option == 1)
		{

			cv::medianBlur(frame, frame, 7);
			cv::Laplacian(frame, frame, CV_8U, 3);
			cv::threshold(frame, frame, 10, 55, cv::THRESH_BINARY_INV);
		}

		if (option == 2)
		{
			cv::Mat edges;
			cvtColor(frame, edges, COLOR_RGB2GRAY);
			GaussianBlur(edges, edges, cv::Size(7, 7), 1.5, 1.5);
			Canny(edges, edges, 0, 30, 3);
			cvtColor(edges, frame, COLOR_GRAY2RGB);
		}
		winrt_imshow();
	}
}







void VideoFunCV::MainPage::Page_Loaded(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cv::winrt_setFrameContainer(imgCV);
	cv::winrt_startMessageLoop(cvVideoTask2);
}

void VideoFunCV::MainPage::imgCV_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	option++;
	if (option > OptionMax)
		option = 0;
}
