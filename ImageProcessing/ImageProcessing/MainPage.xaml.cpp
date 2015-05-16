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

#include <opencv2\objdetect.hpp>
#include <ppltasks.h> 


#include <windows.storage.h>

#include <stdlib.h>



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
using namespace Microsoft::WRL;
using namespace concurrency;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::Pickers;


cv::String ImgName = "Assets/grpPC1.jpg";

// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238v

MainPage::MainPage()
{
	InitializeComponent();
}


void ImageProcessing::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//FileOpenPicker^ openPicker = ref new FileOpenPicker();
	//openPicker->ViewMode = PickerViewMode::Thumbnail;
	//openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
	//openPicker->FileTypeFilter->Append(".png");
	//openPicker->FileTypeFilter->Append(".jpg");
	//openPicker->FileTypeFilter->Append(".jpeg");

	//create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file){
	//	auto filename = file->Path;
	//	
	//	std::wstring localPathW(filename->Begin());
	//	size_t outSize = localPathW.length() + 1;
	//	char* localPathC = new char[outSize];
	//	size_t charsConverted = 0;
	//	wcstombs_s(&charsConverted, localPathC, outSize, localPathW.c_str(), localPathW.length());
	//	cv::String localPath(localPathC);

	//	cv::Mat image = cv::imread(localPathC);
	//	Lena = cv::Mat(image.rows, image.cols, CV_8UC4);
	//	cvtColor(image, Lena, CV_BGR2BGRA);
	//	UpdateImage(Lena);

	//});

	//C:\Users\ericmitt\Pictures\16620387123_b7c40867cb_o.jpg
	cv::Mat image = cv::imread(ImgName);
	//cv::Mat image = cv::imread("C:\\Users\ericmitt\\Pictures\\16620387123_b7c40867cb_o.jpg");
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
	//lenna filter
	cv::Mat result;
	cv::Mat intermediateMat;
	//cv::Canny(Lena, intermediateMat, 80, 90);
	cv::Canny(Lena, intermediateMat, sliderCanny->Value, sliderCanny2->Value);
	cv::cvtColor(intermediateMat, result, CV_GRAY2BGRA);

	UpdateImage(result);
}

void Msg(String^ msg)
{
	
}
void ImageProcessing::MainPage::Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cv::Mat intermediateMat;
	cv::Mat gray;
	Lena.copyTo(gray);
	try{
		int MEDIAN_BLUR_FILTER_SIZE = slider1->Value; //9;
		if (MEDIAN_BLUR_FILTER_SIZE % 2 == 0)
			MEDIAN_BLUR_FILTER_SIZE++;
		medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);
		
		cv::Mat edges;
		int LAPLACIAN_FILTER_SIZE = slider2->Value; //5;
		if (LAPLACIAN_FILTER_SIZE % 2 == 0)
			LAPLACIAN_FILTER_SIZE++;
		Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
		
		cv::Mat mask;
		const int EDGES_THRESHOLD = slider3->Value; //40;
		
		if (chkBin->IsChecked)
			threshold(edges, mask, EDGES_THRESHOLD, 255, cv::THRESH_BINARY_INV);
		else
			threshold(edges, mask, EDGES_THRESHOLD, 255, cv::THRESH_BINARY);

		UpdateImage(mask);
	}
	catch (Exception^ ex){
		Msg(ex->Message);
	}
}


void ImageProcessing::MainPage::Button_Click_3(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	cv::Mat result;
	cv::Mat intermediateMat;
	int nb = 50;
	cv::Ptr<cv::FastFeatureDetector> detector = cv::FastFeatureDetector::create(nb,true,1);
	std::vector<cv::KeyPoint> features;

	Lena.copyTo(result);
	cv::cvtColor(result, intermediateMat, CV_RGBA2GRAY);
	detector->detect(intermediateMat, features);

	for (unsigned int i = 0; i < std::min(features.size(), (size_t)nb); i++)
	{
		const cv::KeyPoint& kp = features[i];
		cv::circle(result, cv::Point((int)kp.pt.x, (int)kp.pt.y), 10, cv::Scalar(0, 0, 255, 255));
	}

	UpdateImage(result);

}


cv::String face_cascade_name = "Assets/haarcascade_frontalface_alt.xml";
cv::CascadeClassifier face_cascade;

cv::String body_cascade_name = "Assets/haarcascade_upperbody.xml"; 
cv::CascadeClassifier body_cascade;

cv::String eyes_cascade_name = "Assets/haarcascade_eye.xml";
cv::CascadeClassifier eyes_cascade;


void internalDetectObjects(cv::Mat& inputImg, std::vector<cv::Rect> & objectVector, std::vector<cv::Rect> & objectVectorBodies, std::vector<cv::Rect>& objectVectorEyes)
{
	cv::Mat frame_gray;

	cvtColor(inputImg, frame_gray, CV_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);

	// Detect faces
	face_cascade.detectMultiScale(frame_gray, objectVector,1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	//detect bodies
	body_cascade.detectMultiScale(frame_gray, objectVectorBodies, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	//detect eyes
	eyes_cascade.detectMultiScale(frame_gray, objectVectorEyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(5, 5));

}

void ImageProcessing::MainPage::InitDetectcion(std::string name){

	
	if (!face_cascade.load(face_cascade_name)) {
		printf("Couldnt load Face detector '%s'\n", face_cascade_name);
		exit(1);
	}

	if (!body_cascade.load(body_cascade_name)) {
		printf("Couldnt load Body detector '%s'\n", body_cascade_name);
		exit(1);
	}

	if (!eyes_cascade.load(eyes_cascade_name)) {
		printf("Couldnt load Eyes detector '%s'\n", eyes_cascade_name);
		exit(1);
	}

	cv::Mat frame = cv::imread(ImgName);
	
	if (frame.empty())
		return ;

	std::vector<cv::Rect> faces;
	std::vector<cv::Rect> bodies;
	std::vector<cv::Rect> eyes;

	internalDetectObjects(frame, faces, bodies,eyes);
	
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		auto face = faces[i];
		
		cv::rectangle(Lena, face, cv::Scalar(0, 255, 255),5);
		
	}

	for (unsigned int i = 0; i < bodies.size(); i++)
	{
		auto body = bodies[i];

		cv::rectangle(Lena, body, cv::Scalar(0, 0, 255), 5);

	}

	for (unsigned int i = 0; i < eyes.size(); i++)
	{
		auto eye = eyes[i];

		cv::circle(Lena, cv::Point( eye.x+eye.width/2,eye.y+eye.height/2),5, cv::Scalar(0, 0, 255), 5);

	}

	UpdateImage(Lena);

}


void ImageProcessing::MainPage::Button_Click_4(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	 	
			InitDetectcion("");
		
}
