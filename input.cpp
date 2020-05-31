#pragma once
#define _CRT_SECURE_NO_WARNING
#include <fstream>
#include <iostream>
#include <string>
#include "functions.h"
#include "structures.h"

using namespace std;

void writeEntry(HANDLE hFile, Entry newEntry)
{
	SetFilePointer(hFile, 0, NULL, FILE_END);
	WriteFile(hFile, reinterpret_cast<const char*>(&newEntry), sizeof(Entry), NULL, NULL);
}

bool findKey(HANDLE hFile, TCHAR* key)
{
	bool result = false;
	int position = 0;
	Entry* buffer = new Entry;

	while (true) {
		DWORD pointer = SetFilePointer(hFile, position * sizeof(Entry), NULL, FILE_BEGIN);

		if (pointer == GetFileSize(hFile, NULL)) {
			break;
		} int NofScannedArguments = 0;
		NofScannedArguments=ReadFile(hFile, (char*)(buffer), sizeof(Entry), NULL, NULL);
		if (NofScannedArguments != 1)
		{
			exit(EXIT_FAILURE);
		}
		if (!lstrcmp(buffer->key, key)) {
			result = true;
			break;
		}

		position++;
	}

	delete buffer;

	return result;
}