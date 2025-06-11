#include<stdio.h>
#include<stdlib.h>

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
    int quantidade;
    struct Caixa * prox;
    struct NO *inicio;
    struct NO *fim;

}Caixa;
typedef struct Pilha
{
    char * tipo;
    struct Caixa * topo;

}Pilha; 

void imprimir(Pilha * pilha){    // Função de imprimir, (APENAS PARA TESTES).
    printf("----- Loja de Surf - Tati Surf Co. ------\n");
    if (pilha->topo == NULL) return;
    
    Caixa *caixa = pilha->topo;

    while (caixa != NULL) {
        NO *aux = caixa->inicio;
        while (aux != NULL) {
        printf("Tipo do produto: %s\n", aux->tipo);
        printf("Id: %d\n", aux->id);
        printf("Descrição: %s\n", aux->descricao);
        printf("Valor: %.2f\n", aux->valor);
            aux = aux->prox;
        }
        caixa = caixa->prox;
    }
}

// Essa função lida com o processo de adicionar um produto ao estoque da loja
void gerenciar_estoque(Pilha * pilha, char * tipo){

    
    if (pilha->topo == NULL || pilha->topo->quantidade == 0){// Caso seja a primeira caixa.

        Caixa * nova_caixa =  inicializar_caixa(tipo);
        add_produto_na_lista(tipo, &nova_caixa->inicio, &nova_caixa->fim);

        pilha->topo = nova_caixa;
        nova_caixa->prox = pilha->topo;
        
    } else { // Caso não seja a primeira caixa
        add_produto_na_lista(tipo, &pilha->topo->inicio, &pilha->topo->fim);
    }
    pilha->topo->quantidade--;
 }

 Caixa * inicializar_caixa(char * tipo){  // Função de criar caixas de produtos.
    
    Caixa * caixa =  malloc(sizeof(Caixa));
    caixa->id = rand() % 10000;
    caixa->tipo = tipo;
    caixa->quantidade = obter_tamanho_max_da_caixa(tipo);
    caixa->prox = NULL;
    caixa->inicio = NULL;
    caixa->fim = NULL;
    return caixa;
}

 
void add_produto_na_lista(char *tipo, NO ** inicio, NO **fim){
    NO *Novo_produto = malloc(sizeof(NO));
    char descricao[100];
    float valor;

    printf("\nDiga uma breve descricao do produto:\n");
    scanf(" %[^\n]", descricao);

    printf("Agora, Diga qual o valor do produto:\n");
    scanf("%f", &valor);

    // Novo produto recebe suas características.
    Novo_produto->tipo = tipo;
    Novo_produto->descricao = strdup(descricao);
    Novo_produto->id = rand() % 10000;
    Novo_produto->valor = valor;
    Novo_produto->prox = NULL;
    Novo_produto->ant = NULL;

    if(*inicio == NULL) {
        *inicio = Novo_produto;
        *fim = Novo_produto;

    } else {

        NO * aux = *inicio;
        
        if (Novo_produto->valor > (*fim)->valor){      // Caso1: Adiciona no fim.
            (*fim)->prox = Novo_produto;
            Novo_produto->ant = *fim;
            *fim = Novo_produto;
            
        } else if(Novo_produto->valor < aux->valor){    // Caso2: Adiciona antes do início.
            aux->ant = Novo_produto;
            Novo_produto->prox = aux;
            Novo_produto->ant = NULL;
            *inicio = Novo_produto;
            
        } else {
            
            while (Novo_produto->valor >= aux->valor)
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

 int obter_tamanho_max_da_caixa(char * tipo){

    if (strcmp(tipo, "Parafina") == 0) return 50;
    if (strcmp(tipo, "Leash") == 0) return 25;
    if (strcmp(tipo, "Quilha") == 0) return 10;
    if (strcmp(tipo, "Deck") == 0) return 5;

    return -1; // tipo inválido

 }

 int main(){

        Pilha * Leash = malloc(sizeof(Pilha));
        Pilha * Quilha = malloc(sizeof(Pilha));;
        Pilha * Parafina = malloc(sizeof(Pilha));;
        Pilha * Deck = malloc(sizeof(Pilha));;
        Leash->topo = NULL;
        Leash->tipo = "Leash";
        Quilha->topo = NULL;
        Quilha->tipo = "Quilha";
        Parafina->topo = NULL;
        Parafina->tipo = "Parafina";
        Deck->topo = NULL;
        Deck->tipo = "Deck";

                                            /*--------MENU INTERATIVO--------*/
        int valid = 1;      // Garante que o loop ocorra no menu.

        printf("\n-------Bem vindo ao sistema da Tati Surf Co.-------\n");

        while (valid)
        {
            // Info de produtos passado pelo ususário.

            int op;     // Tipo de produto que o usuário selecionar para operar no menu.

            printf("\nQual o tipo do novo produto voce deseja adicionar ao estoque da loja?\n0 - Parafina\n1 - Leash\n2 - Quilha\n3 - Deck\n4 - Sair");
            scanf("%i", &op);

            switch (op)
            {
                case 0:     // O usuário deseja adicionar Parafina ao estoque.
                        gerenciar_estoque(Parafina, "Parafina");
                        break;
                case 1:  // O usuário deseja adicionar Leash ao estoque.
                        gerenciar_estoque(Leash, "Leash");
                        break;
                case 2:     
                        gerenciar_estoque(Quilha, "Quilha");
                    break;
                case 3:     // O usuário deseja adicionar Deck ao estoque.
                        gerenciar_estoque(Deck, "Deck");
                    break;
                case 4:   // Usuário deseja encerrar o programa.
                    printf("Programa encerrado...\n");
                    valid = 0;
                    break;
                
                default:
                    break;
            }
        }
        imprimir(Parafina);
        imprimir(Leash);
        imprimir(Quilha);
        imprimir(Deck);

        return 0;
}

/* Se o usuário adicionar um tipo de produto que não tem caixa nem pilha, a função pilha pede a função de criar caixas para criar
uma caixa do tipo daquele produto, depois, a struct caixa, tem guardado os Ponteiros Início e Fim para a lista de produtos dentro
dela, a função de criar caixa pega esses ponteiros e os passa por parâmetro para a função de criar lista de produtos,aí, a lista
é iniciada com estes ponteiros. Caso haja uma pilha deste tipo que o usuário quer inserir, a função pilha irá buscar  a pilha de 
caixas desse tipo, depois, apontar para o topo, o topo será a última caixa ainda não lotada, essa caixa apontará para a lista, a 
função de cirar listas recebe o ponteiro início e fim guardados na caixa, e nesta função o produto será adicionado. A variável da 
caixa que guarda a quantidade máxima, se chegar ao seu máximo, esperará pela nova entrada de produto, no próximo produto que vier,
se na função pilha de caixas o tipo do produto contido nela apontar que o atual topo já está no seu máximo da capacidade, irá 
executar o bloco de código que cria uma nova caixa a partir da função de criar caixa, a caixa topo aponta para ela e ela receberá
a nova lista a partir da função de criar lista.*/

//Por este motivo, eu retirei os ponteiros Início e fim do Global e os Inseri em struct caixa, e esse texto será nossa ordem de funcionamento por função.
