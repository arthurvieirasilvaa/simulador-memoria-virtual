int check_arguments(int, char**);
void display_results(char*, unsigned int, unsigned int, char*, OutputData);
unsigned int hash_function(unsigned int, unsigned int);
unsigned int determine_page(unsigned, unsigned int);
OutputData read_file(char*, Page**, unsigned int, unsigned int, char*);
void free_hash_table(Page**, unsigned int);