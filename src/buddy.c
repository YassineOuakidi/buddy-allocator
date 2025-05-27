
#include "../inc/buddy_alloc.h"

extern segment_t seg;
segment_t *seg_ptr = &seg;


void *buddy_alloc(size_t sz)
{
	size_t new_size = nearest_power_of_two(sz);
	node_t *node = seg_ptr->head;
	node_t *prev = NULL;
	
	while(node)
	{
		if(node->len >= new_size && node->flag == 0)
			break;
		prev = node ;
		node = node->next;
	}
	
	int split_count = 1 + get_how_much(node->len , new_size);
	
	if(split_count==1)
	{
		node->flag = 1;
		return node->ptr_start;
	}

	if(prev == NULL)
	{	
		while(split_count--)
		{
			node_t *dummy = mmap(NULL , sizeof(node_t) , PROT_READ | PROT_WRITE , MAP_ANONYMOUS | MAP_PRIVATE , -1 , 0);
		
			dummy->next = node;
			split(&dummy);
			node = dummy->next;
			seg_ptr->head = node;
		}
		return node->ptr_start;
	}
	else
	{
		while(split_count--)
			split(&prev);
		return prev->next->ptr_start; 
	}
}

void buddy_free(void *ptr)
{
	node_t *node = find_node_by_ptr(ptr);
	
	if(!node || node->flag == 0)
		return;
	
	node->flag = 0;

	merge(node);
	
}
