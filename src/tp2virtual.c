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

void display_results(char *arquivo_entrada, unsigned int total_memoria, unsigned int pagina_tam, char *algoritmo_utilizado) {
    printf("Executando o simulador...\n");
    printf("Arquivo de entrada: %s\n", arquivo_entrada);
    printf("Tamanho da memória: %dKB\n", total_memoria);
    printf("Tamanho das páginas: %dKB\n", pagina_tam);
    printf("Técnica de reposição: %s\n", algoritmo_utilizado);
}

unsigned int hash_function(unsigned int chave, unsigned int TAM_HASH) {
    return chave % TAM_HASH;
}

unsigned int determine_page(unsigned int pagina_tam) {
    unsigned int s, tmp;

    // Derivar o valor de s:
    tmp = pagina_tam;
    s = 0;
    while(tmp > 1) {
        tmp = tmp>>1;
        s++;
    }

    return s;
}

void read_file(char *input_file) {
    FILE *input;
    char caminho[100] = "./logs/";
    strcat(caminho, input_file);

    input = fopen(caminho, "r");

    // Se o arquivo de entrada não for encontrado:
    if(input == NULL) {
        printf("O arquivo de entrada não foi encontrado!\n");
        exit(1);
    }

    ReadingData readingData;
    OutputData outputData;

    outputData.memory_accesses = 0;
    outputData.page_faults = 0;
    outputData.written_pages = 0;

    while((fscanf(input, "%x %c", &readingData.addr, &readingData.rw)) != EOF) {
        outputData.memory_accesses += 1;
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

    unsigned int HASH_SIZE = total_memory / page_size;
    printf("Número de entradas na tabela de páginas: %u\n", HASH_SIZE);
    Page *hash_table = (Page*) malloc(HASH_SIZE * sizeof(Page));

    // Verificando se a memória foi alocada corretamente:
    if(hash_table == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    read_file(input_file);

    display_results(input_file, total_memory, page_size, algorithm);
    
    // Liberando a memória alocada dinamicamente:
    free(input_file);
    free(algorithm);
    free(hash_table);

    return 0;
}