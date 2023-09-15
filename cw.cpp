#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include <queue>

using namespace std;

struct Record
{
    char FullName[32];
    char Street[18];
    unsigned short int HouseNumber;
    unsigned short int ApartmentNumber;
    char CheckInDate[10];
};

void DisplayRecords(Record* array, int n)
{
    const int RecordsPerPage = 20;
    int TotalPages = (n + RecordsPerPage - 1) / RecordsPerPage;
    int CurrentPage = 0;

    while (true)
    {
        system("cls");

        cout << "Page " << CurrentPage + 1 << "/" << TotalPages << ":" << endl;

        cout << "Num\tFull Name\t\t\tStreet\t\tHouse Number\tApartment Number\tCheck-In Date" << endl;

        for (int j = CurrentPage * RecordsPerPage; j < (CurrentPage + 1) * RecordsPerPage && j < n; ++j)
        {
            cout << j + 1 << "\t" << array[j].FullName << "\t" << array[j].Street << "\t" << array[j].HouseNumber << "\t\t" << array[j].ApartmentNumber << "\t\t" << array[j].CheckInDate << endl;
        }

        cout << "Use Left Arrow for Previous Page, Right Arrow for Next Page, 'Q' to Quit, or enter page number and press Enter: ";

        char Input = getch();

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
                NextChar = getch();
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


void SearchAndDisplay(Record* array, int n)
{
    char Key[7];
    cout << "Enter the search key (first 3 letters of Full Name and Street): ";
    cin.getline(Key, sizeof(Key));

    queue<Record> MatchingRecords;

    for (int i = 0; i < n; ++i)
    {
        if (strncmp(array[i].FullName, Key, 3) == 0 && strncmp(array[i].Street, Key + 3, 3) == 0)
        {
            MatchingRecords.push(array[i]);
        }
    }

    if (MatchingRecords.empty())
    {
        cout << "No matching records found." << endl;
    }
    else
    {
        cout << "Matching records:" << endl;
        Record* MatchingArray = new Record[MatchingRecords.size()];

        int Index = 0;
        while (!MatchingRecords.empty())
        {
            MatchingArray[Index++] = MatchingRecords.front();
            MatchingRecords.pop();
        }

        DisplayRecords(MatchingArray, Index);

        delete[] MatchingArray;
    }


    cout << "Press any key to continue...";
    getch();
}

void HoareSort(Record* array, int first, int last)
{
    int i = first, j = last;
    Record tmp, x = array[(first + last) / 2];

    do
    {
        while (strncmp(array[i].FullName, x.FullName, 3) < 0 || (strncmp(array[i].FullName, x.FullName, 3) == 0 && strncmp(array[i].Street, x.Street, 3) < 0))
            i++;
        while (strncmp(array[j].FullName, x.FullName, 3) > 0 || (strncmp(array[j].FullName, x.FullName, 3) == 0 && strncmp(array[j].Street, x.Street, 3) > 0))
            j--;

        if (i <= j)
        {
            if (i < j)
            {
                tmp = array[i];
                array[i] = array[j];
                array[j] = tmp;
            }
            i++;
            j--;
        }
    } while (i <= j);

    if (i < last)
        HoareSort(array, i, last);
    if (first < j)
        HoareSort(array, first, j);
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

    Record Database[4000] = {0};
    int NumberOfRecords = 0;
    NumberOfRecords = fread((Record *)Database, sizeof(Record), 4000, fp);
    cout << "Total records: " << NumberOfRecords << endl;

    Record* SortedDatabase = new Record[NumberOfRecords];
    memcpy(SortedDatabase, Database, sizeof(Record) * NumberOfRecords);

    HoareSort(SortedDatabase, 0, NumberOfRecords - 1);

    while (true)
    {
        system("cls");

        cout << "Menu:" << endl;
        cout << "1. View Unsorted Database" << endl;
        cout << "2. View Sorted Database" << endl;
        cout << "3. Search by Key (Full Name and Street)" << endl;
        cout << "4. Exit" << endl;

        char Input = getch();

        if (Input == '1')
        {
            DisplayRecords(Database, NumberOfRecords);
        }
        else if (Input == '2')
        {
            DisplayRecords(SortedDatabase, NumberOfRecords);
        }
        else if (Input == '3')
        {
            SearchAndDisplay(SortedDatabase, NumberOfRecords);
        }
        else if (Input == '4')
        {
            break;
        }
    }

    delete[] SortedDatabase;

    fclose(fp);
    return 0;
}
