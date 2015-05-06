#include "pch.h"
#include "Class1.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgcodecs/imgcodecs.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

#include <filesystem>
#include <iostream>
#include <ostream>
#include <regex>

#include <string>
#include <ppltasks.h> 

using namespace winrtfacedetect;

WinRTComponent::WinRTComponent()
{
	auto dataFolderPath = ApplicationData::Current->TemporaryFolder->Path;
	if (initializeFaceDetector(dataFolderPath->Data()) == -1)
		throw ref new FailureException();
}

IAsyncActionWithProgress<String^>^ WinRTComponent::ExtractFacesFromFilesAsync(
	IVector<StorageFile^>^ inFileVector)
{
	auto pathVec = getPathsFromFiles(inFileVector);
	return create_async([pathVec, inFileVector](progress_reporter<String^> progress)
	{
		if (inFileVector->Size == 0)
			throw ref new InvalidArgumentException("Why are you doing this to me???");

		std::for_each(begin(pathVec), end(pathVec), [progress](String^ file)
		{
			std::vector<std::wstring> outputFaces;
			detectAndSaveFaces(file->Data(), outputFaces);

			std::for_each(begin(outputFaces), end(outputFaces), [progress](std::wstring face)
			{
				progress.report(ref new String(face.c_str(), face.size()));
			});
		});
	});
}

IVector<String^>^ WinRTComponent::SearchFiles(
	String^ query)
{
	std::vector<std::wstring> outputFaces;
	searchFiles(query->Data(), outputFaces);

	auto facevec = ref new Vector<String^>();
	std::for_each(begin(outputFaces), end(outputFaces), [facevec](std::wstring face)
	{
		facevec->Append(ref new String(face.c_str(), face.size()));
	});

	return facevec;
}

Vector<String^>^ WinRTComponent::getPathsFromFiles(
	IVector<StorageFile^>^ inFileVector)
{
	auto pathVec = ref new Vector<String^>();
	std::for_each(begin(inFileVector), end(inFileVector), [pathVec](StorageFile^ file)
	{
		pathVec->Append(file->Path);
	});

	return pathVec;
}

namespace winrtfacedetect
{
	cv::String face_cascade_name = "haarcascade_frontalface_alt.xml";
	cv::String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;

	std::wstring tmpDataFolder;

	int initializeFaceDetector(std::wstring dataFolder)
	{
		tmpDataFolder = dataFolder;

		// Load the cascades
		if (!face_cascade.load(face_cascade_name))
			return -1;
		if (!eyes_cascade.load(eyes_cascade_name))
			return -1;

		return 0;
	}

	void internaDetectObjects(cv::Mat& inputImg, std::vector<cv::Rect> & objectVector)
	{
		cv::Mat frame_gray;

		cvtColor(inputImg, frame_gray, CV_BGR2GRAY);
		cv::equalizeHist(frame_gray, frame_gray);

		// Detect faces
		face_cascade.detectMultiScale(frame_gray, objectVector, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));
	}

	bool searchFiles(std::wstring wSearchTerm, std::vector<std::wstring>& searchResults)
	{
		bool ret = false;

		std::string searchTerm(wSearchTerm.begin(), wSearchTerm.end());
		const std::regex r(searchTerm);

		for (std::tr2::sys::recursive_directory_iterator i(tmpDataFolder), end; i != end; ++i)
		{
			const std::string filename = i->path().filename().string();

			if (std::regex_search(filename, r))
			{
				ret = true;

				std::string tmpFace = i->path().string();
				std::wstring wtmpFace(tmpFace.begin(), tmpFace.end());
				searchResults.push_back(wtmpFace);
			}
		}
		return ret;
	}

	int detectAndSaveFaces(std::wstring inputFile, std::vector<std::wstring> &outputFaces)
	{
		int ret = 0;

		std::string inFile(inputFile.begin(), inputFile.end());

		cv::Mat frame = cv::imread(inFile);
		if (frame.empty())
			return -1;

		std::vector<cv::Rect> faces;
		internaDetectObjects(frame, faces);

		for (unsigned int i = 0; i < faces.size(); i++)
		{
			auto face = faces[i];

			std::string tmpFace = inFile;
			auto location = tmpFace.find_last_of('.');
			std::string facenum = "_face_" + std::to_string(i);
			tmpFace.insert(location, facenum);

			cv::Mat facemat = frame(face);
			cv::imwrite(tmpFace, facemat);

			std::wstring wtmpFace(tmpFace.begin(), tmpFace.end());
			outputFaces.push_back(wtmpFace);
		}

		return ret;
	}
}