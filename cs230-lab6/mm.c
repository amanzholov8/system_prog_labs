/*
 * mm-explicitlist.c - Relatively fast and  memory-efficient malloc package.
 * 
 * use doublylinked list for allocating and freeing the memory.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Basic constants and macros*/
#define WSIZE     4          /* word and header/footer size (bytes) */
#define DSIZE     8          /* double word size (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */ 

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)            (*(unsigned int *)(p))
#define PUT(p, val)       (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */ 
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer*/
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of its header and footer */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE((char *)(bp) - WSIZE))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))

/* Get predecessor and successor addresses of a free block on the segregated list */
#define PREV(bp) (*(char **)(bp))
#define NEXT(bp) (*(char **)((char *)(bp) + WSIZE))

//#define BLK_HDR_SIZE (ALIGN(sizeof(blockHdr)))

static char *heap_listp = 0;
static char *freeb;

//typedef struct header blockHdr;

void *find_fit(size_t asize);

static void delete_block(void *bp)
{
	void *t1;

	if (NEXT(bp)==NULL && PREV(bp)==NULL)
		freeb = NULL;
	else if (PREV(bp) == NULL) {
		freeb = NEXT(bp);
		PREV(freeb) = NULL;
	}
	else  {
		t1 = PREV(bp);
		NEXT(t1) = NEXT(bp);
		t1 = NEXT(bp);
		if (t1 != NULL)
			PREV(t1) = PREV(bp);
	}
	
			
}

static void insert_block(void *bp)
{
	if (freeb == NULL) {
		freeb = bp;
		PREV(freeb) = NULL;
		NEXT(freeb) = NULL;
	}
	else {
		PREV(bp) = NULL;
		NEXT(bp) = freeb;
		PREV(freeb) = bp;
		freeb = bp;
	}
}

void *find_fit(size_t asize)
{

        /* First fit search*/
        void *bp;

        for (bp = freeb; bp != NULL; bp = NEXT(bp)) {
               if ((asize <= GET_SIZE(HDRP(bp))) && (GET_SIZE(HDRP(bp)) > 0) )
                        return bp;
        }

        return NULL; //no fit

}

static void *coalesce(void *bp)
{
        size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
        size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
        size_t size = GET_SIZE(HDRP(bp));
        //case 1
	if (prev_alloc && next_alloc) {
                insert_block(bp);
		return bp;
        }
	//case 2
        else if (prev_alloc && !next_alloc) {
                size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
		delete_block(NEXT_BLKP(bp));
                PUT(HDRP(bp), PACK(size, 0));
                PUT(FTRP(bp), PACK(size,0));
        }
	//case 3
        else if (!prev_alloc && next_alloc) {
                size += GET_SIZE(HDRP(PREV_BLKP(bp)));
		bp = PREV_BLKP(bp);
		delete_block(bp);
                PUT(HDRP(bp), PACK(size, 0));
                PUT(FTRP(bp), PACK(size, 0));
        }
	//case 4
        else {
                //size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
                //GET_SIZE(FTRP(NEXT_BLKP(bp)));
                //PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
                //PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
                //bp = PREV_BLKP(bp);
		
		insert_block(bp);
		delete_block(PREV_BLKP(bp));
		bp = coalesce(PREV_BLKP(bp));
		delete_block(bp);
		bp = coalesce(bp);
		delete_block(bp);
        }

	insert_block(bp);
        return bp; 
}

static void place(void *bp, size_t asize)
{
        size_t csize = GET_SIZE(HDRP(bp));
        delete_block(bp);

        if ((csize - asize) >= (2*DSIZE)) {
                PUT(HDRP(bp), PACK(asize, 1));
                PUT(FTRP(bp), PACK(asize, 1));
                bp = NEXT_BLKP(bp);
                PREV(bp) = PREV(PREV_BLKP(bp));
                NEXT(bp) = NEXT(PREV_BLKP(bp));
                PUT(HDRP(bp), PACK(csize-asize, 0));
                PUT(FTRP(bp), PACK(csize-asize, 0));
                coalesce(bp);
        }
        else {
                PUT(HDRP(bp), PACK(csize, 1));
                PUT(FTRP(bp), PACK(csize, 1));
    	}
}

static void *extend_heap(size_t words) 
{
	char *bp;
	size_t size;

	/* Allocate an even number of words to maintain alignment */
	size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
	if ((long)(bp = mem_sbrk(size)) == -1)
		return NULL;

	/* Initialize free block header/footer and the epilogue header */
	PUT(HDRP(bp), PACK(size, 0)); /* Free block header */
	PUT(FTRP(bp), PACK(size, 0)); /* Free block footer */
	PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

	/* Coalesce if the previous block was free */
	return coalesce(bp);
}



/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{

	/* Create the initial empty heap */
	if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
		return -1;
	PUT(heap_listp, 0); /* Alignment padding */
	PUT(heap_listp + (1*WSIZE), PACK(DSIZE, 1)); /* Prologue header */
	PUT(heap_listp + (2*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
	PUT(heap_listp + (3*WSIZE), PACK(0, 1)); /* Epilogue header */
	heap_listp += (2*WSIZE);
	freeb = NULL;
	
	/* Extend the empty heap with a free block of CHUNKSIZE bytes */
	if (extend_heap(CHUNKSIZE/WSIZE) == NULL)
		return -1;
	return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{

	size_t asize; /* Adjusted block size */
	size_t extendsize; /* Amount to extend heap if no fit */
	char *bp;

	/* Ignore spurious requests */
	if (size <= 0)
    		return NULL;
	
	/* Adjust block size to include overhead and alignment reqs. */
	if (size <= DSIZE)
    		asize = 2*DSIZE;
	else
		asize = ALIGN(size + DSIZE);

	/* Search the free list for a fit */
	if ((bp = find_fit(asize)) != NULL) {
    		place(bp, asize);
    		return bp;
	}
	/* No fit found. Get more memory and place the block */
	extendsize = MAX(asize,CHUNKSIZE);
	if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        	return NULL;
    	place(bp, asize);
    	return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{

	size_t size = GET_SIZE(HDRP(ptr));

	PUT(HDRP(ptr), PACK(size, 0));
	PUT(FTRP(ptr), PACK(size, 0));
	coalesce(ptr);	
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
       size_t oldsize;
        void *newptr;

        if (size==0) {
                mm_free(ptr);
                return NULL;
        }

        if(ptr==NULL)
                return mm_malloc(size);

        oldsize = GET_SIZE(HDRP(ptr));
        size_t newsize = size + DSIZE;

        if (newsize<= oldsize)
                return ptr;

	size_t nextbsize = GET_SIZE(HDRP(NEXT_BLKP(ptr)));	

        if (!GET_ALLOC(HDRP(NEXT_BLKP(ptr)))
                 && (oldsize+nextbsize >= newsize)) {
                delete_block(NEXT_BLKP(ptr));
                PUT(HDRP(ptr), PACK(oldsize+nextbsize, 1));
                PUT(FTRP(ptr), PACK(oldsize+nextbsize, 1));
                return ptr;
        }

        newptr = mm_malloc(newsize);

        if (newsize<oldsize)
                oldsize = newsize;

        memcpy(newptr, ptr, oldsize);

                mm_free(ptr);

        return newptr;
}















