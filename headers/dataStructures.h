typedef struct Page {
    unsigned int page_number;
    unsigned int present;
    unsigned int referenced;
    unsigned int modified;
    unsigned int valid;
    unsigned int last_access;
    struct Page *next;
}Page;

typedef struct {
    unsigned addr;
    char rw;
}ReadingData;

typedef struct  {
    unsigned int memory_accesses;
    unsigned int page_faults;
    unsigned int written_pages;
}OutputData;

typedef struct Node {
    Page page;
    struct node *next;
}Node;

typedef struct List {
    Node *beginning, *end;
    unsigned int size;
}List;