#include<stdio.h>
#include<stdlib.h>

// lista de produtos à venda
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
    int tamanho_max;
    struct Caixa * prox;
    struct NO * lista;

}Caixa;
typedef struct Pilha
{
    char * tipo;
    struct Caixa * Caixa;

}Pilha;

NO * inicio = NULL;
NO * fim = NULL;
int tam = 0;

void imprimir(){
    NO *aux = inicio;
    printf("----- Loja de Surf - Tati Surf Co. ------\n");

    while(aux != NULL){
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


void criar_caixa(NO * lista, char * tipo){ // Primeira Caixa
    Caixa * caixa =  malloc(sizeof(caixa));
    caixa->lista = lista;
    caixa->tipo = tipo;
    caixa->prox = NULL;
}

void add_produto_na_lista(char *tipo, char *descricao, int id, float valor){
    NO *novo = malloc(sizeof(NO));
    novo->tipo = tipo;
    novo->descricao = descricao;
    novo->id = id;
    novo->valor = valor;

    if(inicio == NULL){
        //basico: lista vazia!
        inicio = novo;
        fim = novo;

        
        novo->prox = NULL;
        novo->ant = NULL;

    } else { // Caso não esteja vazia
        // 10 11 12 13 15 15 
        // 11

        NO * aux = inicio;

        if (novo->valor > fim->valor){ //Adiciona no fim
            fim->prox = novo;
            novo->ant = fim;
            fim = novo;
        }else if(novo->valor < aux->valor){ // Adiciona no início
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

        add_produto_na_lista("Quilha", "Fibra", 123, 120.50);
        add_produto_na_lista("Parafina", "Agua fria", 321, 20);
        add_produto_na_lista("Leash", "BalyGrip", 111, 90); //
        add_produto_na_lista("Quilha", "repetido", 145, 90); // 2 20 90L 90Q 120
        add_produto_na_lista("Parafina", "Ruim", 893, 2);


        imprimir();
        return 0;
    }









































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




