#include "../include/MemoryManager.hpp"

MemoryManager::MemoryManager() {
    this->head = nullptr;
    this->tail = nullptr;
    this->heapPosition = 0;
}

MemoryManager::~MemoryManager() {
}

void* MemoryManager::alloc(const unsigned int& size) {
    if(size) {
        // O(n) => n number of blocks that were previously allocated
        MemoryBlockHeader* header = getFreeBlock(size);

        // Use free block if true
        if(header) {
            std::cout << DEBUG << "Using Free Block for Allocation\n";
            header->isFree = false;
            return (void*)(header+1);
        }

        // Allocate data in our virtual heap
        void* block = allocateMemoryOnHeap(HEADER_SIZE + size);

        // Heap is full
        if(!block) {
            std::cout << CRITICAL << "Could not allocate memory on heap\n";
            return nullptr;
        }

        // Create a Memory Block Header - Has metadata on the allocated memory
        header = createBlockHeader(block, size);

        // Insert the Memory Block header object into the vritual linked list
        insertBlock(header);

        // Return pointer to the data after the Memory Block Headder object
        return (void*)(header+1);
    } else {

        // Invalid size provided so return nullptr - We consider 0 to be an invalid value (with malloc it is considered to be valid)
        return nullptr;
    }    
}

void MemoryManager::free(void* block) {
    if(block) {
        MemoryBlockHeader* header = (MemoryBlockHeader*)block - 1;
        auto deallocatedBlock = (void*)(header->size + (uintptr_t)block);

        if(deallocatedBlock == (heap+heapPosition)) {
            std::cout << DEBUG << "Deallocating " << (HEADER_SIZE + header->size) << " from tail end of heap\n";
            deallocateMemoryOnHeap(HEADER_SIZE + header->size);
            return;
        } else if (block == header->start) {
            std::cout << DEBUG << "Marking block as free\n";
            header->isFree = true;
        } else {
            std::cout << ERROR << "Invalid pointer attempted to be freed\n";
        }

    } else {
        std::cout << ERROR << "Invalid block was attempted to be freed\n";
        return;
    }

}

void MemoryManager::printHeap() {
    for(int i = 0; i < HEAP_CAP_WORDS; i++){
        if(heap[i]) {
            std::cout << INFO << "|" << std::setw(HEX_WORD_WIDTH) << heap+i << "| => ";	
            std::cout << "|" << std::setw(HEX_WORD_WIDTH) << heap[i] << "|\n";	
        } else {
            std::cout << INFO << "|" << std::setw(HEX_WORD_WIDTH) << heap+i << "| => ";	
            std::cout << "|" << std::setw(HEX_WORD_WIDTH) << heap[i] << "|\n";	
        }
    }
}

void MemoryManager::dumpHeap() {
    MemoryBlockHeader* curr = head;

    unsigned int numUsedWords = 0;

    std::vector<MemoryBlockHeader*> freedBlocks;

    std::cout << INFO << "HEAP TOTAL CAPACITY = " << HEAP_CAP_WORDS << " words\n";
    std::cout << INFO << "Allocated Blocks\n";
    while(curr) {
        const unsigned int numWords = getNumWordsFromBytes(HEADER_SIZE + curr->size);
        if(!curr->isFree) {
            numUsedWords += numWords;

            std::cout << INFO << "| Start = " << curr << "| => ";
            std::cout << "| Size = " << numWords << "|\n"; 

        } else {
            freedBlocks.push_back(curr);
        } 
        curr = curr->next;
    }

    std::cout << INFO << "Freed Blocks(" << freedBlocks.size() << ")\n";
    for(auto freedBlock : freedBlocks) {
        const unsigned int numWords = getNumWordsFromBytes(HEADER_SIZE + freedBlock->size);
        std::cout << INFO << "| Start = " << freedBlock << "| => ";
        std::cout << "| Size = " << numWords << "|\n"; 
    }

    // Display the rest of memory that has remain untouched
    std::cout << INFO << "Unused/Released Blocks\n";     
    std::cout << INFO << "| Start = " << (heap+heapPosition) << "| => | Size = " << (HEAP_CAP_WORDS - heapPosition) << "|\n";    


}

MemoryBlockHeader* MemoryManager::getFreeBlock(const unsigned int& size) {
    MemoryBlockHeader* curr = head;
    
    while(curr) {
        if(curr->isFree && curr->size >= size) {
            return curr;
        }

        curr = curr->next;
    }

    return nullptr;
}

void MemoryManager::insertBlock(MemoryBlockHeader* header) {
    if(!head) {
        head = header;
    }
    
    if(tail) {
        tail->next = header;
    }

    tail = header;

    std::cout << DEBUG << "Added block to block list\n";
}

void MemoryManager::removeTailBlock() {
    if(head == tail) {
        head = tail = nullptr;
     } else {
        MemoryBlockHeader* tmp = head;
        while(tmp) {
            if(tmp->next == tail) {
                tmp->next = nullptr;
                tail = tmp;
            } else {
                tmp = tmp->next;
            }
        }
    }
}

MemoryBlockHeader* MemoryManager::createBlockHeader(void* block, const unsigned int& size) {
    MemoryBlockHeader* header = (MemoryBlockHeader*)block;
    header->size = size;
    header->isFree = false;
    header->start = header+1;
    header->next = nullptr;        
    return header;
}

void* MemoryManager::allocateMemoryOnHeap(const unsigned int& size) {
    const unsigned int numWords = getNumWordsFromBytes(size);

    if(numWords > 0 && heapPosition + numWords <  HEAP_CAP_WORDS) {
        const unsigned oldHeapPosition = heapPosition;
        heapPosition += numWords;
        return heap + oldHeapPosition;
    }

    return nullptr;
}

void MemoryManager::deallocateMemoryOnHeap(const unsigned int& size) {
    const unsigned int wordSize = (size + sizeof(uintptr_t) - 1) / sizeof(uintptr_t);
    for(int i = 0; i < wordSize; i++){
        heap[heapPosition--] = 0;
    }
}

