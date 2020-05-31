#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <fstream>
#include <iostream>
#include <string>
#include <iomanip>
#include "functions.h"
#include "structures.h"
#include <vector>
#include <sstream>
#include <tchar.h>

using namespace std;

TCHAR* header()
{

	TCHAR str[MAX_PATH];
	swprintf_s(
		str,
		TEXT("%10s|%10s|%20s|%20s|%20s|%12s"),
		L"Ключ",
		L"Возраст",
		L"Первое животное",
		L"Второе животное",
		L"Третье животное",
		L"На удаление");
	return str;
}

TCHAR* entryToString(Entry entry)
{
	TCHAR str[MAX_PATH];
	swprintf_s(
		str,
		TEXT("%10s|%10i|%20s|%20s|%20s|%12s"),
		entry.key,
		entry.age,
		entry.firstAnimal,
		entry.secondAnimal,
		entry.thirdAnimal,
		(entry.toDelete ? L"да" : L"нет"));
	return str;
}

TCHAR* animalToString(Animal animal)
{
	TCHAR str[120];
	swprintf_s(
		str,
		L"%i раз встретилось \"%s\"",
		animal.amount,
		animal.animal);
	return str;
}

void readEntry(HANDLE hFile, TCHAR* key, Entry& entry)
{
	int NofScannedArguments = 0;
	if (findKey(hFile, key)) {
		SetFilePointer(hFile, -(int)sizeof(Entry), NULL, FILE_CURRENT);
		NofScannedArguments = ReadFile(hFile, &entry, (int)sizeof(Entry), NULL, NULL);
		if (NofScannedArguments != 1)
		{
			exit(EXIT_FAILURE);
		}
	}
}

void readAll(HANDLE hFile, vector<Entry>* entriesList)
{
	int NofScannedArguments = 0;
	int position = 0;
	Entry *buffer = new Entry;
	DWORD size = GetFileSize(hFile, NULL);
	DWORD pointer;

	while (true) {
		pointer = SetFilePointer(hFile, position * sizeof(Entry), NULL, FILE_BEGIN);

		if (pointer == size) {
			break;
		}

		NofScannedArguments = ReadFile(hFile, buffer, sizeof(Entry), NULL, NULL);
		if (NofScannedArguments != 1)
		{
			exit(EXIT_FAILURE);
		}
		entriesList->push_back(*buffer);
		position++;
	}

	delete buffer;
}