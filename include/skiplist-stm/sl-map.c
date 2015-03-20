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
    int deleted;
    int toplevel;
    struct sl_node *next[1];
} sl_node_t;

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


/* 
 * Create a new node without setting its next fields. 
 */
sl_node_t *sl_new_simple_node(sl_key_t key, sl_val_t val, int toplevel)
{
    sl_node_t *node;
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

    node = sl_new_simple_node(key, val, toplevel);

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
    levelmax = MAXLEVEL;
    sl_map_t* map;
    sl_node_t *min, *max;

    if ((map = (sl_map_t *)malloc(sizeof(sl_map_t))) == NULL) {
        perror("malloc");
        exit(1);
    }
    max = sl_new_node(VAL_MAX, (void*)NULL, NULL, levelmax, 0);
    min = sl_new_node(VAL_MIN, (void*)NULL, max, levelmax, 0);
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

unsigned long sl_map_size(sl_map_t* map) {
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
int sl_contains(sl_map_t* map, sl_key_t key, sl_val_t val) {
    int result = 0;


    int i;
    sl_node_t *node, *next;

    node = map->head;
    for (i = node->toplevel-1; i >= 0; i--) {
        next = node->next[i];
        while (next->key < key) {
            node = next;
            next = node->next[i];
        }
    }
    node = node->next[0];
    result = (node->key == key);

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
        node = sl_new_simple_node(key, val, l);
        for (i = 0; i < l; i++) {
            node->next[i] = succs[i];
            preds[i]->next[i] = node;
        }
    }
    return result;
}

int sl_insert(sl_map_t* map, sl_key_t key, sl_val_t val) {
    return sl_seq_add(map, key, val);
}

int sl_remove(sl_map_t* map, sl_key_t key) {
    int result = 0;
    int i;
    sl_node_t *node, *next = NULL;
    sl_node_t *preds[MAXLEVEL], *succs[MAXLEVEL];

    node = map->head;
    for (i = node->toplevel-1; i >= 0; i--) {
        next = node->next[i];
        while (next->key < key) {
            node = next;
            next = node->next[i];
        }
        preds[i] = node;
        succs[i] = node->next[i];
    }
    if ((result = (next->key == key)) == 1) {
        for (i = 0; i < map->head->toplevel; i++) 
            if (succs[i]->key == key)
                preds[i]->next[i] = succs[i]->next[i];
        sl_delete_node(next); 
    }

    return result;
}

sl_val_t sl_find(sl_map_t* map, sl_key_t key) {
    int i;
    sl_node_t *node, *next = NULL;

    node = map->head;
    for (i = node->toplevel-1; i >= 0; i--) {
        next = node->next[i];
        while (next->key < key) {
            node = next;
            next = node->next[i];
        }
    }

    if (next->key == key) {
        return next->val;
    }
    return NULL;
}


sl_node_t* TM_sl_new_simple_node(TM_ARGDECL sl_key_t key, sl_val_t val, int toplevel) {

    sl_node_t *node;

    node = (sl_node_t *)TM_MALLOC(sizeof(sl_node_t) + toplevel * sizeof(sl_node_t *));
    if (node == NULL) {
        perror("malloc");
        exit(1);
    }

    node->val = val;
    node->toplevel = toplevel;
    node->deleted = 0;

    return node;
}

sl_node_t* TMlocate(TM_ARGDECL sl_map_t* map, sl_key_t key, sl_node_t** preds, sl_node_t** succs) {
    int i;
    sl_node_t *node, *next;
    sl_key_t k = VAL_MIN;
    node = map->head;
    for (i = node->toplevel-1; i >= 0; i--) {
        next = (sl_node_t *)TM_SHARED_READ_P(node->next[i]);
        while ((k = next->key) < key) {
            node = next;
            next = (sl_node_t *)TM_SHARED_READ_P(node->next[i]);
        }
        preds[i] = node;
        succs[i] = next;
    }
    return next;
}

int TM_sl_contains(TM_ARGDECL sl_map_t* map, sl_key_t key, sl_val_t val) {

    sl_node_t *next;
    sl_node_t *preds[MAXLEVEL], *succs[MAXLEVEL];

    next = TMlocate(TM_ARG map, key, preds, succs);
    return next->key == key;

}
int TM_sl_insert(TM_ARGDECL sl_map_t* map, sl_key_t key, sl_val_t val) {
    int result = 0;

    int i, l;
    sl_node_t *next, *node;
    sl_node_t *preds[MAXLEVEL], *succs[MAXLEVEL];

    next = TMlocate(TM_ARG map, key, preds, succs);

    if ((result = (next->key != key)) == 1) {
        l = get_rand_level();
        node = TM_sl_new_simple_node(TM_ARG key, val, l);
        for (i = 0; i < l; i++) {
            node->next[i] = (sl_node_t *)TM_SHARED_READ_P(preds[i]->next[i]);	
            TM_SHARED_WRITE_P(preds[i]->next[i], node);
        }
    }

    return result;
}

int TM_sl_remove(TM_ARGDECL sl_map_t* map, sl_key_t key) {

    int result = 0;
    int i;
    sl_node_t *node, *next = NULL;
    sl_node_t *preds[MAXLEVEL], *succs[MAXLEVEL];

    next = TMlocate(TM_ARG map, key, preds, succs);

    if ((result = (next->key == key))) {
        for (i = 0; i < map->head->toplevel; i++) {
            if (succs[i]->key == key) {
                TM_SHARED_WRITE_P(preds[i]->next[i], (sl_node_t *)TM_SHARED_READ_P(succs[i]->next[i])); 
            }
        }
        TM_FREE(next);
    }
    return result;
}

/*
 * Notice: we assume key would not be changed, however val is not. Thus the value
 * returned by this function should be accessed by TM_SHARED_READ
 */
sl_val_t TM_sl_find(TM_ARGDECL sl_map_t* map, sl_key_t key) {
    int result = 0;
    int i;
    sl_node_t *node, *next = NULL;
    sl_node_t *preds[MAXLEVEL], *succs[MAXLEVEL];
    next = TMlocate(TM_ARG map, key, preds, succs);
    if (next->key == key) {
        return (sl_val_t) TM_SHARED_READ(next->val);
    }
    return NULL;
}
