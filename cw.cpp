#include <cstdio>
#include <iostream>
#include <conio.h>
#include <string.h>

using namespace std;

#define TITLE_COUT "num" << "\t\tfull name" << "\t\tsteet" << "\t\t\thouse number" << "\tappartment num" << "\t\tcheck in date"
#define RECORD_COUT "\t" << mas_for_out[i].fullname << "\t" << mas_for_out[i].street << "\t\t" << mas_for_out[i].house_num << "\t\t" << mas_for_out[i].app_num << "\t\t" << mas_for_out[i].check_in_date
typedef struct record
{
    char fullname[32];
    char street[18];
    short int house_num;
    short int app_num;
    char check_in_date[10];
} record;

record *open_and_read_file(const char *filename)
{
    FILE *fp = fopen(filename, "rb");
    record *mass = (record *)malloc(4000 * sizeof(record));
    fread(mass, sizeof(record), 4000, fp);
    fclose(fp);
    return mass;
}

void display(record *mas_for_out)
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

        cout << "Page " << currentPage + 1 << "/" << (4000 / count_zap_on_page) << endl;
        cout << "Press 'Q' to quit, Left Arrow to go to the previous page, Right Arrow to go to the next page: ";

        char input = _getch();

        if (input == 'Q' || input == 'q')
        {
            break;
        }
        else if (input == 75 && currentPage > 0)
        {
            currentPage--;
        }
        else if (input == 77 && currentPage < (4000 / count_zap_on_page - 1))
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

void menu()
{
    record *mass = open_and_read_file("testBase4.dat");

 
    int choice;
    
    
    scanf("%d", &choice);
    
    if(choice == 1)
    {
        system("cls");
        display(mass);
    }
    if(choice == 2)
    {
        system("cls");
        hoare_sort(mass, 0, 3999);
        display(mass);
    }

    free(mass);
}

int main()
{
    menu();   
    return 0;
}
