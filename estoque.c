#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "estoque.h" // Inclui o arquivo de cabeçalho "estoque.h"

// Definição da estrutura Medicamento
struct medicamento {
    char nome[20];   // Nome do medicamento
    int codigo;      // Código de identificação do medicamento
    float valor;     // Valor do medicamento
    int data[3];     // Data de validade do medicamento (dia, mês, ano)
};

// Definição da estrutura Lista
struct lista {
    Medicamento* m;  // Ponteiro para um medicamento
    Lista* prox;     // Ponteiro para o próximo elemento da lista
};

// Função para criar um novo medicamento
// Retorna: ponteiro para o medicamento criado
Medicamento* CriaMedicamento(char* nome, int codigo, float valor, int* data_de_validade) {
    Medicamento* m = (Medicamento*)malloc(sizeof(Medicamento)); // Aloca memória para o medicamento
    strcpy(m->nome, nome); // Copia o nome do medicamento
    m->codigo = codigo; // Define o código do medicamento
    m->valor = valor; // Define o valor do medicamento
    memcpy(m->data, data_de_validade, 3 * sizeof(int)); // Copia a data de validade
    return m; // Retorna o medicamento criado
}

// Função para criar uma lista vazia
// Retorna: NULL (lista vazia)
Lista* CriaLista() {
    return NULL; // Inicializa a lista como vazia
}

// Função para inserir um medicamento na lista
// Retorna: ponteiro para a nova lista com o medicamento inserido
Lista* InsereListaMedicamento(Lista* l, Medicamento* m) {
    Lista* novo = (Lista*)malloc(sizeof(Lista)); // Aloca memória para o novo nó da lista
    novo->m = m; // Define o medicamento do novo nó
    novo->prox = l; // Aponta para o próximo nó da lista (inserção no início)
    return novo; // Retorna o novo início da lista
}

// Função para remover um medicamento da lista
// Retorna: ponteiro para a lista sem o medicamento removido
Lista* RetiraListaMedicamento(Lista* l, int id_medicamento) {
    Lista* ant = NULL; // Ponteiro para o nó anterior
    Lista* p = l; // Ponteiro para percorrer a lista
    while (p != NULL && p->m->codigo != id_medicamento) { // Percorre a lista até encontrar o medicamento
        ant = p; // Atualiza o nó anterior
        p = p->prox; // Avança para o próximo nó
    }
    if (p == NULL) return l; // Medicamento não encontrado, retorna a lista original
    if (ant == NULL) { // O medicamento está no primeiro nó
        l = p->prox; // Remove o primeiro nó
    } else {
        ant->prox = p->prox; // Remove o nó do meio ou do fim
    }
    free(p->m); // Libera a memória do medicamento
    free(p); // Libera a memória do nó
    return l; // Retorna a lista sem o medicamento removido
}

// Função para verificar se um medicamento está na lista
// Retorna: 1 se o medicamento está na lista, 0 caso contrário
int VerificaListaMedicamento(Lista* p, int id_medicamento) {
    while (p != NULL) { // Percorre a lista
        if (p->m->codigo == id_medicamento) { // Verifica se o medicamento está na lista
            return 1; // Medicamento encontrado
        }
        p = p->prox; // Avança para o próximo nó
    }
    return 0; // Medicamento não encontrado
}

// Função para verificar se há medicamentos vencidos na lista
// Retorna: 1 se há medicamentos vencidos, 0 caso contrário
int VerificaListaValidade(Lista* p, int* data) {
    while (p != NULL) { // Percorre a lista
        if (p->m->data[2] < data[2] || // Compara o ano
            (p->m->data[2] == data[2] && p->m->data[1] < data[1]) || // Compara o mês
            (p->m->data[2] == data[2] && p->m->data[1] == data[1] && p->m->data[0] < data[0])) { // Compara o dia
            return 1; // Medicamento vencido encontrado
        }
        p = p->prox; // Avança para o próximo nó
    }
    return 0; // Nenhum medicamento vencido encontrado
}

// Função para imprimir a lista de medicamentos
void ImprimeListaMedicamentos(Lista* p) {
    while (p != NULL) { // Percorre a lista
        printf("%s %d %.2f %d/%d/%d\n", p->m->nome, p->m->codigo, p->m->valor, p->m->data[0], p->m->data[1], p->m->data[2]); // Imprime os detalhes do medicamento
        p = p->prox; // Avança para o próximo nó
    }
}

// Função para ordenar a lista de medicamentos pelo valor
// Retorna: ponteiro para a lista ordenada pelo valor
Lista* OrdenaListaValor(Lista* l) {
    if (l == NULL || l->prox == NULL) return l; // Se a lista estiver vazia ou com um único nó, não há necessidade de ordenar
    Lista* ordenada = NULL; // Ponteiro para a lista ordenada
    Lista* atual = l; // Ponteiro para o nó atual
    Lista* prox; // Ponteiro para o próximo nó
    while (atual != NULL) { // Percorre a lista
        prox = atual->prox; // Armazena o próximo nó
        if (ordenada == NULL || atual->m->valor < ordenada->m->valor) { // Insere o nó no início da lista ordenada
            atual->prox = ordenada;
            ordenada = atual;
        } else { // Insere o nó no meio ou no fim da lista ordenada
            Lista* anterior = ordenada;
            while (anterior->prox != NULL && anterior->prox->m->valor <= atual->m->valor) { // Encontra a posição correta para inserir o nó
                anterior = anterior->prox;
            }
            atual->prox = anterior->prox; // Insere o nó na posição correta
            anterior->prox = atual;
        }
        atual = prox; // Avança para o próximo nó
    }
    return ordenada; // Retorna a lista ordenada
}

// Função para ordenar a lista de medicamentos pela data de validade
// Retorna: ponteiro para a lista ordenada pela data de validade
Lista* OrdenaListaVencimento(Lista* l) {
    if (l == NULL || l-> prox == NULL) return l; // Se a lista estiver vazia ou com um único nó, não há necessidade de ordenar
    Lista* ordenada = NULL; // Ponteiro para a lista ordenada
    Lista* atual = l; // Ponteiro para o nó atual
    Lista* prox; // Ponteiro para o próximo nó
    while (atual != NULL) { // Percorre a lista
        prox = atual->prox; // Armazena o próximo nó
        if (ordenada == NULL || atual->m->data[2] < ordenada->m->data[2] || // Insere o nó no início da lista ordenada
            (atual->m->data[2] == ordenada->m->data[2] && atual->m->data[1] < ordenada->m->data[1]) ||
            (atual->m->data[2] == ordenada->m->data[2] && atual->m->data[1] == ordenada->m->data[1] && atual->m->data[0] < ordenada->m->data[0])) {
            atual->prox = ordenada;
            ordenada = atual;
        } else { // Insere o nó no meio ou no fim da lista ordenada
            Lista* anterior = ordenada;
            while (anterior->prox != NULL && anterior->prox->m->data[2] <= atual->m->data[2]) { // Encontra a posição correta para inserir o nó
                anterior = anterior->prox;
            }
            atual->prox = anterior->prox; // Insere o nó na posição correta
            anterior->prox = atual;
        }
        atual = prox; // Avança para o próximo nó
    }
    return ordenada; // Retorna a lista ordenada
}

// Função para liberar a memória alocada pela lista
void LiberaLista(Lista* l) {
    Lista* atual = l; // Ponteiro para o nó atual
    while (atual != NULL) { // Percorre a lista
        Lista* temp = atual->prox; // Armazena o próximo nó
        free(atual->m); // Libera a memória do medicamento
        free(atual); // Libera a memória do nó
        atual = temp; // Avança para o próximo nó
    }
}
