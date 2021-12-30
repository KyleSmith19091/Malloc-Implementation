<h1 align="center">Malloc Implementation</h1>

<h3>Implementation Discussion</h3>
Heap is represented as an array of uintptr_t, where each index in the heap will have space for an 8 byte value(this depends on the user's system), which is used to represent a form of alignment that allocations need to follow. 

<h4 align="center">📐 Allocation</h4>
Allocations will then start with a process where we will traverse a list of 'headers' which hold the matadata for previous or currently in use memory allocations, by inspecting whether a block of suffiecent size is available.
If a block is available it will then mark that block as in use and return a pointer to the user. This is done by returning the memory location that is one unit after the header(current header address + 16 bytes/size of header struct).
This list is implemented as a singly linked list and will provide O(n) traversals where n is the number of previously allocated / in use blocks of memory on heap. In the case where a block is not found the linked list, we then exapnd our
heap memory at the tail end to make space for the allocation request.

<h4 align="center">🔥 Deallocation</h4>
Deallocation is done by first checking if the block to be deallocated is at the tail end of the heap, if this is the case we proceed to 'give back' the memory (simulating a similar behaviour of the malloc function). Otherwise 
we simply mark the block as free for reuse. Thus dealocation is done O(1) time except when giving back the memory as we manually clear the data in the heap (just a personal design decision).

<h4 align="center">⚡️ Serialising</h4>
The main purpouse of the project is to get a basic understanding of how malloc works with a basic heap memory structure. So for this purpose I created two member functions of the memory manager class
- printHeap()
- dumpHeap()

Printing the heap, will show the actaul values that are stored in the heap. Dumping the heap will show some more of the metadata that is associated with the heap, such as used, freed and unused blocks.
