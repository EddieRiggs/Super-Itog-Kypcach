#pragma once
#include "functions.h"
#include "structures.h"
#include <tchar.h>

void deleteSelectedEntries(HANDLE &hFile, TCHAR* fileName) {
	HANDLE temp = CreateFile(
		L"temp.bin",
		GENERIC_WRITE | GENERIC_READ,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	
	Entry buffer;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	int position = 0;
	
	while (position != GetFileSize(hFile, NULL)) {
		int NofScannedArguments = 0; NofScannedArguments = ReadFile(hFile, (char*)(&buffer), sizeof(Entry), NULL, NULL);
		if (NofScannedArguments != 1)
		{
			exit(EXIT_FAILURE);
		}
		if (!buffer.toDelete) {
			WriteFile(temp, reinterpret_cast<const char*>(&buffer), sizeof(Entry), NULL, NULL);
		}
		position += sizeof(Entry);
	}
	CloseHandle(hFile);
	CloseHandle(temp);
	DeleteFile(fileName);
	MoveFile(L"temp.bin", fileName);
	hFile = CreateFile(
		fileName,
		GENERIC_READ | GENERIC_WRITE,
		NULL,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
}