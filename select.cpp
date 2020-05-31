#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "functions.h"
#include "structures.h"
using namespace std;

void changeSelection(HANDLE hFile, TCHAR* key)
{
	Entry newEntry;
	readEntry(hFile, key, newEntry);
	newEntry.toDelete = newEntry.toDelete ? false : true;
	SetFilePointer(hFile, -(int)sizeof(Entry), NULL, FILE_CURRENT);
	WriteFile(hFile, reinterpret_cast<const char*>(&newEntry), sizeof(Entry), NULL, NULL);
}