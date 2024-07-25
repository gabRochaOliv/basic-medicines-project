
#ifndef ESTOQUE_H
#define ESTOQUE_H

typedef struct medicamento Medicamento;
typedef struct lista Lista;

/* Cria um novo medicamento */
Medicamento* CriaMedicamento(char* nome, int codigo, float valor, int* data_de_validade);

/* Cria uma nova lista */
Lista* CriaLista();

/* Insere um medicamento em uma lista */
Lista* InsereListaMedicamento(Lista* l, Medicamento* m);

/* Retira um medicamento de uma determinada lista */
Lista* RetiraListaMedicamento(Lista* l, int id_medicamento);

/* Verifica se um medicamento está presente em uma determinada lista */
int VerificaListaMedicamento(Lista* p, int id_medicamento);

/* Verifica se existe um medicamento vencido em uma determinada lista */
int VerificaListaValidade(Lista* p, int* data);

/* Imprime todos os medicamentos de uma lista */
void ImprimeListaMedicamentos(Lista* p);

/* Ordena Lista pelo valor do medicamento */
Lista* OrdenaListaValor(Lista* p);

/* Ordena Lista pela data de vencimento do medicamento */
Lista* OrdenaListaVencimento(Lista* p);

/* Libera a memória da lista */
void LiberaLista(Lista* l);

#endif // ESTOQUE_H
