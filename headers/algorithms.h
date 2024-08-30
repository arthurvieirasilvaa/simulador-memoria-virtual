void insert_end(List*, Page);
Node* remove_first_element(List*);
void remove_page(Page**, Page*, unsigned int);
void update_pages_ages(Page**, unsigned int);
void lru(Page**, unsigned int);
void update_referenced_bit(Page**, unsigned int);
void nru(Page**, unsigned int);
void second_chance(Page**, unsigned int, List*);