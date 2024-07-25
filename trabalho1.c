#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estoque.h"

// Definições das estruturas de dados
struct medicamento {
    char nome[20];   // Nome do medicamento
    int codigo;      // Código de identificação do medicamento
    float valor;     // Valor do medicamento
    int data[3];     // Data de validade do medicamento (dia, mês, ano)
};

struct lista {
    Medicamento* m;  // Ponteiro para um medicamento
    Lista* prox;     // Ponteiro para o próximo elemento da lista
};

FILE* saida; // Ponteiro para o arquivo de saída

// Função para imprimir os detalhes de um medicamento no arquivo de saída
void ImprimeMedicamento(FILE* saida, Medicamento* m) {
    fprintf(saida, "%s %d %.2f %02d/%02d/%04d\n", m->nome, m->codigo, m->valor, m->data[0], m->data[1], m->data[2]);
}

int main() {
    FILE* entrada = fopen("entrada.txt", "r"); // Abre o arquivo de entrada para leitura
    saida = fopen("saida.txt", "w"); // Abre o arquivo de saída para escrita
    Lista* l = CriaLista(); // Cria uma lista vazia
    char comando[30]; // Buffer para armazenar os comandos lidos do arquivo
    char nome[20]; // Buffer para armazenar o nome do medicamento
    int codigo, dia, mes, ano; // Variáveis para armazenar dados do medicamento
    float valor; // Variável para armazenar o valor do medicamento

    // Lê comandos do arquivo de entrada até o final do arquivo (EOF)
    while (fscanf(entrada, "%s", comando) != EOF) {
        // Adiciona um medicamento à lista
        if (strcmp(comando, "MEDICAMENTO") == 0) {
            fscanf(entrada, "%s %d %f %d %d %d", nome, &codigo, &valor, &dia, &mes, &ano); // Lê os dados do medicamento
            int data[3] = {dia, mes, ano}; // Armazena a data de validade em um array

            Medicamento* m = CriaMedicamento(nome, codigo, valor, data); // Cria um novo medicamento
            l = InsereListaMedicamento(l, m); // Insere o medicamento na lista
            fprintf(saida, "MEDICAMENTO %s %d ADICIONADO\n", nome, codigo); // Escreve no arquivo de saída
        }
        // Remove um medicamento da lista
        else if (strcmp(comando, "RETIRA") == 0) {
            fscanf(entrada, "%d", &codigo); // Lê o código do medicamento a ser removido
            Lista* ant = NULL; // Ponteiro para o nó anterior
            Lista* p = l; // Ponteiro para percorrer a lista
            while (p != NULL && p->m->codigo != codigo) { // Percorre a lista até encontrar o medicamento
                ant = p; // Atualiza o nó anterior
                p = p->prox; // Avança para o próximo nó
            }
            if (p != NULL) { // Medicamento encontrado
                if (ant == NULL) { // O medicamento está no primeiro nó
                    l = p->prox; // Remove o primeiro nó
                } else {
                    ant->prox = p->prox; // Remove o nó do meio ou do fim
                }
                fprintf(saida, "MEDICAMENTO %s %d RETIRADO\n", p->m->nome, p->m->codigo); // Escreve no arquivo de saída
                free(p->m); // Libera a memória do medicamento
                free(p); // Libera a memória do nó
            } else {
                fprintf(saida, "MEDICAMENTO %d NÃO ENCONTRADO\n", codigo); // Escreve no arquivo de saída se o medicamento não foi encontrado
            }
        }
        // Imprime a lista de medicamentos
        else if (strcmp(comando, "IMPRIME_LISTA") == 0) {
            Lista* p = l; // Ponteiro para percorrer a lista
            while (p != NULL) { // Percorre a lista
                ImprimeMedicamento(saida, p->m); // Imprime os detalhes do medicamento
                p = p->prox; // Avança para o próximo nó
            }
        }
        // Atualiza o preço de um medicamento
        else if (strcmp(comando, "ATUALIZA_PRECO") == 0) {
            fscanf(entrada, "%d %f", &codigo, &valor); // Lê o código e o novo valor do medicamento
            Lista* p = l; // Ponteiro para percorrer a lista
            while (p != NULL && p->m->codigo != codigo) { // Percorre a lista até encontrar o medicamento
                p = p->prox; // Avança para o próximo nó
            }
            if (p != NULL) { // Medicamento encontrado
                p->m->valor = valor; // Atualiza o valor do medicamento
                fprintf(saida, "PREÇO ATUALIZADO %s %d %.2f\n", p->m->nome, codigo, valor); // Escreve no arquivo de saída
            } else {
                fprintf(saida, "MEDICAMENTO %d NÃO ENCONTRADO PARA ATUALIZAR PREÇO\n", codigo); // Escreve no arquivo de saída se o medicamento não foi encontrado
            }
        }
        // Verifica se há medicamentos vencidos na lista
        else if (strcmp(comando, "VERIFICA_VALIDADE") == 0) {
            fscanf(entrada, "%d %d %d", &dia, &mes, &ano); // Lê a data para verificação de validade
            int data[3] = {dia, mes, ano}; // Armazena a data em um array
            Lista* p = l; // Ponteiro para percorrer a lista
            int encontrado = 0; // Flag para verificar se algum medicamento vencido foi encontrado
            while (p != NULL) { // Percorre a lista
                if ((p->m->data[2] < data[2]) || // Compara o ano
                    (p->m->data[2] == data[2] && p->m->data[1] < data[1]) || // Compara o mês
                    (p->m->data[2] == data[2] && p->m->data[1] == data[1] && p->m->data[0] < data[0])) { // Compara o dia
                    fprintf(saida, "MEDICAMENTO VENCIDO ENCONTRADO: %s %d\n", p->m->nome, p->m->codigo); // Escreve no arquivo de saída
                    encontrado = 1; // Atualiza a flag
                }
                p = p->prox; // Avança para o próximo nó
            }
            if (!encontrado) {
                fprintf(saida, "NENHUM MEDICAMENTO VENCIDO ENCONTRADO\n"); // Escreve no arquivo de saída se nenhum medicamento vencido foi encontrado
            }
        }
        // Verifica se um medicamento está na lista
        else if (strcmp(comando, "VERIFICA_LISTA") == 0) {
            fscanf(entrada, "%d", &codigo); // Lê o código do medicamento
            Lista* p = l; // Ponteiro para percorrer a lista
            int encontrado = 0; // Flag para verificar se o medicamento foi encontrado
            while (p != NULL) { // Percorre a lista
                if (p->m->codigo == codigo) { // Verifica se o medicamento está na lista
                    fprintf(saida, "MEDICAMENTO ENCONTRADO: %s %d\n", p->m->nome, p->m->codigo); // Escreve no arquivo de saída
                    encontrado = 1; // Atualiza a flag
                    break; // Sai do loop
                }
                p = p->prox; // Avança para o próximo nó
            }
            if (!encontrado) {
                fprintf(saida, "MEDICAMENTO %d NÃO ENCONTRADO NA LISTA\n", codigo); // Escreve no arquivo de saída se o medicamento não foi encontrado
            }
        }
        // Ordena a lista de medicamentos pela data de validade
        else if (strcmp(comando, "ORDENA_LISTA_VALIDADE") == 0) {
            l = OrdenaListaVencimento(l); // Chama a função para ordenar a lista pela data de validade
        }
        // Ordena a lista de medicamentos pelo valor
        else if (strcmp(comando, "ORDENA_LISTA_VALOR") == 0) {
            l = OrdenaListaValor(l); // Chama a função para ordenar a lista pelo valor
        }
    }

    fclose(entrada); // Fecha o arquivo de entrada
    fclose(saida); // Fecha o arquivo de saída
    LiberaLista(l); // Libera a memória alocada para a lista

    return 0; // Retorna 0 para indicar que o programa terminou com sucesso
}
