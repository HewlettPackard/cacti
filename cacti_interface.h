/*------------------------------------------------------------
 *                              CACTI 6.5
 *         Copyright 2008 Hewlett-Packard Development Corporation
 *                         All Rights Reserved
 *
 * Permission to use, copy, and modify this software and its documentation is
 * hereby granted only under the following terms and conditions.  Both the
 * above copyright notice and this permission notice must appear in all copies
 * of the software, derivative works or modified versions, and any portions
 * thereof, and both notices must appear in supporting documentation.
 *
 * Users of this software agree to the terms and conditions set forth herein, and
 * hereby grant back to Hewlett-Packard Company and its affiliated companies ("HP")
 * a non-exclusive, unrestricted, royalty-free right and license under any changes, 
 * enhancements or extensions  made to the core functions of the software, including 
 * but not limited to those affording compatibility with other hardware or software
 * environments, but excluding applications which incorporate this software.
 * Users further agree to use their best efforts to return to HP any such changes,
 * enhancements or extensions that they make and inform HP of noteworthy uses of
 * this software.  Correspondence should be provided to HP at:
 *
 *                       Director of Intellectual Property Licensing
 *                       Office of Strategy and Technology
 *                       Hewlett-Packard Company
 *                       1501 Page Mill Road
 *                       Palo Alto, California  94304
 *
 * This software may be distributed (but not offered for sale or transferred
 * for compensation) to third parties, provided such third parties agree to
 * abide by the terms and conditions of this notice.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND HP DISCLAIMS ALL
 * WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL HP 
 * CORPORATION BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 *------------------------------------------------------------*/

#ifndef __CACTI_INTERFACE_H__
#define __CACTI_INTERFACE_H__

#include <map>
#include <string>
#include <vector>
#include <list>
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
    
    powerComponents() : dynamic(0), leakage(0) { }
    powerComponents(const powerComponents & obj) { *this = obj; }
    powerComponents & operator=(const powerComponents & rhs)
    {
      dynamic = rhs.dynamic;
      leakage = rhs.leakage;
      return *this;
    }
    void reset() { dynamic = 0; leakage = 0; }
    
    friend powerComponents operator+(const powerComponents & x, const powerComponents & y);
};



class powerDef
{
  public:
    powerComponents readOp;
    powerComponents writeOp;
    
    powerDef() : readOp(), writeOp() { }
    void reset() { readOp.reset(); writeOp.reset(); }
    
    friend powerDef operator+(const powerDef & x, const powerDef & y);
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
    Transmission /* tranmission lines with high area overhead */, 
    Optical /* optical wires */, 
    Invalid_wtype
};



class InputParameter
{
  public:
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
    bool     is_main_mem;
    bool     is_cache;
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

    void find_delay();
    void find_energy();
    void find_area();
    void find_cyc();
};


uca_org_t cacti_interface(const string & infile_name);


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


class mem_array
{
  public:
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
         delay_comparator;

  double all_banks_height,
         all_banks_width,
         area_efficiency;

  powerDef power_routing_to_bank;
  powerDef power_addr_input_htree;
  powerDef power_data_input_htree;
  powerDef power_data_output_htree;
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

  min_values_t *arr_min;
  enum Wire_type wt;

  // dram stats
  double activate_energy, read_energy, write_energy, precharge_energy, 
  refresh_power, leak_power_subbank_closed_page, leak_power_subbank_open_page,
  leak_power_request_and_reply_networks;

  double precharge_delay;

  static bool lt(const mem_array * m1, const mem_array * m2);
};


#endif
