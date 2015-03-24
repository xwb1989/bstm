/*
 * File:
 *   sl-map.h
 * Author(s):
 *   Vincent Gramoli <vincent.gramoli@epfl.ch>
 *   Wenbin Xiao <github.com/xwb1989>
 * Description:
 *   Skip list integer map operations 
 *
 * Copyright (c) 2009-2010.
 *
 * intset.h is part of Synchrobench
 * 
 * Synchrobench is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, version 2
 * of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef SL_MAP_H
#define SL_MAP_H

#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#include "tm.h"



typedef long sl_key_t;
typedef void* sl_val_t;
#define VAL_MIN                         LONG_MIN
#define VAL_MAX                         LONG_MAX

typedef struct sl_node sl_node_t;
typedef struct sl_map {
    sl_node_t *head;
} sl_map_t;

sl_map_t *sl_map_new();
void sl_map_delete(sl_map_t *map);
unsigned long sl_map_size(sl_map_t *map);

int sl_contains(sl_map_t* map, sl_key_t key);
int sl_insert(sl_map_t* map, sl_key_t key, sl_val_t val);
int sl_remove(sl_map_t* map, sl_key_t key);
sl_val_t sl_find(sl_map_t* map, sl_key_t key);

int TM_sl_contains(TM_ARGDECL sl_map_t* map, sl_key_t key);
int TM_sl_insert(TM_ARGDECL sl_map_t* map, sl_key_t key, sl_val_t val);
int TM_sl_remove(TM_ARGDECL sl_map_t* map, sl_key_t key);
sl_val_t TM_sl_find(TM_ARGDECL sl_map_t* map, sl_key_t key);

#endif
