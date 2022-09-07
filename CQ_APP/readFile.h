#pragma once

#include <windows.h>
#include <string>
using namespace std;

auto getFileSize(const char* fileName)
{
	ULARGE_INTEGER LowPart;
	LowPart.LowPart = GetCompressedFileSizeA(fileName, &LowPart.HighPart);
	return LowPart.QuadPart;
}

char* _readFile(string dir)
{
	char* buffer;
	DWORD size;
	auto hFile = CreateFileA(dir.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) return NULL;
	size = getFileSize(dir.c_str());
	buffer = (char*)malloc(size);
    ZeroMemory(buffer, size);
	ReadFile(hFile, buffer, size, NULL, NULL);
	buffer[size] = '\0';
    CloseHandle(hFile);
	return buffer;
}

/*char* _readFile(string dir,int &size) {
	fstream f;
	f.open(dir, ios::in | ios::binary);
	f.seekg(0, std::ios_base::end);
	std::streampos sp = f.tellg();
	size = sp;
	char* buffer = (char*)malloc(sizeof(char) * size);
	f.read(buffer, size);
	f.close();
	return buffer;
}
*/