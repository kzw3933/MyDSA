#ifndef __GG_ALLOC_H
#define __GG_ALLOC_H

#include <cstddef>
#include <stdlib.h>
#include "GG_config.h"

__GG_BEGIN_NAMESPACE

enum {__ALIGN = 8};                         // 小型区块上调边界
enum {__MAX_BYTES = 128};                   // 小型区块的上限
enum {__NFREELISTS = __MAX_BYTES/__ALIGN};  // free-lists个数
enum {__NOBJS = 20};                        // refill时从内存池中索取的新节点数缺省值

class alloc {

    public:
        static void* allocate(size_t bytes);
        static void deallocate(void* ptr,size_t bytes);
        static void* reallocate(void* ptr, size_t old_sz, size_t new_sz);
        
    private:
        union obj {
            union obj* free_list_link;
            char client_data[1];
        };

    private:
        // 将bytes上调至 __ALIGN 的倍数
        static size_t ROUND_UP(size_t bytes) {
            return (((bytes)+__ALIGN-1)& ~(__ALIGN -1));  
        }
        static obj* free_lists[__NFREELISTS];
        static size_t FREELIST_INDEX(size_t bytes){
            return (((bytes)+__ALIGN-1)/__ALIGN -1);
        }
        static void* refill(size_t n);
        static char* chunk_alloc(size_t size,int& nobjs);

        // chunk allocation state
        static char* start_free;
        static char* end_free;
        static size_t heap_size;

};



char* alloc::start_free = 0;
char* alloc::end_free = 0;
size_t alloc::heap_size = 0;

alloc::obj* alloc::free_lists[__NFREELISTS] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void* alloc::allocate(size_t bytes) {
    if(bytes > __MAX_BYTES) {
        return malloc(bytes);
    }
    size_t index = FREELIST_INDEX(bytes);
    obj* list = free_lists[index];

    if(list) {
        free_lists[index] = list->free_list_link;
        return list;
    }
    else {
        return refill(ROUND_UP(bytes));
    }
}

void alloc::deallocate(void *ptr, size_t bytes){
    if(bytes > __MAX_BYTES) {
        free(ptr);
    }
    else {
        size_t index = FREELIST_INDEX(bytes);
        obj* node = static_cast<obj*>(ptr);
        node->free_list_link = free_lists[index];
        free_lists[index] = node;
    }

}


//!TODO: 补全reallocate的处理

void* alloc::reallocate(void* ptr, size_t old_sz, size_t new_sz){

}

void* alloc::refill(size_t bytes){
    int nobjs = __NOBJS;

    char* chunk = chunk_alloc(bytes, nobjs);
    obj** my_free_list = 0;
    obj* result = 0;
    obj* current_obj = 0,*next_obj = 0;

    if(1 == nobjs) {
        return chunk;
    }

    else {
        my_free_list = free_lists + FREELIST_INDEX(bytes);
        result = (obj*)(chunk);
        *my_free_list = next_obj = (obj*) (chunk+bytes);

        for(int i=1;;++i) {
            current_obj = next_obj;
            next_obj = (obj*)((char*)next_obj + bytes);
            if(nobjs - 1 == i){
                current_obj->free_list_link = 0;
                break;
            }
            else {
                current_obj->free_list_link = next_obj;
            }
        }
        return result;
    }
}

char* alloc::chunk_alloc(size_t bytes, int& nobjs){
    char* result = 0;
    size_t total_bytes = bytes * nobjs;
    size_t bytes_left  = end_free - start_free;

    if(bytes_left >= total_bytes) {
        result = start_free;
        start_free = start_free + total_bytes;
        return result;
    }

    else if(bytes_left >= bytes) {
        nobjs = bytes_left / bytes;
        total_bytes = nobjs * bytes;
        result = start_free;
        start_free += total_bytes;
        return result;
    }
    else {
        size_t bytes_to_get = 2*total_bytes + ROUND_UP(heap_size >> 4);

        if (bytes_left > 0) {
            obj** my_free_list = free_lists + FREELIST_INDEX(bytes_left);
            ((obj*)start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj*)start_free;
        }
        start_free = (char*) malloc(bytes_to_get);
        if(!start_free) {
            obj ** my_free_list = 0,*p = 0;
            for(int i= bytes; i<__MAX_BYTES;i+=__ALIGN) {
                my_free_list = free_lists + FREELIST_INDEX(i);
                p = *my_free_list;
                if (0 != p) {
                    *my_free_list = p->free_list_link;
                    start_free = (char*) p;
                    end_free = start_free + i;
                    return chunk_alloc(bytes, nobjs);
                }
            }
            end_free = 0;
        }

        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;

        return chunk_alloc(bytes,nobjs);
    }
}

__GG_END_NAMESPACE

#endif