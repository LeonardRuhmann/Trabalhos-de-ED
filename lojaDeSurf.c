#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// #define PARAFINA 0
// #define LEASH 1
// #define QUILHA 2
// #define DECK 3

typedef struct NO{
    int id;
    char *tipo;
    char *descricao;
    float valor;
    struct NO * prox;
    struct NO * ant;
}NO;

typedef struct Caixa
{
    int id;
    char * tipo;
    int espaco_restante;
    struct Caixa * prox;
    struct NO *inicio_da_lista;
    struct NO *fim_da_lista;
}Caixa;

typedef struct Pilha
{
    char * tipo;
    struct Caixa * topo;
}Pilha;

// Protótipos de função (Para que o compilador conheca de antemão quem são as funções):
Caixa * inicializar_caixa(char * tipo);
void add_produto_na_lista(char *tipo, NO ** inicio_da_lista, NO **fim_da_lista);
int obter_tamanho_max_da_caixa(char * tipo);
void gerenciar_estoque(Pilha * pilha, char * tipo);
void imprimir(Pilha * pilha);

void Limpa_Buffer_i () {
    int a;
    while ((a = getchar()) != '\n' && a != EOF);
}

// Função de criar caixas de produtos.
Caixa * inicializar_caixa(char * tipo) {
    Caixa * caixa =  malloc(sizeof(Caixa));
    caixa->id = rand() % 10000;
    caixa->tipo = tipo;
    caixa->espaco_restante = obter_tamanho_max_da_caixa(tipo);
    caixa->prox = NULL;
    caixa->inicio_da_lista = NULL;
    caixa->fim_da_lista = NULL;
    return caixa;
}

// Essa função lida com o processo de adicionar um produto ao estoque da loja. Eventualmente, revenda, pesquisa ou manipulação de estoque.
void gerenciar_estoque(Pilha * pilha, char * tipo) {

    Limpa_Buffer_i();   // Limpar fila de buffer de entradas do usuário.

    // PRIMEIRO CASO: Usuário deseja estocar um produto que ainda não tem pilha e caixa, ou, tem, mas, a caixa no topo está lotada.
    if (pilha->topo == NULL || pilha->topo->espaco_restante == 0) {
        // Inicializa uma nova caixa.
        Caixa * nova_caixa =  inicializar_caixa(tipo);
        // Cria a lista de produtos dentro da nova caixa.
        add_produto_na_lista(tipo, &nova_caixa->inicio_da_lista, &nova_caixa->fim_da_lista);

        nova_caixa->prox = pilha->topo;
        pilha->topo = nova_caixa;
        
    } else { // SEGUNDO CASO: Caso hajam caixas na pilha mas na caixa topo ainda resta espaço.
        add_produto_na_lista(tipo, &pilha->topo->inicio_da_lista, &pilha->topo->fim_da_lista);
    }
    pilha->topo->espaco_restante--;
}

void add_produto_na_lista(char *tipo, NO ** inicio_da_lista, NO **fim_da_lista) {
    NO *Novo_produto = malloc(sizeof(NO));
    char descricao[100], buffer[100];
    float valor;

    // MÉTODO: Receber valores a partir de um método de fgets e sscanf intermdiado por buffer[100].
    printf("\nDiga uma breve descricao do produto (apenas 99 letras serao lidas):\n");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%[^\n]", descricao);
    Limpa_Buffer_i();   // Caso o danado do usuário digite 99+ cacarteres.

    printf("Agora, Diga qual o valor do produto (Use ponto final para diferenciar Reais de Centavos, Ex: 2.99, nao 2,99):\n");
    while (1) {
        fgets(buffer, sizeof(buffer), stdin);

        if (sscanf(buffer, "%f", &valor) == 1) {
        break;
        }
        printf("Valor inválido! Por favor, digite um número válido (ex: 19.99)\n");
    }

    // Novo produto recebe suas características.
    Novo_produto->tipo = tipo;
    Novo_produto->descricao = strdup(descricao);
    Novo_produto->id = rand() % 10000;
    Novo_produto->valor = valor;
    Novo_produto->prox = NULL;
    Novo_produto->ant = NULL;

    if(*inicio_da_lista == NULL) {      // Caso seja o primeiro produto de todos.
        *inicio_da_lista = Novo_produto;
        *fim_da_lista = Novo_produto;

    } else {

        NO * aux = *inicio_da_lista;
        
        if (Novo_produto->valor > (*fim_da_lista)->valor) {      // Caso1: Adiciona no fim da lista.
            (*fim_da_lista)->prox = Novo_produto;
            Novo_produto->ant = *fim_da_lista;
            *fim_da_lista = Novo_produto;
            
        } else if(Novo_produto->valor < aux->valor) {        // Caso2: Adiciona antes do início.
            aux->ant = Novo_produto;
            Novo_produto->prox = aux;
            Novo_produto->ant = NULL;
            *inicio_da_lista = Novo_produto;
            
        } else {        // Caso3: Adiciona ao meio da lista.
            
            while (aux != NULL && Novo_produto->valor >= aux->valor)
            {    
                aux = aux->prox;
            }
            aux->ant->prox = Novo_produto;
            Novo_produto->ant = aux->ant;
            Novo_produto->prox = aux;
            aux->ant = Novo_produto;
        }
    }
}

// A função retorna um int da capacidade total de produtos por caixa para qualquer tipo de produto.
int obter_tamanho_max_da_caixa(char * tipo) {
    if (strcmp(tipo, "Parafina") == 0) return 50;
    if (strcmp(tipo, "Leash") == 0) return 25;
    if (strcmp(tipo, "Quilha") == 0) return 10;
    if (strcmp(tipo, "Deck") == 0) return 5;

    return -1; // tipo inválido
}

// Função de impressão de produtos por pilha
void imprimir(Pilha * pilha) {
    printf("\n----- PILHA DE PRODUTOS: %s -----\n", pilha->tipo);

    if (pilha->topo == NULL) {
        printf(">> PILHA VAZIA <<\n");
        return;
    }
    
    Caixa *caixa = pilha->topo;
    int num_caixa = 1;

    while (caixa != NULL) {
        printf("\n--- Caixa ID %d | Tipo: %s | Espaço restante: %d ---\n", caixa->id, caixa->tipo, caixa->espaco_restante);

        NO *aux = caixa->inicio_da_lista;
        if (aux == NULL) {
            printf(">> Caixa vazia <<\n");
        } else {
            while (aux != NULL) {
                printf("  Produto ID: %d\n", aux->id);
                printf("    Tipo: %s\n", aux->tipo);
                printf("    Descrição: %s\n", aux->descricao);
                printf("    Valor: %.2f\n", aux->valor);
                aux = aux->prox;
            }
        }
        caixa = caixa->prox;
        num_caixa++;
    }

    printf("----- FIM DA PILHA %s -----\n", pilha->tipo);
}

int main() {

    // Definindo pilhas.
    Pilha * Leash = malloc(sizeof(Pilha));
    Pilha * Quilha = malloc(sizeof(Pilha));
    Pilha * Parafina = malloc(sizeof(Pilha));
    Pilha * Deck = malloc(sizeof(Pilha));
    Leash->topo = NULL;
    Leash->tipo = "Leash";
    Quilha->topo = NULL;
    Quilha->tipo = "Quilha";
    Parafina->topo = NULL;
    Parafina->tipo = "Parafina";
    Deck->topo = NULL;
    Deck->tipo = "Deck";

                                        /*--------MENU INTERATIVO--------*/

    printf("\n-------Bem vindo ao sistema da Tati Surf Co.-------\n");

    int valid = 1;      // Garante que o loop ocorra no menu.
    while (valid)
    {
        int op;     // Tipo de produto que o usuário selecionar para operar o estoque.
        char buffer[10];

        printf("\nDigite qual opcao de produto voce deseja adicionar no estoque da loja? (Opcoes de 0 a 4)\n0 - Parafina\n1 - Leash\n2 - Quilha\n3 - Deck\n4 - Sair\n");
        
        fgets(buffer, sizeof(buffer), stdin);

        // MÉTODO: Receber valores a partir de um método de fgets e sscanf intermdiado por buffer[10].
        if (sscanf(buffer, "%i", &op) != 1) {
            printf("\n======ENTRADA INVALIDA, NAO DIGITE LETRAS, APENAS NUMEROS DAS OPCOES DISPONIVEIS NO MENU======\n");
            Limpa_Buffer_i();   // Caso o danado digite mais que 10 caracteres. E dá mais segurança.
            continue;
        }

        switch (op)
        {
            case 0:     // O usuário deseja adicionar Parafina ao estoque.
                printf("Voce selecionou Parafina.\n");
                gerenciar_estoque(Parafina, "Parafina");
                break;
            case 1:     // O usuário deseja adicionar Leash ao estoque.
                printf("Voce selecionou Leash.\n");
                gerenciar_estoque(Leash, "Leash");
                break;
            case 2:     // O usuário deseja adicionar Quilha ao estoque.
                printf("Voce selecionou Quilha.\n");
                gerenciar_estoque(Quilha, "Quilha");
                break;
            case 3:     // O usuário deseja adicionar Deck ao estoque.
                printf("Voce selecionou Deck.\n");
                gerenciar_estoque(Deck, "Deck");
                break;
            case 4:     // Usuário deseja encerrar o programa.
                printf("Programa encerrado...\n");
                valid = 0;
                break;
            
            default:
                printf("\n======POR FAVOR, DIGITE -APENAS NUMEROS- ENTRE 0 E 4======\n");
                break;
        }
        
    }
    imprimir(Parafina);
    imprimir(Leash);
    imprimir(Quilha);
    imprimir(Deck);

    return 0;
}