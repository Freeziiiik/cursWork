#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <algorithm>
#include <string>

struct Settlement {
	char name[32];
	char street[18];
	short int numberOfHouse;
	short int numberOfApartment;
	char date[10];
};

struct queue {
    queue* next;
    Settlement* data;
}*head = NULL, *tail = NULL, *p;

struct vertex {
    Settlement* data;
    int weight;
    vertex* left;
    vertex* right;
    vertex* next;
};

bool compare(const vertex* a, const vertex* b) {
    return a->weight > b->weight;
}

void insert(vertex*& root, vertex* node) {
    if (root == nullptr) {
        root = node;
        root->left = nullptr;
        root->right = nullptr;
    }
    else if (node->data->numberOfHouse < root->data->numberOfHouse) {
        insert(root->left, node);
    }
    else {
        insert(root->right, node);
    }
}

vertex* A1(queue* qHead) {
    int n = 0;
    queue* ptr = qHead;
    while (ptr != nullptr) {
        n++;
        ptr = ptr->next;
    }
    if (n == 0) return nullptr;

    vertex** uniqueNodes = new vertex * [n];
    int uniqueCount = 0;

    ptr = qHead;
    while (ptr != nullptr) {
        Settlement* currentData = ptr->data;
        bool found = false;

        for (int j = 0; j < uniqueCount; j++) {
            if (currentData->numberOfHouse == uniqueNodes[j]->data->numberOfHouse) {

                vertex* dup = new vertex;
                dup->data = currentData;
                dup->weight = 1;
                dup->left = nullptr;
                dup->right = nullptr;
                dup->next = nullptr;

                vertex* curr = uniqueNodes[j];
                while (curr->next != nullptr) {
                    curr = curr->next;
                }
                curr->next = dup;
                uniqueNodes[j]->weight++;
                found = true;
                break;
            }
        }

        if (!found) {
            vertex* newNode = new vertex;
            newNode->data = currentData;
            newNode->weight = 1;
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->next = nullptr;
            uniqueNodes[uniqueCount++] = newNode;
        }
        ptr = ptr->next;
    }

    std::sort(uniqueNodes, uniqueNodes + uniqueCount, compare);

    vertex* root = nullptr;
    for (int i = 0; i < uniqueCount; i++) {
        insert(root, uniqueNodes[i]);
    }

    delete[] uniqueNodes;
    return root;
}

void printTree(vertex* root, int& counter) {
    if (root == nullptr) return;

    printTree(root->left, counter);

    Settlement* p = root->data;
    std::cout << counter++ << "\t" << p->name << "\t"
        << p->street << "\t" << p->numberOfHouse << "\t"
        << p->numberOfApartment << "\t" << p->date << std::endl;

    vertex* curr = root->next;
    while (curr != nullptr) {
        Settlement* dup = curr->data;
        std::cout << counter++ << "\t" << dup->name << "\t"
            << dup->street << "\t" << dup->numberOfHouse << "\t"
            << dup->numberOfApartment << "\t" << dup->date << std::endl;
        curr = curr->next;
    }

    printTree(root->right, counter);
}

void searchTree(vertex* root, int houseNum) {
    if (root == nullptr) {
        std::cout << "House with  " << houseNum << " not found.\n";
        return;
    }

    if (houseNum == root->data->numberOfHouse) {
        std::cout << "\nFound:\n";
        std::cout << "N\tName\t\t\t\tStreet\t\t\tHouse\tApart\tDate" << std::endl;
        int counter = 1;
        vertex* curr = root;
        while (curr != nullptr) {
            Settlement* p = curr->data;
            std::cout
                << counter++ << "\t"
                << p->name << "\t"
                << p->street << "\t"
                << p->numberOfHouse << "\t"
                << p->numberOfApartment << "\t"
                << p->date << std::endl;
            curr = curr->next;
        }
    }
    else if (houseNum < root->data->numberOfHouse) {
        searchTree(root->left, houseNum);
    }
    else {
        searchTree(root->right, houseNum);
    }
}

void treeSearchMenu(vertex* root) {
    int exit = 1;
    while (exit) {
        int houseNum;
        std::cout << "\nNumber of house for search: ";
        std::cin >> houseNum;

        searchTree(root, houseNum);

        std::cout << "repeat? (1/0): ";
        std::cin >> exit;
    }
}



void clearQueue() {
    while (head != NULL) {
        queue* tmp = head;
        head = head->next;
        delete tmp;
    }
    tail = NULL;
}

void add(Settlement* s) {
    p = new queue;
    p->data = s;
    p->next = NULL;
    if (head == NULL) {
        head = tail = p;
    }
    else {
        tail->next = p;
        tail = p;
    }
}

void printQueue() {
    int n = 1;
    queue* ptr = head;
    while (ptr != NULL) {
        Settlement* s = ptr->data;
        std::cout << n << "\t" << s->name << "\t"
            << s->street << "\t"
            << s->numberOfHouse << "\t"
            << s->numberOfApartment << "\t"
            << s->date << std::endl;
        ptr = ptr->next;
        n++;
    }
}

int isLess(Settlement* a, Settlement* b) {
    if (std::strcmp(a->street, b->street) < 0) return 1;
    if (std::strcmp(a->street, b->street) > 0) return 0;
    if (a->numberOfHouse < b->numberOfHouse) return 1;
    if (a->numberOfHouse > b->numberOfHouse) return 0;
    return 0;
}

void QuicksortV2(Settlement** A, int L, int R) {
    while (L < R) {
        Settlement* x = A[L];
        int i = L;
        int j = R;

        while (i <= j) {
            while (isLess(A[i], x))  i++;  
            while (isLess(x, A[j]))  j--;
            if (i <= j) {
                Settlement* temp = A[i];
                A[i] = A[j];
                A[j] = temp;
                i++;
                j--;
            }
        }
        if (j - L < R - i) {
            QuicksortV2(A, L, j);
            L = i;
        }
        else {
            QuicksortV2(A, i, R);
            R = j;
        }
    }
}

bool prefix(char* str, const char* pref) {
    return std::strncmp(str, pref, std::strlen(pref)) == 0;
}

void binSearchV2(Settlement** A, int n, const char* x) {
    int L = 0;
    int R = n - 1;
    while (L < R) {
        int m = (L + R) / 2;
        if (std::strcmp(A[m]->street, x) < 0) {
            L = m + 1;
        }
        else {
            R = m;
        }
    }
    if (!prefix(A[L]->street, x)) {
        printf("\nWith '%s' nothing found\n", x);
        return;
    }
    clearQueue();
    int i = L;
    while (i < n && prefix(A[i]->street, x)) {
        add(A[i]);
        ++i;
    }

    printf("\nWith '%s' found:\n", x);
    printQueue();
}


void search(Settlement** A, int n) {
    int exit = 0;
    while (!exit) {
        std::string pref;
        std::cout << "\nEnter 3 symbols of street name: ";
        std::cin >> pref;

        binSearchV2(A, n, pref.c_str());

        int repeat = 0;
        std::cout.flush();
        std::cout << "\nRepeat search? (1/0): ";
        std::cin >> repeat;

        if (repeat == 0) exit = 1;
    }
}

int main() {
    FILE* f = fopen("C:\\Users\\freez\\Downloads\\testBase4.dat", "rb");
    if (!f) std::cerr << "error" << std::endl;


    Settlement* arr = new Settlement[4000];
    int count = fread(arr, sizeof(Settlement), 4000, f);
    std::cout << count << std::endl;
    fclose(f);

    Settlement** indexArr = new Settlement*[4000];
    for (int i = 0; i < 4000; i++) {
        indexArr[i] = &arr[i];
    }

    QuicksortV2(indexArr, 0, count - 1);

    std::cout << "N\tName\t\t\t\tStreet\t\t\tHouse\tApart\tDate" << std::endl;
    int i = 0;
    int counter = 0;
    while (i < count) {
        if (counter == 20) {
            int answer;
            std::cout.flush();
            do {
                std::cout << "Continue? (1/0): ";
                std::cin >> answer;
            } while ((answer != 0) && (answer != 1));
            
            if (!answer) break;
            counter = 0;
        }
        Settlement* p = indexArr[i];
        std::cout
            << i + 1 << "\t"
            << p->name << "\t"
            << p->street << "\t"
            << p->numberOfHouse << "\t"
            << p->numberOfApartment << "\t"
            << p->date << std::endl;
        i++;
        counter++;

    }
    search(indexArr, count);
    

    std::cout << "Tree" << std::endl;
    if (head != NULL) {
        vertex* treeRoot = A1(head);
        int treeCounter = 1;
        std::cout << "N\tName\t\t\t\tStreet\t\t\tHouse\tApart\tDate" << std::endl;
        printTree(treeRoot, treeCounter);

        std::cout << "\nSearch:\n";
        treeSearchMenu(treeRoot);

    }
    else {
        std::cout << "\nQueue is empty\n";
    }
   
}

