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

struct TreeNode
{
    Record *record;
    TreeNode *left;
    TreeNode *right;
};

struct QueueElement
{
    Record *record;
    QueueElement *next;
};

struct RecordQueue
{
    QueueElement *front;
    QueueElement *rear;
    int count;
};

TreeNode *InsertIntoSearchTree(TreeNode *root, Record *record)
{
    if (root == nullptr)
    {
        TreeNode *newNode = new TreeNode;
        newNode->record = record;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }

    if (strncmp(record->Street, root->record->Street, 18) < 0)
    {
        root->left = InsertIntoSearchTree(root->left, record);
    }
    else
    {
        root->right = InsertIntoSearchTree(root->right, record);
    }

    return root;
}

Record *SearchInSearchTree(TreeNode *root, const char *key)
{
    if (root == nullptr)
    {
        return nullptr;
    }

    int cmp = strncmp(key, root->record->Street, 18);

    if (cmp == 0)
    {
        return root->record;
    }
    else if (cmp < 0)
    {
        return SearchInSearchTree(root->left, key);
    }
    else
    {
        return SearchInSearchTree(root->right, key);
    }
}

void InitializeRecordQueue(RecordQueue &queue)
{
    queue.front = nullptr;
    queue.rear = nullptr;
    queue.count = 0;
}

void Enqueue(RecordQueue &queue, Record *record)
{
    QueueElement *newElement = new QueueElement;
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

void DisplayQueue(const RecordQueue &queue, int &currentPage)
{
    const int RecordsPerPage = 20;
    QueueElement *current = queue.front;
    int recordCount = 0;

    while (true)
    {
        if (current == nullptr)
        {
            system("cls");
            cout << "Queue is empty." << endl;
            cout << "Press any key to return to the menu...";
            _getch();
            break;
        }

        if (recordCount % RecordsPerPage == 0)
        {
            system("cls");
            cout << "Queue Contents (Page " << currentPage + 1 << "):" << endl;
            cout << "Num\tFull Name\t\t\tStreet\t\tHouse Number\tApartment Number\tCheck-In Date" << endl;
        }

        cout << recordCount + 1 << "\t" << current->record->FullName << "\t" << current->record->Street
             << "\t" << current->record->HouseNumber << "\t\t" << current->record->ApartmentNumber
             << "\t\t" << current->record->CheckInDate << endl;

        current = current->next;
        recordCount++;

        if (recordCount % RecordsPerPage == 0)
        {
            cout << "Press 'Q' to return to the menu, Left Arrow for Previous Page, Right Arrow for Next Page, or any other key to continue...";
            char Input = _getch();

            if (Input == 'Q' || Input == 'q')
                break;
            else if (Input == 75 && currentPage > 0)
                currentPage--; // Previous page
            else if (Input == 77)
                currentPage++; // Next page
        }
    }
}



struct RecordArray
{
    Record records[MaxRecords];
    int count;
};

void InitializeRecordArray(RecordArray &arr)
{
    arr.count = 0;
}

void AddRecord(RecordArray &arr, const Record &record)
{
    if (arr.count < MaxRecords)
    {
        arr.records[arr.count++] = record;
    }
}

void DisplaySearchTree(TreeNode *root, int &currentPage, int totalPages);

void DisplayRecords(const RecordArray &arr, int currentPage, int totalPages)
{
    const int RecordsPerPage = 20;
    int startIdx = currentPage * RecordsPerPage;
    int endIdx = min(startIdx + RecordsPerPage, arr.count);

    system("cls");
    cout << "Page " << currentPage + 1 << "/" << totalPages << ":" << endl;
    cout << "Num\tFull Name\t\t\tStreet\t\tHouse Number\tApartment Number\tCheck-In Date" << endl;

    for (int j = startIdx; j < endIdx; ++j)
    {
        const Record &record = arr.records[j];
        cout << j + 1 << "\t" << record.FullName << "\t" << record.Street << "\t" << record.HouseNumber
             << "\t\t" << record.ApartmentNumber << "\t\t" << record.CheckInDate << endl;
    }

    cout << "Use Left Arrow for Previous Page, Right Arrow for Next Page, 'Q' to Quit, or enter page number and press Enter: ";
}

int Navigate(int currentPage, int totalPages)
{
    char Input = _getch();

    if (Input == 'Q' || Input == 'q')
    {
        return -1; // Exit
    }
    else if (Input == 77)
    {
        if (currentPage < totalPages - 1)
            return currentPage + 1; // Next page
    }
    else if (Input == 75)
    {
        if (currentPage > 0)
            return currentPage - 1; // Previous page
    }
    else if (isdigit(Input))
    {
        int Page = Input - '0';

        char NextChar;
        do
        {
            NextChar = _getch();
            if (isdigit(NextChar))
            {
                Page = Page * 10 + (NextChar - '0');
            }
        } while (isdigit(NextChar));

        if (Page >= 1 && Page <= totalPages)
            return Page - 1;
    }

    return currentPage;
}



void SearchAndDisplay(RecordArray &arr, RecordQueue &searchQueue, TreeNode *&searchTreeRoot)
{
    char Key[4];
    cout << "Enter the search key (first 3 letters of Full Name): ";
    cin.getline(Key, sizeof(Key));

    RecordArray MatchingRecords;
    InitializeRecordArray(MatchingRecords);

    for (int i = 0; i < arr.count; ++i)
    {
        if (strncmp(arr.records[i].FullName, Key, 3) == 0)
        {
            AddRecord(MatchingRecords, arr.records[i]);
            Enqueue(searchQueue, &arr.records[i]);
            searchTreeRoot = InsertIntoSearchTree(searchTreeRoot, &arr.records[i]);
        }
    }

        if (MatchingRecords.count == 0)
    {
        cout << "No matching records found." << endl;
    }
    else
    {
        cout << "Matching records:" << endl;

        int totalPages = (MatchingRecords.count + 20 - 1) / 20;
        int currentPage = 0;

        while (true)
        {
            DisplayRecords(MatchingRecords, currentPage, totalPages);
            currentPage = Navigate(currentPage, totalPages);

            if (currentPage == -1)
                break;
        }
    }

    cout << "Press any key to continue";
    _getch();
}

void HoareSort(RecordArray &arr, int first, int last)
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

void DisplaySearchTree(TreeNode *root, RecordQueue &searchQueue, int currentPage, int totalPages)
{
    if (root == nullptr)
    {
        return;
    }

    const int RecordsPerPage = 20;
    QueueElement *current = searchQueue.front;
    int recordCount = 0;

    while (true)
    {
        if (current == nullptr)
        {
            system("cls");
            cout << "No matching records found in the Search Tree." << endl;
            cout << "Press any key to return to the menu...";
            _getch();
            break;
        }

        if (recordCount % RecordsPerPage == 0)
        {
            system("cls");
            cout << "Search Tree Contents (Page " << currentPage + 1 << "):" << endl;
            cout << "Num\tFull Name\t\t\tStreet\t\tHouse Number\tApartment Number\tCheck-In Date" << endl;
        }

        cout << recordCount + 1 << "\t" << current->record->FullName << "\t" << current->record->Street
             << "\t" << current->record->HouseNumber << "\t\t" << current->record->ApartmentNumber
             << "\t\t" << current->record->CheckInDate << endl;

        current = current->next;
        recordCount++;

        if (recordCount % RecordsPerPage == 0)
        {
            cout << "Press 'Q' to return to the menu, Left Arrow for Previous Page, or any other key to continue...";
            char Input = _getch();

            if (Input == 'Q' || Input == 'q')
                break;
        }
    }
}

// Функция для отображения записей, соответствующих поисковому запросу
void DisplayMatchingRecords(const RecordArray &arr, int currentPage, int totalPages)
{
    const int RecordsPerPage = 20;
    int startIdx = currentPage * RecordsPerPage;
    int endIdx = min(startIdx + RecordsPerPage, arr.count);

    system("cls");
    cout << "Matching Records (Page " << currentPage + 1 << "/" << totalPages << "):" << endl;
    cout << "Num\tFull Name\t\t\tStreet\t\tHouse Number\tApartment Number\tCheck-In Date" << endl;

    for (int j = startIdx; j < endIdx; ++j)
    {
        const Record &record = arr.records[j];
        cout << j + 1 << "\t" << record.FullName << "\t" << record.Street << "\t" << record.HouseNumber
             << "\t\t" << record.ApartmentNumber << "\t\t" << record.CheckInDate << endl;
    }

    cout << "Use Left Arrow for Previous Page, Right Arrow for Next Page, 'Q' to Quit, or enter page number and press Enter: ";
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

    RecordArray SortedDatabase = Database;
    HoareSort(SortedDatabase, 0, SortedDatabase.count - 1);

    RecordQueue searchQueue;
    InitializeRecordQueue(searchQueue);

    TreeNode *searchTreeRoot = nullptr;

    RecordArray MatchingRecords; // Объявим MatchingRecords здесь
    InitializeRecordArray(MatchingRecords);

    while (true)
    {
        system("cls");

        cout << "Menu:" << endl;
        cout << "1. View Unsorted Database" << endl;
        cout << "2. View Sorted Database" << endl;
        cout << "3. Search by Key (Full Name)" << endl;
        cout << "4. Display Search Queue" << endl;
        cout << "5. Display Search Tree" << endl;
        cout << "6. Search in Search Tree" << endl;
        cout << "7. Exit" << endl;

        char Input = _getch();

        if (Input == '1')
        {
            int currentPage = 0;
            int totalPages = (Database.count + 20 - 1) / 20;

            while (true)
            {
                DisplayRecords(Database, currentPage, totalPages);
                currentPage = Navigate(currentPage, totalPages);

                if (currentPage == -1)
                    break;
            }
        }
        else if (Input == '2')
        {
            int currentPage = 0;
            int totalPages = (SortedDatabase.count + 20 - 1) / 20;

            while (true)
            {
                DisplayRecords(SortedDatabase, currentPage, totalPages);
                currentPage = Navigate(currentPage, totalPages);

                if (currentPage == -1)
                    break;
            }
        }
        else if (Input == '3')
        {
            SearchAndDisplay(SortedDatabase, searchQueue, searchTreeRoot);
        }
        else if (Input == '4')
        {
            int currentPage = 0;
            while (true)
            {
                DisplayQueue(searchQueue, currentPage);
                char SubInput = _getch();

                if (SubInput == 'Q' || SubInput == 'q')
                    break;
                else if (SubInput == 75 && currentPage > 0)
                    currentPage--; // Previous page
            }
        }
 if (Input == '5')
    {
        int currentPage = 0;
        int totalPages = (/* Calculate the total number of nodes in the search tree */ + 20 - 1) / 20;

        while (true)
        {
            DisplaySearchTree(searchTreeRoot, searchQueue, currentPage, totalPages);
            char SubInput = _getch();

            if (SubInput == 'Q' || SubInput == 'q')
                break;
        }
    }
    else if (Input == '6')
    {
        // Поиск в дереве
        char searchKey[18];
        cout << "Enter the search key (Street) to search in the Search Tree: ";
        cin.getline(searchKey, sizeof(searchKey));

        Record *foundRecord = SearchInSearchTree(searchTreeRoot, searchKey);

        if (foundRecord)
        {
            int currentPage = 0;
            int totalPages = (MatchingRecords.count + 20 - 1) / 20;

            while (true)
            {
                DisplayMatchingRecords(MatchingRecords, currentPage, totalPages);
                currentPage = Navigate(currentPage, totalPages);

                if (currentPage == -1)
                    break;
            }
        }
        else
        {
            cout << "No matching records found in the Search Tree." << endl;
        }

        cout << "Press any key to continue";
        _getch();
    }
        else if (Input == '7')
        {
            break;
        }
    }

    // Free memory allocated for queue elements
    QueueElement *current = searchQueue.front;
    while (current != nullptr)
    {
        QueueElement *temp = current;
        current = current->next;
        delete temp;
    }

    fclose(fp);
    return 0;
}

