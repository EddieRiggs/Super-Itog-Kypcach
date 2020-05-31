#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <vector>

using namespace std;

#include "Resource.h"
#include <Windows.h>
#include "structures.h"

struct Entry;
struct Animal;

// ������� ������ ������ � ����
void writeEntry(HANDLE hFile, Entry newEntry);

// ������� ������ ����� ������
void readEntry(HANDLE hFile, TCHAR* key, Entry& entry);

// ������� ������ ���� �������
void readAll(HANDLE hFile, vector<Entry>* entriesList);

// ������� ������ ����� ������
TCHAR* header();
TCHAR* entryToString(Entry entry);
TCHAR* animalToString(Animal animal);

// ������� ������ ����� � �����
bool findKey(HANDLE hFile, TCHAR* key);

// ������� ����� ����� �� ��������
void changeSelection(HANDLE hFile, TCHAR* key);

// ������� �������� ���������� �������
void deleteSelectedEntries(HANDLE &hFile, TCHAR* fileName);

// ������� �������������� ������
void editEntry(HANDLE hFile, TCHAR* key, Entry newEntry);

// ������� ���������� ����� ���������� ���������
void findMostPopular(HANDLE hFile, vector<Animal>& animals);

// ����������� ��� ������� ������
bool findAnimalInList(TCHAR* animal, vector<Animal>* animals);
vector<Animal> addEntry(Entry entry, vector<Animal> animals);
vector<Animal> getMostPopular(vector<Animal>* animals);