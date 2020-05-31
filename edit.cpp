#pragma once
#include "functions.h"
#include "structures.h"

using namespace std;

void editEntry(HANDLE hFile, TCHAR* key, Entry newEntry)
{
	
	if (findKey(hFile, key)) {

		SetFilePointer(hFile, -(int)sizeof(Entry), NULL, FILE_CURRENT);
		WriteFile(hFile, reinterpret_cast<const char*>(&newEntry), sizeof(Entry), NULL, NULL);
	}
}