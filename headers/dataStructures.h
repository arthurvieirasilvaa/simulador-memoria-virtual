typedef struct Page {
    unsigned int frame_number;
    unsigned int present;
    unsigned int referenced;
    unsigned int modified;
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
