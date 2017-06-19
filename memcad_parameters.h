#ifndef __MEMCAD_PARAMS_H__
#define __MEMCAD_PARAMS_H__

#include <vector>
#include <iostream>
#include "cacti_interface.h"
#include "const.h"
#include "parameter.h"

using namespace std;

///#define INF 1000000
#define EPS 0.0000001

#define MAX_DIMM_PER_CHANNEL 3
#define MAX_CAP_PER_DIMM 64
#define MAX_RANKS_PER_DIMM 4
#define MIN_BW_PER_CHANNEL 400
#define MAX_DDR3_CHANNEL_BW 800
#define MAX_DDR4_CHANNEL_BW 1600
#define MAX_NUM_CHANNELS_PER_BOB 2
#define MAX_NUM_BOBS 6
#define DIMM_PER_CHANNEL 3

/*
enum Mem_IO_type
{
	DDR3,
	DDR4,
	LPDDR2,
	WideIO,
	Low_Swing_Diff,
	Serial
};

enum Mem_DIMM
{
	UDIMM,
	RDIMM,
	LRDIMM
};
*/



class MemCadParameters
{
	public:

	Mem_IO_type io_type; // DDR3 vs. DDR4

	int capacity; // in GB

	int num_bobs; // default=4me

	///int bw_per_channel; // defaul=1600 MHz;

	///bool with_bob; 

	int num_channels_per_bob; // 1 means no bob

	bool capacity_wise; // true means the load on each channel is proportional to its capacity.

	///int min_bandwith;
	
	MemCad_metrics first_metric;
	
	MemCad_metrics second_metric;
	
	MemCad_metrics third_metric;

	DIMM_Model dimm_model; 
	
	bool low_power_permitted; // Not yet implemented. It determines acceptable VDDs.

	double load; // between 0 to 1

	double row_buffer_hit_rate; 

	double rd_2_wr_ratio;
	
	bool same_bw_in_bob; // true if all the channels in the bob have the same bandwidth.
						 
						  
	bool mirror_in_bob;// true if all the channels in the bob have the same configs
	
	bool total_power; // false means just considering I/O Power
	
	bool verbose;
	
	// Functions
	MemCadParameters(InputParameter * g_ip);
	void print_inputs();
	bool sanity_check();

};


//////////////////////////////////////////////////////////////////////////////////

class MemoryParameters
{
	public:
	// Power Parameteres
	static double VDD[2][2][4];

	static double IDD0[2][4];

	static double IDD1[2][4];

	static double IDD2P0[2][4];

	static double IDD2P1[2][4];

	static double IDD2N[2][4];

	static double IDD3P[2][4];

	static double IDD3N[2][4];

	static double IDD4R[2][4];

	static double IDD4W[2][4];

	static double IDD5[2][4];
	
	static double io_energy_read[2][3][3][4];
	
	static double io_energy_write[2][3][3][4];
	
	// Timing Parameters
	static double T_RAS[2];

	static double T_RC[2];

	static double T_RP[2];

	static double T_RFC[2];

	static double T_REFI[2];
	
	// Bandwidth Parameters
	static int bandwidth_load[2][4];
	
	// Cost Parameters	
	static double cost[2][3][5];
	
	
	// Functions
	MemoryParameters();
	
	int bw_index(Mem_IO_type type, int bandwidth);
};

///////////////////////////////////////////////////////////////////////////

int bw_index(Mem_IO_type type, int bandwidth);


///////////////////////////////////////////////////////////////////////////

class channel_conf
{
	public:
	MemCadParameters *memcad_params;
	
	Mem_DIMM type;
	int num_dimm_per_channel;
	int histogram_capacity[5]; // 0->4GB, 1->8GB, 2->16GB, 3->32GB, 4->64GB
	bool  low_power;
	
	int capacity;
	int bandwidth;
	double energy_per_read;
	double energy_per_write;
	double energy_per_access;
	
	double cost; 
	double latency;
	
	bool valid;
	// Functions
	channel_conf(MemCadParameters * memcad_params, const vector<int>& dimm_cap, int bandwidth, Mem_DIMM type, bool low_power);
	
	void calc_power();
	
	friend channel_conf* clone(channel_conf*);
	friend ostream & operator<<(ostream &os, const channel_conf& ch_cnf);
	
};


///////////////////////////////////////////////////////////////////////////

class bob_conf
{
	public:
	MemCadParameters *memcad_params;
	int num_channels;
	channel_conf *channels[MAX_NUM_CHANNELS_PER_BOB];
	
	int capacity;
	int bandwidth;
	double energy_per_read;
	double energy_per_write;
	double energy_per_access;
	
	double cost;
	double latency;
	
	bool valid;
	
	bob_conf(MemCadParameters * memcad_params, vector<channel_conf*> * channels);
	
	friend bob_conf* clone(bob_conf*);
	friend ostream & operator <<(ostream &os, const bob_conf& bob_cnf);
};

///////////////////////////////////////////////////////////////////////////


class memory_conf
{
	public:
	MemCadParameters *memcad_params;
	int num_bobs;
	bob_conf* bobs[MAX_NUM_BOBS];
	
	int capacity;
	int bandwidth;
	double energy_per_read;
	double energy_per_write;
	double energy_per_access;
	
	double cost;
	double latency;
	
	bool valid;
	
	memory_conf(MemCadParameters * memcad_params, vector<bob_conf*> * bobs);
	friend ostream & operator <<(ostream &os, const memory_conf& bob_cnf);
};






#endif


