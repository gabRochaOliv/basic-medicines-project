#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estoque.h" // Inclui o arquivo de cabe�alho "estoque.h"

// Defini��o da estrutura Medicamento
struct medicamento {
    char nome[20];   // Nome do medicamento
    int codigo;      // C�digo de identifica��o do medicamento
    float valor;     // Valor do medicamento
    int data[3];     // Data de validade do medicamento (dia, m�s, ano)
};

// Defini��o da estrutura Lista
struct lista {
    Medicamento* m;  // Ponteiro para um medicamento
    Lista* prox;     // Ponteiro para o pr�ximo elemento da lista
};

// Fun��o para criar um novo medicamento
// Retorna: ponteiro para o medicamento criado
Medicamento* CriaMedicamento(char* nome, int codigo, float valor, int* data_de_validade) {
    Medicamento* m = (Medicamento*)malloc(sizeof(Medicamento)); // Aloca mem�ria para o medicamento
    strcpy(m->nome, nome); // Copia o nome do medicamento
    m->codigo = codigo; // Define o c�digo do medicamento
    m->valor = valor; // Define o valor do medicamento
    memcpy(m->data, data_de_validade, 3 * sizeof(int)); // Copia a data de validade
    return m; // Retorna o medicamento criado
}

// Fun��o para criar uma lista vazia
// Retorna: NULL (lista vazia)
Lista* CriaLista() {
    return NULL; // Inicializa a lista como vazia
}

// Fun��o para inserir um medicamento na lista
// Retorna: ponteiro para a nova lista com o medicamento inserido
Lista* InsereListaMedicamento(Lista* l, Medicamento* m) {
    Lista* novo = (Lista*)malloc(sizeof(Lista)); // Aloca mem�ria para o novo n� da lista
    novo->m = m; // Define o medicamento do novo n�
    novo->prox = l; // Aponta para o pr�ximo n� da lista (inser��o no in�cio)
    return novo; // Retorna o novo in�cio da lista
}

// Fun��o para remover um medicamento da lista
// Retorna: ponteiro para a lista sem o medicamento removido
Lista* RetiraListaMedicamento(Lista* l, int id_medicamento) {
    Lista* ant = NULL; // Ponteiro para o n� anterior
    Lista* p = l; // Ponteiro para percorrer a lista
    while (p != NULL && p->m->codigo != id_medicamento) { // Percorre a lista at� encontrar o medicamento
        ant = p; // Atualiza o n� anterior
        p = p->prox; // Avan�a para o pr�ximo n�
    }
    if (p == NULL) return l; // Medicamento n�o encontrado, retorna a lista original
    if (ant == NULL) { // O medicamento est� no primeiro n�
        l = p->prox; // Remove o primeiro n�
    } else {
        ant->prox = p->prox; // Remove o n� do meio ou do fim
    }
    free(p->m); // Libera a mem�ria do medicamento
    free(p); // Libera a mem�ria do n�
    return l; // Retorna a lista sem o medicamento removido
}

// Fun��o para verificar se um medicamento est� na lista
// Retorna: 1 se o medicamento est� na lista, 0 caso contr�rio
int VerificaListaMedicamento(Lista* p, int id_medicamento) {
    while (p != NULL) { // Percorre a lista
        if (p->m->codigo == id_medicamento) { // Verifica se o medicamento est� na lista
            return 1; // Medicamento encontrado
        }
        p = p->prox; // Avan�a para o pr�ximo n�
    }
    return 0; // Medicamento n�o encontrado
}

// Fun��o para verificar se h� medicamentos vencidos na lista
// Retorna: 1 se h� medicamentos vencidos, 0 caso contr�rio
int VerificaListaValidade(Lista* p, int* data) {
    while (p != NULL) { // Percorre a lista
        if (p->m->data[2] < data[2] || // Compara o ano
            (p->m->data[2] == data[2] && p->m->data[1] < data[1]) || // Compara o m�s
            (p->m->data[2] == data[2] && p->m->data[1] == data[1] && p->m->data[0] < data[0])) { // Compara o dia
            return 1; // Medicamento vencido encontrado
        }
        p = p->prox; // Avan�a para o pr�ximo n�
    }
    return 0; // Nenhum medicamento vencido encontrado
}

// Fun��o para imprimir a lista de medicamentos
void ImprimeListaMedicamentos(Lista* p) {
    while (p != NULL) { // Percorre a lista
        printf("%s %d %.2f %d/%d/%d\n", p->m->nome, p->m->codigo, p->m->valor, p->m->data[0], p->m->data[1], p->m->data[2]); // Imprime os detalhes do medicamento
        p = p->prox; // Avan�a para o pr�ximo n�
    }
}

// Fun��o para ordenar a lista de medicamentos pelo valor
// Retorna: ponteiro para a lista ordenada pelo valor
Lista* OrdenaListaValor(Lista* l) {
    if (l == NULL || l->prox == NULL) return l; // Se a lista estiver vazia ou com um �nico n�, n�o h� necessidade de ordenar
    Lista* ordenada = NULL; // Ponteiro para a lista ordenada
    Lista* atual = l; // Ponteiro para o n� atual
    Lista* prox; // Ponteiro para o pr�ximo n�
    while (atual != NULL) { // Percorre a lista
        prox = atual->prox; // Armazena o pr�ximo n�
        if (ordenada == NULL || atual->m->valor < ordenada->m->valor) { // Insere o n� no in�cio da lista ordenada
            atual->prox = ordenada;
            ordenada = atual;
        } else { // Insere o n� no meio ou no fim da lista ordenada
            Lista* anterior = ordenada;
            while (anterior->prox != NULL && anterior->prox->m->valor <= atual->m->valor) { // Encontra a posi��o correta para inserir o n�
                anterior = anterior->prox;
            }
            atual->prox = anterior->prox; // Insere o n� na posi��o correta
            anterior->prox = atual;
        }
        atual = prox; // Avan�a para o pr�ximo n�
    }
    return ordenada; // Retorna a lista ordenada
}

// Fun��o para ordenar a lista de medicamentos pela data de validade
// Retorna: ponteiro para a lista ordenada pela data de validade
Lista* OrdenaListaVencimento(Lista* l) {
    if (l == NULL || l-> prox == NULL) return l; // Se a lista estiver vazia ou com um �nico n�, n�o h� necessidade de ordenar
    Lista* ordenada = NULL; // Ponteiro para a lista ordenada
    Lista* atual = l; // Ponteiro para o n� atual
    Lista* prox; // Ponteiro para o pr�ximo n�
    while (atual != NULL) { // Percorre a lista
        prox = atual->prox; // Armazena o pr�ximo n�
        if (ordenada == NULL || atual->m->data[2] < ordenada->m->data[2] || // Insere o n� no in�cio da lista ordenada
            (atual->m->data[2] == ordenada->m->data[2] && atual->m->data[1] < ordenada->m->data[1]) ||
            (atual->m->data[2] == ordenada->m->data[2] && atual->m->data[1] == ordenada->m->data[1] && atual->m->data[0] < ordenada->m->data[0])) {
            atual->prox = ordenada;
            ordenada = atual;
        } else { // Insere o n� no meio ou no fim da lista ordenada
            Lista* anterior = ordenada;
            while (anterior->prox != NULL && anterior->prox->m->data[2] <= atual->m->data[2]) { // Encontra a posi��o correta para inserir o n�
                anterior = anterior->prox;
            }
            atual->prox = anterior->prox; // Insere o n� na posi��o correta
            anterior->prox = atual;
        }
        atual = prox; // Avan�a para o pr�ximo n�
    }
    return ordenada; // Retorna a lista ordenada
}

// Fun��o para liberar a mem�ria alocada pela lista
void LiberaLista(Lista* l) {
    Lista* atual = l; // Ponteiro para o n� atual
    while (atual != NULL) { // Percorre a lista
        Lista* temp = atual->prox; // Armazena o pr�ximo n�
        free(atual->m); // Libera a mem�ria do medicamento
        free(atual); // Libera a mem�ria do n�
        atual = temp; // Avan�a para o pr�ximo n�
    }
}
