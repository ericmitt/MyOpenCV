//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

#include <opencv2\imgproc\types_c.h>
#include <opencv2\imgcodecs\imgcodecs.hpp>
#include <opencv2\core\core.hpp>
#include <ppltasks.h>
#include <wrl\client.h>
#include <Robuffer.h>
#include <vector>
#include <opencv2\imgproc\types_c.h>
#include <opencv2\imgcodecs\imgcodecs.hpp>
#include <opencv2\core\core.hpp>

#include <windows.storage.h>



#include <windows.storage.h>

using namespace ImageProcessing;

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

using namespace Windows::UI::Xaml::Media::Imaging;


using namespace Microsoft::WRL;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Foundation::Collections;



// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

MainPage::MainPage()
{
	InitializeComponent();
}

cv::Mat Lena;
void ImageProcessing::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	cv::Mat image = cv::imread("Assets/iss_earth_viewer.jpg");
	Lena = cv::Mat(image.rows, image.cols, CV_8UC4);
	cvtColor(image, Lena, CV_BGR2BGRA);
	UpdateImage(Lena);
}


void ImageProcessing::MainPage::UpdateImage(const cv::Mat& image)
{
	// Create the WriteableBitmap
	WriteableBitmap^ bitmap = ref new WriteableBitmap(image.cols, image.rows);

	// Get access to the pixels
	IBuffer^ buffer = bitmap->PixelBuffer;
	unsigned char* dstPixels;

	// Obtain IBufferByteAccess
	ComPtr<IBufferByteAccess> pBufferByteAccess;
	ComPtr<IInspectable> pBuffer((IInspectable*)buffer);
	pBuffer.As(&pBufferByteAccess);

	// Get pointer to pixel bytes
	pBufferByteAccess->Buffer(&dstPixels);
	memcpy(dstPixels, image.data, image.step.buf[1] * image.cols*image.rows);

	// Set the bitmap to the Image element
	ctlImg->Source = bitmap;
}

void ImageProcessing::MainPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cv::Mat result;
	cv::Mat intermediateMat;
	cv::Canny(Lena, intermediateMat, 80, 90);
	cv::cvtColor(intermediateMat, result, CV_GRAY2BGRA);

	UpdateImage(result);
}


void ImageProcessing::MainPage::Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cv::Mat intermediateMat;


	cv::Mat gray(Lena);
	//InputFrame.copyTo(gray);
	auto x = Lena.channels();
	if (x >= 3)
		cv::cvtColor(Lena, gray, CV_BGR2GRAY); //bug bug

	const int MEDIAN_BLUR_FILTER_SIZE = slider1->Value; //9;
	medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);
	cv::Mat edges;
	const int LAPLACIAN_FILTER_SIZE = slider2->Value; //5;
	Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
	cv::Mat mask;
	const int EDGES_THRESHOLD = slider3->Value; //40;
	threshold(edges, mask, EDGES_THRESHOLD, 255, cv::THRESH_BINARY_INV);
	cv::cvtColor(mask, intermediateMat, CV_GRAY2BGR);


	UpdateImage(intermediateMat);

}
