#include "memcad.h"
#include <vector>
#include <list>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;


vector<channel_conf*> *memcad_all_channels;

vector<bob_conf*> *memcad_all_bobs;

vector<memory_conf*> *memcad_all_memories;

vector<memory_conf*> *memcad_best_results;

bool compare_channels(channel_conf* first, channel_conf* second)
{
	if(first->capacity != second->capacity)
		return (first->capacity < second->capacity);
	
	MemCad_metrics first_metric = first->memcad_params->first_metric;
	MemCad_metrics second_metric = first->memcad_params->second_metric;
	MemCad_metrics third_metric = first->memcad_params->third_metric;	
	
	switch(first_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	
	switch(second_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	
	switch(third_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	return true;
}


void prune_channels()
{
	vector<channel_conf*> * temp = new vector<channel_conf*>();
	int last_added = -1;
	for(unsigned int i=0;i< memcad_all_channels->size();i++)
	{
		if(last_added != (*memcad_all_channels)[i]->capacity)
		{
			temp->push_back(clone((*memcad_all_channels)[i]));
			last_added = (*memcad_all_channels)[i]->capacity;
		}
	}
	 
	for(unsigned int i=0;i< memcad_all_channels->size();i++)
	{
		delete (*memcad_all_channels)[i];	
	}
	memcad_all_channels->clear();
	delete memcad_all_channels;
	memcad_all_channels  = temp;
}

void find_all_channels(MemCadParameters * memcad_params)
{
	
	int DIMM_size[]={0,4,8,16,32,64};
	Mem_IO_type current_io_type = memcad_params->io_type;
	DIMM_Model current_dimm_model = memcad_params->dimm_model;
	
	
	memcad_all_channels= new vector<channel_conf*>();
	
	// channels can have up to 3 DIMMs per channel
	// di is the capacity if i-th dimm in the channel
	for(int d1=0; d1<6;d1++)
	{
		for(int d2=d1;d2<6;d2++)
		{
			for(int d3=d2;d3<6;d3++)
			{
				// channel capacity should not exceed the entire memory capacity.
				if((DIMM_size[d1]+DIMM_size[d2]+DIMM_size[d3])>memcad_params->capacity)
					continue;
					
				if( ((current_dimm_model== JUST_LRDIMM) || (current_dimm_model== ALL))
				&&  ((d1==0) || (MemoryParameters::cost[current_io_type][2][d1-1]<INF))
				&&  ((d2==0) || (MemoryParameters::cost[current_io_type][2][d2-1]<INF))
				&&  ((d3==0) || (MemoryParameters::cost[current_io_type][2][d3-1]<INF)) )				
				{
					int num_dimm_per_channel =0;
					vector<int> dimm_cap;
					dimm_cap.push_back(DIMM_size[d1]); if(d1>0) num_dimm_per_channel++;
					dimm_cap.push_back(DIMM_size[d2]); if(d2>0) num_dimm_per_channel++;
					dimm_cap.push_back(DIMM_size[d3]); if(d3>0) num_dimm_per_channel++;
					
					int max_index = bw_index(current_io_type, MemoryParameters::bandwidth_load[current_io_type][4-num_dimm_per_channel]);
					for(int bw_id=0;bw_id<=max_index; ++bw_id)
					{
						int bandwidth = MemoryParameters::bandwidth_load[current_io_type][bw_id];
						channel_conf * new_channel = new channel_conf(memcad_params, dimm_cap, bandwidth, LRDIMM, false);
						if(new_channel->cost <INF)
						{
							memcad_all_channels->push_back(new_channel);
						}
						
						if((DIMM_size[d1]+DIMM_size[d2]+DIMM_size[d3])==0)
							continue;
						
						if(memcad_params->low_power_permitted)
						{
							new_channel = new channel_conf(memcad_params, dimm_cap, bandwidth, LRDIMM, true);
							if(new_channel->cost <INF)
							{
								memcad_all_channels->push_back(new_channel);
							}
						}
						
					}
				}
				
				if( (current_dimm_model== JUST_RDIMM) || (current_dimm_model== ALL)
				&&  ((d1==0) || (MemoryParameters::cost[current_io_type][1][d1-1]<INF))
				&&  ((d2==0) || (MemoryParameters::cost[current_io_type][1][d2-1]<INF))
				&&  ((d3==0) || (MemoryParameters::cost[current_io_type][1][d3-1]<INF)) )				
				{
						
					int num_dimm_per_channel =0;
					vector<int> dimm_cap;
					dimm_cap.push_back(DIMM_size[d1]); if(d1>0) num_dimm_per_channel++;
					dimm_cap.push_back(DIMM_size[d2]); if(d2>0) num_dimm_per_channel++;
					dimm_cap.push_back(DIMM_size[d3]); if(d3>0) num_dimm_per_channel++;
					
					if((DIMM_size[d1]+DIMM_size[d2]+DIMM_size[d3])==0)
						continue;
					
					int max_index = bw_index(current_io_type, MemoryParameters::bandwidth_load[current_io_type][4-num_dimm_per_channel]);
					
					for(int bw_id=0;bw_id<=max_index; ++bw_id)
					{
						int bandwidth = MemoryParameters::bandwidth_load[current_io_type][bw_id];
						channel_conf * new_channel = new channel_conf(memcad_params, dimm_cap, bandwidth, RDIMM, false);
						if(new_channel->cost <INF)
						{
							memcad_all_channels->push_back(new_channel);
						}
						
						if(memcad_params->low_power_permitted)
						{
							new_channel = new channel_conf(memcad_params, dimm_cap, bandwidth, RDIMM, true);
							if(new_channel->cost <INF)
							{
								memcad_all_channels->push_back(new_channel);
							}
						}
					}
				}
				
				if( (current_dimm_model== JUST_UDIMM) || (current_dimm_model== ALL)
				&&  ((d1==0) || (MemoryParameters::cost[current_io_type][0][d1-1]<INF))
				&&  ((d2==0) || (MemoryParameters::cost[current_io_type][0][d2-1]<INF))
				&&  ((d3==0) || (MemoryParameters::cost[current_io_type][0][d3-1]<INF)) )				
				{	
					int num_dimm_per_channel =0;
					vector<int> dimm_cap;
					dimm_cap.push_back(DIMM_size[d1]); if(d1>0) num_dimm_per_channel++;
					dimm_cap.push_back(DIMM_size[d2]); if(d2>0) num_dimm_per_channel++;
					dimm_cap.push_back(DIMM_size[d3]); if(d3>0) num_dimm_per_channel++;
					
					if((DIMM_size[d1]+DIMM_size[d2]+DIMM_size[d3])==0)
						continue;
					int max_index = bw_index(current_io_type, MemoryParameters::bandwidth_load[current_io_type][4-num_dimm_per_channel]);
					for(int bw_id=0;bw_id<=max_index; ++bw_id)
					{
						int bandwidth = MemoryParameters::bandwidth_load[current_io_type][bw_id];	
						channel_conf * new_channel = new channel_conf(memcad_params, dimm_cap, bandwidth, UDIMM, false);
						if(new_channel->cost <INF)
						{
							memcad_all_channels->push_back(new_channel);
						}
						
						if(memcad_params->low_power_permitted)
						{
							new_channel = new channel_conf(memcad_params, dimm_cap, bandwidth, UDIMM, true);
							if(new_channel->cost <INF)
							{
								memcad_all_channels->push_back(new_channel);
							}
						}
					}
				}
								
			}
		}
	}
	
	sort(memcad_all_channels->begin(), memcad_all_channels->end(), compare_channels); 
	
	
	prune_channels();
	
	if(memcad_params->verbose)
	{
		for(unsigned int i=0;i<memcad_all_channels->size();i++)
		{
			cout << *(*memcad_all_channels)[i] << endl;
		}
	}
	
}

bool compare_channels_bw(channel_conf* first, channel_conf* second)
{
	return (first->bandwidth < second->bandwidth);
}

bool compare_bobs(bob_conf* first, bob_conf* second)
{
	if(first->capacity != second->capacity)
		return (first->capacity < second->capacity);
	
	MemCad_metrics first_metric = first->memcad_params->first_metric;
	MemCad_metrics second_metric = first->memcad_params->second_metric;
	MemCad_metrics third_metric = first->memcad_params->third_metric;	
	
	switch(first_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	
	switch(second_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	
	switch(third_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	return true;
}

void prune_bobs()
{
	vector<bob_conf*> * temp = new vector<bob_conf*>();
	int last_added = -1;
	for(unsigned int i=0;i< memcad_all_bobs->size();i++)
	{
		if(last_added != (*memcad_all_bobs)[i]->capacity)
		{
			temp->push_back(clone((*memcad_all_bobs)[i]));
			last_added = (*memcad_all_bobs)[i]->capacity;
		}
	}
	 
	for(unsigned int i=0;i< memcad_all_bobs->size();i++)
	{
		delete (*memcad_all_bobs)[i];	
	}
	memcad_all_bobs->clear();
	delete memcad_all_bobs;
	memcad_all_bobs  = temp;
}

void find_bobs_recursive(MemCadParameters * memcad_params,int start,int end,int nb, list<int> *channel_index)
{
	if(nb==1)
	{
		for(int i=start; i<=end;++i)
		{
			channel_index->push_back(i);
			
			vector<channel_conf*> temp;
			for(list<int>::iterator it= channel_index->begin(); it!= channel_index->end(); it++)
			{
				int idx = *it;
				temp.push_back((*memcad_all_channels)[idx]);
			}
			memcad_all_bobs->push_back(new bob_conf(memcad_params, &temp));
			temp.clear();
			
			channel_index->pop_back();
		}
		return;
	}
	for(int i=start;i<=end;++i)
	{
		channel_index->push_back(i);
		find_bobs_recursive(memcad_params,i,end,nb-1,channel_index);	
		channel_index->pop_back();
	}
}

void find_all_bobs(MemCadParameters * memcad_params)
{
	memcad_all_bobs = new vector<bob_conf*>();
	if(memcad_params->mirror_in_bob)
	{
		for(unsigned int i=0;i<memcad_all_channels->size();++i)
		{
			vector<channel_conf*> channels;
			for(int j=0;j<memcad_params->num_channels_per_bob;j++)
				channels.push_back((*memcad_all_channels)[i]);
			memcad_all_bobs->push_back(new bob_conf(memcad_params, &channels));
			channels.clear();	
		}
	}
	else if(memcad_params->same_bw_in_bob)
	{
		sort(memcad_all_channels->begin(), memcad_all_channels->end(), compare_channels_bw); 
		vector<int> start_index; start_index.push_back(0);
		vector<int> end_index;
		int last_bw =(*memcad_all_channels)[0]->bandwidth;
		for(unsigned int i=0;i< memcad_all_channels->size();i++)
		{
			if(last_bw!=(*memcad_all_channels)[i]->bandwidth)
			{
				end_index.push_back(i-1);
				start_index.push_back(i);
				last_bw = (*memcad_all_channels)[i]->bandwidth;
			}
		}
		end_index.push_back(memcad_all_channels->size()-1);
		
		list<int> channel_index;
		
		for(unsigned int i=0;i< start_index.size();++i)
		{
			find_bobs_recursive(memcad_params,start_index[i],end_index[i],memcad_params->num_channels_per_bob, &channel_index);	
		}
		
	}
	else
	{
		cout << "We do not support different frequencies per in a BoB!" << endl;
		assert(false);
	}
	
	
	sort(memcad_all_bobs->begin(), memcad_all_bobs->end(), compare_bobs); 
	prune_bobs();
	if(memcad_params->verbose)
	{
		for(unsigned int i=0;i<memcad_all_bobs->size();i++)
		{
			cout << *(*memcad_all_bobs)[i] << endl;
		}
	}
}

void find_mems_recursive(MemCadParameters * memcad_params, int remaining_capacity, int start,  int nb, list<int>* bobs_index)
{
	
	if(nb==1)
	{
		for(unsigned int i=start; i< memcad_all_bobs->size();++i)
		{
			if((*memcad_all_bobs)[i]->capacity != remaining_capacity)
				continue;
			
			bobs_index->push_back(i);
			vector<bob_conf*> temp;
			for(list<int>::iterator it= bobs_index->begin(); it!= bobs_index->end(); it++)
			{
				int index = *it;
				temp.push_back((*memcad_all_bobs)[index]);
			}
			memcad_all_memories->push_back(new memory_conf(memcad_params, &temp));
			temp.clear();
			bobs_index->pop_back();
		}
		return;
	}
	
	for(unsigned int i=start; i<memcad_all_bobs->size();i++)
	{
		if((*memcad_all_bobs)[i]->capacity > remaining_capacity)
			continue;
		
		int new_remaining_capacity = remaining_capacity-(*memcad_all_bobs)[i]->capacity;
		bobs_index->push_back(i);
		find_mems_recursive(memcad_params, new_remaining_capacity, i, nb-1, bobs_index);
		bobs_index->pop_back();
	}
}

//void find_mems_recursive(MemCadParameters * memcad_params, int start, int 

bool compare_memories(memory_conf* first, memory_conf* second)
{
	if(first->capacity != second->capacity)
		return (first->capacity < second->capacity);
	
	MemCad_metrics first_metric = first->memcad_params->first_metric;
	MemCad_metrics second_metric = first->memcad_params->second_metric;
	MemCad_metrics third_metric = first->memcad_params->third_metric;	
	
	switch(first_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	
	switch(second_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	
	switch(third_metric)
	{
		case(Cost):
			if(first->cost != second->cost)
				return (first->cost < second->cost);
			break;
		case(Bandwidth):
			if(first->bandwidth != second->bandwidth)
				return (first->bandwidth > second->bandwidth);
			break;
		case(Energy):
			if( fabs(first->energy_per_access -  second->energy_per_access)>EPS)
				return (first->energy_per_access <  second->energy_per_access);
			break;
		default:
			assert(false);
	}
	return true;
}

bool find_all_memories(MemCadParameters * memcad_params)
{
	memcad_all_memories = new vector<memory_conf*>();
	
	list<int> bobs_index;
	find_mems_recursive(memcad_params, memcad_params->capacity, 0,memcad_params->num_bobs, &bobs_index);	
	
	sort(memcad_all_memories->begin(), memcad_all_memories->end(), compare_memories);
	
	if(memcad_params->verbose)
	{
		cout << "all possible results:" << endl;
		for(unsigned int i=0;i<memcad_all_memories->size();i++)
		{
			cout << *(*memcad_all_memories)[i] << endl;
		}
	}
	if(memcad_all_memories->size()==0)
	{
		cout << "No result found " << endl;
		return false;
	}
	cout << "top 3 best memory configurations are:" << endl;
	int min_num_results = (memcad_all_memories->size()>3?3:memcad_all_memories->size());
	for(int i=0;i<min_num_results;++i)
	{
		if((*memcad_all_memories)[i])
			cout << *(*memcad_all_memories)[i] << endl;
	}
	return true;
}

void clean_results()
{
	for(unsigned int i=0;i<memcad_all_channels->size();++i)
	{
		delete (*memcad_all_channels)[i];
	}
	delete memcad_all_channels;
	
	for(unsigned int i=0;i<memcad_all_bobs->size();++i)
	{
		delete (*memcad_all_bobs)[i];
	}
	delete memcad_all_bobs;
	
	for(unsigned int i=0;i<memcad_all_memories->size();++i)
	{
		delete (*memcad_all_memories)[i];
	}
	delete memcad_all_memories;
}


void solve_memcad(MemCadParameters * memcad_params)
{
	
	find_all_channels(memcad_params);
	find_all_bobs(memcad_params);
	find_all_memories(memcad_params);
	clean_results();
}

