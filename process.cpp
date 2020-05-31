#pragma once
#include "functions.h"
#include "structures.h"

void findMostPopular(HANDLE hFile, vector<Animal>& animals)
{
	int NofScannedArguments = 0;
	Entry* bufferEntry = new Entry;
	SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
	int position = 0;

	while (position != GetFileSize(hFile, NULL)) {
		NofScannedArguments = ReadFile(hFile, (char*)(&bufferEntry), sizeof(Entry), NULL, NULL);
		if (NofScannedArguments != 1)
		{
			exit(EXIT_FAILURE); 
		}
		animals = addEntry(*bufferEntry, animals);
		position += sizeof(Entry);
	}

	animals = getMostPopular(&animals); delete bufferEntry;
}

bool findAnimalInList(TCHAR* animal, vector<Animal>* animals)
{
	if (animals->size() > 0) {
		for (int i = 0; i < animals->size(); i++)
		{
			if (lstrcmp(animals->at(i).animal, animal) == 0)
			{
				animals->at(i).amount++;
				return true;
			}
		}
	}
	return false;
}

vector<Animal> addEntry(Entry entry, vector<Animal> animals)
{
	
	Animal newAnimal;
	newAnimal.amount = 1;

	if (!findAnimalInList(entry.firstAnimal, &animals))
	{
		lstrcpy(newAnimal.animal, entry.firstAnimal);
		(&animals)->push_back(newAnimal);
	}

	if (!findAnimalInList(entry.secondAnimal, &animals))
	{
		lstrcpy(newAnimal.animal, entry.secondAnimal);
		(&animals)->push_back(newAnimal);
	}

	if (!findAnimalInList(entry.thirdAnimal, &animals))
	{
		lstrcpy(newAnimal.animal, entry.thirdAnimal);
		(&animals)->push_back(newAnimal);
	}

	return animals;
}

vector<Animal> getMostPopular(vector<Animal>* animals)
{
	int max[3] = { 0 };

	for (int i = 0; i < animals->size(); i++)
	{
		if (animals->at(i).amount >= max[0]) {
			max[0] = animals->at(i).amount;
		}
		else if (animals->at(i).amount >= max[1]) {
			max[1] = animals->at(i).amount;
		}
		else if (animals->at(i).amount >= max[2]) {
			max[2] = animals->at(i).amount;
		}
	}

	vector<Animal> mostPopular;

	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < animals->size(); i++)
		{
			if (animals->at(i).amount == max[j]) {
				mostPopular.push_back(animals->at(i));
			}
		}
	}
	
	return mostPopular;
}