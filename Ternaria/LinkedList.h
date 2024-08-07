#ifndef LINKED_LIST
#define LINKED_LIST
#include <stdlib.h>

typedef struct LLNode
{
    void * Value;
    struct LLNode * Next;
} LLNode;

typedef struct LList
{
    LLNode * Head;
    int Size;
} LList;

void LListCreate(LList * List)
{
    List->Head = NULL;
    List->Size = 0;
}

void LListAdd(LList * List, void * Value)
{
    LLNode * N = (LLNode *) malloc(sizeof(LLNode));
    N->Value = Value;
    if(List->Size)
    {
        N->Next = NULL;
        int i = List->Size;
        LLNode * C = List->Head;
        while(--i)
        {
            C = C->Next;
        }
        C->Next = N;
    }else
    {
        N->Next = List->Head;
        List->Head = N;
    }
    ++List->Size;
}

void * LListGet(LList * List, int index)
{
    if ((unsigned) index >= List->Size)
    {
        return 0;
    }

    LLNode * N = List->Head;
    while(index--)
    {
        N = N->Next;
    }
    return N->Value;
}

void LListSet(LList * List, int index, void * Value)
{
    if ((unsigned) index >= List->Size)
    {
        return;
    }

    LLNode * N = List->Head;
    while(index--)
    {
        N = N->Next;
    }
    N->Value = Value;
}

void LListRemove(LList * List, int index)
{
    if((unsigned) index >= List->Size)
    {
        return;
    }
    --List->Size;
    LLNode * N = List->Head;
    if(index == 0)
    {
        List->Head = N->Next;
        free(N);
        return;
    }

    for(int i = 1; i < index; ++i)
    {
        N = N->Next;
    }
    LLNode * P = N->Next;
    N->Next = N->Next->Next;
    free(P);
}

void LListDestroy(LList * List)
{
    for(int i = 0; i < List->Size; ++i)
    {
        LListRemove(List, 0);
    }
}

#endif