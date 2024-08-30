#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/dataStructures.h"
#include "../headers/tp2virtual.h"
#include "../headers/algorithms.h"

// Função utilizada para verificar se os argumentos passados estão corretos:
int check_arguments(int argc, char *argv[]) {
    // Verificando se o número de argumentos passados está correto:
    if(argc != 5 && argc != 6) {
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

    if(argc == 6) {
        if(strcmp(argv[5], "debug") != 0) {
            printf("O último parâmetro fornecido está incorreto. O correto é: debug!\n");
            return -1;       
        }
    }

    return 0;
}

// Função utilizada para gerar o relatório final:
void display_results(char *input_file, unsigned int total_memory, unsigned int page_size, char *algorithm, OutputData outputData) {
    printf("Arquivo de entrada: %s\n", input_file);
    printf("Tamanho da memória: %dKB\n", total_memory);
    printf("Tamanho das páginas: %dKB\n", page_size);
    printf("Técnica de reposição: %s\n", algorithm);
    printf("Páginas lidas: %u\n", outputData.read_pages);
    printf("Páginas escritas: %u\n", outputData.written_pages);
    printf("Faltas de páginas: %u\n", outputData.page_faults);
}

// Função hash utilizada para determinar o índice usado na hash_table:
unsigned int hash_function(unsigned int key, unsigned int TABLE_SIZE) {
    return key % TABLE_SIZE;
}

// Função utilizada para determinar a página associada a um endereço:
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

/*
    Função utilizada para salvar um arquivo "depuracao.txt" se caso debug for
    passado como último argumento, descrevendo o que é feito a cada acesso à
    memória:
*/
void save_test_file(Page *page, int page_found, char rw) {
    FILE *output;
    output = fopen("./depuracao.txt", "a");

    // Erro ao criar/abrir o arquivo de teste:
    if(output == NULL) {
        printf("Ocorreu um erro ao criar/abrir o arquivo de teste!\n");
        exit(1);
    }

    fprintf(output, "Página %u acessada!\n", page->page_number);

    if(page_found == 0) {
        fprintf(output, "Ocorreu um page fault!\n");

        if(rw == 'W') {
            fprintf(output, "Necessário atualizar a cópia que está no disco!\n");
        }

        else {
            fprintf(output, "Página lida!\n");
        }
    }

    else {
        fprintf(output, "Não ocorreu um page fault!\n");

        if(rw == 'W') {
            fprintf(output, "Necessário atualizar a cópia que está no disco!\n");
        }

        else {
            fprintf(output, "Página lida!\n");
        }
    }

    fprintf(output, "\n");

    fclose(output);
}

// Função utilizada para ler o arquivo de entrada fornecido como parâmetro:
OutputData read_file(char *input_file, Page **hash_table, unsigned int TABLE_SIZE, unsigned int page_size, char *algorithm, unsigned int debug_mode) {
    FILE *input;
    char caminho[100] = "./logs/";
    strcat(caminho, input_file);

    input = fopen(caminho, "r");

    // Se o arquivo de entrada não for encontrado:
    if(input == NULL) {
        printf("O arquivo de entrada não foi encontrado!\n");
        exit(1);
    }

    unsigned int page_number, page_index;
    int page_found;
    unsigned int clock_count = 0;

    ReadingData readingData;
    OutputData outputData;

    outputData.read_pages = 0;
    outputData.page_faults = 0;
    outputData.written_pages = 0;

    List list;
    list.beginning = NULL;
    list.end = NULL;

    Page *page = NULL;
    Page *last_page = NULL;

    while((fscanf(input, "%x %c", &readingData.addr, &readingData.rw)) != EOF) {

        if(clock_count >= CLOCK_INTERRUPT) {
            if(strcmp(algorithm, "lru") == 0) {
                update_pages_ages(hash_table, TABLE_SIZE);
            }

            else if(strcmp(algorithm, "nru") == 0) {
                update_referenced_bit(hash_table, TABLE_SIZE);
            }

            clock_count = 0;
        }

        page_number = determine_page(readingData.addr, page_size);
        page_index = hash_function(page_number, TABLE_SIZE);

        /*
            Se o rw do endereço de memória não for 'R' e 'W', ignoramos a
            linha atual e lemos o próximo endereço de memória:
        */
        
        if(readingData.rw != 'R' && readingData.rw != 'W') {
            continue;
        }

        page = hash_table[page_index];
        page_found = 0;
        while(page) {
            if((*page).page_number == page_number) {
                page_found = 1;
                break;
            } 

            page = page->next;
        }

        // A página não está referenciada na memória principal:
        if(page_found == 0) {

            // Se o algoritmo escolhido for o lru, o executamos:
            if(strcmp(algorithm, "lru") == 0) {
                lru(hash_table, TABLE_SIZE);
            }

            // Se o algoritmo escolhido for o nru, o executamos:
            else if(strcmp(algorithm, "nru") == 0) {
                nru(hash_table, TABLE_SIZE);
            }

            // Se o algoritmo escolhido for o segunda_chance, o executamos:
            else {
                second_chance(hash_table, TABLE_SIZE, &list);
            }

            Page *new_page = (Page*) malloc(sizeof(Page));

            // Verificando se a memória foi alocada corretamente:
            if(new_page == NULL) {
                printf("A memória não foi alocada corretamente!\n");
                exit(1);
            }

            (*new_page).page_number = page_number;
            (*new_page).referenced = 1;
            
            if(readingData.rw == 'R') {
                outputData.read_pages++;
            }

            else {
                (*new_page).modified = 1;
                outputData.written_pages++;
            }

            (*new_page).valid = 1;
            
            // Se ainda não há nenhuma página em hash_table[page_index]:
            if(hash_table[page_index] == NULL) {
                hash_table[page_index] = new_page;
            }

            // Se já há páginas em hash_table[page_index]:
            else {
                last_page = hash_table[page_index];
                
                while(last_page->next) {
                    last_page = last_page->next;
                }

                last_page->next = new_page;
            }

            new_page->next = NULL;
            outputData.page_faults++;

            if(debug_mode == 1) {
                save_test_file(new_page, page_found, readingData.rw);
            }
        }
            
        // A página já está referenciada na memória principal:
        else {
            (*page).referenced = 1;
            
            if(readingData.rw == 'R') {
                outputData.read_pages++;
            }

            else {
                (*page).modified = 1;
                outputData.written_pages++;
            }

            if(debug_mode == 1) {
                save_test_file(page, page_found, readingData.rw);
            }
        }

        clock_count++;
    }

    fclose(input);
    return outputData;
}

/*
    Função utilizada para liberar cada página na tabela de páginas e em seguida
    liberar a tabela de páginas como um todo:
*/
void free_hash_table(Page **hash_table, unsigned int TABLE_SIZE) {
    for(unsigned int i = 0; i < TABLE_SIZE; i++) {
        Page *page = hash_table[i];
        while(page) {
            Page *next_page = page->next;
            
            // Liberando a memória alocada dinamicamente:
            free(page);

            page = next_page;
        }
    }

    // Liberando a memória alocada dinamicamente:
    free(hash_table);
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
    Page **hash_table = (Page**) malloc(TABLE_SIZE * sizeof(Page*));

    // Verificando se a memória foi alocada corretamente:
    if(hash_table == NULL) {
        printf("A memória não foi alocada corretamente!\n");
        exit(1);
    }

    // Inicializa hash_table:
    for(unsigned int i = 0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }

    unsigned int debug_mode = 0;
    if(argc == 6) {
        debug_mode = 1;
    }

    printf("Executando o simulador...\n");
    OutputData outputdata = read_file(input_file, hash_table, TABLE_SIZE, page_size, algorithm, debug_mode);

    display_results(input_file, total_memory, page_size, algorithm, outputdata);
    
    // Liberando a memória alocada dinamicamente:
    free(input_file);
    free(algorithm);
    free_hash_table(hash_table, TABLE_SIZE);

    return 0;
}
