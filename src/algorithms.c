#include <stdio.h>
#include <stdlib.h>

#include "../headers/dataStructures.h"
#include "../headers/algorithms.h"
#include "../headers/tp2virtual.h"

/*
    Função utilizada para inserir uma página no final da lista, se caso o
    algoritmo passado como parâmetro for o Segunda Chance:
*/
void insert_end(List *list, Page page) {
    Node *new = (Node*) malloc(sizeof(Node));

    // Verificando se a memória foi alocada corretamente:
    if(new == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    new->page = page;
    new->next = NULL;

    // Verificando se a lista está vazia:
    if(list->beginning == NULL) {
        list->beginning = new;
        list->end = new;
    }

    else {
        list->end->next = new;
        list->end = new;
    }
}

/*
    Função utilizada para remover uma página do início da lista, se caso o
    algoritmo passado como parâmetro for o Segunda Chance:
*/
Node* remove_first_element(List *list) {
    // Vefificando se a lista está vazia:
    if(list->beginning == NULL) {
        printf("Não há nenhum página referenciada na memória principal!\n");
        return NULL;
    }

    else {
        Node *first_node = list->beginning;
        list->beginning = first_node->next;

        if(list->beginning == NULL) {
            list->end = NULL;
        }

        return first_node;
    }
}

/*
    Função utilizada para remover a página selecionado pelo algoritmo de
    substituição:
*/
void remove_page(Page **hash_table, Page *removed_page, unsigned int TABLE_SIZE) {
    Page *aux = NULL;
    Page *prev = NULL;
    
    if(removed_page) {
        unsigned int index = hash_function((*removed_page).page_number, TABLE_SIZE);
        aux = hash_table[index];
    
        while(aux) {
            if(aux == removed_page) {
                if(prev) {
                    prev->next = removed_page->next;
                }

                else {
                    hash_table[index] = removed_page->next;
                }

                free(removed_page);

                prev = aux;
                aux = aux->next;
                break;
            }

            prev = aux;
            aux = aux->next;
        }
    }
}

/*
    Função utilizada para atualizar o contador de cada página no algoritmo LRU
    a cada interrupção de relógio:
*/
void update_pages_ages(Page **hash_table, unsigned int TABLE_SIZE) {
    Page *page = NULL;
    for(unsigned int i = 0; i < TABLE_SIZE; i++) {
        page = hash_table[i];

        while(page) {
            (*page).counter >>= 1; // shift right nos bits de age

            // O bit R é adicionado no bit mais à esquerda de age:
            (*page).counter |= ((*page).referenced << 7);

            (*page).referenced = 0;

            page = page->next;
        }
    }
}

// Algoritmo LRU (Least Recently Used ) de substituição de páginas:
void lru(Page **hash_table, unsigned int TABLE_SIZE) {
    Page *lru_page = NULL;
    Page *page = NULL;
    unsigned int min_counter = 256;

    for(unsigned int i = 0; i < TABLE_SIZE; i++) {
        page = hash_table[i];

        while(page) {
            
            if((*page).counter < min_counter) {
                min_counter = (*page).counter; 
                lru_page = page;
            }

            page = page->next;
        }
    }

    remove_page(hash_table, lru_page, TABLE_SIZE);
}

/*
    Função utilizada para limpar o bit R no algoritmo NRU a cada interrupção de
    relógio:
*/
void update_referenced_bit(Page **hash_table, unsigned int TABLE_SIZE) {
    Page *page = NULL;

    for(unsigned int i = 0; i < TABLE_SIZE; i++) {
        page = hash_table[i];

        while(page) {
            (*page).referenced = 0;
            page = page->next;
        }
    }
}

// Algoritmo NRU (Not Recently Used) de substituição de páginas:
void nru(Page **hash_table, unsigned int TABLE_SIZE) {
    Page *aux = NULL;
    Page *removed_page = NULL;
    unsigned int current_class = 4;
    unsigned int page_class;

    for (unsigned int i = 0; i < TABLE_SIZE; i++) {
        aux = hash_table[i];
        while (aux) {
            page_class = 4;

            // Classe 0: não referenciada, não modificada:
            if((*aux).referenced == 0 && (*aux).modified == 0) {
                page_class = 0;
            }
            
            // Classe 1: não referenciada, modificada:
            else if((*aux).referenced == 0 && (*aux).modified == 1) {
                page_class = 1;
            }

            // Classe 2: referenciada, não modificada:
            else if((*aux).referenced == 1 && (*aux).modified == 0) {
                page_class = 2;
            }

            // Classe 3: referenciada, modificada:
            else if((*aux).referenced == 1 && (*aux).modified == 1) {
                page_class = 3;
            }

            if(page_class < current_class) {
                removed_page = aux;
                current_class = page_class;

                if(current_class == 0) {
                    break;
                }
            }

            aux = aux->next;
        }
    }
    
    remove_page(hash_table, removed_page, TABLE_SIZE);
}

// Algoritmo Segunda Chance de substituição de páginas:
void second_chance(Page **hash_table, unsigned int TABLE_SIZE, List *list) {
    Node *node = list->beginning;
    Node *removed_node = (Node*) malloc(sizeof(Node));

    // Verificando se a memória foi alocada corretamente:
    if(removed_node == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        printf("TESTE!\n");
        exit(1);
    }  

    // Fazer as alterações na tabela de páginas também:

    while(node) {

        if(node->page.referenced == 0) {
            removed_node = remove_first_element(list);
            break;
        }

        else if(node->page.referenced == 1) {
            node->page.referenced = 0;
            removed_node = remove_first_element(list);
            insert_end(list, removed_node->page);
        }

        node = node->next;
    }

    remove_page(hash_table, &(*removed_node).page, TABLE_SIZE);
}