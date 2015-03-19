/*
 * File:
 *   sl-map.c
 * Author(s):
 *   Vincent Gramoli <vincent.gramoli@epfl.ch>
 *   Wenbin Xiao <github.com/xwb1989>
 * Description:
 *   Skip list integer map operations 
 *
 */

#include "sl-map.h"

#define MAXLEVEL   64 

unsigned int levelmax;


typedef struct sl_node {
    sl_key_t key;
    sl_val_t val;
      intptr_t deleted;
    int toplevel;
    struct sl_node *next[1];
} sl_node_t;

typedef struct sl_map {
    sl_node_t *head;
} sl_map_t;

/* 
 * Returns a pseudo-random value in [1;range).
 * Depending on the symbolic constant RAND_MAX>=32767 defined in stdlib.h,
 * the granularity of rand() could be lower-bounded by the 32767^th which might 
 * be too high for given values of range and initial.
 */
inline long rand_range(long r) {
	int m = RAND_MAX;
	int d, v = 0;
	
	do {
		d = (m > r ? r : m);		
		v += 1 + (int)(d * ((double)rand()/((double)(m)+1.0)));
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

  if (transactional)
    node = (sl_node_t *)MALLOC(sizeof(sl_node_t) + toplevel * sizeof(sl_node_t *));
  else 
    node = (sl_node_t *)malloc(sizeof(sl_node_t) + toplevel * sizeof(sl_node_t *));
  if (node == NULL) {
    perror("malloc");
    exit(1);
    }

  node->val = val;
  node->toplevel = toplevel;
  node->deleted = 0;

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

  for (i = 0; i < levelmax; i++)
    node->next[i] = next;
	
  return node;
}

void sl_delete_node(sl_node_t *n)
{
  free(n);
}

sl_map_t *sl_map_new()
{
  sl_map_t* map;
  sl_node_t *min, *max;
	
  if ((map = (sl_map_t *)malloc(sizeof(sl_map_t))) == NULL) {
    perror("malloc");
    exit(1);
  }
  max = sl_new_node(VAL_MAX, (void*)VAL_MAX, NULL, levelmax, 0);
  min = sl_new_node(VAL_MIN, (void*)VAL_MIN, max, levelmax, 0);
  map->head = min;
  return map;
}

void sl_map_delete(sl_map_t* map)
{
  sl_node_t *node, *next;

  node = map->head;
  while (node != NULL) {
    next = node->next[0];
    sl_delete_node(node);
    node = next;
  }
  free(map);
}

unsigned long sl_map_size(sl_map_t* map)
{
  unsigned long size = 0;
  sl_node_t *node;

  node = map->head->next[0];
  while (node->next[0] != NULL) {
    if (!node->deleted)
      size++;
    node = node->next[0];
  }

  return size;
}
int sl_contains(sl_map_t* map, sl_key_t key, sl_val_t val, int transactional)
{
	int result = 0;
	
#ifdef SEQUENTIAL /* Unprotected */
	
	int i;
	sl_node_t *node, *next;
	
	node = set->head;
	for (i = node->toplevel-1; i >= 0; i--) {
		next = node->next[i];
		while (next->val < val) {
			node = next;
			next = node->next[i];
		}
	}
	node = node->next[0];
	result = (node->val == val);
		
#elif defined STM
	
	int i;
	sl_node_t *node, *next;
	val_t v = VAL_MIN;

	if (transactional > 1) {
	
	  TX_START(EL);
	  node = set->head;
	  for (i = node->toplevel-1; i >= 0; i--) {
	    next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    while ((v = TX_LOAD(&next->val)) < val) {
	      node = next;
	      next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    }
	  }
	  node = (sl_node_t *)TX_LOAD(&node->next[0]);
	  result = (v == val);
	  TX_END;

	} else {

	  TX_START(NL);
	  node = set->head;
	  for (i = node->toplevel-1; i >= 0; i--) {
	    next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    while ((v = TX_LOAD(&next->val)) < val) {
	      node = next;
	      next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    }
	  }
	  node = (sl_node_t *)TX_LOAD(&node->next[0]);
	  result = (v == val);
	  TX_END;

	}
	
#endif
	
	return result;
}

inline int sl_seq_add(sl_map_t* map, sl_key_t key, sl_val_t val) {
	int i, l, result;
	sl_node_t *node, *next;
	sl_node_t *preds[MAXLEVEL], *succs[MAXLEVEL];
	
	node = map->head;
	for (i = node->toplevel-1; i >= 0; i--) {
		next = node->next[i];
		while (next->val < val) {
			node = next;
			next = node->next[i];
		}
		preds[i] = node;
		succs[i] = node->next[i];
	}
	node = node->next[0];
	if ((result = (node->val != val)) == 1) {
		l = get_rand_level();
		node = sl_new_simple_node(key, val, l, 0);
		for (i = 0; i < l; i++) {
			node->next[i] = succs[i];
			preds[i]->next[i] = node;
		}
	}
	return result;
}

int sl_add(sl_map_t* map, sl_key_t key, sl_val_t val, int transactional)
{
  int result = 0;
	
  if (!transactional) {
		
    result = sl_seq_add(map, key, val);
	
  } else {

#ifdef SEQUENTIAL
		
	result = sl_seq_add(set, val);
		
#elif defined STM
	
	int i, l;
	sl_node_t *node, *next;
	sl_node_t *preds[MAXLEVEL];
	val_t v;  
	
	if (transactional > 2) {

	  TX_START(EL);
	  v = VAL_MIN;
	  node = set->head;
	  for (i = node->toplevel-1; i >= 0; i--) {
	    next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    while ((v = TX_LOAD(&next->val)) < val) {
	      node = next;
	      next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    }
	    preds[i] = node;
	  }
	  if ((result = (v != val)) == 1) {
	    l = get_rand_level();
	    node = sl_new_simple_node(val, l, transactional);
	    for (i = 0; i < l; i++) {
	      node->next[i] = (sl_node_t *)TX_LOAD(&preds[i]->next[i]);	
	      TX_STORE(&preds[i]->next[i], node);
	    }
	  }
	  TX_END;

	} else {

	  TX_START(NL);
	  v = VAL_MIN;
	  node = set->head;
	  for (i = node->toplevel-1; i >= 0; i--) {
	    next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    while ((v = TX_LOAD(&next->val)) < val) {
	      node = next;
	      next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    }
	    preds[i] = node;
	  }
	  if ((result = (v != val)) == 1) {
	    l = get_rand_level();
	    node = sl_new_simple_node(val, l, transactional);
	    for (i = 0; i < l; i++) {
	      node->next[i] = (sl_node_t *)TX_LOAD(&preds[i]->next[i]);	
	      TX_STORE(&preds[i]->next[i], node);
	    }
	  }
	  TX_END;
	
	}
	
#endif
		
  }
	
  return result;
}

int sl_remove(sl_map_t* map, sl_key_t key, int transactional)
{
	int result = 0;
	
#ifdef SEQUENTIAL
	
	int i;
	sl_node_t *node, *next = NULL;
	sl_node_t *preds[MAXLEVEL], *succs[MAXLEVEL];
	
	node = set->head;
	for (i = node->toplevel-1; i >= 0; i--) {
		next = node->next[i];
		while (next->val < val) {
			node = next;
			next = node->next[i];
		}
		preds[i] = node;
		succs[i] = node->next[i];
	}
	if ((result = (next->val == val)) == 1) {
		for (i = 0; i < set->head->toplevel; i++) 
			if (succs[i]->val == val)
				preds[i]->next[i] = succs[i]->next[i];
		sl_delete_node(next); 
	}

#elif defined STM
	
	int i;
	sl_node_t *node, *next = NULL;
	sl_node_t *preds[MAXLEVEL], *succs[MAXLEVEL];
	val_t v;  
	
	if (transactional > 3) {

	  TX_START(EL);
	  v = VAL_MIN;
	  node = set->head;
	  for (i = node->toplevel-1; i >= 0; i--) {
	    next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    while ((v = TX_LOAD(&next->val)) < val) {
	      node = next;
	      next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    }
	    preds[i] = node;
	    succs[i] = next;
	  }
	  if ((result = (next->val == val))) {
	    for (i = 0; i < set->head->toplevel; i++) {
	      if (succs[i]->val == val) {
		TX_STORE(&preds[i]->next[i], (sl_node_t *)TX_LOAD(&succs[i]->next[i])); 
	      }
	    }
	    FREE(next, sizeof(sl_node_t) + next->toplevel * sizeof(sl_node_t *));
	  }
	  TX_END;

	} else {

	  TX_START(NL);
	  v = VAL_MIN;
	  node = set->head;
	  for (i = node->toplevel-1; i >= 0; i--) {
	    next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    while ((v = TX_LOAD(&next->val)) < val) {
	      node = next;
	      next = (sl_node_t *)TX_LOAD(&node->next[i]);
	    }
	    preds[i] = node;
	    succs[i] = next;
	  }
	  if ((result = (next->val == val))) {
	    for (i = 0; i < set->head->toplevel; i++) {
	      if (succs[i]->val == val) {
		TX_STORE(&preds[i]->next[i], (sl_node_t *)TX_LOAD(&succs[i]->next[i])); 
	      }
	    }
	    FREE(next, sizeof(sl_node_t) + next->toplevel * sizeof(sl_node_t *));
	  }
	  TX_END;

	}
	
#endif
	
	return result;
}


