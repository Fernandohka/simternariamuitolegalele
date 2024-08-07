#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H
#include <stdlib.h>

typedef struct 
{
    void ** List;
    int Size;
    int MaxSize;
} DArray;

void DArrayCreate(DArray * Array, int MaxSize)
{
    Array->Size = 0;
    Array->MaxSize = MaxSize;
    Array->List = (void **) malloc(MaxSize * sizeof(void *));
}

void DArrayGrow(DArray * Array)
{
    Array->List = (void **) realloc(Array->List, Array->MaxSize * sizeof(void *) * 2);
    Array->MaxSize = Array->MaxSize << 1;
}

void DArrayAdd(DArray * Array, void * Value)
{
    Array->List[Array->Size] = Value;
    ++Array->Size;
    if(Array->Size >= Array->MaxSize)
    {
        DArrayGrow(Array);
    }
}

void DArrayRemove(DArray * Array, int index)
{
    --Array->Size;
    for(; index < Array->Size; ++index)
    {
        Array->List[index] = Array->List[index+1];
    }
}

void * DArrayGet(const DArray * Array, int index)
{
    if(index >= Array->Size)
    {
        return NULL;
    }
    return Array->List[index];
}

void DArraySet(const DArray * Array, int index, void * Value)
{
    if(index >= Array->Size)
    {
        return;
    }
    Array->List[index] = Value;
}

void DArrayDestroy(DArray * Array)
{
    free(Array->List);
    Array->MaxSize = 0;
    Array->Size = 0;
}

#endif