#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pilha.c"

#define NOME_ARQUIVO "data.txt"

typedef struct entidade {
    char *nome;
    char *endereco;

} entidade;

void exibe_menu();
void grava_string_no_arquivo(char *string);
entidade le_entidade();
void exibe_entidade(entidade ent);
char *transforma_entidade_em_string(entidade ent);
char *cria_string_para_campo(char *campo);
int conta_digitos(int numero);
char *transforma_int_em_string(int numero);
entidade transforma_string_em_entidade(char *string);
entidade busca_entidade_por_id(int id);
int extrai_tamanho_do_registro_do_arquivo(FILE *arq, int offset);

int main() {
    int opcao_menu;

        while(opcao_menu != 4) {
        exibe_menu();
        scanf("%d", &opcao_menu);

        entidade ent;
        int id;
        char *string;
        switch(opcao_menu) {
        case 1:
            ent = le_entidade();
            exibe_entidade(ent);
            string = transforma_entidade_em_string(ent);
            grava_string_no_arquivo(string);
            break;
        case 2:
            printf("Digite o id a ser buscado: ");
            scanf("%d", &id);
            printf("\nBuscando entidade pelo id: %d\n", id);
            busca_entidade_por_id(id);
            break;
        case 3:
            break;
        case 4:
            exit(0);
            break;
        default:
            printf("\n Opcao invalida.\n");
            break;
        }
    }
    return 0;
}

entidade busca_entidade_por_id(int id) {
    printf("vai abrir o arquivo\n");
    FILE *arq = fopen(NOME_ARQUIVO, "r+");
    if(arq == NULL) {
        printf("Nao conseguiu abrir o arquivo!\n");
        exit(0);
    }
    printf("abriu o arquivo\n");

    int offset = 0;
    int i = 0;
    int tamanho_registro = extrai_tamanho_do_registro_do_arquivo(arq, offset);

    char *registro;
    while(i < id) {
        offset += tamanho_registro + conta_digitos(tamanho_registro) + 1;
        printf("offset=%d\n", offset);
        tamanho_registro = extrai_tamanho_do_registro_do_arquivo(arq, offset);
        printf("tamanho extraido: %d\n", tamanho_registro);
        i++;
    }

    printf("vai colocar o ponteiro do arquivo na posicao: %d\n", offset);
    printf("o registro lido tem o tamanho: %d\n", tamanho_registro);
    fseek(arq, offset, SEEK_SET);
    registro = (char*)malloc(sizeof(char)*tamanho_registro+4);
    fread(registro, sizeof(char), tamanho_registro+3, arq);
    registro[tamanho_registro+3] = '\0';
    printf("string tirada do arquivo: %s\n", registro);

    entidade ent;
    ent = transforma_string_em_entidade(registro);
    printf("vai exibir a entidade retirada do arquivo\n");
    exibe_entidade(ent);
    printf("terminou de exibir a entidade\n");
    fclose(arq);
    return ent;
}

int extrai_tamanho_do_registro_do_arquivo(FILE *arq, int offset) {
    fseek(arq, offset, SEEK_SET);
    char *tamanho_registro_stringficado = (char*)malloc(sizeof(char)*100);
    int num_caracteres_lidos = 0;
    char caracter_lido;

    int tamanho_registro;

    caracter_lido = fgetc(arq);
    while(caracter_lido != '|') {
        if(caracter_lido >= 48 && caracter_lido <= 57) {
            tamanho_registro_stringficado[num_caracteres_lidos] = caracter_lido;
            num_caracteres_lidos++;
        }
        caracter_lido = fgetc(arq);
    }
    tamanho_registro_stringficado[num_caracteres_lidos] = '\0';
    tamanho_registro = atoi(tamanho_registro_stringficado);
    fseek(arq, 0, SEEK_SET);

    free(tamanho_registro_stringficado);
    return tamanho_registro;
}

entidade le_entidade() {
    entidade ent;
    ent.nome = (char*)malloc(sizeof(char)*100);
    printf("Digite o nome: \n");
    scanf("%s", ent.nome);
    int tam_string = strlen(ent.nome);
    ent.nome = (char*)realloc(ent.nome, sizeof(char)*(tam_string+1));
    ent.nome[tam_string+1] = '\0';

    printf("Digite o endereco: \n");
    ent.endereco = (char*)malloc(sizeof(char)*100);
    scanf("%s", ent.endereco);
    tam_string = strlen(ent.endereco);
    ent.endereco = (char*)realloc(ent.endereco, sizeof(char)*(tam_string+1));
    ent.endereco[tam_string+1] = '\0';
    return ent;
}

void exibe_entidade(entidade ent) {
    printf("\nNome: %s\n", ent.nome);
    printf("Endereco: %s\n", ent.endereco);
}

entidade transforma_string_em_entidade(char *string) {
    entidade ent;

    int tamanho_nome = 0;
    int tamanho_endereco = 0;
    //%*[^|] le e ignora tudo ate o primeiro pipe
    // | le e ignora o primeiro pipe
    //%d le um numero inteiro
    // | le e ignora o segundo pipe
    //%*[^|] le e ignora tudo ate o terceiro pipe
    // | le e ignora o terceiro pipe
    //%d le outro numero inteiro
    //%*[^\n] le e ignora tudo ate o fim da linha
    sscanf(string, "%*[^|]|%d|%*[^|]|%d%*[^\n]", &tamanho_nome, &tamanho_endereco);

    ent.nome = (char*)malloc(sizeof(char)*tamanho_nome+1);
    ent.endereco = (char*)malloc(sizeof(char)*tamanho_endereco+1);

    sscanf(string, "%*[^|]|%*[^|]|%s%*[^\n]", ent.nome);
    ent.nome[tamanho_nome] = '\0';
    sscanf(string, "%*[^|]|%*[^|]|%*[^|]|%*[^|]|%s", ent.endereco);
    ent.endereco[tamanho_endereco] = '\0';

    return ent;
}

char *transforma_entidade_em_string(entidade ent) {
    char *string_entidade = cria_string_para_campo(ent.nome);
    string_entidade = strcat(string_entidade, cria_string_para_campo(ent.endereco));

    int tamanho_final_string_entidade = 1 + conta_digitos(strlen(string_entidade)) + strlen(string_entidade);

    char *string_entidade_temp = (char*)malloc(sizeof(char)*tamanho_final_string_entidade+1);
    string_entidade_temp[0] = '#';
    string_entidade_temp[1] = '\0';
    string_entidade_temp = strcat(string_entidade_temp, transforma_int_em_string(strlen(string_entidade)));
    string_entidade_temp = strcat(string_entidade_temp, string_entidade);

    return string_entidade_temp;
}

char *transforma_int_em_string(int numero) {
    char *string = (char*)malloc(sizeof(char)*conta_digitos(numero)+1);
    itoa(numero, string, 10);
    return string;
}

char *cria_string_para_campo(char *campo) {
    int tamanho_campo_entidade = strlen(campo);
    char *tamanho_campo_stringficado= (char*)malloc(sizeof(char)*conta_digitos(tamanho_campo_entidade)+1);
    itoa(tamanho_campo_entidade, tamanho_campo_stringficado, 10);

    int tamanho_string_final = 1 + conta_digitos(tamanho_campo_entidade) + 1 + tamanho_campo_entidade;
    char *string_final = (char*)malloc(sizeof(char)*tamanho_string_final+1);

    string_final[0] = '|';
    string_final[1] = '\0';

    string_final = strcat(string_final, tamanho_campo_stringficado);

    string_final[conta_digitos(tamanho_campo_entidade)+1] = '|';
    string_final[conta_digitos(tamanho_campo_entidade)+2] = '\0';

    string_final = strcat(string_final, campo);
    free(tamanho_campo_stringficado);
    return string_final;
}

int conta_digitos(int numero) {
    return numero == 0 ? 0 : log10(numero)+1;
}

void grava_string_no_arquivo(char *string) {
    FILE *fp = fopen(NOME_ARQUIVO, "r+");
    if(fp == NULL) {
        printf("Arquivo ainda nao existe. Criando...\n");
        fp = fopen(NOME_ARQUIVO, "w+");
        if(fp == NULL) {
            printf("Nao foi possivel criar arquivo.\n");
        }
    }
    fseek(fp, 0, SEEK_END);
    printf("string que vai ser gravada no arquivo: %s\n", string);
    fprintf(fp, "%s", string);
    fclose(fp);
}

void exibe_menu() {
    printf("\n1. Cadastrar novo usuario\n");
    printf("2. Buscar usuario por indice\n");
    printf("3. Listar todos os usuarios\n");
    printf("4. Sair\n");
    printf("Opcao: ");
}
