/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "memlib.h"
#include "mm.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Kenny", 
    /* First member's email address */
    "hi",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""};


#define VERBOSE 0
#ifdef DEBUG
#define VERBOSE 1
#endif

/* 16 bytes alignment */
#define ALIGNMENT 16

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define BLK_HDR_SIZE (ALIGN(sizeof(blockHdr)))

typedef struct header blockHdr;

struct header {
  size_t size;
  struct header *next;
  struct header *prev;
};

void *find_fit(size_t asize);
void print_heap();
/*
 * mm_init - initialize the malloc package.
 */

int mm_init(void) { 
  blockHdr *p = mem_sbrk(BLK_HDR_SIZE);
  p->size = BLK_HDR_SIZE;
  p->next = p;
  p->prev = p;
  return 0;
 }

void print_heap() {
  blockHdr *p = mem_heap_lo();
  while(p < (blockHdr *)mem_heap_hi()) {
    printf("%s block at %p, size %d\n", 
    (p->size&1)?"allocated":"free", 
    p, 
    (int)(p->size & ~1));
    p = (blockHdr*)((char*)p + (p->size & ~1));
}
}


#define MIN_BLOCK_SIZE 

/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size) { //TODO

  int newsize = ALIGN(size + BLK_HDR_SIZE); //16 bytes alignment
  blockHdr *bp = find_fit(newsize);
  if(bp == NULL) {
    bp = mem_sbrk(newsize);
    if (bp == (void *)-1)
    return NULL;
  else {
    bp->size = newsize | 1;
    }
  } else {
    bp->size |= 1;
    bp->next->prev = bp->prev; // |A|B|C|
    bp->prev->next = bp->next;
  }
  return (char*)bp + BLK_HDR_SIZE;
}

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
