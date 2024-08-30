typedef struct Page {
    unsigned int page_number;
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
    unsigned int read_pages;
    unsigned int page_faults;
    unsigned int written_pages;
}OutputData;

typedef struct Node {
    Page page;
    struct Node *next;
}Node;

typedef struct List {
    Node *beginning, *end;
}List;