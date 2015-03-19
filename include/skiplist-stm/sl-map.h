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


#include <assert.h>
#include <getopt.h>
#include <limits.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>



typedef long sl_key_t;
typedef void* sl_val_t;
#define VAL_MIN                         LONG_MIN
#define VAL_MAX                         LONG_MAX

typedef struct sl_map sl_map_t;

int get_rand_level();
int floor_log_2(unsigned int n);

//sl_node_t *sl_new_simple_node(sl_key_t key, sl_val_t val, int toplevel, int transactional);
//sl_node_t *sl_new_node(sl_key_t key, sl_val_t val, sl_node_t *next, int toplevel, int transactional);
//void sl_delete_node(sl_node_t *n);

sl_map_t *sl_map_new();
void sl_map_delete(sl_map_t *map);
unsigned long sl_map_size(sl_map_t *map);

int sl_contains(sl_map_t* map, sl_key_t key, sl_val_t val, int transactional);
int sl_insert(sl_map_t* map, sl_key_t key, sl_val_t val, int transactional);
int sl_remove(sl_map_t* map, sl_key_t key, int transactional);
