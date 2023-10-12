#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

const int MaxRecords = 4000;

struct Record
{
    char FullName[32];
    char Street[18];
    unsigned short int HouseNumber;
    unsigned short int ApartmentNumber;
    char CheckInDate[10];
};

struct QueueElement
{
    Record* record;
    QueueElement* next;
};

struct RecordQueue
{
    QueueElement* front;
    QueueElement* rear;
    int count;
};

void InitializeRecordQueue(RecordQueue& queue)
{
    queue.front = nullptr;
    queue.rear = nullptr;
    queue.count = 0;
}

void Enqueue(RecordQueue& queue, Record* record)
{
    QueueElement* newElement = new QueueElement;
    newElement->record = record;
    newElement->next = nullptr;

    if (queue.front == nullptr)
    {
        queue.front = newElement;
        queue.rear = newElement;
    }
    else
    {
        queue.rear->next = newElement;
        queue.rear = newElement;
    }

    queue.count++;
}

void DisplayQueue(const RecordQueue& queue)
{
    const int RecordsPerPage = 20;
    QueueElement* current = queue.front;
    int recordCount = 0;

    while (current != nullptr)
    {
        if (recordCount % RecordsPerPage == 0)
        {
            system("cls");
            cout << "Queue Contents:" << endl;
            cout << "Num\tFull Name\t\t\tStreet\t\tHouse Number\tApartment Number\tCheck-In Date" << endl;
        }

        cout << recordCount + 1 << "\t" << current->record->FullName << "\t" << current->record->Street
             << "\t" << current->record->HouseNumber << "\t\t" << current->record->ApartmentNumber
             << "\t\t" << current->record->CheckInDate << endl;

        current = current->next;
        recordCount++;

        if (recordCount % RecordsPerPage == 0)
        {
            cout << "Press 'Q' to Quit or any other key to continue...";

            char Input = _getch();
            if (Input == 'Q' || Input == 'q')
                break;
        }
    }

    cout << "Press any key to continue...";
    _getch();
}



struct RecordArray
{
    Record records[MaxRecords];
    int count;
};

void InitializeRecordArray(RecordArray& arr)
{
    arr.count = 0;
}

void AddRecord(RecordArray& arr, const Record& record)
{
    if (arr.count < MaxRecords)
    {
        arr.records[arr.count++] = record;
    }
}

void DisplayRecords(const RecordArray& arr)
{
    const int RecordsPerPage = 20;
    int TotalPages = (arr.count + RecordsPerPage - 1) / RecordsPerPage;
    int CurrentPage = 0;

    while (true)
    {
        system("cls");

        cout << "Page " << CurrentPage + 1 << "/" << TotalPages << ":" << endl;
        cout << "Num\tFull Name\t\t\tStreet\t\tHouse Number\tApartment Number\tCheck-In Date" << endl;

        int startIdx = CurrentPage * RecordsPerPage;
        int endIdx = min(startIdx + RecordsPerPage, arr.count);

        for (int j = startIdx; j < endIdx; ++j)
        {
            const Record& record = arr.records[j];
            cout << j + 1 << "\t" << record.FullName << "\t" << record.Street << "\t" << record.HouseNumber
                 << "\t\t" << record.ApartmentNumber << "\t\t" << record.CheckInDate << endl;
        }

        cout << "Use Left Arrow for Previous Page, Right Arrow for Next Page, 'Q' to Quit, or enter page number and press Enter: ";

        char Input = _getch();

        if (Input == 'Q' || Input == 'q')
        {
            break;
        }
        else if (Input == 77)
        {
            if (CurrentPage < TotalPages - 1)
                CurrentPage++;
        }
        else if (Input == 75)
        {
            if (CurrentPage > 0)
                CurrentPage--;
        }
        else if (isdigit(Input))
        {
            int Page = Input - '0';
            cout << Page;
            Sleep(100);

            char NextChar;
            do
            {
                NextChar = _getch();
                if (isdigit(NextChar))
                {
                    Page = Page * 10 + (NextChar - '0');
                    cout << NextChar;
                }
            } while (isdigit(NextChar));

            if (Page >= 1 && Page <= TotalPages)
                CurrentPage = Page - 1;
        }
    }
}

void SearchAndDisplay(RecordArray& arr, RecordQueue& searchQueue)
{
    char Key[4]; // To store the search key (first 3 letters)
    cout << "Enter the search key (first 3 letters of Full Name): ";
    cin.getline(Key, sizeof(Key));

    RecordArray MatchingRecords;
    InitializeRecordArray(MatchingRecords);

    for (int i = 0; i < arr.count; ++i)
    {
        if (strncmp(arr.records[i].FullName, Key, 3) == 0)
        {
            AddRecord(MatchingRecords, arr.records[i]);
            // Добавить запись в очередь searchQueue
            Enqueue(searchQueue, &arr.records[i]);
        }
    }

    if (MatchingRecords.count == 0)
    {
        cout << "No matching records found." << endl;
    }
    else
    {
        cout << "Matching records:" << endl;
        DisplayRecords(MatchingRecords);
    }

    // Wait for a key press before returning to the menu
    cout << "Press any key to continue...";
    _getch();
}


void HoareSort(RecordArray& arr, int first, int last)
{
    int i = first, j = last;
    Record tmp, x = arr.records[(first + last) / 2];

    do
    {
        while (strncmp(arr.records[i].FullName, x.FullName, 32) < 0 ||
               (strncmp(arr.records[i].FullName, x.FullName, 32) == 0 && strncmp(arr.records[i].Street, x.Street, 18) < 0))
            i++;
        while (strncmp(arr.records[j].FullName, x.FullName, 32) > 0 ||
               (strncmp(arr.records[j].FullName, x.FullName, 32) == 0 && strncmp(arr.records[j].Street, x.Street, 18) > 0))
            j--;

        if (i <= j)
        {
            if (i < j)
            {
                tmp = arr.records[i];
                arr.records[i] = arr.records[j];
                arr.records[j] = tmp;
            }
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
        HoareSort(arr, i, last);
    if (first < j)
        HoareSort(arr, first, j);
}

int main()
{
    FILE *fp;
    fp = fopen("testBase4.dat", "rb");
    if (!fp)
    {
        cout << "Error opening file." << endl;
        return 1;
    }

    RecordArray Database;
    InitializeRecordArray(Database);

    int NumberOfRecords = 0;
    while (NumberOfRecords < MaxRecords && fread((Record *)&Database.records[NumberOfRecords], sizeof(Record), 1, fp) == 1)
    {
        NumberOfRecords++;
    }

    Database.count = NumberOfRecords;
    cout << "Total records: " << NumberOfRecords << endl;

    // Make a copy of the sorted database
    RecordArray SortedDatabase = Database;

    // Sort the copy using HoareSort
    HoareSort(SortedDatabase, 0, SortedDatabase.count - 1);

    RecordQueue searchQueue; // Очередь для записей с одинаковым ключом поиска
    InitializeRecordQueue(searchQueue);

    while (true)
    {
        system("cls");

        cout << "Menu:" << endl;
        cout << "1. View Unsorted Database" << endl;
        cout << "2. View Sorted Database" << endl;
        cout << "3. Search by Key (Full Name)" << endl;
        cout << "4. Display Search Queue" << endl;
        cout << "5. Exit" << endl;

        char Input = _getch();

        if (Input == '1')
        {
            DisplayRecords(Database);
        }
        else if (Input == '2')
        {
            DisplayRecords(SortedDatabase);
        }
        else if (Input == '3')
        {
            SearchAndDisplay(SortedDatabase, searchQueue); // Search in the sorted copy
        }
        else if (Input == '4')
        {
            DisplayQueue(searchQueue); // Отобразить содержимое очереди
        }
        else if (Input == '5')
        {
            break;
        }
    }

    // Освобождаем память, выделенную для элементов очереди
    QueueElement* current = searchQueue.front;
    while (current != nullptr)
    {
        QueueElement* temp = current;
        current = current->next;
        delete temp;
    }

    fclose(fp);
    return 0;
}


