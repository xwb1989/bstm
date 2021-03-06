/*
 * File:
 *   skiplist-lock-map.c
 * Author(s):
 *   Vincent Gramoli <vincent.gramoli@epfl.ch>
 *   Wenbin Xiao <github.com/xwb1989>
 * Description:
 *   Skip list implementation of an integer set
 */

#include "skiplist-lock-map.h"

//unsigned int levelmax;

/* 
 * Returns a pseudo-random value in [1;range).
 * Depending on the symbolic constant RAND_MAX>=32767 defined in stdlib.h,
 * the granularity of rand() could be lower-bounded by the 32767^th which might 
 * be too high for given values of range and initial.
 */
inline long rand_range(long r) {
	int m = RAND_MAX;
	long d, v = 0;
	
	do {
		d = (m > r ? r : m);		
		v += 1 + (long)(d * ((double)rand()/((double)(m)+1.0)));
		r -= m;
	} while (r > 0);
	return v;
}


int get_rand_level() {
	int i, level = 1;
	for (i = 0; i < levelmax - 1; i++) {
		if ((rand_range(100)-1) < 50)
			level++;
		else
			break;
	}
	/* 1 <= level <= levelmax */
	return level;
}

int floor_log_2(unsigned int n) {
	int pos = 0;
	if (n >= 1<<16) { n >>= 16; pos += 16; }
	if (n >= 1<< 8) { n >>=  8; pos +=  8; }
	if (n >= 1<< 4) { n >>=  4; pos +=  4; }
	if (n >= 1<< 2) { n >>=  2; pos +=  2; }
	if (n >= 1<< 1) {           pos +=  1; }
	return ((n == 0) ? (-1) : pos);
}

/* 
 * Create a new node without setting its next fields. 
 */
sl_node_t *sl_new_simple_node(sl_key_t key, sl_val_t val, int toplevel, int transactional)
{
	sl_node_t *node;
	
	node = (sl_node_t *)malloc(sizeof(sl_node_t));
	node->next = (sl_node_t **)malloc(toplevel * sizeof(sl_node_t *));
    node->key = key;
	node->val = val;
	node->toplevel = toplevel;
	node->marked = 0;
	node->fullylinked = 0;
	INIT_LOCK(&node->lock);
	return node;
}

/* 
 * Create a new node with its next field. 
 * If next=NULL, then this create a tail node. 
 */
sl_node_t *sl_new_node(sl_key_t key, sl_val_t val, sl_node_t *next, int toplevel, int transactional)
{
	sl_node_t *node;
	int i;
	
	node = sl_new_simple_node(key, val, toplevel, transactional);
	
	for (i = 0; i < toplevel; i++)
		node->next[i] = next;
	
	return node;
}

void sl_delete_node(sl_node_t *n)
{
	DESTROY_LOCK(&n->lock);
	free(n->next);
	free(n);
}

sl_map_t* sl_map_new()
{
	sl_map_t *map;
	sl_node_t *min, *max;
	
    levelmax = 64;

	map = (sl_map_t*) malloc(sizeof(sl_map_t));
	max = sl_new_node(VAL_MAX, NULL, NULL, levelmax, 0);
	min = sl_new_node(VAL_MIN, NULL, max, levelmax, 0);
	max->fullylinked = 1;
	min->fullylinked = 1;
	map->head = min;
	return map;
}

void sl_map_delete(sl_map_t *set)
{
	sl_node_t *node, *next;
	
	node = set->head;
	while (node != NULL) {
		next = node->next[0];
		sl_delete_node(node);
		node = next;
	}
	free(set);
}

int sl_map_size(sl_map_t *map)
{
	int size = -1;
	sl_node_t *node;
	
	/* We have at least 2 elements */
	node = map->head->next[0];
	while (node->next[0] != NULL) {
		if (node->fullylinked && !node->marked)
			size++;
		node = node->next[0];
	}
	
	return size;
}
