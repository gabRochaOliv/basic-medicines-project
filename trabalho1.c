#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estoque.h"

// Defini��es das estruturas de dados
struct medicamento {
    char nome[20];   // Nome do medicamento
    int codigo;      // C�digo de identifica��o do medicamento
    float valor;     // Valor do medicamento
    int data[3];     // Data de validade do medicamento (dia, m�s, ano)
};

struct lista {
    Medicamento* m;  // Ponteiro para um medicamento
    Lista* prox;     // Ponteiro para o pr�ximo elemento da lista
};

FILE* saida; // Ponteiro para o arquivo de sa�da

// Fun��o para imprimir os detalhes de um medicamento no arquivo de sa�da
void ImprimeMedicamento(FILE* saida, Medicamento* m) {
    fprintf(saida, "%s %d %.2f %02d/%02d/%04d\n", m->nome, m->codigo, m->valor, m->data[0], m->data[1], m->data[2]);
}

int main() {
    FILE* entrada = fopen("entrada.txt", "r"); // Abre o arquivo de entrada para leitura
    saida = fopen("saida.txt", "w"); // Abre o arquivo de sa�da para escrita
    Lista* l = CriaLista(); // Cria uma lista vazia
    char comando[30]; // Buffer para armazenar os comandos lidos do arquivo
    char nome[20]; // Buffer para armazenar o nome do medicamento
    int codigo, dia, mes, ano; // Vari�veis para armazenar dados do medicamento
    float valor; // Vari�vel para armazenar o valor do medicamento

    // L� comandos do arquivo de entrada at� o final do arquivo (EOF)
    while (fscanf(entrada, "%s", comando) != EOF) {
        // Adiciona um medicamento � lista
        if (strcmp(comando, "MEDICAMENTO") == 0) {
            fscanf(entrada, "%s %d %f %d %d %d", nome, &codigo, &valor, &dia, &mes, &ano); // L� os dados do medicamento
            int data[3] = {dia, mes, ano}; // Armazena a data de validade em um array

            Medicamento* m = CriaMedicamento(nome, codigo, valor, data); // Cria um novo medicamento
            l = InsereListaMedicamento(l, m); // Insere o medicamento na lista
            fprintf(saida, "MEDICAMENTO %s %d ADICIONADO\n", nome, codigo); // Escreve no arquivo de sa�da
        }
        // Remove um medicamento da lista
        else if (strcmp(comando, "RETIRA") == 0) {
            fscanf(entrada, "%d", &codigo); // L� o c�digo do medicamento a ser removido
            Lista* ant = NULL; // Ponteiro para o n� anterior
            Lista* p = l; // Ponteiro para percorrer a lista
            while (p != NULL && p->m->codigo != codigo) { // Percorre a lista at� encontrar o medicamento
                ant = p; // Atualiza o n� anterior
                p = p->prox; // Avan�a para o pr�ximo n�
            }
            if (p != NULL) { // Medicamento encontrado
                if (ant == NULL) { // O medicamento est� no primeiro n�
                    l = p->prox; // Remove o primeiro n�
                } else {
                    ant->prox = p->prox; // Remove o n� do meio ou do fim
                }
                fprintf(saida, "MEDICAMENTO %s %d RETIRADO\n", p->m->nome, p->m->codigo); // Escreve no arquivo de sa�da
                free(p->m); // Libera a mem�ria do medicamento
                free(p); // Libera a mem�ria do n�
            } else {
                fprintf(saida, "MEDICAMENTO %d N�O ENCONTRADO\n", codigo); // Escreve no arquivo de sa�da se o medicamento n�o foi encontrado
            }
        }
        // Imprime a lista de medicamentos
        else if (strcmp(comando, "IMPRIME_LISTA") == 0) {
            Lista* p = l; // Ponteiro para percorrer a lista
            while (p != NULL) { // Percorre a lista
                ImprimeMedicamento(saida, p->m); // Imprime os detalhes do medicamento
                p = p->prox; // Avan�a para o pr�ximo n�
            }
        }
        // Atualiza o pre�o de um medicamento
        else if (strcmp(comando, "ATUALIZA_PRECO") == 0) {
            fscanf(entrada, "%d %f", &codigo, &valor); // L� o c�digo e o novo valor do medicamento
            Lista* p = l; // Ponteiro para percorrer a lista
            while (p != NULL && p->m->codigo != codigo) { // Percorre a lista at� encontrar o medicamento
                p = p->prox; // Avan�a para o pr�ximo n�
            }
            if (p != NULL) { // Medicamento encontrado
                p->m->valor = valor; // Atualiza o valor do medicamento
                fprintf(saida, "PRE�O ATUALIZADO %s %d %.2f\n", p->m->nome, codigo, valor); // Escreve no arquivo de sa�da
            } else {
                fprintf(saida, "MEDICAMENTO %d N�O ENCONTRADO PARA ATUALIZAR PRE�O\n", codigo); // Escreve no arquivo de sa�da se o medicamento n�o foi encontrado
            }
        }
        // Verifica se h� medicamentos vencidos na lista
        else if (strcmp(comando, "VERIFICA_VALIDADE") == 0) {
            fscanf(entrada, "%d %d %d", &dia, &mes, &ano); // L� a data para verifica��o de validade
            int data[3] = {dia, mes, ano}; // Armazena a data em um array
            Lista* p = l; // Ponteiro para percorrer a lista
            int encontrado = 0; // Flag para verificar se algum medicamento vencido foi encontrado
            while (p != NULL) { // Percorre a lista
                if ((p->m->data[2] < data[2]) || // Compara o ano
                    (p->m->data[2] == data[2] && p->m->data[1] < data[1]) || // Compara o m�s
                    (p->m->data[2] == data[2] && p->m->data[1] == data[1] && p->m->data[0] < data[0])) { // Compara o dia
                    fprintf(saida, "MEDICAMENTO VENCIDO ENCONTRADO: %s %d\n", p->m->nome, p->m->codigo); // Escreve no arquivo de sa�da
                    encontrado = 1; // Atualiza a flag
                }
                p = p->prox; // Avan�a para o pr�ximo n�
            }
            if (!encontrado) {
                fprintf(saida, "NENHUM MEDICAMENTO VENCIDO ENCONTRADO\n"); // Escreve no arquivo de sa�da se nenhum medicamento vencido foi encontrado
            }
        }
        // Verifica se um medicamento est� na lista
        else if (strcmp(comando, "VERIFICA_LISTA") == 0) {
            fscanf(entrada, "%d", &codigo); // L� o c�digo do medicamento
            Lista* p = l; // Ponteiro para percorrer a lista
            int encontrado = 0; // Flag para verificar se o medicamento foi encontrado
            while (p != NULL) { // Percorre a lista
                if (p->m->codigo == codigo) { // Verifica se o medicamento est� na lista
                    fprintf(saida, "MEDICAMENTO ENCONTRADO: %s %d\n", p->m->nome, p->m->codigo); // Escreve no arquivo de sa�da
                    encontrado = 1; // Atualiza a flag
                    break; // Sai do loop
                }
                p = p->prox; // Avan�a para o pr�ximo n�
            }
            if (!encontrado) {
                fprintf(saida, "MEDICAMENTO %d N�O ENCONTRADO NA LISTA\n", codigo); // Escreve no arquivo de sa�da se o medicamento n�o foi encontrado
            }
        }
        // Ordena a lista de medicamentos pela data de validade
        else if (strcmp(comando, "ORDENA_LISTA_VALIDADE") == 0) {
            l = OrdenaListaVencimento(l); // Chama a fun��o para ordenar a lista pela data de validade
        }
        // Ordena a lista de medicamentos pelo valor
        else if (strcmp(comando, "ORDENA_LISTA_VALOR") == 0) {
            l = OrdenaListaValor(l); // Chama a fun��o para ordenar a lista pelo valor
        }
    }

    fclose(entrada); // Fecha o arquivo de entrada
    fclose(saida); // Fecha o arquivo de sa�da
    LiberaLista(l); // Libera a mem�ria alocada para a lista

    return 0; // Retorna 0 para indicar que o programa terminou com sucesso
}
