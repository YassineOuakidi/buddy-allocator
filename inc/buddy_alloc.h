#ifndef BUDDY_ALLOC_H
#define BUDDY_ALLOC_H


#include <sys/mman.h>
#include <unistd.h>


#define POOL_SIZE 1024
#define MAX_ORDER 10

typedef struct List 
{
	int start ;
	size_t len ;
	void *ptr_start;
	int flag;
	struct List *next;

} 	node_t ;

typedef struct Segment
{
	void *base;
	node_t *head;

} 	segment_t ;

void init(segment_t *seg);
size_t nearest_power_of_two(size_t sz);
int get_how_much(int len , size_t new_size);
void split(node_t **node);
node_t *find_node_by_ptr(void *ptr);
void merge(node_t *node);

void *buddy_alloc(size_t sz);
void buddy_free(void *ptr);

#endif
