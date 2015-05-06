#include "pch.h"
#include "BindingData.h"

namespace FaceFinder
{
	std::vector<IRandomAccessStream^> streamVec;
}

void FaceFinder::ImgInfo::addFileToList(StorageFile^ file, IVector<ImgInfo^>^ fileList)
{
	task<IRandomAccessStream^>(file->OpenAsync(FileAccessMode::Read)).then([file, fileList](IRandomAccessStream^ strm)
	{
		streamVec.push_back(strm);

		auto img = ref new BitmapImage();
		img->SetSource(strm);

		fileList->Append(ref new ImgInfo(file, img, file->DisplayName, file->DisplayType ));
	});
}

void FaceFinder::ImgInfo::addPathToList(String^ path, IVector<ImgInfo^>^ fileList)
{
	task<StorageFile^>(StorageFile::GetFileFromPathAsync(path)).then([fileList](StorageFile^ file)
	{
		addFileToList(file, fileList);
	});
}