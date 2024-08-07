/*

Essa header possui os itens que serão implementados no jogo

*/

#ifndef ITEMS
#define ITEMS
#include "imports.h"

typedef struct Item
{
    int id;
    int damage;
    int durability;
    int ammo;
    wchar_t * imgLeft;
    wchar_t * imgRight;
} Item;

// Define todos os itens
void ItemDefine(Item * item)
{
    item->durability = 64;
    switch (item->id)
    {
        case 0:
            item->damage = 0;
            item->ammo = -1;
            item->imgLeft = L"PickE";
            item->imgRight = L"PickD";
        break;

        case 1:
            item->damage = 2;
            item->ammo = -1;
            item->imgLeft = L"EspadaE";
            item->imgRight = L"EspadaD";
        break;

        case 2:
            item->damage = 5;
            item->ammo = 10;
            item->imgLeft = L"ShotE";
            item->imgRight = L"ShotD";
        break;

        case 3:
            switch(item->damage)
            {
                case 2:
                    item->imgLeft = L"Bloquinho";
                    item->imgRight = L"Bloquinho";
                break;
                case 3:
                    item->imgLeft = L"Bloquinho2";
                    item->imgRight = L"Bloquinho2";
                break;
                case 5:
                    item->imgLeft = L"Bloquinho3";
                    item->imgRight = L"Bloquinho3";
                break;
                default:
                break;
            }
        break;
        default:
        break;
    }
}

#endif