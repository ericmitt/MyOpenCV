#pragma once

namespace FaceFinder
{
	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class ImgInfo sealed
	{
	public:
		ImgInfo(
			Windows::Storage::StorageFile^ storageFile, 
			Windows::UI::Xaml::Media::Imaging::BitmapImage^ bitmapImage,
			Platform::String^ name,
			Platform::String ^desc)
		{
			File = storageFile;
			SrcImage = bitmapImage;
			Title = name;
			Description = desc;
		}

		property Windows::Storage::StorageFile^ File;
		property Windows::UI::Xaml::Media::Imaging::BitmapImage^ SrcImage;
		property Platform::String^ Title;
		property Platform::String^ Description;

	internal:
		static void addFileToList(Windows::Storage::StorageFile^ file, Windows::Foundation::Collections::IVector<ImgInfo^>^ fileList);
		static void addPathToList(Platform::String^ path, Windows::Foundation::Collections::IVector<ImgInfo^>^ fileList);
	};
}