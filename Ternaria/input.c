#include "imports.h"
#include "render.c"

void MoveLeft(character * Player, zombie * Zombie, int Pixels)
{
    Player->state = Player->state + 1;
    Player->facing = 1;
    Player->hitbox.left -= Pixels;
    mapax -= Pixels;
    block * B = MapCollision(&Player->hitbox);
    if(B != NULL)
    {
        mapax += B->hitbox.right - Player->hitbox.left;
    }
    Player->hitbox.left += Pixels;
}

void MoveRight(character * Player, zombie * Zombie, int Pixels)
{
    Player->state = Player->state + 1;
    Player->facing = 2;
    Player->hitbox.right += Pixels;
    mapax += Pixels;
    block * B = MapCollision(&Player->hitbox);
    if(B != NULL)
    {
        mapax -= Player->hitbox.right - B->hitbox.left + 1;
    }
    Player->hitbox.right -= Pixels;
}

void MoveDown(character * Player, zombie * Zombie, int Pixels)
{
    Player->hitbox.bottom += Pixels;
    mapay -= Pixels;
    block * B = MapCollision(&Player->hitbox);
    if(B != NULL)
    {
        Player->gravity = 0;
        Player->canJump = 1;
        Player->knockback = 0;
        Player->canMove = 1;
        mapay += Player->hitbox.bottom - B->hitbox.top;
    }
    Player->hitbox.bottom -= Pixels;
}

void MoveUp(character * Player, zombie * Zombie, int Pixels)
{
    Player->hitbox.top -= Pixels;
    mapay += Pixels;
    block * B = MapCollision(&Player->hitbox);
    if(B != NULL)
    {
        Player->canJump = 0;
        Player->gravity = 0;
        Player->knockback = 0;
        Player->canMove = 1;
        mapay -= B->hitbox.bottom - Player->hitbox.top;

    }
    Player->hitbox.top += Pixels;
}

void Jump(character * Player, zombie * Zombie, int Pixels) 
{
    if(Player->canJump == 1)
    {
        if(Player->gravity == 15)
        {
            Player->canJump = 0;
            Player->gravity = 0;
            return;
        }
        MoveUp(Player, Zombie, Pixels);
    }
}

void Gravity(character * Player, zombie * Zombie)
{
    MoveDown(Player, Zombie, Player->gravity);
    if(Player->gravity < 20)
    {
        Player->gravity++;
    }
}

void Knockback(character * Player, zombie * Zombie)
{
    if(Player->knockbackSide == 1)
    {
        MoveLeft(Player, Zombie, 7);
    }
    else
    {
        MoveRight(Player, Zombie, 7);
    }
    MoveUp(Player, Zombie, 7);
}

int CoolDown = 0;

void Input(HDC hdc, character * Player, zombie * Zombie, DArray * Map)
{
    if(CoolDown)
    {
        --CoolDown;
    }
    if(GetAsyncKeyState(VK_LSHIFT))
    {
        Player->run = 1;
    }else
    {
        Player->run = 0;
    }
    if(GetAsyncKeyState(VK_1))
    {
        Player->mainSlot = 0;
    }else
    if(GetAsyncKeyState(VK_2))
    {
        Player->mainSlot = 1;
    }else
    if(GetAsyncKeyState(VK_3))
    {
        Player->mainSlot = 2;
    }else
    if(GetAsyncKeyState(VK_4))
    {
        Player->mainSlot = 3;
    }else
    if(GetAsyncKeyState(VK_5))
    {
        Player->mainSlot = 4;
    }else
    if(GetAsyncKeyState(VK_6))
    {
        Player->mainSlot = 5;
    }
    if(GetAsyncKeyState(VK_LBUTTON))
    {
        if(!CoolDown)
        {
            POINT Mouse, P;
            GetCursorPos(&Mouse);
            ScreenToClient(Ghwnd, &Mouse);
            P.x = Player->hitbox.right - Player->hitbox.left;
            P.y = Player->hitbox.bottom - Player->hitbox.top;
            switch(Player->inventory[Player->mainSlot].id)
            {
                case 0:
                {
                    if(DestroyBlocks(Player, Mouse))
                    {
                        CoolDown = 5;
                        RenderBkgd(hdc);
                        RenderMap(Map, hdc);
                        RenderInv(hdc, Player);
                        RenderLife(hdc, Player->life);
                    }
                }
                break;
                case 1:
                {
                    int res = Slash(Zombie, Player);
                    CoolDown = 20;
                    if(res < 0)
                    {
                        Zombie->knockback = 5;
                        Zombie->knockbackSide = 1;
                        KnockbackZombie(Zombie);
                    }else
                    if(res > 0)
                    {
                        Zombie->knockback = 5;
                        Zombie->knockbackSide = 2;
                        KnockbackZombie(Zombie);
                    }
                }
                break;
                case 2:
                {
                    int res = EstragarVelorio(Zombie, Player, Mouse);
                    CoolDown = 60;
                    if(res < 0)
                    {
                        if(Zombie->damage < 5)
                        {
                            Zombie->knockback = 15;
                        }
                        else
                        {
                            Zombie->knockback = 5;
                        }
                        Zombie->knockbackSide = 1;
                        KnockbackZombie(Zombie);
                    }else
                    if(res > 0)
                    {
                        if(Zombie->damage < 5)
                        {
                            Zombie->knockback = 15;
                        }
                        else
                        {
                            Zombie->knockback = 5;
                        }
                        Zombie->knockbackSide = 2;
                        KnockbackZombie(Zombie);
                    }
                }
                break;
                case 3:
                {
                    if(PlaceBlocks(Player, Zombie, Mouse, Player->inventory[Player->mainSlot].damage))
                    {
                        RenderBkgd(hdc);
                        RenderMap(Map, hdc);
                        RenderInv(hdc, Player);
                        RenderLife(hdc, Player->life);
                    }
                }
                break;
                default:
                break;
            }
        }

    }
    if(GetAsyncKeyState(VK_A) && Player->canMove)
    {
        MoveLeft(Player, Zombie, 5 << Player->run);
    }
    if(GetAsyncKeyState(VK_D) && Player->canMove)
    {
        MoveRight(Player, Zombie, 5 << Player->run);
    }
    if(GetAsyncKeyState(VK_SPACE) && Player->canMove)
    {
        Jump(Player, Zombie, 15);
    }
    else if(Player->canJump == 1)
    {
        Player->canJump = 0;
        Player->gravity = 0;
    }
    Gravity(Player, Zombie);
    if (Collision(&Player->hitbox, &Zombie->hitbox) && Player->vulnerability == 0)
    {
        Player->vulnerability = 30;
        Player->life -= Zombie->damage;
        RenderLife(hdc, Player->life);
        Player->knockback = 1;
        if(Player->hitbox.left > Zombie->hitbox.left)
        {
            Player->knockbackSide = 2;
        }
        else if (Zombie->hitbox.right > Player->hitbox.right)
        {
            Player->knockbackSide = 1;
        }
    }
    if(Player->knockback)
    {
        Knockback(Player, Zombie);
        Player->canMove = 0;
    }
    if(Player->vulnerability > 0)
    {
        Player->vulnerability--;
    }
}

void Regeneration(character * player)
{
    if(player->life < 10)
    {
        player->life += 1;
    }
}