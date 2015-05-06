//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace FaceFinder;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Foundation::Metadata;
using namespace Windows::ApplicationModel;
using namespace concurrency;

MainPage::MainPage()
{
	InitializeComponent();

	InputFileList = ref new Vector<ImgInfo^>();
	FaceFileList = ref new Vector<ImgInfo^>();

	this->DataContext = this;

	InputFilesVector = ref new Vector<StorageFile^>();
}

void MainPage::OnNavigatedTo(NavigationEventArgs^ e)
{
}

void MainPage::loadButton_Click(::Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	auto openPicker = ref new FileOpenPicker();
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
	openPicker->ViewMode = PickerViewMode::Thumbnail;
	openPicker->FileTypeFilter->Append("*");

	task<IVectorView<StorageFile^>^>(openPicker->PickMultipleFilesAsync()).then([this](IVectorView<StorageFile^>^ fileVector)
	{
		InputFileList->Clear();
		InputFilesVector->Clear();
		m_copyTasks.clear();

		for (auto file : fileVector)
			ImgInfo::addFileToList(file, InputFileList);

		for (auto file : fileVector)
		{
			m_copyTasks.push_back(create_task(file->CopyAsync(ApplicationData::Current->TemporaryFolder, file->Name, NameCollisionOption::ReplaceExisting)));
		}

		CopySelectedFiles();
	});
}

void MainPage::CopySelectedFiles()
{
	when_all(begin(m_copyTasks), end(m_copyTasks)).then([this](std::vector<StorageFile^> results)
	{
		for (auto copiedFile : results)
			InputFilesVector->Append(copiedFile);
	});
}

void MainPage::detectButton_Click(::Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
	if (InputFilesVector->Size != 0)
	{
		FaceFileList->Clear();

		auto faceDetectComponent = ref new winrtfacedetect::WinRTComponent();

		auto action = faceDetectComponent->ExtractFacesFromFilesAsync(InputFilesVector);
		action->Progress = ref new AsyncActionProgressHandler<String^>([this](IAsyncActionWithProgress<String^>^ act,
			String^ progress)
		{
			ImgInfo::addPathToList(progress, FaceFileList);
		});

		task<void>(action).then([this]()
		{
		});
	}
	else
	{
		auto msgDlg = ref new Windows::UI::Popups::MessageDialog("Please select some images first", "Error!");
		msgDlg->ShowAsync();
	}
}