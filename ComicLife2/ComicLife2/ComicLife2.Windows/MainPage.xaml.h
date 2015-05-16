//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"
#include <opencv2\imgproc\types_c.h>

using namespace concurrency;
using namespace Windows::Devices::Enumeration;

namespace ComicLife2
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();
		void LoadImage();
		
	private:
		void UpdateImage(cv::Mat image);
		int frameWidth;
		int frameHeight;
		cv::Mat Lena;
		Platform::Agile<Windows::Media::Capture::MediaCapture> m_mediaCaptureMgr;
		DeviceInformationCollection^ m_devInfoCollection;
		void EnumerateWebcamsAndInitializeFirstOneAsync();
		
		void ShowStatusMessage(Platform::String^ text);
		cv::String ComicLife2::MainPage::ConvertPath(Platform::String^ path);
		void tmpFct();
		void tmpFct2();

		bool m_bPreviewing;
		int mode;

		MainPage^ rootPage;
		Windows::Foundation::Collections::IPropertySet^ appSettings;



	private:
		void btn1_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void btn2_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void btn3_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void btn4_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_1(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void Button_Click_2(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
	};
}
