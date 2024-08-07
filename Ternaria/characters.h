#ifndef CHARACTERS
#define CHARACTERS
#include "imports.h"
/* 
Estrutura do jogador.
*/

typedef struct character
{
    RECT hitbox;//Retângulo usado para colisão e desenho
    Item inventory[9];//Lista de itens do jogador
    int mainSlot;//Indice do item na mão do jogador
    int life;//Vida do jogador
    int damage;
    int vulnerability;//Flag para invencibilidade
    int state;//Estado da animação de corrida
    int facing;//Direção que está olhando
    int canJump;//Flag para permitir pulo
    int gravity;//Velocidade atual da gravidade
    int knockback;
    int knockbackSide;//Direção do knockback
    int canMove;
    int run;
} character;

/*
Estrutura do zumbi.
*/

typedef struct zombie
{
    RECT hitbox;//Retângulo usado para colisão e desenho
    int life;//Vida atual do zumbi
    int baseLife;//Vida base do zumbi
    int damage;//Dano de contato do zumbi
    int state;//Estado da animação de corrida
    int facing;//Direção que está olhando
    int canJump;//Flag para permitir pulo
    int knockback;
    int knockbackSide;//Direção do knockback
    int canMove;
    int gravity;//Velocidade atual da gravidade
    int respawn;//Tempo até que o zumbi volte após morrer
} zombie;

#endif
