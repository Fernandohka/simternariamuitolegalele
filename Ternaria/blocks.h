#ifndef BLOCKS
#define BLOCKS
#include <stdio.h>
#include "imports.h"

char * MapPath = "Map.txt";

// Estrutura dos blocos, especificando com posição e tipo.
typedef struct block
{
    RECT hitbox;//Retângulo usado para colisão e desenho
    int x;//posições simplificadas
    int y;
    int type;
    wchar_t * img;//Caminho para a imagem que deve ser desenhada
} block;

// Função de definição da textura e colisão dos blocos
void BlockDefine(block * B)
{
    B->hitbox.left = (B->x*32) - mapax;
    B->hitbox.top = 977 - (B->y*32) + mapay;
    B->hitbox.right = B->hitbox.left + 32;
    B->hitbox.bottom = B->hitbox.top + 32;
    switch (B->type)
    {
    case 1:
        B->img = L"Grama";
        break;

    case 2:
        B->img = L"Terra";
        break;

    case 3:
        B->img = L"Tronco";
        break;

    case 4:
        B->img = L"Folhas";
        break;

    case 5:
        B->img = L"Stone";
        break;

    default:
        break;
    }
}

void DefineMap(DArray *lista)
{
    for(int i = 0; i < lista->Size; i++)
    {
        block * B = lista->List[i];
        BlockDefine(B);
    }
}

// função para criar o mapa aleatoriamente
void CreateArchive(int xi, int yi)
{
    FILE * map = fopen("Map.txt", "a");
    int camada = (yi > mapatop) ? mapatop : yi;
    int arvore = 0, arvtam = 0;
    int ypedra = camada - (rand()%2 + 5);
    
    for(int x = xi; x < xi + 60; x++)
    {
        if(arvore == 0 && rand()%10 == 1) // criação aleatoria da arvore, 10% de chance de criar uma arvore caso o bloco anterior não tenha uma arvore
        {
            arvore = camada + 1;
            arvtam = rand()%6 + 3; // tamanho aleatorio da arvore
            for(int arvh = arvore; arvh < arvore + arvtam; arvh++) // criação dos troncos em linha reta para cima de acordo com o tamanho da arvore
            {
                fprintf(map, "%d,%d,%d\n", x, arvh, 3); // escreve o local do bloco no txt do mapa
            }
            // criação das folhas da arvore em volta do tronco de acordo com o tamanho da arvore
            for(int folhay = arvore + arvtam; folhay < arvore + arvtam * 2; folhay++)
            {
                for(int folhax = (x - arvtam / 2) + ((folhay - arvore - arvtam) / 2); folhax <= (x + arvtam / 2) - ((folhay - arvore - arvtam) / 2); folhax++)
                {
                    fprintf(map, "%d,%d,%d\n", folhax, folhay, 4); // escreve o local do bloco no txt do mapa
                }
            }
        }
        else if(arvore != 0) // caso o bloco anterior possua uma arvore retorna a variavel para 0
        {
            arvore = 0;
        }
        for(int y = camada; y >= yi - 32; y--) // criação dos blocos do chão
        {
            fprintf(map, "%d,%d,%d\n", x, y, (y <= ypedra) ? 5 : (y == camada && arvore != camada + 1) ? 1 : 2); // escreve o local do bloco no txt do mapa e escolhe o tipo do bloco de acordo com a camada            
        }
        if(mapatop == camada) // aumenta ou diminui em 1 ou mantem a camada aleatoriamente
        {
            camada += rand()%3 - 1;
            mapatop = camada;
        }
        if(camada < 0) // limita o minimo da camada que será gerada
        {
            camada = 0;
            mapatop = 0;
        }
        ypedra += rand()%3 - 1; // aumenta ou diminui em 1 ou mantem a camada da pedra aleatoriamente
        if(ypedra > camada - 5) // limita o minimo da camada de pedra que será gerada
        {
            ypedra = camada - 5;
        }
        if(ypedra < camada - 7) // limita o maximo da camada de pedra que será gerada
        {
            ypedra = camada - 7;
        }
    }
    fclose(map); // finaliza a criação do mapa
}
/* 
Função de leitura do arquivo de mapa, fazemos a construção do mapa do Ternaria por meio da leitura desse arquivo.
*/ 
void ReadArchive(DArray *lista)
{
    int count = 0;
    FILE * File = fopen("Map.txt", "r");
    MapPath = "Map.txt";
    RECT M = {mapax - 60 * 32, mapax + 60 * 32, mapay - 60 * 32, mapay + 60 * 32};

    while(!feof(File))
    {
        ++count;
        block * bloco = malloc(sizeof(block));
        fscanf(File, "%i,%i,%i", &bloco->x, &bloco->y, &bloco->type);
        BlockDefine(bloco);
        if(Collision(&M, &bloco->hitbox))
        {
            DArrayAdd(lista, bloco);
        }
        else
        {
            free(bloco);
        }
    }
    fclose(File);
}

// Lê o Arquivo do Mundo Senai
void ReadSenai(DArray *lista)
{
    int count = 0;
    FILE * File = fopen("Map Senai.txt", "r");
    MapPath = "Map Senai.txt";

    while(!feof(File))
    {
        ++count;
        block *bloco = malloc(sizeof(block));
        fscanf(File, "%i,%i,%i", &bloco->x, &bloco->y, &bloco->type);
        BlockDefine(bloco);
        DArrayAdd(lista, bloco);
    }
    fclose(File);
}

// Escreve no txt
void WriteArchive(DArray *lista)
{
    int count = 0;
    FILE * File = fopen(MapPath, "w");
    for(int i = 0; i < lista->Size; ++i)
    {
        block * B = (block *) lista->List[i];
        fprintf(File, "%i,%i,%i\n", B->x, B->y, B->type);
    }
    fclose(File);
}
#endif
