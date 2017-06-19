#include "memcad_parameters.h"
#include <cmath>
#include <cassert>

MemCadParameters::MemCadParameters(InputParameter * g_ip)
{
	// default value
	io_type=DDR4; // DDR3 vs. DDR4
	capacity=400; // in GB
	num_bobs=4; // default=4me
	num_channels_per_bob=2; // 1 means no bob
	capacity_wise=true; // true means the load on each channel is proportional to its capacity.
	first_metric=Cost;
	second_metric=Bandwidth;
	third_metric=Energy;
	dimm_model=ALL; 
	low_power_permitted=false;
	load=0.9; // between 0 to 1
	row_buffer_hit_rate=1; 
	rd_2_wr_ratio=2;
	same_bw_in_bob=true; // true if all the channels in the bob have the same bandwidth
	mirror_in_bob=true;// true if all the channels in the bob have the same configs
	total_power=false; // false means just considering I/O Power. 
	verbose=false;
	// values for input
	io_type=g_ip->io_type; 
	capacity=g_ip->capacity; 
	num_bobs=g_ip->num_bobs; 
	num_channels_per_bob=g_ip->num_channels_per_bob; 
	first_metric=g_ip->first_metric;
	second_metric=g_ip->second_metric;
	third_metric=g_ip->third_metric;
	dimm_model=g_ip->dimm_model; 
	///low_power_permitted=g_ip->low_power_permitted;
	///load=g_ip->load; 
	///row_buffer_hit_rate=g_ip->row_buffer_hit_rate; 
	///rd_2_wr_ratio=g_ip->rd_2_wr_ratio;
	///same_bw_in_bob=g_ip->same_bw_in_bob; 
	mirror_in_bob=g_ip->mirror_in_bob;
	///total_power=g_ip->total_power; 
	verbose=g_ip->verbose;
	
}
	
void MemCadParameters::print_inputs()
{
	
}

bool MemCadParameters::sanity_check()
{
	
	return true;
}


double MemoryParameters::VDD[2][2][4]= //[lp:hp][ddr3:ddr4][frequency index]
{
	{
		{1.5,1.5,1.5,1.5},
		{1.2,1.2,1.2,1.2}
	},
	{
		{1.35,1.35,1.35,1.35},
		{1.0,1.0,1.0,1.0}
	}
};

double MemoryParameters::IDD0[2][4]=
{
	{55,60,65,75},
	{58,58,60,64}
};

double MemoryParameters::IDD2P0[2][4]=
{
	{20,20,20,20},
	{20,20,20,20}
};

double MemoryParameters::IDD2P1[2][4]=
{
	{30,30,32,37},
	{30,30,30,32}
};

double MemoryParameters::IDD2N[2][4]=
{
	{40,42,45,50},
	{44,44,46,50}
};

double MemoryParameters::IDD3P[2][4]=
{
	{45,50,55,60},
	{44,44,44,44}
};

double MemoryParameters::IDD3N[2][4]=
{
	{42,47,52,57},
	{44,44,44,44}
};

double MemoryParameters::IDD4R[2][4]=
{
	{120,135,155,175},
	{140,140,150,160}
};

double MemoryParameters::IDD4W[2][4]=
{
	{100,125,145,165},
	{156,156,176,196}
};

double MemoryParameters::IDD5[2][4]=
{
	{150,205,210,220},
	{190,190,190,192}
};

double MemoryParameters::io_energy_read[2][3][3][4] =// [ddr3:ddr4][udimm:rdimm:lrdimm][load 1:2:3][frequency 0:1:2:3]
{
	{ //ddr3
		{//udimm
			{2592.33,	2593.33,	3288.784,	4348.612},
			{2638.23,	2640.23,	3941.584,	5415.492},
			{2978.659,	2981.659,	4816.644,	6964.162}

		},
		{//rdimm
			{2592.33,	3087.071,	3865.044,	4844.982},
			{2932.759,	3733.318,	4237.634,	5415.492},
			{3572.509,	4603.109,	5300.004,	6964.162}
		},
		{//lrdimm
			{4628.966,	6357.625,	7079.348,	9680.454},
			{5368.26,	6418.788,	7428.058,	10057.164},
			{5708.689,	7065.038,	7808.678,	10627.674}

		}
		
	},
	{ //ddr
		{//udimm
			{2135.906,	2633.317,	2750.919,	2869.406},
			{2458.714,	2695.791,	2822.298,	3211.111},
			{2622.85,	3030.048,	3160.265,	3534.448}

		},
		{//rdimm
			{2135.906,	2633.317,	2750.919,	2869.406},
			{2458.714,	2695.791,	3088.886,	3211.111},
			{2622.85,	3030.048,	3312.468,	3758.445}

		},
		{//lrdimm
			{4226.903,	5015.342,	5490.61,	5979.864},
			{4280.471,	5319.132,	5668.945,	6060.216},
			{4603.279,	5381.605,	5740.325,	6401.926}

		}
		
	}
};

double MemoryParameters::io_energy_write[2][3][3][4] =
{
	{ //ddr3
		{//udimm
			{2758.951,	2984.854,	3571.804,	4838.902},
			{2804.851,	3768.524,	4352.214,	5580.362},
			{3213.897,	3829.684,	5425.854,	6933.512}

		},
		{//rdimm
			{2758.951,	3346.104,	3931.154,	4838.902},
			{3167.997,	4114.754,	4696.724,	5580.362},
			{3561.831,	3829.684,	6039.994,	8075.542}

		},
		{//lrdimm
			{4872.238,	5374.314,	7013.868,	9267.574},
			{5701.502,	6214.348,	7449.758,	10045.004},
			{5747.402,	6998.018,	8230.168,	10786.464}

		}
		
	},
	{ //ddr4
		{//udimm
			{2525.129,	2840.853,	2979.037,	3293.608},
			{2933.756,	3080.126,	3226.497,	3979.698},
			{3293.964,	3753.37,	3906.137,	4312.448}

		},
		{//rdimm
			{2525.129,	2840.853,	3155.117,	3293.608},
			{2933.756,	3080.126,	3834.757,	3979.698},
			{3293.964,	3753.37,	4413.037,	5358.078}

		},
		{//lrdimm
			{4816.453,	5692.314,	5996.134,	6652.936},
			{4870.021,	5754.788,	6067.514,	6908.636},
			{5298.373,	5994.07,	6491.054,	7594.726}

		}
		
	}
};

double MemoryParameters::T_RAS[2] = {35,35};

double MemoryParameters::T_RC[2] = {47.5,47.5};

double MemoryParameters::T_RP[2] = {13,13};

double MemoryParameters::T_RFC[2] = {340,260};

double MemoryParameters::T_REFI[2] = {7800,7800};

int MemoryParameters::bandwidth_load[2][4]={{400,533,667,800},{800,933,1066,1200}};

double MemoryParameters::cost[2][3][5] =
{
	{
		{40.38,76.13,INF,INF,INF},
		{42.24,64.17,122.6,304.3,INF},
		{INF,INF,211.3,287.5,1079.5}
	},
	{
		{25.99,45.99,INF,INF,INF},
		{32.99,60.45,126,296.3,INF},
		{INF,INF,278.99,333,1474}
	}
};



///////////////////////////////////////////////////////////////////////////////////

double calculate_power(double load, double row_buffer_hr, double rd_wr_ratio, int chips_per_rank, int frequency_index, int lp)
{
	return 0;
}

int bw_index(Mem_IO_type type, int bandwidth)
{
	if(type==DDR3)
	{
		if(bandwidth<=400)
			return 0;
		else if(bandwidth <= 533)
			return 1;
		else if(bandwidth <= 667)
			return 2;
		else
			return 3;
	}
	else
	{
		if(bandwidth<=800)
			return 0;
		else if(bandwidth <= 933)
			return 1;
		else if(bandwidth <= 1066)
			return 2;
		else
			return 3;
	}
	return 0;
} 

channel_conf::channel_conf(MemCadParameters * memcad_params, const vector<int>& dimm_cap, int bandwidth, Mem_DIMM type, bool low_power)
:memcad_params(memcad_params),type(type),low_power(low_power),bandwidth(bandwidth),latency(0),valid(true)
{
	//assert(memcad_params);
	assert(dimm_cap.size() <=DIMM_PER_CHANNEL);
	assert(memcad_params->io_type<2); // So far, we just support DDR3 and DDR4.
	// upading capacity
	num_dimm_per_channel=0;
	capacity =0;
	for(int i=0;i<5;i++) histogram_capacity[i]=0;
	for(unsigned int i=0;i<dimm_cap.size();i++)
	{
		if(dimm_cap[i]==0)
			continue;
			
		int index =(int)(log2(dimm_cap[i]+0.1))-2;
		assert(index<5);
		histogram_capacity[index]++;
		num_dimm_per_channel++;
		capacity += dimm_cap[i];
	}
	
	// updating bandwidth
	if(capacity>0)
		bandwidth =0;
	
		//bandwidth = MemoryParameters::bandwidth_load[memcad_params->io_type][4-num_dimm_per_channel];
	// updating channel cost
	cost =0;
	for(int i=0;i<5;++i)
		cost += histogram_capacity[i] * MemoryParameters::cost[memcad_params->io_type][type][i];
	
	// update energy
	calc_power();
	
}

void channel_conf::calc_power()
{
	
	double read_ratio = memcad_params->rd_2_wr_ratio/(1.0+memcad_params->rd_2_wr_ratio);
	double write_ratio = 1.0/(1.0+memcad_params->rd_2_wr_ratio);
	Mem_IO_type current_io_type = memcad_params->io_type;
	double capacity_ratio = (capacity/(double) memcad_params->capacity );
	
	double T_BURST = 4; // memory cycles
	
	energy_per_read = MemoryParameters::io_energy_read[current_io_type][type][num_dimm_per_channel-1][bw_index(current_io_type,bandwidth)];
	energy_per_read /= (bandwidth/T_BURST);
	
	energy_per_write = MemoryParameters::io_energy_write[current_io_type][type][num_dimm_per_channel-1][bw_index(current_io_type,bandwidth)];
	energy_per_write /= (bandwidth/T_BURST);
	if(memcad_params->capacity_wise)
	{
		energy_per_read  *= capacity_ratio; 
		energy_per_write *= capacity_ratio;			  
	}

	energy_per_access = read_ratio* energy_per_read + write_ratio*energy_per_write;
	
}

channel_conf* clone(channel_conf* origin)
{
	vector<int> temp;
	int size =4;
	for(int i=0;i<5;++i)
	{
		for(int j=0;j<origin->histogram_capacity[i];++j)
		{
			temp.push_back(size);
		}
		size *=2;
	}
	channel_conf * new_channel = new channel_conf(origin->memcad_params,temp,origin->bandwidth, origin->type,origin->low_power);
	return new_channel;
}

ostream& operator<<(ostream &os, const channel_conf& ch_cnf) 
{
	os << "cap: " << ch_cnf.capacity << " GB ";
	os << "bw: " << ch_cnf.bandwidth << " (MHz) ";
	os << "cost: $" << ch_cnf.cost << " ";
	os << "dpc: "  << ch_cnf.num_dimm_per_channel << " ";
	os << "energy: " << ch_cnf.energy_per_access << " (nJ) ";
	os << " DIMM: " << ((ch_cnf.type==UDIMM)?" UDIMM ":((ch_cnf.type==RDIMM)?" RDIMM ":"LRDIMM "));
	os << " low power: " << ((ch_cnf.low_power)? "T ":"F ");
	os << "[ ";
	for(int i=0;i<5;i++)
		os <<  ch_cnf.histogram_capacity[i] << "(" << (1<<(i+2)) << "GB) ";
    os << "]";
    return os;
}


bob_conf::bob_conf(MemCadParameters * memcad_params, vector<channel_conf*> * in_channels)
:memcad_params(memcad_params),num_channels(0),capacity(0),bandwidth(0)
,energy_per_read(0),energy_per_write(0),energy_per_access(0),cost(0),latency(0),valid(true)
{
	
	assert(in_channels->size() <= MAX_NUM_CHANNELS_PER_BOB);
	for(int i=0;i<MAX_NUM_CHANNELS_PER_BOB;i++)
		channels[i]=0;
	
	for(unsigned int i=0;i< in_channels->size();++i)
	{
		channels[i] = (*in_channels)[i];
		num_channels++;
		capacity += (*in_channels)[i]->capacity;
		cost += (*in_channels)[i]->cost;
		bandwidth += (*in_channels)[i]->bandwidth;
		energy_per_read += (*in_channels)[i]->energy_per_read;
		energy_per_write += (*in_channels)[i]->energy_per_write;
		energy_per_access += (*in_channels)[i]->energy_per_access;
	}
}

bob_conf* clone(bob_conf* origin)
{
	vector<channel_conf*> temp;
	for(int i=0;i<MAX_NUM_CHANNELS_PER_BOB;++i)
	{
		if( (origin->channels)[i]==0 )
			break;
		temp.push_back(	(origin->channels)[i] );
	}
	
	bob_conf * new_bob = new bob_conf(origin->memcad_params,&temp);
	return new_bob;
}

ostream & operator <<(ostream &os, const bob_conf& bob_cnf)
{
	os << " " << "BoB       " ;
	os << "cap: " << bob_cnf.capacity << " GB ";
	os << "num_channels: " << bob_cnf.num_channels << " ";
	os << "bw: " << bob_cnf.bandwidth << " (MHz) ";
	os << "cost: $" << bob_cnf.cost << " ";
	os << "energy: " << bob_cnf.energy_per_access << " (nJ) ";
	os << endl;
	os << "   " << " ==============" << endl;
	for(int i=0;i<bob_cnf.num_channels;i++)
	{
		channel_conf * temp = bob_cnf.channels[i];
		os << "    (" << i << ") " << (*temp) << endl ;
	}
    os << "   " << " =============="<< endl;
    return os;
}


memory_conf::memory_conf(MemCadParameters * memcad_params, vector<bob_conf*> * in_bobs)
:memcad_params(memcad_params),num_bobs(0),capacity(0),bandwidth(0)
,energy_per_read(0),energy_per_write(0),energy_per_access(0),cost(0),latency(0),valid(true)
{
	assert(in_bobs->size() <= MAX_NUM_BOBS);
	for(int i=0;i<MAX_NUM_BOBS;i++)
		bobs[i]=0;
	
	for(unsigned int i=0;i< in_bobs->size();++i)
	{
		bobs[i] = (*in_bobs)[i];
		num_bobs++;
		capacity += (*in_bobs)[i]->capacity;
		cost += (*in_bobs)[i]->cost;
		bandwidth += (*in_bobs)[i]->bandwidth;
		energy_per_read += (*in_bobs)[i]->energy_per_read;
		energy_per_write += (*in_bobs)[i]->energy_per_write;
		energy_per_access += (*in_bobs)[i]->energy_per_access;
	}
}

ostream & operator <<(ostream &os, const memory_conf& mem_cnf)
{
	os << "Memory    " ;
	os << "cap: " << mem_cnf.capacity << " GB ";
	os << "num_bobs: " << mem_cnf.num_bobs << " ";
	os << "bw: " << mem_cnf.bandwidth << " (MHz) ";
	os << "cost: $" << mem_cnf.cost << " ";
	os << "energy: " << mem_cnf.energy_per_access << " (nJ) ";
	os << endl;
	os << " {" << endl;
	for(int i=0;i<mem_cnf.num_bobs;i++)
	{
		bob_conf * temp = mem_cnf.bobs[i];
		os<< " (" << i <<") " <<(*temp) << endl ;
	}
    os << " }"<< endl;
    return os;
}

