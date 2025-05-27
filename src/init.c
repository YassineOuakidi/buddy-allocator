
#include "../inc/buddy_alloc.h"


void init(segment_t *seg)
{
	seg->base = mmap(NULL , 1024 , PROT_READ | PROT_WRITE , MAP_ANONYMOUS | MAP_PRIVATE , -1 , 0);

	node_t *node = mmap(NULL , sizeof(node_t) , PROT_READ | PROT_WRITE , MAP_ANONYMOUS | MAP_PRIVATE , -1 , 0);

	node->start = 0 ;
	node->len = 1024 ;
	node->ptr_start = seg->base;
	node->next = NULL;
	node->flag = 0;

	seg->head = node;
}
