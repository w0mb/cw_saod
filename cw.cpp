#include <cstdio>
#include <iostream>
#include <conio.h>
#include <string.h>

using namespace std;

#define TITLE_COUT "num" << "\t\tfull name" << "\t\tsteet" << "\t\t\thouse number" << "\tappartment num" << "\t\tcheck in date"
#define RECORD_COUT "\t" << mas_for_out[i].fullname << "\t" << mas_for_out[i].street << "\t\t" << mas_for_out[i].house_num << "\t\t" << mas_for_out[i].app_num << "\t\t" << mas_for_out[i].check_in_date
#define QUEUE_RECORD_COUT "\t" << q->qu[i].fullname << "\t" << q->qu[i].street << "\t\t" << q->qu[i].house_num << "\t\t" << q->qu[i].app_num << "\t\t" << q->qu[i].check_in_date

#define FILE_NAME "testBase4.dat"
#define SIZE_MASS_REC 4000
#define QUEUE_SIZE 4000
typedef struct record
{
    char fullname[32];
    char street[18];
    short int house_num;
    short int app_num;
    char check_in_date[10];
} record;

typedef struct queue {
    record qu[QUEUE_SIZE];
    int rear, frnt;
} queue;

record *initialize_mass_for_rofls(int mass_size)
{
    record *mass = (record *)malloc(mass_size * sizeof(record));
    return mass;
}

record *open_and_read_file()
{
    FILE *fp = fopen(FILE_NAME, "rb");
    record *mass = (record *)malloc(SIZE_MASS_REC * sizeof(record));
    fread(mass, sizeof(record), SIZE_MASS_REC, fp);
    fclose(fp);
    return mass;
}

void init_queue(queue *q) {
    q->frnt = 0;
    q->rear = -1;
}

void insert_queue(queue *q, record x) {
    if (q->rear == QUEUE_SIZE - 1) {
        cout << "Queue is full. Cannot insert more elements." << endl;
    } else {
        q->rear++;
        q->qu[q->rear] = x;
    }
}

void display_queue(queue *q) {
    if (q->frnt > q->rear) {
        cout << "Queue is empty." << endl;
    } else {
        cout << TITLE_COUT << endl << endl;
        for (int i = q->frnt; i <= q->rear; i++) {
            cout << QUEUE_RECORD_COUT << endl;
        }
    }
}

void display(record *mas_for_out, int size_mas_for_out)
{
    size_t count_zap_on_page = 20;
    size_t currentPage = 0;
    while (true)
    {
        system("cls");
        cout << TITLE_COUT << endl << endl;
        for (size_t i = currentPage * count_zap_on_page; i < (currentPage + 1) * count_zap_on_page; i++)
        {
            cout  << i << RECORD_COUT << endl;
        }

        cout << "Page " << currentPage + 1 << "/" << (size_mas_for_out / count_zap_on_page) << endl;
        cout << "Press 'Q' to quit, Left Arrow to go to the previous page, Right Arrow to go to the next page: ";

        char input = _getch();

        if (input == 'Q' || input == 'q')
        {
            system("cls");
            break;
        }
        else if (input == 75 && currentPage > 0)
        {
            currentPage--;
        }
        else if (input == 77 && currentPage < (size_mas_for_out / count_zap_on_page - 1))
        {
            currentPage++;
        }
    }
}

int compare_records(const record &a, const record &b) {
    int nameComparison = strcmp(a.fullname, b.fullname);
    if (nameComparison == 0) {
        return strcmp(a.street, b.street);
    }
    return nameComparison;
}

void hoare_sort(record *array, int low, int high) {
    if (low < high) {
        int left = low;
        int right = high;
        record pivot = array[low];

        while (left < right) {
            while (compare_records(array[right], pivot) >= 0 && left < right) {
                right--;
            }
            if (left < right) {
                array[left++] = array[right];
            }

            while (compare_records(array[left], pivot) <= 0 && left < right) {
                left++;
            }
            if (left < right) {
                array[right--] = array[left];
            }
        }

        array[left] = pivot;
        hoare_sort(array, low, left - 1);
        hoare_sort(array, right + 1, high);
    }
}

void search_func(record *mass_for_search, queue *q) {
    char key[4] = {0};
    int size = 0;
    
    cin.ignore();  // Очистка буфера ввода
    cout << "Enter the search key (first 3 letters of Full Name): ";
    cin.getline(key, sizeof(key));

    for (int i = 0; i < SIZE_MASS_REC; i++) {
        if (strncmp(mass_for_search[i].fullname, key, 3) == 0) {
            insert_queue(q, mass_for_search[i]);
        }
    }
}

void menu()
{
    record *mass = open_and_read_file();
    queue *q = (queue*)malloc(sizeof(queue*) * QUEUE_SIZE); 
    int choice;

    while (true)
    {
        cout << "\n" << "1. Display records" << "\n" << endl;
        cout << "\n" << "2. Sort records" << "\n" << endl;
        cout << "\n" << "3. Search records" << "\n" << endl;
        cout << "\n" << "4. Exit" << "\n" << endl;

        cin >> choice;

        switch (choice)
        {
        case 1:
            system("cls");
            display(mass, SIZE_MASS_REC);
            free(mass);
            break;
        case 2:
            system("cls");
            hoare_sort(mass, 0, 3999);
            display(mass, SIZE_MASS_REC);
            free(mass);
            break;
        case 3:
            system("cls");
            hoare_sort(mass, 0, 3999);
            search_func(mass, q);
            free(mass);
            break;
        case 4:
            // Выход из цикла
            break;
        default:
            cout << "Invalid choice. Please choose a valid option." << endl;
            break;
        }
    }

    free(mass);
}

int main()
{
    menu();   
    return 0;
}
