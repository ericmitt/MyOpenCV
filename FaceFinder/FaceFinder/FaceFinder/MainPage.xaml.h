//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

namespace FaceFinder
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class MainPage sealed
	{
	public:
		MainPage();
		property IVector<ImgInfo^>^ InputFileList;
		property IVector<ImgInfo^>^ FaceFileList;

	protected:
		virtual void OnNavigatedTo(Windows::UI::Xaml::Navigation::NavigationEventArgs^ e) override;
	private:
		void loadButton_Click(::Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void CopySelectedFiles();
		void detectButton_Click(::Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
		void shareButton_Click(::Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);

		Vector<StorageFile^>^ InputFilesVector;
		std::vector<task<StorageFile^>> m_copyTasks;
	};
}
