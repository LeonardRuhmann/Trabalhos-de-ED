#include<stdio.h>
#include<stdlib.h>

typedef struct NO   // lista de produtos à venda.
{
    int id;
    char *tipo;
    char *descricao;
    float valor;
    struct NO * prox;
    struct NO * ant;
}NO;

typedef struct Caixa    // Caixa que contém lista de produtos.
{
    int id;
    char * tipo;
    int tamanho_max;
    struct Caixa * prox;
    struct NO * lista;

    struct lista *inicio;
    struct lista *fim;

}Caixa;

typedef struct Pilha    // Pilha de caixas, uma para cada tipo de produto.
{
    char * tipo;
    struct Caixa * topo;

}Pilha;

int tam = 0;

void imprimir(){    // Função de imprimir, (APENAS PARA TESTES).
    NO *aux = inicio;
    printf("----- Loja de Surf - Tati Surf Co. ------\n");

    for(int i=0; i<=tam; i++){
        printf("Tipo do produto: %s\n", aux->tipo);
        printf("Id: %d\n", aux->id);
        printf("Descrição: %s\n", aux->descricao);
        printf("Valor: %.2f\n", aux->valor);

        aux = aux->prox;
    }
}
// void criar_pilha(){
//     Caixa * caixa;
// }


void criar_caixa(NO * lista, char * tipo){  // Função de criar caixas de produtos.
    Caixa * caixa =  malloc(sizeof(caixa));

    // Caso seja a primeira caixa.
    caixa->lista = lista;
    caixa->tipo = tipo;
    caixa->prox = NULL;
}

void add_produto_na_lista(char *tipo, char *descricao, int id, float valor){    //Adiciona um novo produto à lista.
    NO *novo = malloc(sizeof(NO));

    // Novo produto recebe suas características.
    novo->tipo = tipo;
    novo->descricao = descricao;
    novo->id = id;
    novo->valor = valor;

    if(inicio == NULL){     // Se for o primeiro produto.
        inicio = novo;
        fim = novo;

        
        novo->prox = NULL;
        novo->ant = NULL;

    } else {    // Caso haja mais de um produto na lista.

        NO * aux = inicio;

        if (novo->valor > fim->valor){      // Caso1: Adiciona no fim.
            fim->prox = novo;
            novo->ant = fim;
            fim = novo;

        } else if(novo->valor < aux->valor){    // Caso2: Adiciona antes do início.
            aux->ant = novo;
            novo->prox = aux;
            novo->ant = NULL;
            inicio = novo;

        } else {

            while (novo->valor >= aux->valor)
            {    
                aux = aux->prox;
            }
            aux->ant->prox = novo;
            novo->ant = aux->ant;
            novo->prox = aux;
            aux->ant = novo;
        }
        tam++;
    }
 }


    int main(){

        // add_produto_na_lista("Quilha", "Fibra", 123, 120.50);
        // add_produto_na_lista("Parafina", "Agua fria", 321, 20);
        // add_produto_na_lista("Leash", "BalyGrip", 111, 90); //
        // add_produto_na_lista("Quilha", "repetido", 145, 90); // 2 20 90L 90Q 120
        // add_produto_na_lista("Parafina", "Ruim", 893, 2);

                                            /*--------MENU INTERATIVO--------*/
        int valid = 1;      // Garante que o loop ocorra no menu.

        printf("\n-------Bem vindo ao sistema da Tati Surf Co.-------\n");

        while (valid)
        {
            int op;     // Tipo de produto que o usuário selecionar para operar no menu.

            printf("\nQual o tipo do novo produto voce deseja adicionar ao estoque da loja?\n");
            scanf("%d", &op);

            switch (op)
            {
                char *descr;
                float val;
                int id;

            case 0:     // Usuário deseja encerrar o programa.
                valid = 0;
                break;

            case 1:     // O usuário deseja adicionar Quilha ao estoque.
                printf("Voce deseja adicionar Quilha ao estoque:\nDiga a descricao:\n");
                scanf("%c", descr);

                printf("Agora, diga qual a ID do produto:\n");
                scanf("%d", id);

                printf("Agora, Diga qual o valor do produto:\n");
                scanf("%d", val);


                add_produto_na_lista("Quilha", descr, id, val);
                
                break;
            
            case 2:     // O usuário deseja adicionar Leash ao estoque.
            printf("Voce deseja adicionar Leash ao estoque:\nDiga a descricao:\n");
                scanf("%c", descr);

                printf("Agora, diga qual a ID do produto:\n");
                scanf("%d", id);

                printf("Agora, Diga qual o valor do produto:\n");
                scanf("%d", val);


                add_produto_na_lista("Leash", descr, id, val);

                break;
            
            case 3:     // O usuário deseja adicionar Parafina ao estoque.
            printf("Voce deseja adicionar Parafina ao estoque:\nDiga a descricao:\n");
                scanf("%c", descr);

                printf("Agora, diga qual a ID do produto:\n");
                scanf("%d", id);

                printf("Agora, Diga qual o valor do produto:\n");
                scanf("%d", val);


                add_produto_na_lista("Parafina", descr, id, val);

                break;
            
            case 4:     // O usuário deseja adicionar Deck ao estoque.
            printf("Voce deseja adicionar Deck ao estoque:\nDiga a descricao:\n");
                scanf("%c", descr);

                printf("Agora, diga qual a ID do produto:\n");
                scanf("%d", id);

                printf("Agora, Diga qual o valor do produto:\n");
                scanf("%d", val);


                add_produto_na_lista("Deck", descr, id, val);

                break;
            
            default:
                break;
            }
        }
        


        imprimir();
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



















//     if(pos >= 0 & pos <= tam){
//         //adiciona!
//         novo->nome = nome;
//         novo->prox = NULL;
//         novo->ant = NULL;

//         // todos casos de insercao
//         if(inicio == NULL){
//         }else if(pos == 0){
//             //lista nao esta vazia e eu quero adicionar no inicio
//             novo->prox = inicio;
//             inicio->ant = novo;
//             inicio = novo;
//         }else if(pos == tam){
//             //lista nao esta vazia e eu quero adicionar no fim
//             fim->prox = novo;
//             novo->ant = fim;
//             fim = novo;
//         }else{
//             //caso do "meio da lista""
//             NO * aux = inicio;
//             for(int i = 0; i<pos; i++){
//                 aux = aux->prox;
//             }
//             novo->prox = aux;
//             novo->ant = aux->ant;
//             //novo->ant->prox = novo;
//             aux->ant->prox = novo;
//             aux->ant = novo;
//         }
//         tam++;

//     }else{
//         printf("Insercao incorreta. Posicao invalida!\n");
//     }    

// }

// void imprimir_um_filme(NO *p){
//     if(p != NULL){
//         printf("Endereço p passado por parametro: %p", p);
//         printf("\n FILME REMOVIDO COM SUCESSO. DADOS:\n");    
//     }
// }

// void remover(int pos){
//     if(pos >= 0 && pos < tam){
        
//         if(inicio != NULL){ //tam>0
//             NO * lixo;
//             if(pos == 0){ //remover no inicio  
//                 lixo = inicio;
//                 inicio = inicio->prox;
//                 if(tam == 1){
//                     fim = NULL;
//                 }
//                 if(tam > 1){
//                     inicio->ant = NULL;
//                 }
//             }else if(pos == tam-1){ //remover no fim
//                 lixo = fim;
//                 fim->ant->prox = NULL;
//                 fim = fim->ant;

//             }else{
//                 //"meio"
//                 lixo = inicio;
//                 for(int i = 0; i <pos; i++){
//                     lixo = lixo->prox;
//                 }
//                 lixo->ant->prox = lixo->prox;
//                 lixo->prox->ant = lixo->ant;
//             }

//             tam--;
//             imprimir_um_filme(lixo);
//             free(lixo);

//         }else{
//             printf("Lista esta vazia! \n");
//         }


//     }else{
//         printf("Posicao invalida! \n");
//     }
// }



// int main(){
//     add("Branca...", "", 2.0, 30, 0, "animacao", 0);
//     add("Minecraft...", "", 1.0, 15, 0, "animacao", 0);
//     add("Lilo...", "", 2.0, 60, 0, "live action", 0);
//     add("Conclave...", "", 3.0, 100, 0, "drama", 3);
//     add("Transformers ...", "", 2.0, 60, 14, "ação", 3);

//     remover(0);
//     printf("Filmes que ainda estão em cartaz:\n");
//     imprimir();
//     return 0;
// }
