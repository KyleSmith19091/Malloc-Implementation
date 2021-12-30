#ifndef MEMORY_BLOCK_HEADER_HPP 
#define MEMORY_BLOCK_HEADER_HPP

#include <cstdint>

struct MemoryBlockHeader{
    unsigned int size; // Size of allocated memory => Needed for freeing memory
    bool isFree; // Used to indicate that memory can be used by alloc request
    void* start; // Start address of allocated data
    struct MemoryBlockHeader* next; // Next in linkedlist 
};	

#define HEADER_SIZE sizeof(MemoryBlockHeader)
#endif
