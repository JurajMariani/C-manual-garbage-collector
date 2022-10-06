#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "c_collector_lib.h"

// Lajbave testy jak svina

int main()
{
    collector_t garb;

    int status = collector(&garb);

    printf("Status: %d\n", status);
    printf("arrd: %p, act: %d, max: %d\n", garb.addr_array, garb.index, garb.max_index);

    int* a = g_alloc(&garb, sizeof(int));
    printf("Alloc'd address: %p\n",a);

    *a = 10;

    printf("A: %d\n", *a);

    g_free(&garb, a);

    _collector(&garb);

    collector(&garb);

    int i = 0;
    int* b;
    while (i < 10)
    {
        b = g_alloc(&garb, sizeof(int));
        printf("Addr of b: %p\n", b);
        i++;
    }

    // no g_free this time

    _collector(&garb);

    collector(&garb);

    int* p = g_alloc(&garb, 5*sizeof(int));
    p[0] = 10;
    p[1] = 15;
    p[2] = 20;

    for(int i = 0; i < 3; i++)
    {
        printf("cislo: %d\n", p[i]);
    }

    p = g_realloc(&garb, p, 2*sizeof(int));

    for(int i = 0; i < 3; i++)
    {
        printf("cislo: %d\n", p[i]);
    }

    _collector(&garb);


    collector(&garb);
    // Test zataze:

    for(int i = 0; i < 150; i++)
    {
        // should trigger collector realloc
        g_alloc(&garb, sizeof(char));
    }

    _collector(&garb);


    return 0;

}