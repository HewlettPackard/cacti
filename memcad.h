#ifndef __MEMCAD_H__
#define __MEMCAD_H__

#include "memcad_parameters.h"
#include <vector>


extern vector<channel_conf*> *memcad_all_channels;

extern vector<bob_conf*> *memcad_all_bobs;

extern vector<memory_conf*> *memcad_all_memories;

extern vector<memory_conf*> *memcad_best_results;



void find_all_channels(MemCadParameters * memcad_params);

void find_all_bobs(MemCadParameters * memcad_params);

bool find_all_memories(MemCadParameters * memcad_params);

void clean_results();

void solve_memcad(MemCadParameters * memcad_params);

#endif


