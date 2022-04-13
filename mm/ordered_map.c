#include "ordered_array.h"
#include "string.h"
#include "kheap.h"

ordered_array_t create_ordered_array(uint32_t max_size,cmp_func cmp)
{
    ordered_array_t to_ret;
    to_ret.array=(type_t *)kmalloc(max_size*sizeof(type_t));
    memset(to_ret.array,0,sizeof(to_ret.array));
    to_ret.size=0;
    to_ret.max_size=max_size;
    to_ret.cmp=cmp;
    return to_ret;
}
ordered_array_t place_ordered_array(void * addr,uint32_t max_size,cmp_func cmp)//在指定addr处开始建造
{
    ordered_array_t to_ret;
    to_ret.array=(type_t *)addr;
    memset(to_ret.array,0,max_size*sizeof(type_t));
    to_ret.size=0;
    to_ret.max_size=max_size;
    to_ret.cmp=cmp;
    return to_ret;
}


void destroy_ordered_array(ordered_array_t *array)
{
    //liangtodo
}

void insert_ordered_array(type_t item,ordered_array_t *array)
{
    uint32_t i=0;
    //cmp 左<右 true
    while(i<array->size&&array->cmp(array->array[i],item))
    {
        i++;
    }
    if(i==array->size)
    {
        //没有小于的位置 应该放到最后
        array->array[array->size++]=item;
    }
    else 
    {
        for(uint32_t idx=array->size;idx>i;--i)
        {
            array->array[idx]=array->array[idx-1];
        }
        array->array[i]=item;
        array->size++;
    }
}

type_t lookup_ordered_array(uint32_t i,ordered_array_t *array)
{
    //assert
    return array->array[i];
}

void remove_ordered_array(uint32_t i,ordered_array_t *array)
{
    for(;i<(array->size-1);++i)//liangtodo james_bug?
    {
        array->array[i]=array->array[i+1];
    }
    array->size--;
}
