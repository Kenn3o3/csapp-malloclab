## **ICS- Malloclab Report**


<hr>

I have used explicit list allocator for this lab, It treats the memory allocated by malloc as a linked list of blocks.
Details data structure is as follows:


### block header 
```cpp
typedef struct header blockHdr;

struct header {
  size_t size;
  struct header *next;
  struct header *prev;
};
```
### Initialisation
```cpp
int mm_init(void) { 
  blockHdr *p = mem_sbrk(BLK_HDR_SIZE);
  p->size = BLK_HDR_SIZE;
  p->next = p;
  p->prev = p;
  return 0;
 }
```
> each block doublely links with each other
### Find the first fit
```cpp
void *find_fit(size_t asize) {
  blockHdr *p = mem_heap_lo();
  while (p->next != p) {
    if (p->size >= asize)
      return p;
    p = p->next;
  }
  return NULL;
}

// each block: [ size | Content | Allo ] (16 bytes alignmen)

```
> looping over the heap and find the first possible heap slot for the memory to sit in.

> improvement can be made here by choose the smallest possible slot of memory instead of the first one
### Free memory
```cpp

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
  blockHdr *bp = (blockHdr *)((char *)ptr - BLK_HDR_SIZE);
  bp->size &= ~1;
  bp->next = bp;
  bp->prev = bp;
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */

```
> freeing the memory by searching from `*ptr` and remove the linking relationships of that block
###  Reallocation and Allocation
```cpp
void *mm_realloc(void *ptr, size_t size) {
  void *newptr;
  size_t copySize;

  newptr = mm_malloc(size);
  if (newptr == NULL)
    return NULL;
  copySize = *(size_t *)((char *)ptr - SIZE_T_SIZE);
  if (size < copySize)
    copySize = size;
  memcpy(newptr, ptr, copySize);
  mm_free(ptr);
  return newptr;
}
```
## Reflection
I have used almost the simpliest method (Explicit list allocator) to implement the memory allocator and the result doesn't go well. Many of the cases are not passed. 

Using Implicit list allocation or Buddy allocation may provide better performance and reliability. I will try to implement the following types of allocators in the future.