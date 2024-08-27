#include <stdio.h>
#include <stdlib.h>

#include "../headers/algorithms.h"
#include "../headers/dataStructures.h"

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

    list->size++;
}

Node* remove_first_element(List *list) {
    // Vefificando se a lista está vazia:
    if(list->beginning == NULL) {
        printf("Não há nenhum página referenciada na memória principal!\n");
        return NULL;
    }

    else {
        Node *first_node = list->beginning;
        list->beginning = first_node->next;
        list->size--;

        if(list->beginning == NULL) {
            list->end = NULL;
        }

        return first_node;
    }
}

void lru(Page **hash_table, unsigned int TABLE_SIZE) {
    Page *aux, removed_page;
    int current_class = -1;

    for (int i = 0; i < TABLE_SIZE; i++) {
        aux = hash_table[i];
        while ((*aux).next) {
            // Classe 0: não referenciada, não modificada:
            if((*aux).referenced == 0 && (*aux).modified == 0) {
                removed_page = *aux;
                return;
            }
            
            // Classe 1: não referenciada, modificada:
            else if((*aux).referenced == 0 && (*aux).modified == 1) {
                if(1 < current_class) {
                    removed_page = *aux;
                }
            }

            // Classe 2: referenciada, não modificada:
            else if((*aux).referenced == 1 && (*aux).modified == 0) {
                if(2 < current_class) {
                    removed_page = *aux;
                }
            }

            // Classe 3: referenciada, modificada:
            else if((*aux).referenced == 1 && (*aux).modified == 1) {
                removed_page = *aux;
            }
        }
    }

    return;
}

void nru(Page **hash_table, unsigned int TABLE_SIZE) {


}


void second_chance(Page **hash_table, unsigned int TABLE_SIZE, List *list) {
    Node *node = list->beginning;
    Node *removed_node = (Node*) malloc(sizeof(Node));

    // Verificando se a memória foi alocada corretamente:
    if(node == NULL || removed_node == NULL) {
        printf("A memória não foi alocada corretamente!\n");
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
}
