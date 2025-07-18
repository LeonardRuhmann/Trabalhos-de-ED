#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

# define PREFIXO_ID_PRODUTO_PARAFINA 100000
# define PREFIXO_ID_PRODUTO_QUILHA 200000
# define PREFIXO_ID_PRODUTO_LEASH 300000
# define PREFIXO_ID_PRODUTO_DECK 400000

// Definindo valor de tolerância para comparar valores em Reais (Float)
# define TOLERANCIA 0.001

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
int id_unica_caixa (char * tipo);
int id_unica_produto (char * tipo);
int obter_tamanho_max_da_caixa(char * tipo);
void gerenciar_estoque(Pilha * pilha, char * tipo);
void imprimir_p_tipo(Pilha * Tipo);
void imprimi_p_intervalo_de_preco(Pilha *Parafina, Pilha *Leash, Pilha *Quilha, Pilha *Deck);
void Liberar_memoria(Pilha * pilha);
void gerenciar_compra_produto(Pilha *Parafina, Pilha *Leash, Pilha *Quilha, Pilha *Deck);
void comprar_produto(Pilha * pilha, int id_produto);
NO * buscar_produto_por_id(Pilha * pilha, int id_produto);



//  Futuras versões os contadores incrementais de IDs serão encapsulados em Structs.
//  Contadores Incrementais para IDs de produtos.
int id_inc_parafina = 1, id_inc_quilha = 1, id_inc_leash = 1, id_inc_deck = 1;

//  Contadores Incrementais para IDs de caixas.
int id_inc_caixa_parafina = 1, id_inc_caixa_quilha = 1, id_inc_caixa_leash = 1, id_inc_caixa_deck = 1;

void Limpa_Buffer_i () {
    int a;
    while ((a = getchar()) != '\n' && a != EOF);
}
//Essa função é responsável por caminhar na lista
NO * caminhar_na_lista(NO * produto, int id_produto){
    while (produto != NULL && produto->id != id_produto)
    {
        produto = produto->prox;
    }
    return produto;
}
// Essa função lida com todo o trabalho de ponteiros das listas, caixas e pilhas e retorna o produto ou NULL(Caso não exista)
NO * buscar_produto_por_id(Pilha * pilha, int id_produto){
    Pilha * pilha_auxiliar = malloc(sizeof(Pilha));
    pilha_auxiliar->topo = NULL;
    pilha_auxiliar->tipo = pilha->tipo;

    Caixa * caixa_atual = pilha->topo;
    Caixa * caixa_onde_o_produto_foi_encontrado = NULL;
    NO * produto_encontrado = NULL;

    while(caixa_atual != NULL){ //// Irá percorrer pelas caixas até encontrar o produto

        produto_encontrado = caminhar_na_lista(caixa_atual->inicio_da_lista, id_produto);

        if (produto_encontrado !=  NULL) { //Encontramos o produto! 

            caixa_onde_o_produto_foi_encontrado = caixa_atual;
            break;
        }
        //Caso não seja encontrado, próxima caixa

        pilha->topo = caixa_atual->prox;
        caixa_atual->prox = pilha_auxiliar->topo;
        pilha_auxiliar->topo = caixa_atual;
        caixa_atual = pilha->topo;
    }

    if (produto_encontrado != NULL) { //Produto Encontrado!

        // E se o produto que encontramos esta na PRIMEIRA CAIXA, mas ela so tem UM produto?
        if(produto_encontrado->ant == NULL && produto_encontrado->prox == NULL) {
            // Garante que inicio e fim sejam nulos, pois venderemos o unico produto que eh inicio e fim ao mesmo tempo.
            caixa_onde_o_produto_foi_encontrado->inicio_da_lista = NULL;
            caixa_onde_o_produto_foi_encontrado->fim_da_lista = NULL;

            // Precisamos eliminar a caixa, NUNCA deve haver uma caixa vazia.
            Caixa *antigo_topo = caixa_onde_o_produto_foi_encontrado;
            pilha->topo = caixa_onde_o_produto_foi_encontrado->prox;
            caixa_onde_o_produto_foi_encontrado = NULL;
            free(antigo_topo);
            
        } else { // Se nao, ele so pode ser ou inicio, ou fim, ou meio.

            if (produto_encontrado->ant != NULL){ //O produto NÃO é o primeiro elemento
                produto_encontrado->ant->prox = produto_encontrado->prox;
            }
            else { //O produto é o primeiro elemento
                caixa_onde_o_produto_foi_encontrado->inicio_da_lista = produto_encontrado->prox;
                caixa_onde_o_produto_foi_encontrado->inicio_da_lista->ant = NULL;
            }
            
            if (produto_encontrado->prox != NULL) {//O produto NÃO é o último elemento
                produto_encontrado->prox->ant = produto_encontrado->ant;
            }
            else {//O produto é o último elemento
                caixa_onde_o_produto_foi_encontrado->fim_da_lista = produto_encontrado->ant;
                caixa_onde_o_produto_foi_encontrado->fim_da_lista->prox = NULL;
            }
            caixa_onde_o_produto_foi_encontrado->espaco_restante++;
        }

        
        // Isola completamente o nó para um retorno seguro
        produto_encontrado->ant = NULL;
        produto_encontrado->prox = NULL;
    }

    while (pilha_auxiliar->topo != NULL)
    {
        Caixa * caixa_a_devolver = pilha_auxiliar->topo;
        pilha_auxiliar->topo = pilha_auxiliar->topo->prox;
        caixa_a_devolver->prox = pilha->topo;
        pilha->topo = caixa_a_devolver;
    }
    free(pilha_auxiliar);
    return produto_encontrado;
}

//Essa função é responsável por buscar o produto e printar este na tela
void comprar_produto(Pilha * pilha, int id_produto){
    NO * produto = buscar_produto_por_id(pilha, id_produto);

    if(produto != NULL){
        printf("O produto comprado foi:\n");
        printf("ID: %d\n", produto->id);
        printf("Tipo: %s\n", produto->tipo);
        printf("Descrição: %s\n", produto->descricao);
        printf("Valor: %.2f\n", produto->valor);
    } else {
        printf("AVISO: Produto com ID %d nao foi encontrado no estoque.\n", id_produto);
    }
    
}
//Função que gerencia todas as opções de compra de um produto
void gerenciar_compra_produto(Pilha *Parafina, Pilha *Leash, Pilha *Quilha, Pilha *Deck){
    int escolha;
    int id_do_produto;
    printf("Qual é o tipo do produto que você deseja comprar?\n");
    printf("0 - Parafina\n1 - Leash\n2 - Quilha\n3 - Deck\n4- Voltar ao Menu inicial.\nSua Escolha: ");
    scanf("%d", &escolha);
    Limpa_Buffer_i();
    if(escolha == 4){
        printf("Retornando ao Menu inicial.\n");
        return;
    }

    printf("\nPor favor, digite o código do produto\nCodigo do produto:");
    scanf("%d", &id_do_produto);
    Limpa_Buffer_i();
    
    
    switch (escolha)
    {
    case 0:
        comprar_produto(Parafina, id_do_produto);
        break;
    case 1:
        comprar_produto(Leash, id_do_produto);
        break;
    case 2:
        comprar_produto(Quilha, id_do_produto);
        break;
    case 3:
        comprar_produto(Deck, id_do_produto);
        break;
    default:
        break;
    }
};

// Função de criar caixas de produtos.
Caixa * inicializar_caixa(char * tipo) {
    Caixa * caixa =  malloc(sizeof(Caixa));
    caixa->id = id_unica_caixa(tipo);
    caixa->tipo = tipo;
    caixa->espaco_restante = obter_tamanho_max_da_caixa(tipo);
    caixa->prox = NULL;
    caixa->inicio_da_lista = NULL;
    caixa->fim_da_lista = NULL;
    return caixa;
}

// Essa função lida com o processo de adicionar um produto ao estoque da loja. Eventualmente, revenda, pesquisa ou manipulação de estoque.
void gerenciar_estoque(Pilha * pilha, char * tipo) {

    // PRIMEIRO CASO: Usuário deseja estocar um produto que ainda não tem pilha e caixa, ou, tem, mas, a caixa no topo está lotada.
    if (pilha->topo == NULL || pilha->topo->espaco_restante == 0) {
        // Inicializa uma nova caixa.
        Caixa * nova_caixa =  inicializar_caixa(tipo);
        // Cria a lista de produtos dentro da nova caixa.
        add_produto_na_lista(tipo, &nova_caixa->inicio_da_lista, &nova_caixa->fim_da_lista);

        nova_caixa->prox = pilha->topo;
        pilha->topo = nova_caixa;
        
    } else {
        // SEGUNDO CASO: Caso hajam caixas na pilha mas na caixa topo ainda resta espaço.
        add_produto_na_lista(tipo, &pilha->topo->inicio_da_lista, &pilha->topo->fim_da_lista);
        
    }
    pilha->topo->espaco_restante--; 

}

void add_produto_na_lista(char *tipo, NO ** inicio_da_lista, NO **fim_da_lista) {
    NO *Novo_produto = malloc(sizeof(NO));
    char buffer[100];
    float valor;

    // MÉTODO: Receber valores a partir de um método de fgets e sscanf intermdiado por buffer[100].
    printf("\nDiga uma breve descricao do produto (apenas 99 letras serao lidas):\n");
    
    fgets(buffer, sizeof(buffer), stdin);
    if (!strchr(buffer, '\n')) Limpa_Buffer_i();
    buffer[strcspn(buffer, "\n")] = '\0';
    Novo_produto->descricao = strdup(buffer); 

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
    Novo_produto->id = id_unica_produto(Novo_produto->tipo);
    Novo_produto->valor = valor;
    Novo_produto->prox = NULL;
    Novo_produto->ant = NULL;

    if(*inicio_da_lista == NULL) {      // Caso seja o primeiro produto de todos.
        *inicio_da_lista = Novo_produto;
        *fim_da_lista = Novo_produto;

    } else {

        NO * aux = *inicio_da_lista;
        
        if (Novo_produto->valor >= (*fim_da_lista)->valor) {      // Caso1: Adiciona no fim da lista.
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
void imprimir_p_tipo (Pilha *Tipo) {
    printf("\n           ======= EXIBINDO TODOS OS PRODUTOS DO TIPO %s =======\n", Tipo->tipo);
    if (Tipo->topo == NULL) {
    printf("\n            XX | NAO HA CAIXAS DE PRODUTOS PARA O TIPO %s | XX\n", Tipo->tipo);
        return;
    }

    Caixa *Caixa = Tipo->topo;
    int N_caixa = 1;
    
    while (Caixa != NULL)
    {
        NO *Produto = Caixa->inicio_da_lista;
        printf("------- CAIXA NUMERO: %d | TIPO: %s | ID: %d | ESPACO RESTANTE: %i -------\n", N_caixa, Caixa->tipo, Caixa->id, Caixa->espaco_restante);
        printf("                            ---- PRODUTOS ----\n");
        
        while (Produto != NULL)
        {
            printf("                        | Tipo:............. %s\n", Produto->tipo);
            printf("                        | Descricao:........ %s\n", Produto->descricao);
            printf("                        | Valor:............ %.2f Reais\n", Produto->valor);
            printf("                        | ID:............... %i\n", Produto->id);
            printf("                        |\n");
            Produto = Produto->prox;
        }
        Caixa = Caixa->prox;
        N_caixa++;
    }
    printf("                   ------- FIM DA PILHA DE %s -------\n", Tipo->tipo);
}

void Menu_imprimir_p_tipo (Pilha *Parafina, Pilha *Leash, Pilha *Quilha, Pilha *Deck) {
    printf("\n---------------------------------------MENU DE EXIBIR POR TIPO-----------------------------------------");
    printf("\nVoce deseja exibir todos os produtos pelo tipo de sua escolha.\nDigite um numero no menu que corresponde ao tipo de produto que voce deseja que seja exibido do estoque:\n");
    printf("0 - Parafina\n1 - Leash\n2 - Quilha\n3 - Deck\n4- Voltar ao Menu inicial.\nSua Escolha: ");
    int op;
    char buffer[100];

        fgets(buffer, sizeof(buffer), stdin);
    
    if (sscanf(buffer, "%i", &op) != 1) {
        printf("\n======ENTRADA INVALIDA, NAO DIGITE LETRAS, APENAS NUMEROS DAS OPCOES DISPONIVEIS NO MENU======\n Retornando ao Menu inicial\n");
        Limpa_Buffer_i();   // Caso o danado digite mais que 100 caracteres. E dá mais segurança.
        return;
    }
    switch (op)
    {
    case 0:
        imprimir_p_tipo(Parafina);
        break;
    case 1:
        imprimir_p_tipo(Leash);
        break;
    case 2:
        imprimir_p_tipo(Quilha);
        break;
    case 3:
        imprimir_p_tipo(Deck);
        break;
    case 4:
        printf("Retornando ao Menu inicial.\n");
        break;
    default:
        printf("\n======ENTRADA INVALIDA, DIGITE APENAS OS NUMEROS DAS OPCOES DISPONIVEIS NO MENU======\n Retornando ao Menu inicial\n");
        break;
    }
}

void imprimi_p_intervalo_de_preco(Pilha *Parafina, Pilha *Leash, Pilha *Quilha, Pilha *Deck) {
    char buffer[100];
    float menor_valor, maior_valor;
    int Escolha;
    Pilha * Tipo;

    printf("\n__________FILTRO DE EXIBICAO DE PRODUTOS POR TIPO__________\n");

    printf("Infome qual o tipo de produto que voce deseja filtrar (Escolha de 0 a 4):\n0 - Parafina\n1 - Leash\n2 - Quilha\n3 - Deck\n4 - Cancelar e voltar ao Menu Inicial.\n");
    while (1)
    {
        printf("Sua escolha: ");
        fgets(buffer, sizeof(buffer), stdin);

        // MÉTODO: Receber valores a partir de um método de fgets e sscanf intermdiado por buffer[10].
        if (sscanf(buffer, "%i", &Escolha) != 1) {
            printf("\n======ENTRADA INVALIDA, NAO DIGITE LETRAS, APENAS NUMEROS DAS OPCOES DISPONIVEIS NO MENU======\n");
            Limpa_Buffer_i();   // Caso o danado digite mais que 10 caracteres. E dá mais segurança.
            continue;
        }

        switch (Escolha)
        {
        case 0:
            Tipo = Parafina;
            break;
        case 1:
            Tipo = Leash;
            break;
        case 2:
            Tipo = Quilha;
            break;
        case 3:
            Tipo = Deck;
            break;
        case 4:
            return;
        
        default:
            break;
        }
        break;
    }
    
    printf("\nA seguir, voce deve informar o intervalo de valor que voce deseja filtrar para o produto [%s].\nDIGITE UM PONTO FINAL PARA DIFERENCIAR REAIS DE CENTAVOS. EX.: 2.99, E NAO 2,99.\n", Tipo->tipo);
    while (1) {
        printf("\nDigite o -MENOR VALOR- que voce deseja ver na lista: ");
        fgets(buffer, sizeof(buffer), stdin);
    
        // MÉTODO: Receber valores a partir de um método de fgets e sscanf intermdiado por buffer[10].
        if (sscanf(buffer, "%f", &menor_valor) != 1) {
            printf("\n======ENTRADA INVALIDA, NAO DIGITE LETRAS, APENAS NUMEROS DAS OPCOES DISPONIVEIS NO MENU======\n");
            Limpa_Buffer_i();   // Caso o danado digite mais que 10 caracteres. E dá mais segurança.
            continue;
        }
        printf("\nAgora digite o -MAIOR VALOR- que voce deseja ver na lista: ");
        fgets(buffer, sizeof(buffer), stdin);
    
        // MÉTODO: Receber valores a partir de um método de fgets e sscanf intermdiado por buffer[10].
        if (sscanf(buffer, "%f", &maior_valor) != 1) {
            printf("\n======ENTRADA INVALIDA, NAO DIGITE LETRAS, APENAS NUMEROS DAS OPCOES DISPONIVEIS NO MENU======\n");
            Limpa_Buffer_i();   // Caso o danado digite mais que 10 caracteres. E dá mais segurança.
            continue;

        }
        if (fabsf(menor_valor - maior_valor) > TOLERANCIA && menor_valor > maior_valor) {
            printf("\nXX | AVISO, O PRIMEIRO VALOR DEVE SER O MENOR, E O SEGUNDO O MAIOR | XX\n");
            Limpa_Buffer_i();
            continue;
        }
        break;
    }

    Caixa *Caixa = Tipo->topo;
    int N_caixa = 1;
    if (Caixa == NULL) printf("\n\t\t      XX | ESTA PILHA ESTA VAZIA | XX\n");

    while (Caixa != NULL) {
        NO *Produto = Caixa->inicio_da_lista;
        int Nao_Existe_Produto = 1;
        printf("\n------- CAIXA NUMERO: %d | TIPO: %s | ID: %d | ESPACO RESTANTE: %i -------\n", N_caixa, Caixa->tipo, Caixa->id, Caixa->espaco_restante);
        printf("                        |    ---- PRODUTOS ----\n");
        while (Produto != NULL) {
            if (Produto->valor >= menor_valor && Produto->valor <= maior_valor) {
                printf("                        | Tipo:............. %s\n", Produto->tipo);
                printf("                        | Descricao:........ %s\n", Produto->descricao);
                printf("                        | Valor:............ %.2f Reais\n", Produto->valor);
                printf("                        | ID:............... %i\n", Produto->id);
                printf("                        |\n");
                Nao_Existe_Produto = 0;
            }
            Produto = Produto->prox;
        }
        if (Nao_Existe_Produto) printf("\n       XX | ESTA CAIXA NÃO POSSUI PRODUTOS PARA O FILTRO FORNECIDO | XX\n");
        Caixa = Caixa->prox;
        N_caixa++;
    }
    printf("\n                    ------- FIM DA PILHA DE %s -------\n", Tipo->tipo);
    

}

// Função responsável por liberar a memória de tudo que foi alocado ao fechar o programa.
void Liberar_memoria(Pilha * pilha) {
    if (pilha->topo == NULL) return;

    Caixa *caixa_atual = pilha->topo;
    
    while (caixa_atual != NULL)
    {
        NO * produto_atual = caixa_atual->inicio_da_lista;
        NO * aux_produto;

        while (produto_atual != NULL)
        {
            aux_produto = produto_atual->prox;
            if (produto_atual->descricao != NULL) {
                free(produto_atual->descricao);
            }
            free(produto_atual);
            produto_atual = aux_produto;
        }
        Caixa * aux_caixa = caixa_atual->prox;
        free(caixa_atual);
        caixa_atual = aux_caixa;
    }
}

// Função responsável por criar uma nova id para o produto de maneira incremental. (Ainda em processo de Melhorias)
int id_unica_produto (char * tipo) {
    int pfixo, nova_id;
    if (strcmp(tipo, "Parafina") == 0) { 
        pfixo = PREFIXO_ID_PRODUTO_PARAFINA;
        nova_id = pfixo + id_inc_parafina;
        id_inc_parafina ++;
        return nova_id;
    }
    if (strcmp(tipo, "Leash") == 0) {
        pfixo = PREFIXO_ID_PRODUTO_LEASH;
        nova_id = pfixo + id_inc_leash;
        id_inc_leash ++;
        return nova_id;
    }
    if (strcmp(tipo, "Quilha") == 0) {
        pfixo = PREFIXO_ID_PRODUTO_QUILHA;
        nova_id = pfixo + id_inc_quilha;
        id_inc_quilha++;
        return nova_id;
    }
    if (strcmp(tipo, "Deck") == 0) {
        pfixo = PREFIXO_ID_PRODUTO_DECK;
        nova_id = pfixo + id_inc_deck;
        id_inc_deck++;
        return nova_id;
    }

    return -1;  // Excessão será implementada em breve.
}

int id_unica_caixa (char * tipo) {
    int pfixo, nova_id;
    if (strcmp(tipo, "Parafina") == 0) { 
        pfixo = PREFIXO_ID_PRODUTO_PARAFINA;
        nova_id = pfixo + id_inc_caixa_parafina;
        id_inc_caixa_parafina ++;
        return nova_id;
    }
    if (strcmp(tipo, "Leash") == 0) {
        pfixo = PREFIXO_ID_PRODUTO_LEASH;
        nova_id = pfixo + id_inc_caixa_leash;
        id_inc_caixa_leash ++;
        return nova_id;
    }
    if (strcmp(tipo, "Quilha") == 0) {
        pfixo = PREFIXO_ID_PRODUTO_QUILHA;
        nova_id = pfixo + id_inc_caixa_quilha;
        id_inc_caixa_quilha++;
        return nova_id;
    }
    if (strcmp(tipo, "Deck") == 0) {
        pfixo = PREFIXO_ID_PRODUTO_DECK;
        nova_id = pfixo + id_inc_caixa_deck;
        id_inc_caixa_deck++;
        return nova_id;
    }

    return -1;  // Excessão será implementada em breve para novas versões.
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
        printf("\n-------------------------------------MENU INICIAL----------------------------------------");
        printf("\nDigite qual opcao de produto voce deseja adicionar no estoque da loja (Opcoes de 0 a 7)\n0 - Parafina\n1 - Leash\n2 - Quilha\n3 - Deck\n4 - Exibir Estoque\n5 - Exibir Produtos por Tipo\n6 - Filtrar Produtos por preco\n7 - Comprar Produto\n8 - Fechar programa\n");
        printf("Sua escolha: ");
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
                printf("\nVoce selecionou Parafina.\n");
                gerenciar_estoque(Parafina, "Parafina");
                break;
            case 1:     // O usuário deseja adicionar Leash ao estoque.
                printf("\nVoce selecionou Leash.\n");
                gerenciar_estoque(Leash, "Leash");
                break;
            case 2:     // O usuário deseja adicionar Quilha ao estoque.
                printf("\nVoce selecionou Quilha.\n");
                gerenciar_estoque(Quilha, "Quilha");
                break;
            case 3:     // O usuário deseja adicionar Deck ao estoque.
                printf("\nVoce selecionou Deck.\n");
                gerenciar_estoque(Deck, "Deck");
                break;
            case 4:     //Usuário deseja imprimir todos os produtos do estoque.
                imprimir_p_tipo(Parafina);
                imprimir_p_tipo(Leash);
                imprimir_p_tipo(Quilha);
                imprimir_p_tipo(Deck);
                break;
            case 5:     // Usuário deseja imprimir todos os produtos por tipo.
                Menu_imprimir_p_tipo(Parafina, Leash, Quilha, Deck);
                break;
            case 6:     // Usuário deseja imprimir um tipo pelo intervalo de preço.
                imprimi_p_intervalo_de_preco(Parafina, Leash, Quilha, Deck);
                break;
            case 7:
                gerenciar_compra_produto(Parafina, Leash, Quilha, Deck);
                //Processo de compra de produto
                break;
            case 8:     // Usuário deseja encerrar o programa.
                printf("\nPrograma encerrado...\n");
                valid = 0;
                Liberar_memoria(Parafina);
                free(Parafina);
                Liberar_memoria(Leash);
                free(Leash);
                Liberar_memoria(Quilha);
                free(Quilha);
                Liberar_memoria(Deck);
                free(Deck);
                break;
            
            default:
                printf("\n======POR FAVOR, DIGITE -APENAS NUMEROS- ENTRE 0 E 6======\n");
                break;
        }
    }
    printf("Programa Encerrado");
    return 0;
}