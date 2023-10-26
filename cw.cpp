#include <cstdio>
#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <algorithm>
#include <iostream>
#include <stack>

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

struct BSTNode {
    record data;
    BSTNode* left;
    BSTNode* right;
};

// Function to create a new BST node
BSTNode* createBSTNode(record data) {
    BSTNode* newNode = new BSTNode();
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

int compare_records(const record &a, const record &b) {
    int nameComparison = strcmp(a.fullname, b.fullname);
    if (nameComparison == 0) {
        return strcmp(a.street, b.street);
    }
    return nameComparison;
}



// Function to insert a record into the BST
BSTNode* insertBST(BSTNode* root, record data) {
    if (root == NULL) {
        return createBSTNode(data);
    }

    if (compare_records(data, root->data) < 0) {
        root->left = insertBST(root->left, data);
    } else {
        root->right = insertBST(root->right, data);
    }

    return root;
}
BSTNode* search_and_insert(BSTNode* root, record* records, int size) {
    char key[4] = {0};
    cin.ignore();
    cin.getline(key, sizeof(key));
    for (int i = 0; i < size; i++) {
        if (strncmp(records[i].fullname, key, 3) == 0) {
            root = insertBST(root, records[i]);
        }
    }
    return root;
}
// Function to build a BST from an array of records
BSTNode* buildOptimalBST(record* records, int start, int end) {
    if (start > end) {
        return NULL;
    }

    int mid = (start + end) / 2;
    BSTNode* root = createBSTNode(records[mid]);

    root->left = buildOptimalBST(records, start, mid - 1);
    root->right = buildOptimalBST(records, mid + 1, end);

    return root;
}

void insert_queue(queue *q, record x) {
    if (q->rear == QUEUE_SIZE - 1) {
        cout << "Queue is full. Cannot insert more elements." << endl;
    } else {
        q->rear++;
        q->qu[q->rear] = x;
    }
}
// Function to display the BST using in-order traversal
int displayAllBST(BSTNode* root) {
    if (root == nullptr) {
        return 0;
    }

    int recordsPrinted = 0;

    // Traverse the BST using in-order traversal
    stack<BSTNode*> s;
    BSTNode* current = root;

    while (current != nullptr || !s.empty()) {
        while (current != nullptr) {
            s.push(current);
            current = current->left;
        }

        current = s.top();
        s.pop();

        // Print the record
        cout << recordsPrinted << "\t" << current->data.fullname << "\t" << current->data.street
            << "\t" << current->data.house_num << "\t" << current->data.app_num
            << "\t" << current->data.check_in_date << endl;
        recordsPrinted++;

        current = current->right;
    }

    return recordsPrinted;
}




void searchAllByHouseNumber(BSTNode* root, short int houseNumber, queue* q) {
    if (root == nullptr) {
        return;
    }

    // If the house number matches, add it to the queue
    if (houseNumber == 255) {
        insert_queue(q, root->data);
    }
    
    if (houseNumber == root->data.house_num) {
        insert_queue(q, root->data);
    }

    // Recursively search the left and right subtrees
    searchAllByHouseNumber(root->left, houseNumber, q);
    searchAllByHouseNumber(root->right, houseNumber, q);
}

void displaySearchResults(BSTNode* resultNode) {
    if (resultNode == nullptr) {
        cout << "House number not found." << endl;
        return;
    }

    // Display the found record
    cout << "Record found:" << endl;
    cout << resultNode->data.fullname << "\t" << resultNode->data.street
        << "\t" << resultNode->data.house_num << "\t" << resultNode->data.app_num
        << "\t" << resultNode->data.check_in_date << endl;
}



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


void display_queue(queue *q) {
    size_t count_zap_on_page = 20;
    size_t currentPage = 0;
    while (true) {
        system("cls");
        cout << TITLE_COUT << endl << endl;

        int start = currentPage * count_zap_on_page;
        size_t end = (currentPage + 1) * count_zap_on_page - 1;
        if (end > static_cast<size_t>(q->rear))
        {
            end = static_cast<size_t>(q->rear);
        }

        for (int i = start; i <= end; i++) {
            cout << i << QUEUE_RECORD_COUT << endl;
        }

        cout << "Page " << currentPage + 1 << "/" << (q->rear / count_zap_on_page + 1) << endl;
        cout << "Press 'Q' to quit, Left Arrow to go to the previous page, Right Arrow to go to the next page: ";

        char input = _getch();

        if (input == 'Q' || input == 'q') {
            system("cls");
            break;
        } else if (input == 75 && currentPage > 0) {
            currentPage--;
        } else if (input == 77 && end < q->rear) {
            currentPage++;
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

void menu() {
    record* mass = open_and_read_file();
    queue* q = (queue*)malloc(sizeof(queue));
    BSTNode* searchRoot = nullptr;
    init_queue(q);
    int choice;
    BSTNode* root = nullptr; // Declare root here and initialize it

    while (choice != 'q' && choice != 'Q') {
        cout << "\n" << "1. Display records" << "\n" << endl;
        cout << "\n" << "2. Sort records" << "\n" << endl;
        cout << "\n" << "3. Search records" << "\n" << endl;
        cout << "\n" << "4. print tree" << "\n" << endl;
        cout << "\n" << "5. search in tree" << "\n" << endl;

        cin >> choice;

        switch (choice) {
            case 1:
                // Display records code
                break;
            case 2:
                // Sort records code
                break;
            case 3:
                // Search records by name
                system("cls");
                hoare_sort(mass, 0, 3999);
                search_func(mass, q);
                display_queue(q);
                free(mass);
                break;
            case 4:
                // Build the BST and display optimal BST
                hoare_sort(mass, 0, SIZE_MASS_REC - 1);
                searchRoot = search_and_insert(searchRoot, mass, SIZE_MASS_REC);
                cout << "Optimal Binary Search Tree (In-order Traversal):" << endl;
                display_queue(q);
                break;
            case 5:
                // Search records by house number
                short int houseNumberToSearch;
                cout << "Enter the house number to search: ";
                cin >> houseNumberToSearch;

                // Search for house numbers in the BST
                searchAllByHouseNumber(searchRoot, houseNumberToSearch, q);

                // Display search results
                display_queue(q);
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
