//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"
#include <Robuffer.h> //comptr etc
#include <opencv2\core\core.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\imgproc.hpp>

#include <opencv2\imgproc\types_c.h>
#include <opencv2\imgcodecs\imgcodecs.hpp>

#include <opencv2\objdetect.hpp>



using namespace ComicLife2;

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

using namespace Windows::Storage::Streams;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Graphics::Imaging;
using namespace Microsoft::WRL;
using namespace concurrency;
using namespace Windows::Storage::Streams;

using namespace Windows::Storage;
using namespace Windows::Storage::Pickers;

using namespace Windows::Media;
using namespace Windows::Media::Capture;


// The Blank Page item template is documented at http://go.microsoft.com/fwlink/?LinkId=234238

ComicLife2::MainPage::MainPage()
{
	InitializeComponent();

	m_bPreviewing = false;
	mode = -1;
	appSettings = ApplicationData::Current->LocalSettings->Values;
	/*rootPage = MainPage::BottomAppBar:*/

}



Uri^ InputImageUri = ref new Uri(L"ms-appx:///Assets/stars.png");
void ComicLife2::MainPage::LoadImage()
{
	RandomAccessStreamReference^ streamRef = RandomAccessStreamReference::CreateFromUri(InputImageUri);

	task<IRandomAccessStreamWithContentType^>(streamRef->OpenReadAsync()).
		then([](task<IRandomAccessStreamWithContentType^> thisTask)
	{
		IRandomAccessStreamWithContentType^ fileStream = thisTask.get();
		return BitmapDecoder::CreateAsync(fileStream);
	}).
		then([](task<BitmapDecoder^> thisTask)
	{
		BitmapDecoder^ decoder = thisTask.get();
		return decoder->GetFrameAsync(0);
	}).
		then([this](task<BitmapFrame^> thisTask)
	{
		BitmapFrame^ frame = thisTask.get();

		// Save some information as fields
		frameWidth = frame->PixelWidth;
		frameHeight = frame->PixelHeight;

		return frame->GetPixelDataAsync();
	}).
		then([this](task<PixelDataProvider^> thisTask)
	{
		PixelDataProvider^ pixelProvider = thisTask.get();
		Platform::Array<byte>^ srcPixels = pixelProvider->DetachPixelData();
		Lena = cv::Mat(frameHeight, frameWidth, CV_8UC4);
		memcpy(Lena.data, srcPixels->Data, 4 * frameWidth*frameHeight);
		UpdateImage(Lena);
	});

}

void MainPage::UpdateImage(cv::Mat image)
{
	
		// Create the WriteableBitmap
		WriteableBitmap^ bitmap = ref new WriteableBitmap(image.cols, image.rows);

		// Get access to the pixels
		IBuffer^ buffer = bitmap->PixelBuffer;
		unsigned char* dstPixels;

		// Obtain IBufferByteAccess
		ComPtr<IBufferByteAccess> pBufferByteAccess;
		ComPtr<IUnknown> pBuffer((IUnknown*)buffer);
		pBuffer.As(&pBufferByteAccess);

		// Get pointer to pixel bytes
		pBufferByteAccess->Buffer(&dstPixels);
		memcpy(dstPixels, image.data, 4 * image.cols*image.rows);

		// Set the bitmap to the Image element
		img->Source = bitmap; 

}



void ComicLife2::MainPage::btn1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//Load Image
	
	//LoadImage();

	cv::Mat image = cv::imread("Assets/grpPC1.jpg");
	Lena = cv::Mat(image.rows, image.cols, CV_8UC4);
	cvtColor(image, Lena, CV_BGR2BGRA);
	UpdateImage(Lena);
}

cv::Mat result;
void ComicLife2::MainPage::btn2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//apply effect on Image
	
	cv::Mat intermediateMat;
	
	cv::cvtColor(Lena, intermediateMat, CV_RGBA2GRAY);
	cv::cvtColor(intermediateMat, result, CV_GRAY2BGRA);
	
	//or apply a filter
	//cv::Mat result;
	//cv::Canny(Lena, intermediateMat, 80, 90);
	//cv::cvtColor(intermediateMat, result, CV_GRAY2BGRA);

	UpdateImage(result);
}

cv::String ComicLife2::MainPage::ConvertPath(Platform::String^ path) {
	std::wstring localPathW(path->Begin());

	// Opt #1
	//std::string localPath(localPathW.begin(), localPathW.end());

	// Opt #2
	//std::string localPath(StrToWStr(localPathW));

	// Opt #3
	size_t outSize = localPathW.length() + 1;
	char* localPathC = new char[outSize];
	size_t charsConverted = 0;
	wcstombs_s(&charsConverted, localPathC, outSize, localPathW.c_str(), localPathW.length());
	cv::String localPath(localPathC);

	// Implicit conversion from std::string to cv::String
	return localPath;
}

void ComicLife2::MainPage::btn3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{

	//Save Image
	StorageFolder^ localFolderRT = ApplicationData::Current->LocalFolder;
	cv::String localFile = ConvertPath(ApplicationData::Current->LocalFolder->Path) + "\\MyLena.png";
	auto x = cv::imwrite(localFile, result);

	
	
} 

void ComicLife2::MainPage::ShowStatusMessage(Platform::String^ text)
{
	txtMsg->Text += "\n" + text;
}

void ComicLife2::MainPage::EnumerateWebcamsAndInitializeFirstOneAsync()
{
	try
	{
		ShowStatusMessage("Enumerating Webcams...");
		m_devInfoCollection = nullptr;

		task<DeviceInformationCollection^>(DeviceInformation::FindAllAsync(DeviceClass::VideoCapture)).then([this](task<DeviceInformationCollection^> findTask)
		{
			try
			{
				m_devInfoCollection = findTask.get();
				if (m_devInfoCollection == nullptr || m_devInfoCollection->Size == 0)
				{
					ShowStatusMessage("No WebCams found.");
				}
				else
				{
					for (unsigned int i = 0; i < m_devInfoCollection->Size; i++)
					{
						auto devInfo = m_devInfoCollection->GetAt(i);
						ShowStatusMessage(devInfo->Name);
					}
					ShowStatusMessage("Enumerating Webcams completed successfully.");
					
				}
			}
			catch (Exception ^e)
			{
				ShowStatusMessage(e->Message);
			}
		}).then([this](){
		
			auto mediaCapture = ref new Windows::Media::Capture::MediaCapture();
			m_mediaCaptureMgr = mediaCapture;
			auto settings = ref new Windows::Media::Capture::MediaCaptureInitializationSettings();
			auto devInfo = m_devInfoCollection->GetAt(0);
			settings->VideoDeviceId = devInfo->Id;

			create_task(mediaCapture->InitializeAsync(settings)).then([this](task<void> initTask)
			{
				try
				{
					initTask.get();
					ShowStatusMessage("Initialize done");
				}
				catch (Exception ^ e)
				{
					ShowStatusMessage(e->Message);
				}
			});

		});
	}
	catch (Platform::Exception^ e)
	{
		ShowStatusMessage(e->Message);
	}
}


void ComicLife2::MainPage::btn4_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	EnumerateWebcamsAndInitializeFirstOneAsync();
}


void ComicLife2::MainPage::Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//Test button

	
	//Windows::Media::Capture::CameraCaptureUI MCUI;
	
	try
	{
		CameraCaptureUI^ dialog = ref new CameraCaptureUI();

		dialog->PhotoSettings->CroppedAspectRatio = Size(16, 9);

		concurrency::task<StorageFile^>(dialog->CaptureFileAsync(CameraCaptureUIMode::Photo)).then([this](StorageFile^ file)
		{
			if (nullptr != file)
			{
				concurrency::task<Streams::IRandomAccessStream^>(file->OpenAsync(FileAccessMode::Read)).then([this](Streams::IRandomAccessStream^ stream)
				{
					BitmapImage^ bitmapImage = ref new BitmapImage();
					bitmapImage->SetSource(stream);
					this->img->Source = bitmapImage;



					WriteableBitmap^  wb = ref new WriteableBitmap(bitmapImage->PixelHeight, bitmapImage->PixelWidth);
					wb->SetSource(stream);

					

					//Bug bug
				
					Lena = cv::Mat(bitmapImage->PixelHeight, bitmapImage->PixelWidth, CV_8UC4);
					memcpy(Lena.data, (const void*)stream, 4 * bitmapImage->PixelHeight* bitmapImage->PixelWidth);
					
					auto x = wb->PixelBuffer->Length;
					auto y = 4 * bitmapImage->PixelHeight* bitmapImage->PixelWidth;
					memcpy(Lena.data, (const void*)wb->PixelBuffer, 4 * bitmapImage->PixelHeight* bitmapImage->PixelWidth);


					//rootPage->NotifyUser("", NotifyType::StatusMessage);
				});

				// Store the path in Application Data
				appSettings->Insert("capturedPhoto", PropertyValue::CreateString(file->Path));
			}
			else
			{
				//rootPage->NotifyUser("No photo captured", NotifyType::ErrorMessage);
			}
		});

	}
	catch (Platform::Exception^ ex)
	{
		//rootPage->NotifyUser(ex->Message, NotifyType::ErrorMessage);
	}

	
	
	
	//tmpFct();

	/*StorageFile localfile = ApplicationData::Current->LocalFolder->;
	auto encoder = ref new  Windows::Media::MediaProperties::ImageEncodingProperties();
	encoder->CreateJpeg;

	preview1->Source->CapturePhotoToStorageFileAsync(encoder, localFile);*/

}



void ComicLife2::MainPage::Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	//Effect on Video
	//Will load the dll
	auto mediaCapture = m_mediaCaptureMgr.Get();
	
	
	if (m_bPreviewing == false){

		preview1->Source = mediaCapture;
		create_task(mediaCapture->StartPreviewAsync()).then([this](task<void> previewTask)
		{
			try
			{
				previewTask.get();
				m_bPreviewing = true;
				ShowStatusMessage("Start preview successful");
			}
			catch (Exception ^e)
			{
				ShowStatusMessage(e->Message);
			}
		});
		
	}
	
	//m_mediaCaptureMgr->StartPreviewAsync();

	create_task(m_mediaCaptureMgr->ClearEffectsAsync(Windows::Media::Capture::MediaStreamType::VideoPreview)).then([this](task<void> cleanTask)
	{
		//cleanTask.get();

	}).then([this](){

						PropertySet^ props = ref new PropertySet();
						props->Insert(L"{698649BE-8EAE-4551-A4CB-3EC98FBD3D86}", mode);
						create_task(m_mediaCaptureMgr->AddEffectAsync(Windows::Media::Capture::MediaStreamType::VideoPreview, "OcvTransform.OcvImageManipulations", props)).then([this](task<void> effectTask)
						{
			
							try
							{
								//effectTask.get();
														

							}
							catch (Exception ^e)
							{
								ShowStatusMessage(e->Message);

							}
						});
	});


	mode++;
	if (mode == 6)
		mode = 0;
}


void ComicLife2::MainPage::tmpFct(){

	auto mediaCapture = m_mediaCaptureMgr.Get();
	preview1->Source = mediaCapture;

	create_task(mediaCapture->StartPreviewAsync()).then([this](task<void> previewTask)
	{
		try
		{
			previewTask.get();

			ShowStatusMessage("Start preview successful");
		}
		catch (Exception ^e)
		{
			ShowStatusMessage(e->Message);
		}
	});


}


void ComicLife2::MainPage::tmpFct2(){
	auto mediaCapture = m_mediaCaptureMgr.Get();

	//Windows::Media::Capture::VideoDeviceCharacteristic charecteristic = mediaCapture->MediaCaptureSettings->VideoDeviceCharacteristic;

	create_task(m_mediaCaptureMgr->ClearEffectsAsync(Windows::Media::Capture::MediaStreamType::VideoPreview)).then([this](task<void> cleanTask)
	{
		preview1->Source = m_mediaCaptureMgr.Get();



		PropertySet^ props = ref new PropertySet();
		props->Insert(L"{698649BE-8EAE-4551-A4CB-3EC98FBD3D86}", 2);
		create_task(m_mediaCaptureMgr->AddEffectAsync(Windows::Media::Capture::MediaStreamType::VideoPreview, "OcvTransform.OcvImageManipulations", props)).then([this](task<void> effectTask)
		{

			try
			{
				effectTask.get();
				auto mediaCapture = m_mediaCaptureMgr.Get();
				m_mediaCaptureMgr->StartPreviewAsync();
				/*task<void>(mediaCapture->AddEffectAsync(Windows::Media::Capture::MediaStreamType::VideoRecord, "OcvTransform.OcvImageManipulations", nullptr)).then([this](task<void> effectTask2)
				{
				try
				{
				effectTask2.get();
				ShowStatusMessage("Add effect successful to record stream successful");

				AddEffectToImageStream();

				}
				catch (Exception ^e)
				{
				ShowStatusMessage(e->Message);

				}
				});
				*/

			}
			catch (Exception ^e)
			{
				ShowStatusMessage(e->Message);

			}
		});
	});

}


cv::String face_cascade_name = "Assets/haarcascade_frontalface_alt.xml";
cv::CascadeClassifier face_cascade;


void ComicLife2::MainPage::Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	
	if (!face_cascade.load(face_cascade_name)) {
		printf("Couldnt load Face detector '%s'\n", face_cascade_name);
		exit(1);
	}

	cv::Mat frame = cv::imread("Assets/grpPC1.jpg");

	if (frame.empty())
		return;

	std::vector<cv::Rect> faces;
	cv::Mat frame_gray;

	cvtColor(frame, frame_gray, CV_BGR2GRAY);
	cv::equalizeHist(frame_gray, frame_gray);

	// Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		auto face = faces[i];

		cv::rectangle(Lena, face, cv::Scalar(0, 255, 255), 5);

	}

	UpdateImage(Lena);
}
