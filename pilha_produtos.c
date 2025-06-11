#include<stdio.h>
#include<stdlib.h>
#include <string.h>

int main(){

    char * teste;
    scanf("%s", teste);

    char teste1[100];
    scanf("%s", teste1);

    char * teste2;
    scanf("%s", strdup(teste2));

    printf("%s\n",teste);
    printf("%s\n",teste1);
    printf("%s\n",teste2);

    return 0;
}
