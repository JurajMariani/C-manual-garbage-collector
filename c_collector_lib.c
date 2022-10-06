#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "c_collector_lib.h"

int collector(collector_t* garbage)
{
    if (garbage == NULL)
    {
        fprintf(stderr, "WARNING: Null input.\n");
        return 5;
    }

    garbage->index = 0;
    garbage->max_index = 100;
    garbage->addr_array = malloc(garbage->max_index*sizeof(void*));
    if (garbage->addr_array == NULL)
    {
        fprintf(stderr, "ERROR: Malloc_err.\n");
        return 1;
    }

    return 0;
}


void* g_alloc(collector_t* garbage, size_t __byte_size__)
{
    if (__ok_to_save__(garbage->index, garbage->max_index))
    {
        garbage->addr_array[garbage->index] = malloc(__byte_size__);

        if (garbage->addr_array[garbage->index] != NULL)
        {
            garbage->index++;
            return garbage->addr_array[garbage->index - 1];
        }
        
        return garbage->addr_array[garbage->index];
    }
    else
    {
        if (!__expand_garbage__(garbage))
        {
            return g_alloc(garbage, __byte_size__);
        }

        return NULL;
    }

}


void* g_realloc(collector_t* garbage, void* __current_pointer__, size_t __new_byte_size__)
{
    long idx = __search__(garbage, __current_pointer__);

    if( idx >= 0 )
    {
        void* placeholder = garbage->addr_array[idx];
        garbage->addr_array[idx] = realloc(garbage->addr_array[idx], __new_byte_size__);

        if (garbage->addr_array[idx] == NULL)
        {
            garbage->addr_array[idx] = placeholder;
            return NULL;
        }

        return garbage->addr_array[idx];
    }
        fprintf(stderr, "WARNING: Pointer not found.\n");
        return NULL;
}


int g_free(collector_t* __garbage__, void* __current_pointer__)
{
    long idx = __search__(__garbage__, __current_pointer__);

    if( idx >= 0 )
    {
        free(__garbage__->addr_array[idx]);
        __shift__(__garbage__, idx, __garbage__->index);
        __garbage__->index--;
        return 0;
    }
        fprintf(stderr, "WARNING: Pointer not found.\n");
        return 1;
}


void _collector(collector_t* __garbage__)
{
    while (true)
    {
        free(__garbage__->addr_array[__garbage__->index]);
        if (__garbage__->index == 0)
            break;
        __garbage__->index--;
    }
    free(__garbage__->addr_array);
    __garbage__->addr_array = NULL;
}


bool __ok_to_save__(unsigned int __idx__, unsigned int __max__)
{
    if (__idx__ < __max__)
        return true;
    return false;
}


int __expand_garbage__(collector_t* __garbage__)
{
    __garbage__->max_index += 100;
    void** placeholder = __garbage__->addr_array;

    __garbage__->addr_array = realloc(__garbage__->addr_array, __garbage__->max_index * sizeof(void*));

    if (__garbage__->addr_array == NULL)
    {
        __garbage__->addr_array = placeholder;
        fprintf(stderr, "ERROR: MALLOC Err.\n");
        return 1;
    }

    return 0;
}


void __shift__(collector_t* __garbage__, unsigned int __beg__, unsigned int __end__)
{
    for(unsigned int i = __beg__; i < __end__; i++)
    {
        __garbage__->addr_array[i] = __garbage__->addr_array[i + 1];
    }
    __garbage__->addr_array[__end__] = NULL;
}


long __search__(collector_t* __garbage__, void* __pointer__)
{
    //sequence search
    //LATER: utilisation of heuristics -> if an element is searched, it moves closer = swaps with it's left neighbour

    for(unsigned int idx = 0; idx <= __garbage__->index; idx++)
    {
        if (__garbage__->addr_array[idx] == __pointer__)
        {
            return idx;
        }
    }

    return -1;
}

