#include "oslabs.h"

struct MEMORY_BLOCK NULLBLOCK = {0, 0, 0, 0}; 

// Best Fit
struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id){

    int closest = __INT_MAX__;
    int idx = -1; 

    for (int i = 0; i < *map_cnt; i++){

        // find free memory block 
        if (memory_map[i].process_id == 0 && memory_map[i].segment_size >= request_size) {

            // choose size closest to requested size
            if (abs(request_size - memory_map[i].segment_size) < closest)
            {
                closest = memory_map[i].segment_size;
                idx = i;
            };
        };
    };

    // exact size
    if (memory_map[idx].segment_size == request_size){

        process_id = memory_map[idx].process_id;
        return memory_map[idx];
    };
    
    // if no block at least greater     
    if (idx == -1) {
        return NULLBLOCK; 
    };

    struct MEMORY_BLOCK allocated_block; 

    // free block larger than requested size
    if (memory_map[idx].segment_size > request_size) {

        // allocate exact portions 

        // free block portion 

        // adjust memory map

    }
};

// First Fit
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id){

};

// Worst (Largest) Fit
struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id){

};

// Next Fit
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id, int last_address){

};

// Release
void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt){
    
};