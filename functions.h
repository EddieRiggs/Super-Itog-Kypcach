#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>

using namespace std;

#include "Resource.h"
#include <Windows.h>
#include "structures.h"

struct Entry;
struct Animal;

// Функция записи строки в файл
void writeEntry(HANDLE hFile, Entry newEntry);

// Функция чтения одной записи
void readEntry(HANDLE hFile, TCHAR* key, Entry& entry);

// Функция чтения всех записей
void readAll(HANDLE hFile, vector<Entry>* entriesList);

// Функция вывода одной записи
TCHAR* header();
TCHAR* entryToString(Entry entry);
TCHAR* animalToString(Animal animal);

// Функция поиска ключа в файле
bool findKey(HANDLE hFile, TCHAR* key);

// Функция смены меток на удаление
void changeSelection(HANDLE hFile, TCHAR* key);

// Функция удаления помеченных записей
void deleteSelectedEntries(HANDLE &hFile, TCHAR* fileName);

// Функция редактирования записи
void editEntry(HANDLE hFile, TCHAR* key, Entry newEntry);

// Функция нахождения самых популярных животинок
void findMostPopular(HANDLE hFile, vector<Animal>& animals);

// Обработчики для функции поиска
bool findAnimalInList(TCHAR* animal, vector<Animal>* animals);
vector<Animal> addEntry(Entry entry, vector<Animal> animals);
vector<Animal> getMostPopular(vector<Animal>* animals);