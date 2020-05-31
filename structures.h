#pragma once

struct Entry {
    int age;
    TCHAR firstAnimal[20];
    TCHAR secondAnimal[20];
    TCHAR thirdAnimal[20];
    TCHAR key[20];
    bool toDelete;
};

struct Animal {
    int amount;
    TCHAR animal[20];
};