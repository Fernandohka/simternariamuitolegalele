#include "imports.h"
#include "functions.c"


// Apaga o retângulo, desenhando uma parte do background em cima dele
void EraseRect(HDC hdc, const RECT * Rect)
{
    BITMAP bm;
    HBITMAP Image = LoadBitmap(GInstance, L"Background");
    HDC BitmapDC = CreateCompatibleDC(hdc);
    SelectObject(BitmapDC, Image);
    GetObject((HGDIOBJ)Image, sizeof(bm), &bm);
    BitBlt(hdc, Rect->left, Rect->top, Rect->right - Rect->left+1, Rect->bottom - Rect->top+1, BitmapDC, Rect->left, Rect->top, SRCCOPY);
    DeleteDC(BitmapDC);
    DeleteObject(Image);
}

//Desenha a imagem
void DrawImg(HDC hdc, const RECT * Rect, wchar_t * ImgPath)
{
    BITMAP bm;
    static HBITMAP Image = NULL;
    static wchar_t * LastImg = NULL;
    if(ImgPath != LastImg)
    {
        DeleteObject(Image);
        Image = LoadBitmap(GInstance, ImgPath);
    }
    HDC BitmapDC = CreateCompatibleDC(hdc);
    SelectObject(BitmapDC, Image);
    GetObject((HGDIOBJ)Image, sizeof(bm), &bm);
    TransparentBlt(hdc, Rect->left, Rect->top, bm.bmWidth, bm.bmHeight, BitmapDC,
    0, 0, Rect->right - Rect->left, Rect->bottom - Rect->top, RGB(255, 0, 255));
    DeleteDC(BitmapDC);
    LastImg = ImgPath;
}

//Renderiza a imagem de Background
void RenderBkgd(HDC hdc)
{
    BITMAP bm;
    HBITMAP Image = LoadBitmap(GInstance, L"Background");
    HDC BitmapDC = CreateCompatibleDC(hdc);
    SelectObject(BitmapDC, Image);
    GetObject((HGDIOBJ)Image, sizeof(bm), &bm);
    BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, BitmapDC, 0, 0, SRCCOPY);
    DeleteDC(BitmapDC);
    DeleteObject(Image);
}

//Função que renderiza o mapa
void RenderMap(const DArray * Map, HDC hdc)
{
    for(int i = 0; i < Map->Size; ++i)
    {
        block * B = (block *) Map->List[i];
        DrawImg(hdc, &B->hitbox, B->img);
    }
}

//Renderiza a ferramenta na mão do jogador
void RenderTool(const character * Player, HDC hdc)
{
    RECT R;
    wchar_t * img;
    R.top = Player->hitbox.top + 24;
    R.bottom = R.top + 15;
    if(Player->state & 4 >> Player->run)
    {
        if(Player->facing == 1)
        {
            img = Player->inventory[Player->mainSlot].imgLeft;
        }else
        {
            img = Player->inventory[Player->mainSlot].imgRight;
        }
        R.left = Player->hitbox.left + 8;
        R.right = R.left + 15;
    } else
    {
        if(Player->facing == 1)
        {
            R.left = Player->hitbox.left;
            R.right = R.left + 15;
            img = Player->inventory[Player->mainSlot].imgLeft;
        } else
        {
            R.left = Player->hitbox.left + 16;
            R.right = R.left + 15;
            img = Player->inventory[Player->mainSlot].imgRight;
        }
    }

    DrawImg(hdc, &R, img);
}

//Renderiza o jogador, com a imagem correta
void RenderPlayer(const character * Player, HDC hdc)
{
    wchar_t * img;
    if(Player->state & 4 >> Player->run)
    {
        if (Player->facing == 1)
        {
            img = L"FerE";
        } else {
            img = L"FerD";
        }
    }
    else
    {
        if (Player->facing == 1)
        {
            img = L"FerEA";
        } else {
            img = L"FerDA";
        }
    }

    DrawImg(hdc, &Player->hitbox, img);
}

//Renderiza o zumbi
void RenderZombie(const zombie * zombie, HDC hdc)
{
    wchar_t * img;
    if(zombie->state & 8)
    {
        if(zombie->damage < 5)
        {
            if(zombie->facing == 1)
            {
                img = L"ZumbiE";
            } else {
                img = L"ZumbiD";
            }
        } else {
            if(zombie->facing == 1)
            {
                img = L"BossE";
            } else {
                img = L"BossD";
            }
        }
    }
    else
    {
        if(zombie->damage < 5)
        {
            if(zombie->facing == 1)
            {
                img = L"ZumbiEA";
            } else {
                img = L"ZumbiDA";
            }
        } else {
            if(zombie->facing == 1)
            {
                img = L"BossEA";
            } else {
                img = L"BossDA";
            }
        }
    }

    DrawImg(hdc, &zombie->hitbox, img);
}

//Renderiza o inventário
void RenderInv(HDC hdc, character * player)
{
    wchar_t * img = L"BarraInv";
    RECT rect = {10, 10, 317, 42};
    int left = 20, top = 18, right = 36, bottom = 34;
    DrawImg(hdc, &rect, img);
    for(int i = 0; i <= 5; i ++)
    {
        img = (player->inventory[i].imgLeft);
        RECT rect = {left, top, right, bottom};
        DrawImg(hdc, &rect, img);
        left +=34;
        right +=34;
    }
}

//Renderiza a vida do jogador
void RenderLife(HDC hdc, int life)
{
    wchar_t * img1;
    wchar_t * img2;
    int left = 5, top = 50, right = 37, bottom = 82;
    RECT Frame = {10, 50, 215, 82};
    DrawRect(hdc, &Frame, RGB(200,200,255));
    for(int i = 1; i <= life; i ++)
    {
        img1 = L"Heart";
        RECT R1 = {left, top, right, bottom};
        DrawImg(hdc, &R1, img1);
        left += 20;
        right += 20;
    }
    for(int i = life + 1; i <= 10; i++)
    {
        img1 = L"HeartBroken";
        RECT R1 = {left, top, right, bottom};
        DrawImg(hdc, &R1, img1);
        left += 20;
        right += 20;
    }
}

void RenderMenu(int Mode)
{
    HDC hdc = GetDC(Ghwnd);
    RECT R;
    GetClientRect(Ghwnd, &R);
    R.left = R.right / 2 - 200;
    R.right = R.left + 400;
    R.top += 450;
    R.bottom = R.top + 210;
    HFONT Font = CreateFont(30, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
    SelectObject(hdc, Font);
    DrawRect(hdc, &R, RGB(128,128,128));
    SetBkColor(hdc, RGB(128,128,128));
    TextOut(hdc, R.left + 50, R.top + 30, L"Novo Mapa", 9);
    TextOut(hdc, R.left + 50, R.top + 90, L"Carregar Mapa", 13);
    TextOut(hdc, R.left + 50, R.top + 150, L"Jogar Mapa Do Mundo Senai", 25);
    R.left += 10;
    R.right = R.left + 32;
    if(Mode == 1)
    {
        R.top += 30;
    }else
    if(Mode == 2)
    {
        R.top += 90;
    }else
    {
        R.top += 150;
    }
    R.bottom = R.top + 32;
    DrawImg(hdc, &R, L"Heart");
    DeleteObject(Font);
    ReleaseDC(Ghwnd, hdc);
}

void RenderLogo(HDC hdc, int GameOver)
{
    RECT R;
    GetClientRect(Ghwnd, &R);
    R.left = R.right / 2 - 237;
    R.right = R.left + 475;
    R.top += 200;
    R.bottom = R.top + 150;
    if(GameOver)
    {
        HFONT Font = CreateFont(72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, NULL);
        SelectObject(hdc, Font);
        DrawRect(hdc, &R, RGB(128,128,128));
        SetBkColor(hdc, RGB(128,128,128));
        TextOut(hdc, R.left + 50, R.top + 40, L"GAME OVER", 10);
        DeleteObject(Font);
    }else
    {
        DrawImg(hdc, &R, L"TernariaLogo");
    }
}