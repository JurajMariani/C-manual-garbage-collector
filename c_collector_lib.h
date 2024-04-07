/**
 * @file c_collector_lib.h
 * @author Juraj Mariani <juraj2.mariani@gmail.com>
 * @brief This is a primitive manual garbage collector library for C
 * @version 1.0
 * @date 2024-04-07
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef __ALLOC_COLLECTOR_LIB__
#define __ALLOC_COLLECTOR_LIB__


typedef struct collector{  
    void** addr_array;
    unsigned int index;
    unsigned int max_index;
}collector_t;

int collector(collector_t* __garbage__);

void* g_alloc(collector_t* __garbage__, size_t __byte_size__);

void* g_realloc(collector_t* __garbage__, void* __current_pointer__, size_t __new_byte_size__);

int g_free(collector_t* __garbage__, void* __current_pointer__);

void _collector(collector_t* __garbage__);

// ADDITIONAL FUNCTIONS:

void __shift__(collector_t* __garbage__, unsigned int __beg__, unsigned int __end__);

long __search__(collector_t* __garbage__, void* __pointer__);

bool __ok_to_save__(unsigned int __idx__, unsigned int __max__);

int __expand_garbage__(collector_t* __garbage__);

#endif
