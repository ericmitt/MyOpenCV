#pragma once

#include <string>
#include <collection.h>


namespace winrtfacedetect
{
	public ref class WinRTComponent sealed
	{
	public:
		WinRTComponent();

		IAsyncActionWithProgress<String^>^ ExtractFacesFromFilesAsync(
			IVector<StorageFile^>^ inFileVector);

		IVector<String^>^ SearchFiles(
			String^ query);

	private:
		Vector<String^>^ getPathsFromFiles(
			IVector<StorageFile^>^ inFileVector);
	};

	int initializeFaceDetector(std::wstring dataFolder);

	// Detect faces in the file "inputFile" and saves each face in a separate file.  The vector "outputFaces" contains the paths of output files.
	int detectAndSaveFaces(std::wstring inputFile, std::vector<std::wstring> &outputFaces);

	// Search if there are files or "face-files" matching the string searchTerm
	bool searchFiles(std::wstring wSearchTerm, std::vector<std::wstring>& searchResults);
}