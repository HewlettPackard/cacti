/*****************************************************************************
 *                                CACTI 7.0
 *                      SOFTWARE LICENSE AGREEMENT
 *            Copyright 2015 Hewlett-Packard Development Company, L.P.
 *                          All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.”
 *
 ***************************************************************************/



#ifndef __CACTI_INTERFACE_H__
#define __CACTI_INTERFACE_H__

#include <map>
#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "const.h"

using namespace std;


class min_values_t;
class mem_array;
class uca_org_t;


class powerComponents
{
  public:
    double dynamic;
    double leakage;
    double gate_leakage;
    double short_circuit;
    double longer_channel_leakage;

    powerComponents() : dynamic(0), leakage(0), gate_leakage(0), short_circuit(0), longer_channel_leakage(0)  { }
    powerComponents(const powerComponents & obj) { *this = obj; }
    powerComponents & operator=(const powerComponents & rhs)
    {
      dynamic = rhs.dynamic;
      leakage = rhs.leakage;
      gate_leakage  = rhs.gate_leakage;
      short_circuit = rhs.short_circuit;
      longer_channel_leakage = rhs.longer_channel_leakage;
      return *this;
    }
    void reset() { dynamic = 0; leakage = 0; gate_leakage = 0; short_circuit = 0;longer_channel_leakage = 0;}

    friend powerComponents operator+(const powerComponents & x, const powerComponents & y);
    friend powerComponents operator*(const powerComponents & x, double const * const y);
};



class powerDef
{
  public:
    powerComponents readOp;
    powerComponents writeOp;
    powerComponents searchOp;//: for CAM and FA

    powerDef() : readOp(), writeOp(), searchOp() { }
    void reset() { readOp.reset(); writeOp.reset(); searchOp.reset();}

    friend powerDef operator+(const powerDef & x, const powerDef & y);
    friend powerDef operator*(const powerDef & x, double const * const y);
};

enum Wire_type
{
    Global /* gloabl wires with repeaters */,
    Global_5 /* 5% delay penalty */,
    Global_10 /* 10% delay penalty */,
    Global_20 /* 20% delay penalty */,
    Global_30 /* 30% delay penalty */,
    Low_swing /* differential low power wires with high area overhead */,
    Semi_global /* mid-level wires with repeaters*/,
    Full_swing /* models all global wires with different latencies (Global_x    )*/,
    Transmission /* tranmission lines with high area overhead */,
    Optical /* optical wires */,
    Invalid_wtype
};

enum TSV_type
{
	Fine, /*ITRS high density*/
	Coarse /*Industry reported in 2010*/
};

// ali

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

enum Mem_state
{
	READ,
	WRITE,
	IDLE,
	SLEEP
};

enum Mem_ECC
{
	NO_ECC,
	SECDED, // single error correction, double error detection
	CHIP_KILL
};

enum DIMM_Model
{
	JUST_UDIMM,JUST_RDIMM,JUST_LRDIMM,ALL
};

enum MemCad_metrics 
{
	Bandwidth, Energy, Cost
};

/**
enum BoB_LINK
{
	PARALLEL, // e.g. Intel SMB c104 
	SERIAL // e.g. Intel SMB 7510, IBM AMB
};
**/
// end ali


class InputParameter
{
  public:

    InputParameter();
    void parse_cfg(const string & infile);

    bool error_checking();  // return false if the input parameters are problematic
    void display_ip();

    unsigned int cache_sz;  // in bytes
    unsigned int line_sz;
    unsigned int assoc;
    unsigned int nbanks;
    unsigned int out_w;// == nr_bits_out
    bool     specific_tag;
    unsigned int tag_w;
    unsigned int access_mode;
    unsigned int obj_func_dyn_energy;
    unsigned int obj_func_dyn_power;
    unsigned int obj_func_leak_power;
    unsigned int obj_func_cycle_t;

    double   F_sz_nm;          // feature size in nm
    double   F_sz_um;          // feature size in um
    unsigned int num_rw_ports;
    unsigned int num_rd_ports;
    unsigned int num_wr_ports;
    unsigned int num_se_rd_ports;  // number of single ended read ports
    unsigned int num_search_ports;  // : number of search ports for CAM
    bool     is_main_mem;
    bool 	 is_3d_mem;
    bool	 print_detail_debug;
    bool     is_cache;
    bool     pure_ram;
    bool     pure_cam;
    bool     rpters_in_htree;  // if there are repeaters in htree segment
    unsigned int ver_htree_wires_over_array;
    unsigned int broadcast_addr_din_over_ver_htrees;
    unsigned int temp;

    unsigned int ram_cell_tech_type;
    unsigned int peri_global_tech_type;
    unsigned int data_arr_ram_cell_tech_type;
    unsigned int data_arr_peri_global_tech_type;
    unsigned int tag_arr_ram_cell_tech_type;
    unsigned int tag_arr_peri_global_tech_type;

    unsigned int burst_len;
    unsigned int int_prefetch_w;
    unsigned int page_sz_bits;

    unsigned int num_die_3d;
    unsigned int burst_depth;
    unsigned int io_width;
    unsigned int sys_freq_MHz;

    unsigned int tsv_is_subarray_type;
    unsigned int tsv_os_bank_type;
    unsigned int TSV_proj_type;

    int partition_gran;
    unsigned int num_tier_row_sprd;
    unsigned int num_tier_col_sprd;
    bool fine_gran_bank_lvl;

    unsigned int ic_proj_type;      // interconnect_projection_type
    unsigned int wire_is_mat_type;  // wire_inside_mat_type
    unsigned int wire_os_mat_type; // wire_outside_mat_type
    enum Wire_type wt;
    int force_wiretype;
    bool print_input_args;
    unsigned int nuca_cache_sz; // TODO
    int ndbl, ndwl, nspd, ndsam1, ndsam2, ndcm;
    bool force_cache_config;

    int cache_level;
    int cores;
    int nuca_bank_count;
    int force_nuca_bank;

    int delay_wt, dynamic_power_wt, leakage_power_wt,
        cycle_time_wt, area_wt;
    int delay_wt_nuca, dynamic_power_wt_nuca, leakage_power_wt_nuca,
        cycle_time_wt_nuca, area_wt_nuca;

    int delay_dev, dynamic_power_dev, leakage_power_dev,
        cycle_time_dev, area_dev;
    int delay_dev_nuca, dynamic_power_dev_nuca, leakage_power_dev_nuca,
        cycle_time_dev_nuca, area_dev_nuca;
    int ed; //ED or ED2 optimization
    int nuca;

    bool     fast_access;
    unsigned int block_sz;  // bytes
    unsigned int tag_assoc;
    unsigned int data_assoc;
    bool     is_seq_acc;
    bool     fully_assoc;
    unsigned int nsets;  // == number_of_sets
    int print_detail;


    bool     add_ecc_b_;
	//parameters for design constraint
	double throughput;
	double latency;
	bool pipelinable;
	int pipeline_stages;
	int per_stage_vector;
	bool with_clock_grid;

	bool array_power_gated;
	bool bitline_floating;
	bool wl_power_gated;
	bool cl_power_gated;
	bool interconect_power_gated;
	bool power_gating;

	double perfloss;

	bool cl_vertical;

	// Parameters related to off-chip I/O
	
	double addr_timing, duty_cycle, mem_density, bus_bw, activity_dq, activity_ca, bus_freq;
	int mem_data_width, num_mem_dq, num_clk, num_ca, num_dqs, num_dq; 
	
	double rtt_value, ron_value, tflight_value; //FIXME
	
	Mem_state iostate;
	
	///char iostate, dram_ecc, io_type;
	
	Mem_ECC dram_ecc;
	Mem_IO_type io_type;
	Mem_DIMM dram_dimm;
	
	int num_bobs; // BoB is buffer-on-board such as Intel SMB c102
	
	int capacity; // in GB

	int num_channels_per_bob; // 1 means no bob
	
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
	

	
};


typedef struct{
  int Ndwl;
  int Ndbl;
  double Nspd;
  int deg_bl_muxing;
  int Ndsam_lev_1;
  int Ndsam_lev_2;
  int number_activated_mats_horizontal_direction;
  int number_subbanks;
  int page_size_in_bits;
  double delay_route_to_bank;
  double delay_crossbar;
  double delay_addr_din_horizontal_htree;
  double delay_addr_din_vertical_htree;
  double delay_row_predecode_driver_and_block;
  double delay_row_decoder;
  double delay_bitlines;
  double delay_sense_amp;
  double delay_subarray_output_driver;
  double delay_bit_mux_predecode_driver_and_block;
  double delay_bit_mux_decoder;
  double delay_senseamp_mux_lev_1_predecode_driver_and_block;
  double delay_senseamp_mux_lev_1_decoder;
  double delay_senseamp_mux_lev_2_predecode_driver_and_block;
  double delay_senseamp_mux_lev_2_decoder;
  double delay_input_htree;
  double delay_output_htree;
  double delay_dout_vertical_htree;
  double delay_dout_horizontal_htree;
  double delay_comparator;
  double access_time;
  double cycle_time;
  double multisubbank_interleave_cycle_time;
  double delay_request_network;
  double delay_inside_mat;
  double delay_reply_network;
  double trcd;
  double cas_latency;
  double precharge_delay;
  powerDef power_routing_to_bank;
  powerDef power_addr_input_htree;
  powerDef power_data_input_htree;
  powerDef power_data_output_htree;
  powerDef power_addr_horizontal_htree;
  powerDef power_datain_horizontal_htree;
  powerDef power_dataout_horizontal_htree;
  powerDef power_addr_vertical_htree;
  powerDef power_datain_vertical_htree;
  powerDef power_row_predecoder_drivers;
  powerDef power_row_predecoder_blocks;
  powerDef power_row_decoders;
  powerDef power_bit_mux_predecoder_drivers;
  powerDef power_bit_mux_predecoder_blocks;
  powerDef power_bit_mux_decoders;
  powerDef power_senseamp_mux_lev_1_predecoder_drivers;
  powerDef power_senseamp_mux_lev_1_predecoder_blocks;
  powerDef power_senseamp_mux_lev_1_decoders;
  powerDef power_senseamp_mux_lev_2_predecoder_drivers;
  powerDef power_senseamp_mux_lev_2_predecoder_blocks;
  powerDef power_senseamp_mux_lev_2_decoders;
  powerDef power_bitlines;
  powerDef power_sense_amps;
  powerDef power_prechg_eq_drivers;
  powerDef power_output_drivers_at_subarray;
  powerDef power_dataout_vertical_htree;
  powerDef power_comparators;
  powerDef power_crossbar;
  powerDef total_power;
  double area;
  double all_banks_height;
  double all_banks_width;
  double bank_height;
  double bank_width;
  double subarray_memory_cell_area_height;
  double subarray_memory_cell_area_width;
  double mat_height;
  double mat_width;
  double routing_area_height_within_bank;
  double routing_area_width_within_bank;
  double area_efficiency;
//  double perc_power_dyn_routing_to_bank;
//  double perc_power_dyn_addr_horizontal_htree;
//  double perc_power_dyn_datain_horizontal_htree;
//  double perc_power_dyn_dataout_horizontal_htree;
//  double perc_power_dyn_addr_vertical_htree;
//  double perc_power_dyn_datain_vertical_htree;
//  double perc_power_dyn_row_predecoder_drivers;
//  double perc_power_dyn_row_predecoder_blocks;
//  double perc_power_dyn_row_decoders;
//  double perc_power_dyn_bit_mux_predecoder_drivers;
//  double perc_power_dyn_bit_mux_predecoder_blocks;
//  double perc_power_dyn_bit_mux_decoders;
//  double perc_power_dyn_senseamp_mux_lev_1_predecoder_drivers;
//  double perc_power_dyn_senseamp_mux_lev_1_predecoder_blocks;
//  double perc_power_dyn_senseamp_mux_lev_1_decoders;
//  double perc_power_dyn_senseamp_mux_lev_2_predecoder_drivers;
//  double perc_power_dyn_senseamp_mux_lev_2_predecoder_blocks;
//  double perc_power_dyn_senseamp_mux_lev_2_decoders;
//  double perc_power_dyn_bitlines;
//  double perc_power_dyn_sense_amps;
//  double perc_power_dyn_prechg_eq_drivers;
//  double perc_power_dyn_subarray_output_drivers;
//  double perc_power_dyn_dataout_vertical_htree;
//  double perc_power_dyn_comparators;
//  double perc_power_dyn_crossbar;
//  double perc_power_dyn_spent_outside_mats;
//  double perc_power_leak_routing_to_bank;
//  double perc_power_leak_addr_horizontal_htree;
//  double perc_power_leak_datain_horizontal_htree;
//  double perc_power_leak_dataout_horizontal_htree;
//  double perc_power_leak_addr_vertical_htree;
//  double perc_power_leak_datain_vertical_htree;
//  double perc_power_leak_row_predecoder_drivers;
//  double perc_power_leak_row_predecoder_blocks;
//  double perc_power_leak_row_decoders;
//  double perc_power_leak_bit_mux_predecoder_drivers;
//  double perc_power_leak_bit_mux_predecoder_blocks;
//  double perc_power_leak_bit_mux_decoders;
//  double perc_power_leak_senseamp_mux_lev_1_predecoder_drivers;
//  double perc_power_leak_senseamp_mux_lev_1_predecoder_blocks;
//  double perc_power_leak_senseamp_mux_lev_1_decoders;
//  double perc_power_leak_senseamp_mux_lev_2_predecoder_drivers;
//  double perc_power_leak_senseamp_mux_lev_2_predecoder_blocks;
//  double perc_power_leak_senseamp_mux_lev_2_decoders;
//  double perc_power_leak_bitlines;
//  double perc_power_leak_sense_amps;
//  double perc_power_leak_prechg_eq_drivers;
//  double perc_power_leak_subarray_output_drivers;
//  double perc_power_leak_dataout_vertical_htree;
//  double perc_power_leak_comparators;
//  double perc_power_leak_crossbar;
//  double perc_leak_mats;
//  double perc_active_mats;
  double refresh_power;
  double dram_refresh_period;
  double dram_array_availability;
  double dyn_read_energy_from_closed_page;
  double dyn_read_energy_from_open_page;
  double leak_power_subbank_closed_page;
  double leak_power_subbank_open_page;
  double leak_power_request_and_reply_networks;
  double activate_energy;
  double read_energy;
  double write_energy;
  double precharge_energy;
} results_mem_array;


class uca_org_t
{
  public:
    mem_array * tag_array2;
    mem_array * data_array2;
    double access_time;
    double cycle_time;
    double area;
    double area_efficiency;
    powerDef power;
    double leak_power_with_sleep_transistors_in_mats;
    double cache_ht;
    double cache_len;
    char file_n[100];
    double vdd_periph_global;
    bool valid;
    results_mem_array tag_array;
    results_mem_array data_array;

    uca_org_t();
    void find_delay();
    void find_energy();
    void find_area();
    void find_cyc();
    void adjust_area();//for McPAT only to adjust routing overhead
    void cleanup();
    ~uca_org_t(){};
};


class IO_org_t
{
	public:
	double io_area;
	double io_timing_margin;
	double io_voltage_margin;
	double io_dynamic_power;
	double io_phy_power;
	double io_wakeup_time;
	double io_termination_power;
	IO_org_t():io_area(0),io_timing_margin(0),io_voltage_margin(0)
	,io_dynamic_power(0),io_phy_power(0),io_wakeup_time(0),io_termination_power(0)
	{}
};


void reconfigure(InputParameter *local_interface, uca_org_t *fin_res);

uca_org_t cacti_interface(const string & infile_name);
//McPAT's plain interface, please keep !!!
uca_org_t cacti_interface(InputParameter * const local_interface);
//McPAT's plain interface, please keep !!!
uca_org_t init_interface(InputParameter * const local_interface);
//McPAT's plain interface, please keep !!!
uca_org_t cacti_interface(
	    int cache_size,
	    int line_size,
	    int associativity,
	    int rw_ports,
	    int excl_read_ports,
	    int excl_write_ports,
	    int single_ended_read_ports,
	    int search_ports,
	    int banks,
	    double tech_node,
	    int output_width,
	    int specific_tag,
	    int tag_width,
	    int access_mode,
	    int cache,
	    int main_mem,
	    int obj_func_delay,
	    int obj_func_dynamic_power,
	    int obj_func_leakage_power,
	    int obj_func_cycle_time,
	    int obj_func_area,
	    int dev_func_delay,
	    int dev_func_dynamic_power,
	    int dev_func_leakage_power,
	    int dev_func_area,
	    int dev_func_cycle_time,
	    int ed_ed2_none, // 0 - ED, 1 - ED^2, 2 - use weight and deviate
	    int temp,
	    int wt, //0 - default(search across everything), 1 - global, 2 - 5% delay penalty, 3 - 10%, 4 - 20 %, 5 - 30%, 6 - low-swing
	    int data_arr_ram_cell_tech_flavor_in,
	    int data_arr_peri_global_tech_flavor_in,
	    int tag_arr_ram_cell_tech_flavor_in,
	    int tag_arr_peri_global_tech_flavor_in,
	    int interconnect_projection_type_in,
	    int wire_inside_mat_type_in,
	    int wire_outside_mat_type_in,
	    int REPEATERS_IN_HTREE_SEGMENTS_in,
	    int VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in,
	    int BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in,
	    int PAGE_SIZE_BITS_in,
	    int BURST_LENGTH_in,
	    int INTERNAL_PREFETCH_WIDTH_in,
	    int force_wiretype,
	    int wiretype,
	    int force_config,
	    int ndwl,
	    int ndbl,
	    int nspd,
	    int ndcm,
	    int ndsam1,
	    int ndsam2,
	    int ecc);
//    int cache_size,
//    int line_size,
//    int associativity,
//    int rw_ports,
//    int excl_read_ports,
//    int excl_write_ports,
//    int single_ended_read_ports,
//    int banks,
//    double tech_node,
//    int output_width,
//    int specific_tag,
//    int tag_width,
//    int access_mode,
//    int cache,
//    int main_mem,
//    int obj_func_delay,
//    int obj_func_dynamic_power,
//    int obj_func_leakage_power,
//    int obj_func_area,
//    int obj_func_cycle_time,
//    int dev_func_delay,
//    int dev_func_dynamic_power,
//    int dev_func_leakage_power,
//    int dev_func_area,
//    int dev_func_cycle_time,
//    int temp,
//    int data_arr_ram_cell_tech_flavor_in,
//    int data_arr_peri_global_tech_flavor_in,
//    int tag_arr_ram_cell_tech_flavor_in,
//    int tag_arr_peri_global_tech_flavor_in,
//    int interconnect_projection_type_in,
//    int wire_inside_mat_type_in,
//    int wire_outside_mat_type_in,
//    int REPEATERS_IN_HTREE_SEGMENTS_in,
//    int VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in,
//    int BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in,
////    double MAXAREACONSTRAINT_PERC_in,
////    double MAXACCTIMECONSTRAINT_PERC_in,
////    double MAX_PERC_DIFF_IN_DELAY_FROM_BEST_DELAY_REPEATER_SOLUTION_in,
//    int PAGE_SIZE_BITS_in,
//    int BURST_LENGTH_in,
//    int INTERNAL_PREFETCH_WIDTH_in);

//Naveen's interface
uca_org_t cacti_interface(
    int cache_size,
    int line_size,
    int associativity,
    int rw_ports,
    int excl_read_ports,
    int excl_write_ports,
    int single_ended_read_ports,
    int banks,
    double tech_node,
    int page_sz,
    int burst_length,
    int pre_width,
    int output_width,
    int specific_tag,
    int tag_width,
    int access_mode, //0 normal, 1 seq, 2 fast
    int cache, //scratch ram or cache
    int main_mem,
    int obj_func_delay,
    int obj_func_dynamic_power,
    int obj_func_leakage_power,
    int obj_func_area,
    int obj_func_cycle_time,
    int dev_func_delay,
    int dev_func_dynamic_power,
    int dev_func_leakage_power,
    int dev_func_area,
    int dev_func_cycle_time,
    int ed_ed2_none, // 0 - ED, 1 - ED^2, 2 - use weight and deviate
    int temp,
    int wt, //0 - default(search across everything), 1 - global, 2 - 5% delay penalty, 3 - 10%, 4 - 20 %, 5 - 30%, 6 - low-swing
    int data_arr_ram_cell_tech_flavor_in,
    int data_arr_peri_global_tech_flavor_in,
    int tag_arr_ram_cell_tech_flavor_in,
    int tag_arr_peri_global_tech_flavor_in,
    int interconnect_projection_type_in, // 0 - aggressive, 1 - normal
    int wire_inside_mat_type_in,
    int wire_outside_mat_type_in,
    int is_nuca, // 0 - UCA, 1 - NUCA
    int core_count,
    int cache_level, // 0 - L2, 1 - L3
    int nuca_bank_count,
    int nuca_obj_func_delay,
    int nuca_obj_func_dynamic_power,
    int nuca_obj_func_leakage_power,
    int nuca_obj_func_area,
    int nuca_obj_func_cycle_time,
    int nuca_dev_func_delay,
    int nuca_dev_func_dynamic_power,
    int nuca_dev_func_leakage_power,
    int nuca_dev_func_area,
    int nuca_dev_func_cycle_time,
    int REPEATERS_IN_HTREE_SEGMENTS_in,//TODO for now only wires with repeaters are supported
    int p_input);


//CACTI3DD interface
uca_org_t cacti_interface(
    int cache_size,
    int line_size,
    int associativity,
    int rw_ports,
    int excl_read_ports,// para5
    int excl_write_ports,
    int single_ended_read_ports,
    int search_ports,
    int banks,
    double tech_node,//para10
    int output_width,
    int specific_tag,
    int tag_width,
    int access_mode,
    int cache,      //para15
    int main_mem,
    int obj_func_delay,
    int obj_func_dynamic_power,
    int obj_func_leakage_power,
    int obj_func_cycle_time, //para20
    int obj_func_area,
    int dev_func_delay,
    int dev_func_dynamic_power,
    int dev_func_leakage_power,
    int dev_func_area, //para25
    int dev_func_cycle_time,
    int ed_ed2_none, // 0 - ED, 1 - ED^2, 2 - use weight and deviate
    int temp,
    int wt, //0 - default(search across everything), 1 - global, 2 - 5% delay penalty, 3 - 10%, 4 - 20 %, 5 - 30%, 6 - low-swing
    int data_arr_ram_cell_tech_flavor_in,//para30
    int data_arr_peri_global_tech_flavor_in,
    int tag_arr_ram_cell_tech_flavor_in,
    int tag_arr_peri_global_tech_flavor_in,
    int interconnect_projection_type_in,
    int wire_inside_mat_type_in,//para35
    int wire_outside_mat_type_in,
    int REPEATERS_IN_HTREE_SEGMENTS_in,
    int VERTICAL_HTREE_WIRES_OVER_THE_ARRAY_in,
    int BROADCAST_ADDR_DATAIN_OVER_VERTICAL_HTREES_in,
    int PAGE_SIZE_BITS_in,//para40
    int BURST_LENGTH_in,
    int INTERNAL_PREFETCH_WIDTH_in,
    int force_wiretype,
    int wiretype,
    int force_config,//para45
    int ndwl,
    int ndbl,
    int nspd,
    int ndcm,
    int ndsam1,//para50
    int ndsam2,
    int ecc,
    int is_3d_dram,
    int burst_depth,
    int IO_width,
    int sys_freq,
    int debug_detail,
    int num_dies,
    int tsv_gran_is_subarray,
    int tsv_gran_os_bank,
    int num_tier_row_sprd,
    int num_tier_col_sprd,
    int partition_level);

class mem_array
{
  public:
  int    Ndcm;
  int    Ndwl;
  int    Ndbl;
  double Nspd;
  int    deg_bl_muxing;
  int    Ndsam_lev_1;
  int    Ndsam_lev_2;
  double access_time;
  double cycle_time;
  double multisubbank_interleave_cycle_time;
  double area_ram_cells;
  double area;
  powerDef power;
  double delay_senseamp_mux_decoder;
  double delay_before_subarray_output_driver;
  double delay_from_subarray_output_driver_to_output;
  double height;
  double width;

  double mat_height;
  double mat_length;
  double subarray_length;
  double subarray_height;

  double delay_route_to_bank,
         delay_input_htree,
         delay_row_predecode_driver_and_block,
         delay_row_decoder,
         delay_bitlines,
         delay_sense_amp,
         delay_subarray_output_driver,
         delay_dout_htree,
         delay_comparator,
         delay_matchlines;
  //CACTI3DD 3d stats
  double delay_row_activate_net,
		  delay_local_wordline,

		  delay_column_access_net,
		  delay_column_predecoder,
		  delay_column_decoder,
		  delay_column_selectline,
		  delay_datapath_net,
		  delay_global_data,
		  delay_local_data_and_drv,
		  delay_data_buffer;

  double energy_row_activate_net,
		  energy_row_predecode_driver_and_block,
		  energy_row_decoder,
		  energy_local_wordline,
		  energy_bitlines,
		  energy_sense_amp,
		  energy_column_access_net,
		  energy_column_predecoder,
		  energy_column_decoder,
		  energy_column_selectline,
		  energy_datapath_net,
		  energy_global_data,
		  energy_local_data_and_drv,
		  energy_data_buffer,
		  energy_subarray_output_driver;

  double all_banks_height,
         all_banks_width,
         area_efficiency;

  powerDef power_routing_to_bank;
  powerDef power_addr_input_htree;
  powerDef power_data_input_htree;
  powerDef power_data_output_htree;
  powerDef power_htree_in_search;
  powerDef power_htree_out_search;
  powerDef power_row_predecoder_drivers;
  powerDef power_row_predecoder_blocks;
  powerDef power_row_decoders;
  powerDef power_bit_mux_predecoder_drivers;
  powerDef power_bit_mux_predecoder_blocks;
  powerDef power_bit_mux_decoders;
  powerDef power_senseamp_mux_lev_1_predecoder_drivers;
  powerDef power_senseamp_mux_lev_1_predecoder_blocks;
  powerDef power_senseamp_mux_lev_1_decoders;
  powerDef power_senseamp_mux_lev_2_predecoder_drivers;
  powerDef power_senseamp_mux_lev_2_predecoder_blocks;
  powerDef power_senseamp_mux_lev_2_decoders;
  powerDef power_bitlines;
  powerDef power_sense_amps;
  powerDef power_prechg_eq_drivers;
  powerDef power_output_drivers_at_subarray;
  powerDef power_dataout_vertical_htree;
  powerDef power_comparators;

  powerDef power_cam_bitline_precharge_eq_drv;
  powerDef power_searchline;
  powerDef power_searchline_precharge;
  powerDef power_matchlines;
  powerDef power_matchline_precharge;
  powerDef power_matchline_to_wordline_drv;

  min_values_t *arr_min;
  enum Wire_type wt;

  // dram stats
  double activate_energy, read_energy, write_energy, precharge_energy,
  refresh_power, leak_power_subbank_closed_page, leak_power_subbank_open_page,
  leak_power_request_and_reply_networks;

  double precharge_delay;

  //Power-gating stats
  double  array_leakage;
  double  wl_leakage;
  double  cl_leakage;

  double sram_sleep_tx_width, wl_sleep_tx_width, cl_sleep_tx_width;
  double sram_sleep_tx_area, wl_sleep_tx_area, cl_sleep_tx_area;
  double sram_sleep_wakeup_latency, wl_sleep_wakeup_latency, cl_sleep_wakeup_latency, bl_floating_wakeup_latency;
  double sram_sleep_wakeup_energy, wl_sleep_wakeup_energy, cl_sleep_wakeup_energy, bl_floating_wakeup_energy;

  int num_active_mats;
  int num_submarray_mats;

  static bool lt(const mem_array * m1, const mem_array * m2);

  //CACTI3DD 3d dram stats
  double t_RCD, t_RAS, t_RC, t_CAS, t_RP, t_RRD;
  double activate_power, read_power, write_power, peak_read_power;
  int num_row_subarray, num_col_subarray;
  double delay_TSV_tot, area_TSV_tot, dyn_pow_TSV_tot, dyn_pow_TSV_per_access;
  unsigned int num_TSV_tot;
  double area_lwl_drv, area_row_predec_dec, area_col_predec_dec,
  area_subarray, area_bus, area_address_bus, area_data_bus, area_data_drv, area_IOSA, area_sense_amp;

};


#endif

