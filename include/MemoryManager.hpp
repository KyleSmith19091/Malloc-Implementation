#ifndef MEMORY_MANAGER_HPP 
#define MEMORY_MANAGER_HPP

#include "MemoryBlockHeader.hpp"

#include <cstdint>
#include <unistd.h>
#include <Logging/Logger.hpp>
#include <iostream>
#include <iomanip>
#include <vector>

#define ALLOCATION_ERROR (void*)-1
#define HEX_WORD_WIDTH 16

class MemoryManager {
    public:
        MemoryManager();
        ~MemoryManager();

        // Allocate Memory using given number of bytes
        void* alloc(const unsigned int&);

        // Free allocated memory using given pointer
        void free(void*);
        
        // Print heap
        void printHeap();

        // Dump Heap Info
        void dumpHeap();

    private:
        // Heap capcity (Word = uintptr_t)
        static const unsigned int HEAP_CAP_WORDS = 256 / (sizeof(uintptr_t)); 

        // Actual heap
        uintptr_t heap[HEAP_CAP_WORDS] = {0};

        // Index value representing tail of heap
        unsigned int heapPosition;

        // Header of Memory Block Header linked list
        MemoryBlockHeader* head;
    
        // Tail of Memory Block Header linked list
        MemoryBlockHeader* tail;

        // Find Memory Block using given size that is marked free in linked list 
        MemoryBlockHeader* getFreeBlock(const unsigned int&);

        // Create Memory Block Header to store metadata for allocated data
        MemoryBlockHeader* createBlockHeader(void*, const unsigned int&);

        // Insert the block into linked list
        void insertBlock(MemoryBlockHeader*);

        // Remove block from tail of linked list
        void removeTailBlock();

        // Allocate memory in the internal heap array using given size value
        void* allocateMemoryOnHeap(const unsigned int&);

        // Deallocate memory in the internal heap array using given size value 
        void deallocateMemoryOnHeap(const unsigned int&);

        // Convert bytes to word value (Word = 8 bytes)
        constexpr const unsigned int getNumWordsFromBytes(const unsigned int& size) const {
            return (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
        }

};

#endif
