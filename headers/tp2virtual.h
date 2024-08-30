#define CLOCK_INTERRUPT 10

int check_arguments(int, char**);
void display_results(char*, unsigned int, unsigned int, char*, OutputData);
unsigned int hash_function(unsigned int, unsigned int);
unsigned int determine_page(unsigned, unsigned int);
void save_test_file(Page*, int, char);
OutputData read_file(char*, Page**, unsigned int, unsigned int, char*, unsigned int);
void free_hash_table(Page**, unsigned int);
