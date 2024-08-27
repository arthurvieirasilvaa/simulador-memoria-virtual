#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/tp2virtual.h"
#include "../headers/dataStructures.h"

int check_arguments(int argc, char *argv[]) {
    // Verificando se o número de argumentos passados está correto:
    if(argc < 5) {
        printf("Número de argumentos inválido!\n");
        return -1;
    }

    // Vefificando se um dos algoritmos de substituição foi digitado:
    char *alg1 = "lru";
    char *alg2 = "nru";
    char *alg3 = "segunda_chance";

    if((strcmp(argv[1], alg1) != 0) && (strcmp(argv[1], alg2) != 0) && (strcmp(argv[1], alg3) != 0)) {
        printf("Algoritmo de substituição inválido!\n");
        return -1;
    }

    // Verificando se o tamanho de cada página/quadro é válido:
    if(atoi(argv[3]) <= 0) {
        printf("O tamanho de cada página/quadro está inválido!\n");
        return -1;
    }

    // Verificando se o tamanho total da memória física disponível é válido:
    if(atoi(argv[4]) <= 0 || atoi(argv[4]) < atoi(argv[3])) {
        printf("O tamanho total da memória fisíca disponível está inválido!\n");
        return -1;
    }

    return 0;
}

void display_results(char *input_file, unsigned int total_memory, unsigned int page_size, char *algorithm) {
    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", input_file);
    printf("Tamanho da memória: %dKB\n", total_memory);
    printf("Tamanho das páginas: %dKB\n", page_size);
    printf("Técnica de reposição: %s\n", algorithm);
}

unsigned int hash_function(unsigned int key, unsigned int TABLE_SIZE) {
    return key % TABLE_SIZE;
}

unsigned int determine_page(unsigned addr, unsigned int page_size) {
    unsigned int s, tmp;

    // Derivar o valor de s:
    tmp = page_size;
    s = 0;
    while(tmp > 1) {
        tmp = tmp>>1;
        s++;
    }

    return addr >> s;
}

void read_file(char *input_file, Page **hash_table, unsigned int TABLE_SIZE, unsigned int page_size) {
    FILE *input;
    char caminho[100] = "./logs/";
    strcat(caminho, input_file);

    input = fopen(caminho, "r");

    // Se o arquivo de entrada não for encontrado:
    if(input == NULL) {
        printf("O arquivo de entrada não foi encontrado!\n");
        exit(1);
    }

    // Número de quadros restantes que ainda podem ser utilizados:
    unsigned int frames_left = TABLE_SIZE;
    unsigned int page_number, page_index;
    int page_found;

    ReadingData readingData;
    OutputData outputData;

    outputData.memory_accesses = 0;
    outputData.page_faults = 0;
    outputData.written_pages = 0;

    Page *aux = NULL;
    while((fscanf(input, "%x %c", &readingData.addr, &readingData.rw)) != EOF) {
        page_number = determine_page(readingData.addr, page_size);
        page_index = hash_function(page_number, TABLE_SIZE);

        /*
            Se o rw do endereço de memória não for 'R' ou 'W', ignoramos a
            linha atual e lemos o próximo endereço de memória:
        */
        
        if(readingData.rw != 'R' || readingData.rw != 'W') {
            continue;
        }

        aux = hash_table[page_index];
        page_found = -1;
        while(aux) {
            if((*aux).page_number == page_number) {
                page_found = 0;
                break;
            } 

            aux = aux->next;
        }

        if(page_found == -1) {
            outputData.page_faults++;
        }

        else {

        } 
    }
}

int main(int argc, char *argv[]) {

    if(check_arguments(argc, argv) == -1) {
        exit(1);
    }

    char *input_file = (char*) malloc(strlen(argv[2]) * sizeof(char));
    char *algorithm = (char*) malloc(strlen(argv[1]) * sizeof(char));

    // Verificando se a memória foi alocada corretamente:
    if(input_file == NULL || algorithm == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    strcpy(input_file, argv[2]);
    strcpy(algorithm, argv[1]);

    unsigned int page_size = atoi(argv[3]);
    unsigned int total_memory = atoi(argv[4]);

    unsigned int TABLE_SIZE = total_memory / page_size;
    printf("Número de entradas na tabela de páginas: %u\n", TABLE_SIZE);
    Page *hash_table = (Page*) malloc(TABLE_SIZE * sizeof(Page));

    // Verificando se a memória foi alocada corretamente:
    if(hash_table == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    read_file(input_file, &hash_table, TABLE_SIZE, page_size);

    display_results(input_file, total_memory, page_size, algorithm);
    
    // Liberando a memória alocada dinamicamente:
    free(input_file);
    free(algorithm);
    free(hash_table);

    return 0;
}
