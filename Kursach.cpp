// Kursach.cpp : Определяет точку входа для приложения.
//
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include "Resource.h"
#include "Kursach.h"
#include "structures.h"
#include "functions.h"
#include <stdio.h>
#include <commctrl.h>

#define MAX_LOADSTRING 100

// Глобальные переменные:
TCHAR path[MAX_PATH];                           // путь к файлу
HANDLE hFile;                                   // хэндл файла
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    AddEntry(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    OpenFile(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SearchForEntry(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    EditEntry(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    GetTop(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    ShowAll(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_KURSACH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_KURSACH));
    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KURSACH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_KURSACH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(
       szWindowClass,
       szTitle,
       WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
       200,
       100,
       400,
       100,
       nullptr,
       nullptr,
       hInstance,
       nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_DELETE: // удалить помеченные
                deleteSelectedEntries(hFile, path);
                MessageBox(hWnd, L"Записи удалены", L"Успех", MB_ICONINFORMATION);
                break;
            case ID_SHOWALL: // все записи
                DialogBox(hInst, MAKEINTRESOURCE(IDD_TABLEOUTPUT), hWnd, ShowAll);
                break;
            case IDM_GETTOP: // список самых популярных
                DialogBox(hInst, MAKEINTRESOURCE(IDD_TABLEOUTPUT), hWnd, GetTop);
                break;
            case IDM_LOAD: // загрузка файла
                DialogBox(hInst, MAKEINTRESOURCE(IDD_OPENFILE), hWnd, OpenFile);
                break;
            case ID_SEARCHSHOW: // поиск по ключу и показ
                DialogBox(hInst, MAKEINTRESOURCE(IDD_SEARCH), hWnd, SearchForEntry);
                break;
            case ID_SEARCHEDIT: // поиск и редактирование
                DialogBox(hInst, MAKEINTRESOURCE(IDD_EDITENTRY), hWnd, EditEntry);
                break;
            case IDM_ADDREC: // добавить запись
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ADDRECORD), hWnd, AddEntry);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_OPENFILE), hWnd, OpenFile);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
    {
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "Редактирование"
INT_PTR CALLBACK EditEntry(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_FIND:
        {
            EnableWindow(GetDlgItem(hDlg, IDC_KEYENTEREDITOR), FALSE);
            TCHAR buf[20];
            GetDlgItemText(hDlg, IDC_KEYENTEREDITOR, buf, 20);
            Entry foundEntry = { 0 };
            if (!hFile == NULL) {
                readEntry(hFile, buf, foundEntry);
                if (lstrcmp(buf, foundEntry.key) == 0) {
                    SetDlgItemText(hDlg, IDC_FIRSTANIMALEDITOR, foundEntry.firstAnimal);
                    SetDlgItemText(hDlg, IDC_SECONDANIMALEDITOR, foundEntry.secondAnimal);
                    SetDlgItemText(hDlg, IDC_THIRDANIMALEDITOR, foundEntry.thirdAnimal);
                    _itot(foundEntry.age, buf, 10);
                    SetDlgItemText(hDlg, IDC_AGEEDITOR, buf);
                }
            }
            else {
                MessageBox(hDlg, L"Файл не загружен", L"Ошибка", MB_ICONERROR | MB_OK);
            }
            break;
        }
        case IDEDITRECBTN:
        {
            Entry newEntry = { 0 };
            TCHAR buf[20];
            GetDlgItemText(hDlg, IDC_AGEEDITOR, buf, 20);
            newEntry.age = _ttoi(buf);
            GetDlgItemText(hDlg, IDC_FIRSTANIMALEDITOR, newEntry.firstAnimal, 20);
            GetDlgItemText(hDlg, IDC_SECONDANIMALEDITOR, newEntry.secondAnimal, 20);
            GetDlgItemText(hDlg, IDC_THIRDANIMALEDITOR, newEntry.thirdAnimal, 20);
            if (lstrcmp(newEntry.firstAnimal, newEntry.secondAnimal) == 0 ||
                lstrcmp(newEntry.thirdAnimal, newEntry.secondAnimal) == 0 ||
                lstrcmp(newEntry.firstAnimal, newEntry.thirdAnimal) == 0) {
                MessageBox(hDlg, L"Не может быть одинаковых животных", L"Предупреждение", MB_ICONEXCLAMATION);
            }
            else {
                GetDlgItemText(hDlg, IDC_KEYENTEREDITOR, newEntry.key, 20);
                editEntry(hFile, newEntry.key, newEntry);
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        case IDCANCELEDITBTN:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            break;
        default:
            return DefWindowProc(hDlg, message, wParam, lParam);
        }
        break;

    }
    return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "Поиск записи"
INT_PTR CALLBACK SearchForEntry(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDTODELETE:
        {
            TCHAR key[20];
            GetDlgItemText(hDlg, IDC_KEYFINDENTER, key, 20);
            changeSelection(hFile, key);
            Entry foundEntry = { 0 };
            if (!hFile == NULL) {
                readEntry(hFile, key, foundEntry);
                if (lstrcmp(key, foundEntry.key) == 0) {
                    SetDlgItemText(hDlg, IDC_TODELETE, (foundEntry.toDelete ? L"да" : L"нет"));
                }
            }
            else {
                MessageBox(hDlg, L"Файл не загружен", L"Ошибка", MB_ICONERROR | MB_OK);
            }
            break;
        }
        case IDCANCELSEARCH:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDC_FIND:
        {
            TCHAR buf[20];
            GetDlgItemText(hDlg, IDC_KEYFINDENTER, buf, 20);
            Entry foundEntry = { 0 };
            if (!hFile == NULL) {
                readEntry(hFile, buf, foundEntry);
                if (lstrcmp(buf, foundEntry.key) == 0) {
                    SetDlgItemText(hDlg, IDC_KEYEDIT, foundEntry.key);
                    SetDlgItemText(hDlg, IDC_FIRSTANIMALEDIT, foundEntry.firstAnimal);
                    SetDlgItemText(hDlg, IDC_SECONDANIMALEDIT, foundEntry.secondAnimal);
                    SetDlgItemText(hDlg, IDC_THIRDANIMALEDIT, foundEntry.thirdAnimal);
                    _itot(foundEntry.age, buf, 10);
                    SetDlgItemText(hDlg, IDC_AGEEDIT, buf);
                    SetDlgItemText(hDlg, IDC_TODELETE, (foundEntry.toDelete ? L"да" : L"нет"));
                }
            }
            else {
                MessageBox(hDlg, L"Файл не загружен", L"Ошибка", MB_ICONERROR | MB_OK);
            }
            break;
        }
        default:
            return DefWindowProc(hDlg, message, wParam, lParam);
        }
    }
    return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "Все записи"
INT_PTR CALLBACK ShowAll(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND hWndList = GetDlgItem(hDlg, IDC_LIST);
        TCHAR str[MAX_PATH];
        HFONT hFont = CreateFont(
            15,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            DEFAULT_CHARSET,
            0,
            0,
            0, 
            0,
            _T("Consolas"));
        SendMessage(hWndList, WM_SETFONT, (WPARAM)hFont, TRUE);
        vector<Entry> entries;
        readAll(hFile, &entries);
        lstrcpy(str, header());
        SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)str);
        for (Entry entry : entries) {
            lstrcpy(str, entryToString(entry));
            SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)str);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCLOSETABLE || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "Список самых популярных"
INT_PTR CALLBACK GetTop(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
    {
        HWND hWndList = GetDlgItem(hDlg, IDC_LIST);
        HFONT hFont = CreateFont(
            15,
            0,
            0,
            0,
            0,
            0,
            0,
            0,
            DEFAULT_CHARSET,
            0,
            0,
            0,
            0,
            _T("Consolas"));
        SendMessage(hWndList, WM_SETFONT, (WPARAM)hFont, TRUE);
        vector<Animal> entries;
        findMostPopular(hFile, entries);
        TCHAR str[MAX_PATH];
        for (Animal animal : entries) {
            lstrcpy(str, animalToString(animal));
            SendMessage(hWndList, LB_ADDSTRING, 0, (LPARAM)str);
        }
        return (INT_PTR)TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDCLOSETABLE || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
    }
    return (INT_PTR)FALSE;
}

// Обработчик для окна ввода файла
INT_PTR CALLBACK OpenFile(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDOPEN:
        {
            GetDlgItemText(hDlg, IDC_FILEPATH, path, MAX_PATH);
            hFile = CreateFile(
                path,
                GENERIC_WRITE | GENERIC_READ,
                NULL,
                NULL,
                OPEN_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
                MessageBox(hDlg, L"Не удалось открыть файл", L"Ошибка", MB_ICONERROR | MB_OK);
            }
            
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        case IDNOTOPEN:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        default:
            return DefWindowProc(hDlg, message, wParam, lParam);
        }
    }
    return (INT_PTR)FALSE;
}

// Обработчик сообщений для окна "Добавить запись".
INT_PTR CALLBACK AddEntry(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case IDCANCEL:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        case IDADDRECBTN:
        {
            Entry newEntry = { 0 };
            TCHAR buff[20];
            GetDlgItemText(hDlg, IDC_AGE, buff, 20);
            newEntry.age = _ttoi(buff);
            GetDlgItemText(hDlg, IDC_FIRSTANIMAL, newEntry.firstAnimal, 20);
            GetDlgItemText(hDlg, IDC_SECONDANIMAL, newEntry.secondAnimal, 20);
            GetDlgItemText(hDlg, IDC_THIRDANIMAL, newEntry.thirdAnimal, 20);
            GetDlgItemText(hDlg, IDC_KEYENTER, newEntry.key, 20);
            if (lstrcmp(newEntry.key, L"\0") == 0) {
                MessageBox(hDlg, L"Ключ не может быть пустым", L"Предупреждение", MB_ICONEXCLAMATION);
            }
            else {
                if (findKey(hFile, newEntry.key)) {
                    MessageBox(hDlg, L"Запись с таким ключом уже существует", L"Предупреждение", MB_ICONEXCLAMATION);
                }
                else {
                    if (lstrcmp(newEntry.firstAnimal, newEntry.secondAnimal) == 0 ||
                        lstrcmp(newEntry.thirdAnimal, newEntry.secondAnimal) == 0 ||
                        lstrcmp(newEntry.firstAnimal, newEntry.thirdAnimal) == 0) {
                        MessageBox(hDlg, L"Не может быть одинаковых животных", L"Предупреждение", MB_ICONEXCLAMATION);
                    }
                    else {
                        writeEntry(hFile, newEntry);
                        EndDialog(hDlg, LOWORD(wParam));
                        return (INT_PTR)TRUE;
                    }
                }
            }
            break;
        }
        case IDCANCELRECBTN:
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
            break;
        default:
            return DefWindowProc(hDlg, message, wParam, lParam);
        }
    }
    return (INT_PTR)FALSE;
}