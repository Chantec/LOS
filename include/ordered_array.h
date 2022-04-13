#ifndef INCLUDE_ORDERED_ARRAY_H_
#define INCLUDE_ORDERED_ARRAY_H_

#include "types.h"

typedef void * type_t;//void * liangtodo

//用来排序的函数
typedef int8_t (*cmp_func)(type_t ,type_t);

typedef struct 
{
    type_t *array;//指向指针的数组（指针
    uint32_t size;
    uint32_t max_size;
    cmp_func cmp;
}ordered_array_t;

ordered_array_t create_ordered_array(uint32_t max_size,cmp_func cmp);
ordered_array_t place_ordered_array(void * addr,uint32_t max_size,cmp_func cmp);//在指定addr处开始建造


void destroy_ordered_array(ordered_array_t *array);

void insert_ordered_array(type_t item,ordered_array_t *array);

type_t lookup_ordered_array(uint32_t i,ordered_array_t *array);//return the item at index i

void remove_ordered_array(uint32_t i,ordered_array_t *array);//del the item at index i






#endif