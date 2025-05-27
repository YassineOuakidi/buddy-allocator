
#include "../inc/buddy_alloc.h"

extern segment_t *seg_ptr;

int min(int a , int b)
{
	return (a<b ? a : b);
}


size_t nearest_power_of_two(size_t sz)
{
	size_t new_size = 2;
	while(new_size<sz)
		new_size *= 2;
	return new_size;
}

int get_how_much(int len , size_t new_size)
{
	int ans = 0;
	
	while((len/2)>new_size)
	{
		len/=2;
		ans++;
	}
	return ans;
}


void split(node_t **node)
{
	if((*node)->next==NULL)
		return;
	int curr = (*node)->next->len;

	if(curr<2)
		return ;
	
	node_t *first_buddy = mmap(NULL , sizeof(node_t) , PROT_READ | PROT_WRITE , MAP_ANONYMOUS | MAP_PRIVATE , -1 , 0);

	node_t *second_buddy = mmap(NULL , sizeof(node_t) , PROT_READ | PROT_WRITE , MAP_ANONYMOUS | MAP_PRIVATE , -1 , 0);
	
	first_buddy->start = (*node)->next->start ;
	first_buddy->len = curr / 2 ;
	first_buddy->flag = 1;
	first_buddy->ptr_start = seg_ptr->base + first_buddy->start;	

	second_buddy->start = first_buddy->start + curr / 2;
	second_buddy->len = curr / 2;
	second_buddy->flag = 0;
	second_buddy->ptr_start = seg_ptr->base + second_buddy->start;
	
	first_buddy->next = second_buddy;

	second_buddy->next = (*node)->next->next;

	node_t *tmp = (*node)->next;

	(*node)->next = first_buddy;

	munmap(tmp , sizeof(node_t));
}

node_t *find_node_by_ptr(void *ptr)
{
	node_t *node = seg_ptr->head;
	while(node)
	{
		if(node->ptr_start == ptr)
			return node;
		node = node->next;
	}
	return NULL;
}

void delete_node(node_t *node)
{
	node_t *curr = seg_ptr->head;
	node_t *prev = NULL;
	while(curr)
	{
		if(curr==node)
			break;
		prev = curr;
		curr = curr->next;
	}
	if(prev == NULL)
		seg_ptr->head = curr->next;
	else 
		prev->next = curr->next;
	munmap(curr , sizeof(node_t));
}

node_t *find_buddy(node_t *node)
{
	int buddy_start = node->start ^ node->len;
	
	node_t *temp = seg_ptr->head;

	while(temp)
	{
		if(temp->start == buddy_start && temp->len == node->len && temp->flag==0)
			return temp;
		temp = temp->next;
	}
	return NULL;
}

node_t *merge_nodes(node_t *first , node_t *second)
{
	node_t *new_node = mmap(NULL , sizeof(node_t) , PROT_READ | PROT_WRITE , MAP_ANONYMOUS | MAP_PRIVATE , -1 , 0);

	new_node->start = min(first->start , second->start);
	new_node->len = first->len * 2;
	new_node->ptr_start = ( first->start < second->start ? first->ptr_start : second->ptr_start);
	new_node->flag = 0;
	new_node->next = NULL;
	return new_node;
}

void append(node_t *node)
{
	node_t *curr =seg_ptr->head;
	node_t *prev = NULL;
	while( curr && curr->start <node->start )
	{
		prev = curr;
		curr = curr->next;	
	}
	if(prev == NULL)
	{
		node->next = seg_ptr->head;
		seg_ptr->head = node;
	}
	else
	{
		node->next = prev->next;
		prev->next = node;
	}
}

void merge(node_t *node)
{
	node_t *buddy =  find_buddy(node);
	
	if(!buddy || buddy->len != node->len || buddy->flag != 0)
		return;

	node_t *merged = merge_nodes(node , buddy);

	delete_node(node);
	
	delete_node(buddy);

	append(merged);

	merge(merged);
}
